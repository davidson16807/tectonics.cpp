#pragma once

// C libraries
#include "math.h"

// in-house libraries

#include <models/compound/property/elasticity.hpp>
#include <models/compound/property/strength.hpp>
#include <models/compound/property/reflectance.hpp>

#include <models/compound/field/spectral/SpectralFunction.hpp>

namespace compound{
namespace phase{
    
    
    /*
    Return a `PartlyKnownSolid` that has the properties of `known` where present, 
    in addition to properties that can derived from the properties of `known`. The function is idempotent.
    The function can be thought of as the traversal of the category defined within the `property` namespace.
    */
    PartlyKnownSolid infer(const PartlyKnownSolid& known){
        // copy what you do know
        PartlyKnownSolid guess = known;

        // guess.absorption_coefficient = known.absorption_coefficient.value_or(
        //         std::function<si::attenuation<double>(field::SpectralParameters, double, double)>(
        //             [](const field::SpectralParameters spectral_parameters, const double n, const double k)
        //             {
        //                 // TODO: remove assumption that representative wavelength is the middle value
        //                 return compound::property::get_absorption_coefficient_from_refractive_index(n, k, 2.0/(spectral_parameters.nlo+spectral_parameters.nhi));
        //             }
        //         ),
        //         known.refractive_index,
        //         known.extinction_coefficient
        //     );

        // guess.refractive_index = known.refractive_index.value_or(
        //         std::function<double(field::SpectralParameters, si::attenuation<double>, double)>(
        //             [](const field::SpectralParameters spectral_parameters, const si::attenuation<double> alpha, const double k)
        //             {
        //                 // TODO: remove assumption that representative wavelength is the middle value
        //                 return compound::property::get_refractive_index_from_absorption_coefficient(alpha, k, 2.0/(spectral_parameters.nlo+spectral_parameters.nhi));
        //             }
        //         ),
        //         known.absorption_coefficient,
        //         known.refractive_index
        //     );

        // guess.extinction_coefficient = known.extinction_coefficient.value_or(
        //         std::function<double(field::SpectralParameters, si::attenuation<double>, double)>(
        //             [](const field::SpectralParameters spectral_parameters, const si::attenuation<double> alpha, const double n)
        //             {
        //                 // TODO: remove assumption that representative wavelength is the middle value
        //                 return compound::property::get_extinction_coefficient_from_absorption_coefficient(alpha, n, 2.0/(spectral_parameters.nlo+spectral_parameters.nhi));
        //             }
        //         ),
        //         known.absorption_coefficient,
        //         known.extinction_coefficient
        //     );

        /*
        We can correlate tensile and shear yield strengths using the Von Misces Theorem.

        If none of the yield strengths are known, the most common explanation 
        is that the material is too brittle for yield strength to be measured.
        So in this case we assume yield strength equal to the ultimate strength equivalent

        We reuse abbreviations for modulii to represent yield strengths:
        */

        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>)> Pp;
        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>,si::pressure<double>)> Ppp;
        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>,double)> Ppd;
        typedef std::function<si::pressure<double>(field::StateParameters, double,si::pressure<double>)> Pdp;
        typedef std::function<double(field::StateParameters, si::pressure<double>,si::pressure<double>)> Dpp;

        guess.shear_yield_strength = known.shear_yield_strength
            .value_or(Pp([](field::StateParameters, si::pressure<double> E){ return property::get_shear_yield_strength_from_tensile_yield_strength(E); }), 
                known.tensile_yield_strength
            );
        guess.tensile_yield_strength = known.tensile_yield_strength
            .value_or(Pp([](field::StateParameters, si::pressure<double> G){ return property::get_tensile_yield_strength_from_shear_yield_strength(G); }), 
                known.shear_yield_strength
            );

        if (!known.tensile_yield_strength.has_value() && 
            !known.shear_yield_strength.has_value() && 
            !known.compressive_yield_strength.has_value())
        {
            guess.tensile_yield_strength = guess.tensile_yield_strength.value_or(guess.tensile_fracture_strength);
            guess.compressive_yield_strength = guess.compressive_yield_strength.value_or(guess.compressive_fracture_strength);
            guess.shear_yield_strength = guess.shear_yield_strength.value_or(guess.shear_fracture_strength);
        }

        /*
        Standardize on bulk and tensile modulus since they seem to be reported most often,
        then use those two to calculate the remaining modulii.
        To reduce the number of cases, consider only scenarios where bulk, tensile, shear, and lame are present, 
        since again, those are reported often.
        */

        auto K = known.bulk_modulus;
        auto E = known.tensile_modulus;
        auto l = known.lame_parameter;
        auto G = known.shear_modulus;
        auto nu = known.poisson_ratio;
        auto M = known.pwave_modulus;

        guess.tensile_modulus = guess.tensile_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> G) { return property::get_tensile_from_bulk_and_shear(K, G); }), K, G )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> K, double nu)      { return property::get_tensile_from_bulk_and_poisson(K, nu); }), K, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> M) { return property::get_tensile_from_bulk_and_pwave(K, M); }), K, M )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> G) { return property::get_tensile_from_lame_and_shear(l, G); }), l, G )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> l, double nu)      { return property::get_tensile_from_lame_and_poisson(l, nu); }), l, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> M) { return property::get_tensile_from_lame_and_pwave(l, M); }), l, M )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> G, double nu)      { return property::get_tensile_from_shear_and_poisson(G, nu); }), G, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> G, si::pressure<double> M) { return property::get_tensile_from_shear_and_pwave(G, M); }), G, M )
            .value_or(Pdp([](field::StateParameters params, double nu,      si::pressure<double> M) { return property::get_tensile_from_poisson_and_pwave(nu, M); }), nu, M )
            ;

        guess.bulk_modulus = guess.bulk_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> E, si::pressure<double> l) { return property::get_bulk_from_tensile_and_lame(E, l); }), E, l)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> E, si::pressure<double> G) { return property::get_bulk_from_tensile_and_shear(E, G); }), E, G)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> E, double nu)      { return property::get_bulk_from_tensile_and_poisson(E, nu); }), E, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> G) { return property::get_bulk_from_lame_and_shear(l, G); }), l, G)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> l, double nu)      { return property::get_bulk_from_lame_and_poisson(l, nu); }), l, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> M) { return property::get_bulk_from_lame_and_pwave(l, M); }), l, M)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> G, double nu)      { return property::get_bulk_from_shear_and_poisson(G, nu); }), G, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> G, si::pressure<double> M) { return property::get_bulk_from_shear_and_pwave(G, M); }), G, M)
            .value_or(Pdp([](field::StateParameters params, double nu,      si::pressure<double> M) { return property::get_bulk_from_poisson_and_pwave(nu, M); }), nu, M)
            ;

        K = guess.bulk_modulus;
        E = guess.tensile_modulus;

        guess.lame_parameter = guess.lame_parameter
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return property::get_lame_from_bulk_and_tensile(K,E); }), K, E );
        guess.shear_modulus = guess.shear_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return property::get_shear_from_bulk_and_tensile(K,E); }), K, E );
        guess.poisson_ratio = guess.poisson_ratio
            .value_or(Dpp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return property::get_poisson_from_bulk_and_tensile(K,E); }), K, E );
        guess.pwave_modulus = guess.pwave_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return property::get_pwave_from_bulk_and_tensile(K,E); }), K, E );

        return guess;

    }

}}
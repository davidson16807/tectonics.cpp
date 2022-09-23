#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    template<typename Tx, typename Ty, int Plo, int Phi>
    using PolynomialRailyardRelation = Relation<Tx,Ty,math::PolynomialRailyard<float,Plo,Phi>>;

    template<typename T, typename Ty>
    Relation<si::temperature<T>,Ty,math::PolynomialRailyard<float,0,1>> get_linear_interpolation_function(
        const si::celcius_type<T> celcius, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return Relation<si::temperature<T>,Ty,math::PolynomialRailyard<float,0,1>>(
                math::compose(
                    math::spline::linear_spline<double>(xs, ys), 
                    math::Shifting(-si::standard_temperature/si::kelvin)), 
                si::kelvin, yunits);
    }

    template<typename T, typename Ty>
    Relation<si::temperature<T>,Ty, math::PolynomialRailyard<float,0,1>> get_linear_interpolation_function(
        const si::temperature<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return Relation<si::temperature<T>,Ty, math::PolynomialRailyard<float,0,1>>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }

    template<typename T, typename Ty>
    Relation<si::pressure<T>,Ty, math::PolynomialRailyard<float,0,1>> get_linear_interpolation_function(
        const si::pressure<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return Relation<si::pressure<T>,Ty, math::PolynomialRailyard<float,0,1>>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Ty>
    Relation<si::temperature<double>, Ty, math::PolynomialRailyard<float, 0,4>> get_dippr_quartic_temperature_relation_100( 
        const si::temperature<double> Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = math::Polynomial<double,0,4>;
        using R = math::Railcar<double,P>;
        math::Polynomial<double,0,4> p = math::Polynomial<double,0,4>({intercept, slope, square, cube, fourth});
        return Relation<si::temperature<double>,Ty, math::PolynomialRailyard<float,0,4>>(
            math::PolynomialRailyard<double,0,4>({
                R(-oo, Tmin, P(p(Tmin))), 
                R(Tmin,Tmax, p),
                R(Tmax, oo,  P(p(Tmax)))
            }), Tunits, yunits);
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    // 23 uses, all for heat capacity of solids
    Relation<si::temperature<double>, si::specific_heat_capacity<double>, math::PolynomialRailyard<float,  -2,3>> get_perry_johnson_temperature_function( 
        const si::temperature<double> Tunits, 
        const si::specific_heat_capacity<double> y_units_johnson, 
        const double linear_johnson, const double cube_johnson, 
        const double T_max_johnson,
        const si::specific_heat_capacity<double> y_units_perry,
        const double intercept_perry, const double linear_perry, const double inverse_square_perry, const double square_perry, 
        const double T_min_perry, const double T_max_perry
    ){
            const double oo = std::numeric_limits<double>::max();
            using P = math::Polynomial<double,-2,3>;
            using R = math::Railcar<double,P>;
            math::Polynomial<double,1,3>  johnson    = math::Polynomial<double,1,3> ({linear_johnson, 0.0, cube_johnson});
            math::Polynomial<double,-2,2> perry      = y_units_perry/y_units_johnson * math::Polynomial<double,-2,2>({inverse_square_perry, 0.0, intercept_perry, linear_perry, square_perry});
            math::Polynomial<double,0,1>  transition = math::linear_newton_polynomial(T_max_johnson, T_min_perry, johnson(T_max_johnson), perry(T_min_perry));
            return Relation<si::temperature<double>,si::specific_heat_capacity<double>, math::PolynomialRailyard<float,-2,3>>(
                math::PolynomialRailyard<double,-2,3>({
                    R(0.0, T_max_johnson,         P(johnson)),            // johnson polynomial
                    R(T_max_johnson, T_min_perry, P(transition)),         // linear perry/johnson interpolation
                    R(T_min_perry, T_max_perry,   P(perry)),              // perry polynomial
                    R(T_max_perry, oo,            P(perry(T_max_perry))), // perry polynomial
                }), Tunits, y_units_johnson);
    }
    
    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavelength(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> lys
    ){
        assert(ls.size() == lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,3>> get_spectral_cubic_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,3>> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double>      ls, 
        const std::vector<double>log10lys
    ){
        assert(ls.size() == log10lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> log10ys;
        for (std::size_t i=0; i<log10lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            log10ys.push_back(log10lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(log10ys.begin(), log10ys.end());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }


    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_input(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> log10ns, 
        const std::vector<double>      ys
    ){
        assert(log10ns.size() == ys.size());
        std::vector<double> ns;
        for (std::size_t i=0; i<log10ns.size(); i++){
            ns.push_back(pow(10.0, log10ns[i]));
        }
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> log10ls, 
        const std::vector<double>      lys
    ){
        assert(log10ls.size() == lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(pow(10.0, log10ls[i])));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return Relation<si::wavenumber<double>,Ty, math::PolynomialRailyard<float,0,1>> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }




    Relation<si::wavenumber<double>,si::attenuation<double>, math::PolynomialRailyard<float,0,3>> get_absorption_coefficient_function_from_reflectance_at_wavelengths(
        const si::length<double> lunits,
        const si::length<double> particle_diameter, 
        const std::vector<double>wavelengths, 
        const std::vector<double>reflectances
    ){
        std::vector<double> wavenumbers;
        std::vector<double> wavenumber_reflectances = reflectances;
        for (std::size_t i=0; i<reflectances.size(); i++){
            wavenumbers.push_back(1.0/wavelengths[i]);
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(wavenumber_reflectances.begin(), wavenumber_reflectances.end());

        /*
        NOTE: we assume extinction efficiency is close to 1, in which case the 
          single scattering albedo equals scattering efficiency
        In this case:
          absorption_coefficient = approx_single_scatter_albedo_from_reflectance ∘ approx_single_scatter_albedo_from_reflectance
        This is equivalent to:
          absorption_coefficient = max((reflectance + 1.0)*(reflectance + 1.0) / (4.0 * reflectance) - 1.0, 0.0) / De;
        where:
          De = 2*particle_diameter;
        We leverage the fact that 0 ≤ reflectance ≤ 1 to create a segmented 
          linear approximation of 1/reflectance that is suited for that range.
          It has an maximum error of 12% over the targeted range.
        We use a linear approximation since the result will be composed 
          with another segmented linear approximation for reflectance,
          and we don't want the degree of the resulting polynomial 
          to be so high that it produces nans upon evaluation.
        */

        const float oo = std::numeric_limits<float>::max();

        const math::PolynomialRailyard<float,0,1> inverse{
            math::PolynomialRailcar<float,0,1>(0.0357, 0.0778, math::linear_newton_polynomial<float>(0.0357, 0.0778,  28.041, 11.458)),
            math::PolynomialRailcar<float,0,1>(0.0778, 0.1938, math::linear_newton_polynomial<float>(0.0778, 0.1938,  11.458,  4.937)),
            math::PolynomialRailcar<float,0,1>(0.1938, 0.4649, math::linear_newton_polynomial<float>(0.1938, 0.4649,   4.937,  1.881)),
            math::PolynomialRailcar<float,0,1>(0.4649, 1.0000, math::linear_newton_polynomial<float>(0.4649, 1.0000,   1.881,  1.000)),
            math::PolynomialRailcar<float,0,1>(1.0000,     oo, math::linear_newton_polynomial<float>(1.0000, 2.0000,   1.000,  1.000))
        };

        return Relation<si::wavenumber<double>,si::attenuation<double>, math::PolynomialRailyard<float,0,3>>(
            compose(
                (0.25f * math::Shifting(1.0f) * math::Shifting(1.0f) * inverse - 1.0f)/(2.0f*float(particle_diameter/si::length<double>(1.0))),
                math::spline::linear_spline<float>(wavenumbers, wavenumber_reflectances)
            ),
            si::wavenumber<double>(1.0),
            si::attenuation<double>(1.0)
        );
    }

}}


#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <models/compound/published/published.hpp>
#include <models/compound/correlation/published.hpp>
#include <models/compound/correlation/speculative.hpp>
#include <models/compound/correlation/reflectance.hpp>
#include <models/compound/relation/gas/GasDensityStateRelation.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    // Refer to property/published.png to see the category that we are traversing here.
    // Critical point properties and molecular diameter are guaranteed to be inferrable, so start there.
    // After critical point properties, we want to get to acentric factor ("omega") as soon as possible, 
    // since that lets us calculate so many other things.
    // We want to make sure we have at least some route to estimate acentric factor,
    // So we first try to estimate all the things that correlate with acentric factor but are derived by other means.
    // This includes latent_heat_of_vaporization, vapor_pressure_as_liquid, 
    // dynamic_viscosity_as_liquid, and molar_volume_as_liquid.
    // Some of these properties can be found using triple point properties, 
    // Some of these properties can be found using the pressure/temperature at which measurements were collected,
    // and one property ("molar_volume_as_liquid") can be found using liquid or solid density.
    // 
    // So to summarize, the order we use to derive is: 
    //   properties that are already table::completed by published values
    //   critical point properties
    //   pressure/temperature samples
    //   triple_point_temperature and densities
    //   latent_heat_of_vaporization, vapor_pressure_as_liquid, dynamic_viscosity_as_liquid, molar_volume_as_liquid
    //   acentric factor
    //   properties derived by acentric factor
    //   miscellaneous properties


    auto name = published::name;
    auto formula = published::formula;
    
    auto atoms_per_molecule = published::atoms_per_molecule;
    auto freezing_sample_point = published::freezing_sample_point;
    auto boiling_sample_point = published::boiling_sample_point;
    auto molar_mass = published::molar_mass;

    // via Klincewicz
    table::FullTable<si::pressure<double>> critical_point_pressure = 
        table::complete(published::critical_point_pressure, 
            table::derive<si::pressure<double>>(correlation::estimate_critical_pressure_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz
    table::FullTable<si::molar_volume<double>> critical_point_volume = 
        table::complete(published::critical_point_volume, 
            table::derive<si::molar_volume<double>>(correlation::estimate_critical_molar_volume_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz -> Ihmels
    table::FullTable<si::temperature<double>> critical_point_temperature = 
        table::complete(published::critical_point_temperature, 
            table::derive<si::temperature<double>>(correlation::estimate_critical_temperature_from_ihmels, critical_point_pressure, critical_point_volume));

    // via Klincewicz -> Ihmels -> definition of compressibility
    table::FullTable<double> critical_point_compressibility = 
        table::complete(published::critical_point_compressibility, 
            table::derive<double>(correlation::get_critical_compressibility, critical_point_pressure, critical_point_temperature, critical_point_volume));

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward
    table::FullTable<si::length<double>> molecular_diameter = 
        table::complete(published::molecular_diameter, 
            table::derive<si::length<double>>(correlation::estimate_molecular_diameter_from_tee_gotoh_steward, critical_point_temperature, critical_point_pressure));

    point<double> standard = point<double>(si::standard_pressure, si::standard_temperature);

    table::FullTable<point<double>> critical_point = 
        table::derive<point<double>>(
            [](si::pressure<double> p, si::temperature<double> T){ return point<double>(p,T); }, 
            critical_point_pressure, critical_point_temperature);

    table::FullTable<point<double>> solid_sample_point = 
        table::derive<point<double>>(
            [](point<double> freezing){ return  standard.temperature < freezing.temperature? standard : freezing; }, 
            freezing_sample_point);

    table::FullTable<point<double>> liquid_sample_point = 
        table::complete(
            table::gather<point<double>>(
                [](point<double> freezing, point<double> boiling){ 
                    return freezing.temperature < standard.temperature && standard.temperature < boiling.temperature? 
                        standard : (freezing + boiling)/2.0; }, 
                table::partial(freezing_sample_point), boiling_sample_point),
            freezing_sample_point
        );

    table::FullTable<point<double>> gas_sample_point = 
        table::complete(
            table::gather<point<double>>([](point<double> boiling){ return boiling.temperature < standard.temperature? standard : boiling; }, published::boiling_sample_point),
            critical_point
        );


    table::PartialTable<si::temperature<double>> boiling_sample_point_temperature = 
        table::gather<si::temperature<double>>(
            [](point<double> pT){ return pT.temperature; }, 
            boiling_sample_point);

    table::PartialTable<si::pressure<double>> boiling_sample_point_pressure = 
        table::gather<si::pressure<double>>(
            [](point<double> pT){ return pT.pressure; }, 
            boiling_sample_point);


    table::FullTable<si::temperature<double>> freezing_sample_point_temperature = 
        table::derive<si::temperature<double>>(
            [](point<double> pT){ return pT.temperature; }, 
            freezing_sample_point);

    table::FullTable<si::pressure<double>> freezing_sample_point_pressure = 
        table::derive<si::pressure<double>>(
            [](point<double> pT){ return pT.pressure; }, 
            freezing_sample_point);

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward -> Bird-Steward-Lightfoot2
    using LiquidDensityTemperatureRelation = published::LiquidDensityTemperatureRelation;
    table::FullTable<LiquidDensityTemperatureRelation> density_as_liquid = 
        table::complete(published::density_as_liquid, 
            table::derive<LiquidDensityTemperatureRelation>([](si::molar_mass<double> M, si::length<double> sigma){
                    return LiquidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));}, 
                molar_mass, molecular_diameter));

    // via Klincewicz -> Ihmels -> definition of compressibility -> definition of compressibility
    using GasDensityStateRelation = relation::GasDensityStateRelation;
    table::FullTable<GasDensityStateRelation> density_as_gas = 
        table::derive<GasDensityStateRelation>([](si::molar_mass<double> M, si::pressure<double> pc, si::temperature<double> Tc, double Zc){
                return GasDensityStateRelation(M,pc,Tc,Zc);}, 
            molar_mass, critical_point_pressure, critical_point_temperature, critical_point_compressibility);

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward -> Bird-Steward-Lightfoot2 -> Goodman (with speculative fallback)
    using SolidDensityTemperatureRelation = published::SolidDensityTemperatureRelation;
    table::FullTable<SolidDensityTemperatureRelation> density_as_solid = 
        table::complete(
            table::first<SolidDensityTemperatureRelation>({
                published::density_as_solid, 
                table::gather<SolidDensityTemperatureRelation>([](si::molar_mass<double> M, LiquidDensityTemperatureRelation rhoL, point<double> liquid, point<double> triple){
                        return SolidDensityTemperatureRelation(M / correlation::estimate_molar_volume_as_solid_from_goodman(M / rhoL(liquid.temperature), liquid.temperature, triple.temperature));}, 
                    table::partial(molar_mass), 
                    table::partial(density_as_liquid), 
                    table::partial(liquid_sample_point), 
                    published::triple_point),
            }),
            table::derive<SolidDensityTemperatureRelation>([](LiquidDensityTemperatureRelation rhoL, point<double> liquid){
                    return correlation::guess_density_as_solid_from_density_as_liquid(rhoL(liquid.temperature));}, 
                density_as_liquid, liquid_sample_point) // WARNING: results are speculative
        );



    // // CALCULATE POSSIBLE ROUTES TO ACENTRIC FACTOR
    using LatentHeatTemperatureRelation = published::LatentHeatTemperatureRelation;

    using SolidVaporPressureTemperatureRelation = published::SolidVaporPressureTemperatureRelation;
    table::PartialTable<SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid = 
        table::first<SolidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_solid,
            table::gather<SolidVaporPressureTemperatureRelation>(
                []( LatentHeatTemperatureRelation Hs, 
                    si::molar_mass<double> M, 
                    point<double> triple){
                        auto T = analytic::Identity<float>();
                        auto R = si::universal_gas_constant;
                        float T3 = triple.temperature / si::kelvin;
                        float P3 = triple.pressure / si::pascal;
                        float k = (Hs(triple.temperature)*M/R)/si::kelvin;
                        using Polynomial = analytic::Polynomial<float,-1,1>;
                        using Clamped = analytic::Clamped<float,Polynomial>;
                        using Railyard = analytic::Railyard<float,Polynomial>;
                        return P3 * SolidVaporPressureTemperatureRelation(
                            Railyard(Clamped(0.0f, T3, Polynomial(-k*(1.0f/T - 1.0f/T3)))),
                            si::kelvin, si::pascal);
                        // NOTE: the above is translated from property/published.hpp:
                        // return triple_point_pressure * exp(-((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
                }, 
                published::latent_heat_of_sublimation, // NOTE: this should be latent heat of sublimation at the triple point, update this line if ever we convert latent_heat_of_fusion to a relation
                table::partial(molar_mass), 
                published::triple_point)
        });

    // CALCULATE ACENTRIC FACTOR
    using LiquidDynamicViscosityTemperatureRelation = published::LiquidDynamicViscosityTemperatureRelation;
    table::PartialTable<double> acentric_factor = 
        table::first<double>({
            published::acentric_factor,
            table::gather<double>(
                []( point<double> liquid,
                    point<double> critical,
                    si::molar_mass<double> M,
                    LatentHeatTemperatureRelation Hv){
                    auto T = si::clamp(liquid.temperature, si::absolute_zero, critical.temperature);
                    auto Tc = critical.temperature;
                    return std::clamp(correlation::estimate_accentric_factor_from_pitzer(Hv(T), M, T, Tc), -1.0, 1.0);
                },
                table::partial(liquid_sample_point),
                table::partial(critical_point),
                table::partial(molar_mass),
                published::latent_heat_of_vaporization
            ),
            table::gather<double>(
                []( point<double> liquid,
                    point<double> critical,
                    si::molar_mass<double> M,
                    LiquidDynamicViscosityTemperatureRelation nuL){
                    auto T = liquid.temperature;
                    auto Tc = critical.temperature;
                    auto pc = critical.pressure;
                    return std::clamp(correlation::estimate_acentric_factor_from_letsou_stiel(nuL(T), M, T, Tc, pc), -1.0, 1.0);
                },
                table::partial(liquid_sample_point),
                table::partial(critical_point),
                table::partial(molar_mass),
                published::dynamic_viscosity_as_liquid
            ),
        });

    // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR
    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_vaporization,
            table::gather<LatentHeatTemperatureRelation>(
                relation::get_pitzer_latent_heat_of_vaporization_temperature_relation,
                table::partial(molar_mass), 
                table::partial(critical_point_temperature), 
                acentric_factor
            ),
        });

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_sublimation = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_sublimation,
            // latent_heat_of_vaporization + published::latent_heat_of_fusion,
        });

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_fusion = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_fusion,
            published::latent_heat_of_sublimation - latent_heat_of_vaporization,
        });

    using LiquidDynamicViscosityTemperatureRelation = published::LiquidDynamicViscosityTemperatureRelation;
    table::PartialTable<LiquidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_liquid = 
        table::first<LiquidDynamicViscosityTemperatureRelation>({
            published::dynamic_viscosity_as_liquid,
            table::gather<LiquidDynamicViscosityTemperatureRelation>(
                relation::estimate_viscosity_as_liquid_from_letsou_stiel,
                acentric_factor,
                table::partial(molar_mass),
                table::partial(critical_point_temperature),
                table::partial(critical_point_pressure)
            ),
        });

    using LiquidVaporPressureTemperatureRelation = published::LiquidVaporPressureTemperatureRelation;
    table::PartialTable<LiquidVaporPressureTemperatureRelation> vapor_pressure_as_liquid = 
        table::first<LiquidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_liquid,
            // table::gather<LiquidVaporPressureTemperatureRelation>(
            //     relation::estimate_vapor_pressure_as_liquid_from_lee_kesler,
            //     acentric_factor,
            //     table::partial(critical_point_temperature),
            //     table::partial(critical_point_pressure)
            // ),
        });

    // CALCULATE MISCELLANEOUS PROPERTIES
    using LiquidThermalConductivityTemperatureRelation = published::LiquidThermalConductivityTemperatureRelation;
    table::FullTable<LiquidThermalConductivityTemperatureRelation> thermal_conductivity_as_liquid = 
        table::complete(
            published::thermal_conductivity_as_liquid,
            table::derive<LiquidThermalConductivityTemperatureRelation>(
                relation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson,
                molar_mass, 
                freezing_sample_point_temperature,
                critical_point_temperature
            )
        );

    using LiquidHeatCapacityTemperatureRelation = published::LiquidHeatCapacityTemperatureRelation;
    auto isobaric_specific_heat_capacity_as_liquid = published::isobaric_specific_heat_capacity_as_liquid;

    using GasDynamicViscosityStateRelation = published::GasDynamicViscosityStateRelation;
    auto dynamic_viscosity_as_gas = published::dynamic_viscosity_as_gas;

    using GasThermalConductivityStateRelation = published::GasThermalConductivityStateRelation;
    auto thermal_conductivity_as_gas = published::thermal_conductivity_as_gas;

    using GasHeatCapacityStateRelation = published::GasHeatCapacityStateRelation;
    auto isobaric_specific_heat_capacity_as_gas = published::isobaric_specific_heat_capacity_as_gas;

    using SolidDynamicViscosityTemperatureRelation = published::SolidDynamicViscosityTemperatureRelation;
    table::PartialTable<SolidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_solid = 
        table::first<SolidDynamicViscosityTemperatureRelation>({
            published::dynamic_viscosity_as_solid,
            table::gather<SolidDynamicViscosityTemperatureRelation>(
                []( LiquidDynamicViscosityTemperatureRelation nuL,
                    point<double> liquid){
                    return SolidDynamicViscosityTemperatureRelation(
                        correlation::guess_viscosity_as_solid_from_viscosity_as_liquid(nuL(liquid.temperature)));
                    // WARNING: results above are speculative, 
                    // see property/speculative.hpp for reasoning and justification
                },
                dynamic_viscosity_as_liquid,
                table::partial(liquid_sample_point)
            )
        });

    using SolidRefractiveIndexWavenumberRelation = published::SolidRefractiveIndexWavenumberRelation;
    using SolidExtinctionCoefficientWavenumberRelation = published::SolidExtinctionCoefficientWavenumberRelation;
    using SolidAbsorptionCoefficientWavenumberRelation = published::SolidAbsorptionCoefficientWavenumberRelation;
    // table::PartialTable<SolidAbsorptionCoefficientWavenumberRelation> absorption_coefficient_as_solid = 
    //     table::first<SolidAbsorptionCoefficientWavenumberRelation>({
    //         published::absorption_coefficient_as_solid,
    //         table::gather<SolidAbsorptionCoefficientWavenumberRelation>(
    //             []( published::SolidRefractiveIndexWavenumberRelation n,
    //                 published::SolidExtinctionCoefficientWavenumberRelation k){
    //                 return SolidAbsorptionCoefficientWavenumberRelation(
    //                     [n,k](si::wavenumber<double> w){return correlation::get_absorption_coefficient_from_refractive_index(n(w), k(w), 1.0/w);});
    //             },
    //             published::refractive_index_as_solid,
    //             published::extinction_coefficient_as_solid
    //         )
    //     });

    table::FullTable<double> molecular_degrees_of_freedom = 
        table::complete(
            table::first<double>({
                published::molecular_degrees_of_freedom,
                table::gather<double>(
                    correlation::guess_molecular_degrees_of_freedom,
                    acentric_factor,
                    table::partial(atoms_per_molecule)
                )
            }),
            6.0,
            ids::count
        );


}}



/*
#pragma once

// C libraries
#include "math.h"

// in-house libraries

#include <models/compound/correlation/elasticity.hpp>
#include <models/compound/correlation/strength.hpp>
#include <models/compound/correlation/reflectance.hpp>

#include <models/compound/field/spectral/SpectralFunction.hpp>

namespace compound{
namespace phase{
    
    
    
    // Return a `PartlyKnownSolid` that has the properties of `known` where present, 
    // in addition to properties that can derived from the properties of `known`. The function is idempotent.
    // The function can be thought of as the traversal of the category defined within the `property` namespace.
    
    PartlyKnownSolid infer(const PartlyKnownSolid& known){
        // copy what you do know
        PartlyKnownSolid guess = known;


        // guess.refractive_index = known.refractive_index.value_or(
        //         std::function<double(field::SpectralParameters, si::attenuation<double>, double)>(
        //             [](const field::SpectralParameters spectral_parameters, const si::attenuation<double> alpha, const double k)
        //             {
        //                 // TODO: remove assumption that representative wavelength is the middle value
        //                 return compound::correlation::get_refractive_index_from_absorption_coefficient(alpha, k, 2.0/(spectral_parameters.nlo+spectral_parameters.nhi));
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
        //                 return compound::correlation::get_extinction_coefficient_from_absorption_coefficient(alpha, n, 2.0/(spectral_parameters.nlo+spectral_parameters.nhi));
        //             }
        //         ),
        //         known.absorption_coefficient,
        //         known.extinction_coefficient
        //     );

        
        // We can correlate tensile and shear yield strengths using the Von Misces Theorem.

        // If none of the yield strengths are known, the most common explanation 
        // is that the material is too brittle for yield strength to be measured.
        // So in this case we assume yield strength equal to the ultimate strength equivalent

        // We reuse abbreviations for modulii to represent yield strengths:
        

        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>)> Pp;
        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>,si::pressure<double>)> Ppp;
        typedef std::function<si::pressure<double>(field::StateParameters, si::pressure<double>,double)> Ppd;
        typedef std::function<si::pressure<double>(field::StateParameters, double,si::pressure<double>)> Pdp;
        typedef std::function<double(field::StateParameters, si::pressure<double>,si::pressure<double>)> Dpp;

        guess.shear_yield_strength = known.shear_yield_strength
            .value_or(Pp([](field::StateParameters, si::pressure<double> E){ return correlation::get_shear_yield_strength_from_tensile_yield_strength(E); }), 
                known.tensile_yield_strength
            );
        guess.tensile_yield_strength = known.tensile_yield_strength
            .value_or(Pp([](field::StateParameters, si::pressure<double> G){ return correlation::get_tensile_yield_strength_from_shear_yield_strength(G); }), 
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

        
        // Standardize on bulk and tensile modulus since they seem to be reported most often,
        // then use those two to calculate the remaining modulii.
        // To reduce the number of cases, consider only scenarios where bulk, tensile, shear, and lame are present, 
        // since again, those are reported often.
        

        auto K = known.bulk_modulus;
        auto E = known.tensile_modulus;
        auto l = known.lame_parameter;
        auto G = known.shear_modulus;
        auto nu = known.poisson_ratio;
        auto M = known.pwave_modulus;

        guess.tensile_modulus = guess.tensile_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> G) { return correlation::get_tensile_from_bulk_and_shear(K, G); }), K, G )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> K, double nu)      { return correlation::get_tensile_from_bulk_and_poisson(K, nu); }), K, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> M) { return correlation::get_tensile_from_bulk_and_pwave(K, M); }), K, M )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> G) { return correlation::get_tensile_from_lame_and_shear(l, G); }), l, G )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> l, double nu)      { return correlation::get_tensile_from_lame_and_poisson(l, nu); }), l, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> M) { return correlation::get_tensile_from_lame_and_pwave(l, M); }), l, M )
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> G, double nu)      { return correlation::get_tensile_from_shear_and_poisson(G, nu); }), G, nu )
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> G, si::pressure<double> M) { return correlation::get_tensile_from_shear_and_pwave(G, M); }), G, M )
            .value_or(Pdp([](field::StateParameters params, double nu,      si::pressure<double> M) { return correlation::get_tensile_from_poisson_and_pwave(nu, M); }), nu, M )
            ;

        guess.bulk_modulus = guess.bulk_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> E, si::pressure<double> l) { return correlation::get_bulk_from_tensile_and_lame(E, l); }), E, l)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> E, si::pressure<double> G) { return correlation::get_bulk_from_tensile_and_shear(E, G); }), E, G)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> E, double nu)      { return correlation::get_bulk_from_tensile_and_poisson(E, nu); }), E, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> G) { return correlation::get_bulk_from_lame_and_shear(l, G); }), l, G)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> l, double nu)      { return correlation::get_bulk_from_lame_and_poisson(l, nu); }), l, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> l, si::pressure<double> M) { return correlation::get_bulk_from_lame_and_pwave(l, M); }), l, M)
            .value_or(Ppd([](field::StateParameters params, si::pressure<double> G, double nu)      { return correlation::get_bulk_from_shear_and_poisson(G, nu); }), G, nu)
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> G, si::pressure<double> M) { return correlation::get_bulk_from_shear_and_pwave(G, M); }), G, M)
            .value_or(Pdp([](field::StateParameters params, double nu,      si::pressure<double> M) { return correlation::get_bulk_from_poisson_and_pwave(nu, M); }), nu, M)
            ;

        K = guess.bulk_modulus;
        E = guess.tensile_modulus;

        guess.lame_parameter = guess.lame_parameter
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return correlation::get_lame_from_bulk_and_tensile(K,E); }), K, E );
        guess.shear_modulus = guess.shear_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return correlation::get_shear_from_bulk_and_tensile(K,E); }), K, E );
        guess.poisson_ratio = guess.poisson_ratio
            .value_or(Dpp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return correlation::get_poisson_from_bulk_and_tensile(K,E); }), K, E );
        guess.pwave_modulus = guess.pwave_modulus
            .value_or(Ppp([](field::StateParameters params, si::pressure<double> K, si::pressure<double> E){ return correlation::get_pwave_from_bulk_and_tensile(K,E); }), K, E );

        return guess;

    }

}}


*/

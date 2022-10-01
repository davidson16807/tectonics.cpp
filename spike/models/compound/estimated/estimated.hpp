#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <models/compound/published/published.hpp>
#include <models/compound/property/published.hpp>
#include <models/compound/property/speculative.hpp>
#include <models/compound/relation/GasDensityStateRelation.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    // Refer to property/published.png to see the category that we are traversing here.
    // Critical point properties and molecular diameter are guaranteed to be inferrable, so start there.
    // After critical point properties, we want to get to acentric factor ("omega") as soon as possible, 
    // since that lets us calculate so many other things.
    // We want to make sure we have at least some route to estimate acentric factor,
    // So we table::first try to estimate all the things that correlate with acentric factor but are derived by other means.
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
            table::derive<si::pressure<double>>(property::estimate_critical_pressure_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz
    table::FullTable<si::molar_volume<double>> critical_point_volume = 
        table::complete(published::critical_point_volume, 
            table::derive<si::molar_volume<double>>(property::estimate_critical_molar_volume_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz -> Ihmels
    table::FullTable<si::temperature<double>> critical_point_temperature = 
        table::complete(published::critical_point_temperature, 
            table::derive<si::temperature<double>>(property::estimate_critical_temperature_from_ihmels, critical_point_pressure, critical_point_volume));

    // via Klincewicz -> Ihmels -> definition of compressibility
    table::FullTable<double> critical_point_compressibility = 
        table::complete(published::critical_point_compressibility, 
            table::derive<double>(property::get_critical_compressibility, critical_point_pressure, critical_point_temperature, critical_point_volume));

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward
    table::FullTable<si::length<double>> molecular_diameter = 
        table::complete(published::molecular_diameter, 
            table::derive<si::length<double>>(property::estimate_molecular_diameter_from_tee_gotoh_steward, critical_point_temperature, critical_point_pressure));

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
                partial(freezing_sample_point), boiling_sample_point),
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
                    return LiquidDensityTemperatureRelation(M / property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));}, 
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
                        return SolidDensityTemperatureRelation(M / property::estimate_molar_volume_as_solid_from_goodman(M / rhoL(liquid.temperature), liquid.temperature, triple.temperature));}, 
                    partial(molar_mass), partial(density_as_liquid), partial(liquid_sample_point), published::triple_point),
            }),
            table::derive<SolidDensityTemperatureRelation>([](LiquidDensityTemperatureRelation rhoL, point<double> liquid){
                    return property::guess_density_as_solid_from_density_as_liquid(rhoL(liquid.temperature));}, 
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
                partial(molar_mass), 
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
                    return std::clamp(property::estimate_accentric_factor_from_pitzer(Hv(T), M, T, Tc), -1.0, 1.0);
                },
                partial(liquid_sample_point),
                partial(critical_point),
                partial(molar_mass),
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
                    return std::clamp(property::estimate_acentric_factor_from_letsou_stiel(nuL(T), M, T, Tc, pc), -1.0, 1.0);
                },
                partial(liquid_sample_point),
                partial(critical_point),
                partial(molar_mass),
                published::dynamic_viscosity_as_liquid
            ),
        });

    // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR
    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization = 
        table::first<LatentHeatTemperatureRelation>({
            published::latent_heat_of_vaporization,
            table::gather<LatentHeatTemperatureRelation>(
                relation::get_pitzer_latent_heat_of_vaporization_temperature_relation,
                partial(molar_mass), partial(critical_point_temperature), acentric_factor
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
                partial(molar_mass),
                partial(critical_point_temperature),
                partial(critical_point_pressure)
            ),
        });

    using LiquidVaporPressureTemperatureRelation = published::LiquidVaporPressureTemperatureRelation;
    table::PartialTable<LiquidVaporPressureTemperatureRelation> vapor_pressure_as_liquid = 
        table::first<LiquidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_liquid,
            table::gather<LiquidVaporPressureTemperatureRelation>(
                relation::estimate_vapor_pressure_as_liquid_from_lee_kesler,
                acentric_factor,
                partial(critical_point_temperature),
                partial(critical_point_pressure),
                boiling_sample_point_temperature
            ),
        });
}}



    /*

    guess.liquid.vapor_pressure = guess.liquid.vapor_pressure.value_or( 
        [Tc, pc](field::StateParameters parameters, double acentric_factor){ 
            return property::estimate_vapor_pressure_as_liquid_from_lee_kesler(
                acentric_factor, parameters.temperature, Tc, pc
            );
        }, 
        guess.acentric_factor
    );

    // CALCULATE MISCELLANEOUS PROPERTIES
    guess.liquid.thermal_conductivity = guess.liquid.thermal_conductivity.value_or(
        [M, Tc](field::StateParameters parameters, si::temperature<double> boiling_sample_point_temperature){
            return property::estimate_thermal_conductivity_as_liquid_from_sato_riedel(
                M, parameters.temperature, boiling_sample_point_temperature, Tc
            );
        },
        guess.boiling_sample_point_temperature
    );

    guess.liquid.thermal_conductivity = guess.liquid.thermal_conductivity.value_or(
        [M](field::StateParameters parameters, si::temperature<double> freezing_sample_point_temperature){
            return property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
                M, parameters.temperature, freezing_sample_point_temperature
            );
        },
        guess.freezing_sample_point_temperature
    );

    guess.gas.dynamic_viscosity = guess.gas.dynamic_viscosity.value_or(
        [M](relation::GasPropertyStateRelation<si::specific_heat_capacity<double>> heat_capacity_as_gas, 
            relation::GasPropertyStateRelation<si::thermal_conductivity<double>>   thermal_conductivity_as_gas
        ) -> relation::GasPropertyStateRelation<si::dynamic_viscosity<double>>{ 
            return relation::approx_inferred_pressure_temperature_gas_relation(
                si::kelvin, si::pascal, si::dynamic_viscosity<double>(1.0),
                [=](si::pressure<double> p, si::temperature<double> T) -> si::dynamic_viscosity<double> {
                    return property::estimate_viscosity_as_gas_from_eucken(
                        heat_capacity_as_gas(p,T), M, thermal_conductivity_as_gas(p,T));
                },
                // TODO: add logic to compose estimates for valid ranges from source relations
                3.0,    // kelvin,
                1000.0, // kelvin,
                0.1,    // pascal
                1e6,    // pascal
                0.0     // known_max_fractional_error
            );
        },
        guess.gas.isobaric_specific_heat_capacity, 
        guess.gas.thermal_conductivity
    );

    guess.gas.thermal_conductivity = guess.gas.thermal_conductivity.value_or( 
        [M](relation::GasPropertyStateRelation<si::dynamic_viscosity<double>>      dynamic_viscosity_as_gas, 
            relation::GasPropertyStateRelation<si::specific_heat_capacity<double>> heat_capacity_as_gas
        ) -> relation::GasPropertyStateRelation<si::thermal_conductivity<double>> { 
            relation::GasPropertyStateRelation<si::thermal_conductivity<double>> result =
                relation::approx_inferred_pressure_temperature_gas_relation(
                si::kelvin, si::pascal, si::thermal_conductivity<double>(1.0),
                [=](si::pressure<double> p, si::temperature<double> T) -> si::thermal_conductivity<double> {
                    return property::estimate_thermal_conductivity_as_gas_from_eucken(
                        dynamic_viscosity_as_gas(p,T), M, heat_capacity_as_gas(p,T));
                },
                // TODO: add logic to compose estimates for valid ranges from source relations
                3.0,    // kelvin,
                1000.0, // kelvin,
                0.1,    // pascal
                1e6,    // pascal
                0.0     // known_max_fractional_error
            );
            return result;
        },
        guess.gas.dynamic_viscosity, 
        guess.gas.isobaric_specific_heat_capacity
    );


    // Return a `PartlyKnownCompound` that has the properties of `known` where present, 
    // in addition to properties that can derived from the properties of `known`. The function is idempotent.
    // The function can be thought of as the traversal of the category defined within the `property` namespace.
    PartlyKnownCompound speculate(const PartlyKnownCompound& known){
        PartlyKnownCompound guess = known;
        int A = known.atoms_per_molecule;


        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i].dynamic_viscosity = guess.solids[i].dynamic_viscosity.value_or( 
                [](field::StateParameters parameters, si::dynamic_viscosity<double> dynamic_viscosity_as_liquid){ 
                    return property::guess_viscosity_as_solid_from_viscosity_as_liquid(dynamic_viscosity_as_liquid);
                },
                guess.liquid.dynamic_viscosity 
            );
        }

        guess.molecular_degrees_of_freedom = guess.molecular_degrees_of_freedom.value_or(
            [A](double acentric_factor){
                return property::guess_molecular_degrees_of_freedom(acentric_factor, A);
            },
            guess.acentric_factor
        );

        guess.molecular_degrees_of_freedom = guess.molecular_degrees_of_freedom.value_or(6.0);

        return guess;
    }

    */






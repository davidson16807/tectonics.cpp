#pragma once

// C libraries
#include <cmath>

// std libraries

#include <models/compound/published/published.hpp>
#include <models/compound/property/published.hpp>
#include <models/compound/property/speculative.hpp>
#include <models/compound/estimated/tools.hpp>
#include <models/compound/relation/GasDensityStateRelation.hpp>

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
    //   properties that are already completed by published values
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
    auto molar_mass = published::molar_mass;

    // via Klincewicz
    std::vector<si::pressure<double>> critical_point_pressure = 
        complete(published::critical_point_pressure, 
            derive<si::pressure<double>>(property::estimate_critical_pressure_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz
    std::vector<si::molar_volume<double>> critical_point_volume = 
        complete(published::critical_point_volume, 
            derive<si::molar_volume<double>>(property::estimate_critical_molar_volume_from_klincewicz, molar_mass, atoms_per_molecule));

    // via Klincewicz -> Ihmels
    std::vector<si::temperature<double>> critical_point_temperature = 
        complete(published::critical_point_temperature, 
            derive<si::temperature<double>>(property::estimate_critical_temperature_from_ihmels, critical_point_pressure, critical_point_volume));

    // via Klincewicz -> Ihmels -> definition of compressibility
    std::vector<double> critical_point_compressibility = 
        complete(published::critical_point_compressibility, 
            derive<double>(property::get_critical_compressibility, critical_point_pressure, critical_point_temperature, critical_point_volume));

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward
    std::vector<si::length<double>> molecular_diameter = 
        complete(published::molecular_diameter, 
            derive<si::length<double>>(property::estimate_molecular_diameter_from_tee_gotoh_steward, critical_point_temperature, critical_point_pressure));





    point<double> standard = point<double>(si::standard_pressure, si::standard_temperature);

    std::vector<point<double>> critical_point = 
        derive<point<double>>([](si::pressure<double> p, si::temperature<double> T){ return point<double>(p,T); }, critical_point_pressure, critical_point_temperature);

    std::vector<point<double>> solid_sample_point = 
        derive<point<double>>([](point<double> freezing){ return freezing.temperature > standard.temperature? standard : freezing; }, freezing_sample_point);

    std::vector<point<double>> liquid_sample_point = 
        complete(
            attempt<point<double>>([](point<double> freezing, point<double> boiling){ 
                        return freezing.temperature < standard.temperature && standard.temperature < boiling.temperature? standard : (freezing + boiling)/2.0; }, 
                    maybe(freezing_sample_point), published::boiling_sample_point),
            freezing_sample_point
        );

    std::vector<point<double>> gas_sample_point = 
        complete(
            attempt<point<double>>([](point<double> boiling){ return boiling.temperature < standard.temperature? standard : boiling; }, published::boiling_sample_point),
            critical_point
        );



    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward -> Bird-Steward-Lightfoot2
    using LiquidDensityTemperatureRelation = published::LiquidDensityTemperatureRelation;
    std::vector<LiquidDensityTemperatureRelation> density_as_liquid = 
        complete(published::density_as_liquid, 
            derive<LiquidDensityTemperatureRelation>([](si::molar_mass<double> M, si::length<double> sigma){
                    return LiquidDensityTemperatureRelation(M / property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));}, 
                molar_mass, molecular_diameter));

    // via Klincewicz -> Ihmels -> definition of compressibility -> definition of compressibility
    using GasDensityStateRelation = relation::GasDensityStateRelation;
    std::vector<GasDensityStateRelation> density_as_gas = 
        derive<GasDensityStateRelation>([](si::molar_mass<double> M, si::pressure<double> pc, si::temperature<double> Tc, double Zc){
                return GasDensityStateRelation(M,pc,Tc,Zc);}, 
            molar_mass, critical_point_pressure, critical_point_temperature, critical_point_compressibility);

    // via Klincewicz -> Ihmels -> Tee-Gotoh-Steward -> Bird-Steward-Lightfoot2 -> Goodman (with speculative fallback)
    using SolidDensityTemperatureRelation = published::SolidDensityTemperatureRelation;
    std::vector<SolidDensityTemperatureRelation> density_as_solid = 
        complete(
            first<SolidDensityTemperatureRelation>({
                published::density_as_solid, 
                attempt<SolidDensityTemperatureRelation>([](si::molar_mass<double> M, LiquidDensityTemperatureRelation rhoL, point<double> liquid, point<double> triple){
                        return SolidDensityTemperatureRelation(M / property::estimate_molar_volume_as_solid_from_goodman(M / rhoL(liquid.temperature), liquid.temperature, triple.temperature));}, 
                    maybe(molar_mass), maybe(density_as_liquid), maybe(liquid_sample_point), published::triple_point),
            }),
            derive<SolidDensityTemperatureRelation>([](LiquidDensityTemperatureRelation rhoL, point<double> liquid){
                    return property::guess_density_as_solid_from_density_as_liquid(rhoL(liquid.temperature));}, 
                density_as_liquid, liquid_sample_point) // WARNING: results are speculative
        );



    // // CALCULATE POSSIBLE ROUTES TO ACENTRIC FACTOR

    using SolidVaporPressureTemperatureRelation = published::SolidVaporPressureTemperatureRelation;
    std::map<int, SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid = 
        first<SolidVaporPressureTemperatureRelation>({
            published::vapor_pressure_as_solid,
            attempt<SolidVaporPressureTemperatureRelation>(
                []( si::specific_energy<double> Hf, 
                    si::molar_mass<double> M, 
                    point<double> triple){
                        auto T = analytic::Identity<float>();
                        auto R = si::universal_gas_constant;
                        float T3 = triple.temperature / si::kelvin;
                        float P3 = triple.pressure / si::pascal;
                        float k = (Hf*M/R)/si::kelvin;
                        using Polynomial = analytic::Polynomial<float,-1,1>;
                        using Clamped = analytic::Clamped<float,Polynomial>;
                        using Railyard = analytic::Railyard<float,Polynomial>;
                        return P3 * SolidVaporPressureTemperatureRelation(
                            Railyard(Clamped(0.0f, T3, Polynomial(-k*(1.0f/T - 1.0f/T3)))),
                            si::kelvin, si::pascal);
                        // NOTE: the above is translated from property/published.hpp:
                        // return triple_point_pressure * exp(-((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
                }, 
                published::latent_heat_of_fusion, // NOTE: this should be latent heat of fusion at the triple point, update this line if ever we convert latent_heat_of_fusion to a relation
                maybe(molar_mass), 
                published::triple_point)
        });

    std::map<int, si::specific_energy<double>> latent_heat_of_vaporization = 
        first<si::specific_energy<double>>({
            published::latent_heat_of_vaporization,
            attempt<si::specific_energy<double>>(
                []( point<double> triple, 
                    point<double> critical, 
                    point<double> solid, 
                    si::molar_mass<double> M,
                    SolidVaporPressureTemperatureRelation PvS
                    ){
                        auto T = solid.temperature;
                        auto T3 = triple.temperature;
                        auto P3 = triple.pressure;
                        return property::estimate_latent_heat_of_sublimation_at_triple_point_from_clapeyron(PvS(T), M, T, T3, P3);
                },
                published::triple_point,
                maybe(critical_point),
                maybe(solid_sample_point),
                maybe(molar_mass),
                vapor_pressure_as_solid)
        });

}}



    /*
    PartlyKnownCompound infer(const PartlyKnownCompound& known){
        PartlyKnownCompound guess = known;
        si::molar_mass<double> M = known.molar_mass;
        // int A = known.atoms_per_molecule;
        si::length<double> sigma = known.molecular_diameter;
        si::temperature<double> Tc = known.critical_point_temperature;
        si::pressure<double> pc = known.critical_point_pressure;
        // auto k = si::boltzmann_constant;
        double Zc = known.critical_point_compressibility;

        if (guess.solids.size() > 0)
        {
            guess.latent_heat_of_vaporization = guess.latent_heat_of_vaporization.value_or(
                [M](field::StateParameters parameters,
                    si::pressure<double> vapor_pressure, 
                    si::temperature<double> triple_point_temperature, 
                    si::pressure<double> triple_point_pressure, 
                    si::specific_energy<double> latent_heat_of_fusion){
                    si::specific_energy<double> estimate = property::estimate_latent_heat_of_sublimation_from_clapeyron(
                        vapor_pressure,
                        M, 
                        parameters.temperature, 
                        triple_point_temperature, 
                        triple_point_pressure
                    ) - latent_heat_of_fusion;
                    bool is_valid = si::specific_energy<double>(0.0) < estimate && !std::isinf(estimate/si::specific_energy<double>(1.0));
                    return is_valid? field::OptionalConstantField<si::specific_energy<double>>(estimate) : field::OptionalConstantField<si::specific_energy<double>>();
                },
                samples.solid,
                guess.solids[0].vapor_pressure,
                guess.triple_point_temperature, 
                guess.triple_point_pressure,
                guess.latent_heat_of_fusion
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.latent_heat_of_fusion = guess.latent_heat_of_fusion.value_or(
                [M](field::StateParameters parameters, 
                    si::pressure<double> vapor_pressure, 
                    si::temperature<double> triple_point_temperature, 
                    si::pressure<double> triple_point_pressure, 
                    si::specific_energy<double> latent_heat_of_vaporization){ 
                    si::specific_energy<double> estimate = property::estimate_latent_heat_of_sublimation_from_clapeyron(
                        vapor_pressure,
                        M, 
                        parameters.temperature, 
                        triple_point_temperature, 
                        triple_point_pressure
                    ) - latent_heat_of_vaporization;
                    bool is_valid = si::specific_energy<double>(0.0) < estimate && !std::isinf(estimate/si::specific_energy<double>(1.0));
                    return is_valid? field::OptionalConstantField<si::specific_energy<double>>(estimate) : field::OptionalConstantField<si::specific_energy<double>>();
                },
                samples.solid,
                guess.solids[0].vapor_pressure,
                guess.triple_point_temperature, 
                guess.triple_point_pressure,
                guess.latent_heat_of_vaporization
            );
        }

        // CALCULATE ACENTRIC FACTOR
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [M, Tc](field::StateParameters parameters, si::specific_energy<double> latent_heat_of_vaporization){ 
                double estimate = property::estimate_accentric_factor_from_pitzer(
                        latent_heat_of_vaporization, M, 
                        parameters.temperature, Tc
                    );
                bool is_valid = -1.0 < estimate && estimate < 1.0;
                return is_valid? field::OptionalConstantField<double>(estimate) : field::OptionalConstantField<double>();
            },
            samples.liquid,
            guess.latent_heat_of_vaporization
        );
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [M, pc, Tc](field::StateParameters parameters, si::dynamic_viscosity<double> dynamic_viscosity_as_liquid){ 
                double estimate = property::estimate_acentric_factor_from_letsou_stiel(
                    dynamic_viscosity_as_liquid,
                    M, parameters.temperature, Tc, pc
                );
                bool is_valid = -1.0 < estimate && estimate < 1.0;
                return is_valid? field::OptionalConstantField<double>(estimate) : field::OptionalConstantField<double>();
            },
            samples.liquid,
            guess.liquid.dynamic_viscosity
        );
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [pc, Tc](field::StateParameters parameters, si::pressure<double> vapor_pressure_as_liquid){ 
                double estimate = property::estimate_accentric_factor_from_lee_kesler(
                    vapor_pressure_as_liquid, 
                    parameters.temperature, Tc, pc
                );
                bool is_valid = -1.0 < estimate && estimate < 1.0;
                return is_valid? field::OptionalConstantField<double>(estimate) : field::OptionalConstantField<double>();
            },
            samples.boiling,
            guess.liquid.vapor_pressure
        );

        // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR
        guess.latent_heat_of_vaporization = guess.latent_heat_of_vaporization.value_or( 
            [M, Tc](field::StateParameters parameters, double acentric_factor){ 
                si::specific_energy<double> estimate = property::estimate_latent_heat_of_vaporization_from_pitzer(
                    acentric_factor, M, parameters.temperature, Tc
                );
                bool is_valid = si::specific_energy<double>(0.0) < estimate && !std::isinf(estimate/si::specific_energy<double>(1.0));
                return is_valid? field::OptionalConstantField<si::specific_energy<double>>(estimate) : field::OptionalConstantField<si::specific_energy<double>>();
            }, 
            samples.liquid,
            guess.acentric_factor
        );
        guess.liquid.dynamic_viscosity = guess.liquid.dynamic_viscosity.value_or(    
            [M, Tc, pc](field::StateParameters parameters, double acentric_factor){ 
                return property::estimate_viscosity_as_liquid_from_letsou_stiel(
                    acentric_factor, M, parameters.temperature, Tc, pc
                );
            }, 
            guess.acentric_factor
        );
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


        return guess;
    }
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






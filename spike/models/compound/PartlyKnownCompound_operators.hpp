#pragma once

// C libraries
#include "math.h"

#include <models/compound/property/published.hpp>
#include <models/compound/property/speculative.hpp>

#include <models/compound/field/OptionalConstantField_OptionalSpectralField.hpp>
#include <models/compound/field/OptionalConstantField_OptionalStateField.hpp>

#include "PartlyKnownCompound.hpp"

namespace compound
{
        
    /*
    Return a `PartlyKnownCompound` that has the properties of `known` where present, 
    in addition to properties that can derived from the properties of `known`. The function is idempotent.
    The function can be thought of as the traversal of the category defined within the `property` namespace.
    */
    PartlyKnownCompound infer(const PartlyKnownCompound& known){
        PartlyKnownCompound guess = known;
        si::molar_mass M = known.molar_mass;
        // int A = known.atoms_per_molecule;
        si::length sigma = known.molecular_diameter;
        si::temperature Tc = known.critical_point_temperature;
        si::pressure pc = known.critical_point_pressure;
        // auto k = si::boltzmann_constant;
        double Zc = known.critical_point_compressibility;

        // Refer to property/published.png to see the category that we are traversing here.
        // We want to get to acentric factor ("omega") as soon as possible, 
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
        //   pressure/temperature samples
        //   triple_point_temperature and densities
        //   latent_heat_of_vaporization, vapor_pressure_as_liquid, dynamic_viscosity_as_liquid, molar_volume_as_liquid
        //   acentric factor
        //   properties derived by acentric factor
        //   miscellaneous properties

        field::OptionalStateParameters optional_gas_sample;
        field::OptionalStateParameters optional_boiling_sample;
        field::OptionalStateParameters optional_liquid_sample;
        field::OptionalStateParameters optional_melting_sample;
        field::OptionalStateParameters optional_solid_sample;
        field::StateParameters standard_sample(si::standard_pressure, si::standard_temperature);
        field::StateParameters critical_point_sample(guess.critical_point_pressure, guess.critical_point_temperature);

        // CALCULATE PRESSURE/TEMPERATURE SAMPLE POINTS
        if (guess.freezing_point_sample_pressure == guess.boiling_point_sample_pressure)
        {
            optional_liquid_sample = optional_liquid_sample.value_or(
                []( si::pressure freezing_point_sample_pressure, si::temperature freezing_point_sample_temperature, 
                    si::pressure boiling_point_sample_pressure, si::temperature boiling_point_sample_temperature)
                {
                    if (freezing_point_sample_temperature < si::standard_temperature && 
                                                            si::standard_temperature < boiling_point_sample_temperature)
                    {
                        return field::OptionalStateParameters(
                            field::StateParameters(
                                freezing_point_sample_pressure, 
                                si::standard_temperature
                            )
                        );
                    }
                    else
                    {
                        return field::OptionalStateParameters(
                            field::StateParameters(
                                freezing_point_sample_pressure, 
                                (freezing_point_sample_temperature + boiling_point_sample_temperature) / 2.0
                            )
                        );
                    }
                },
                guess.freezing_point_sample_pressure,
                guess.freezing_point_sample_temperature,
                guess.boiling_point_sample_pressure,
                guess.boiling_point_sample_temperature
            );
        }

        if (guess.freezing_point_sample_temperature.has_value())
        {
            field::StateParameters sample(
                guess.freezing_point_sample_pressure.complete(si::standard_pressure), 
                guess.freezing_point_sample_temperature.complete(si::standard_temperature)
            );
            if (sample.pressure == si::standard_pressure)
            {
                if (si::standard_temperature < sample.temperature)
                {
                    optional_solid_sample = optional_solid_sample.complete(standard_sample);
                }
                if (sample.temperature < si::standard_temperature)
                {
                    optional_liquid_sample = optional_liquid_sample.complete(standard_sample);
                }
            }
            optional_solid_sample   = optional_solid_sample.complete(sample);
            optional_melting_sample = optional_melting_sample.complete(sample);
            optional_liquid_sample  = optional_liquid_sample.complete(sample);
        }

        if (guess.boiling_point_sample_temperature.has_value())
        {
            field::StateParameters sample(
                guess.boiling_point_sample_pressure.complete(si::standard_pressure), 
                guess.boiling_point_sample_temperature.complete(si::standard_temperature)
            );
            if (sample.pressure == si::standard_pressure)
            {
                if (si::standard_temperature < sample.temperature)
                {
                    optional_solid_sample = optional_solid_sample.complete(standard_sample);
                }
                if (sample.temperature < si::standard_temperature)
                {
                    optional_liquid_sample = optional_liquid_sample.complete(standard_sample);
                }
            }
            optional_liquid_sample  = optional_liquid_sample.complete(sample);
            optional_boiling_sample = optional_boiling_sample.complete(sample);
            optional_gas_sample     = optional_gas_sample.complete(sample);
        }

        if (guess.triple_point_temperature.has_value() && guess.triple_point_pressure.has_value())
        {
            field::StateParameters sample(
                guess.triple_point_pressure.complete(si::standard_pressure), 
                guess.triple_point_temperature.complete(si::standard_temperature)
            );
            optional_solid_sample   = optional_solid_sample.complete(sample);
            optional_melting_sample = optional_melting_sample.complete(sample);
            optional_liquid_sample  = optional_liquid_sample.complete(sample);
            optional_boiling_sample = optional_boiling_sample.complete(sample);
            optional_gas_sample     = optional_gas_sample.complete(sample);
        }

        // field::StateParameters solid_sample   = optional_solid_sample.complete(standard_sample);
        // field::StateParameters melting_sample = optional_melting_sample.complete(standard_sample);
        field::StateParameters liquid_sample  = optional_liquid_sample.complete(critical_point_sample);
        // field::StateParameters boiling_sample = optional_boiling_sample.complete(critical_point_sample);
        // field::StateParameters gas_sample     = optional_gas_sample.complete(critical_point_sample);

        // CALCULATE DENSITY
        guess.liquid.density = guess.liquid.density
            .complete(M / property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));

        guess.gas.density = guess.gas.density
            .complete([M,pc,Tc,Zc](si::pressure p, si::temperature T) { 
                return M / property::estimate_molar_volume_as_gas(p, pc, T, Tc, Zc);
            });

        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i].density = guess.solids[i].density.value_or(
                [M](field::StateParameters parameters, si::density density_as_liquid, si::temperature triple_point_temperature){ 
                    return M / property::estimate_molar_volume_as_solid_from_goodman(M / density_as_liquid, parameters.temperature, triple_point_temperature); 
                },
                liquid_sample,
                guess.liquid.density,
                guess.triple_point_temperature
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.liquid.density = guess.liquid.density.value_or(
                [M](field::StateParameters parameters, si::density density_as_solid, si::temperature triple_point_temperature){ 
                    return M / property::estimate_molar_volume_as_liquid_from_goodman(M / density_as_solid, parameters.temperature, triple_point_temperature); 
                },
                liquid_sample,
                guess.solids[0].density,
                guess.triple_point_temperature
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.triple_point_temperature = guess.triple_point_temperature.value_or(
                [](field::StateParameters parameters, si::density density_as_liquid){ 
                    return si::standard_temperature; // property::estimate_triple_point_temperature_from_goodman(M / density_as_liquid, M / density_as_solid, parameters.temperature);
                },
                liquid_sample,
                guess.liquid.density
            );
        }

        return guess;
/*

        // CALCULATE POSSIBLE ROUTES TO ACENTRIC FACTOR
        guess.solids[i].vapor_pressure = guess.solids[i].vapor_pressure.value_or( 
            [M](field::StateParameters parameters, 
               si::specific_energy latent_heat_of_vaporization, si::specific_energy latent_heat_of_fusion, 
               si::pressure triple_point_pressure, si::temperature triple_point_temperature){
                return property::estimate_vapor_pressure_as_solid_from_clapeyron(
                    M, 
                    latent_heat_of_vaporization + latent_heat_of_fusion,
                    parameters.temperature, triple_point_temperature,
                    triple_point_pressure
                );
            },
            solid_sample,
            guess.latent_heat_of_vaporization,
            guess.latent_heat_of_fusion,
            guess.triple_point_temperature,
            guess.triple_point_pressure
        );

        guess.latent_heat_of_vaporization = guess.latent_heat_of_vaporization.value_or(
            [M](field::StateParameters parameters
                si::pressure vapor_pressure, 
                si::temperature triple_point_temperature, 
                si::pressure triple_point_pressure, 
                si::specific_energy latent_heat_of_fusion){
                return property::estimate_latent_heat_of_sublimation_from_clapeyron(
                    vapor_pressure,
                    M, 
                    parameters.temperature, 
                    triple_point_temperature, 
                    triple_point_pressure
                ) - latent_heat_of_fusion;
            },
            solid_sample,
            guess.solids[i].vapor_pressure,
            guess.triple_point_temperature, 
            guess.triple_point_pressure,
            guess.latent_heat_of_vaporization
        );

        guess.latent_heat_of_fusion = guess.latent_heat_of_fusion.value_or(
            [M](field::StateParameters parameters, 
                si::pressure vapor_pressure, 
                si::temperature triple_point_temperature, 
                si::pressure triple_point_pressure, 
                si::specific_energy latent_heat_of_vaporization){ 
                return property::estimate_latent_heat_of_sublimation_from_clapeyron(
                    vapor_pressure,
                    M, 
                    parameters.temperature, 
                    triple_point_temperature, 
                    triple_point_pressure
                ) - latent_heat_of_vaporization;
            },
            solid_sample,
            guess.solids[i].vapor_pressure,
            guess.triple_point_temperature, 
            guess.triple_point_pressure,
            guess.latent_heat_of_vaporization
        );


        guess.solids[i].dynamic_viscosity = guess.solids[i].dynamic_viscosity.value_or( 
            [](si::dynamic_viscosity dynamic_viscosity_as_liquid){ 
                return property::guess_viscosity_as_solid_from_viscosity_as_liquid(dynamic_viscosity_as_liquid);
            }
            guess.liquid.dynamic_viscosity 
        );

        // CALCULATE ACENTRIC FACTOR
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [M, Tc](field::StateParameters parameters, si::specific_energy latent_heat_of_vaporization){ 
                return property::estimate_accentric_factor_from_pitzer(
                        latent_heat_of_vaporization, M, 
                        parameters.temperature, Tc
                    );
            },
            liquid_sample,
            guess.latent_heat_of_vaporization
        );
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [M, Pc, Tc](field::StateParameters parameters, si::dynamic_viscosity dynamic_viscosity_as_liquid){ 
                return property::estimate_acentric_factor_from_letsou_stiel(
                    dynamic_viscosity_as_liquid,
                    M, parameters.temperature, Tc, Pc
                );
            },
            liquid_sample,
            guess.liquid.dynamic_viscosity
        );
        guess.acentric_factor = guess.acentric_factor.value_or( 
            [M, Pc, Tc](field::StateParameters parameters, si::pressure vapor_pressure_as_liquid){ 
                return property::estimate_accentric_factor_from_lee_kesler(
                    vapor_pressure_as_liquid, M, 
                    parameters.temperature, Tc, Pc
                );
            }
            boiling_sample,
            guess.liquid.vapor_pressure
        );

        // CALCULATE PROPERTIES THAT CAN BE DERIVED FROM ACENTRIC FACTOR
        guess.latent_heat_of_vaporization = latent_heat_of_vaporization.value_or( 
            [M, Tc](field::StateParameters parameters, double acentric_factor){ 
                return property::estimate_latent_heat_of_vaporization_from_pitzer(
                    acentric_factor, M, T, Tc
                );
            }, 
            liquid_sample,
            guess.acentric_factor
        );
        guess.liquid.dynamic_viscosity = liquid.dynamic_viscosity.value_or(    
            [M, Tc, Pc](field::StateParameters parameters, double acentric_factor){ 
                return property::estimate_viscosity_as_liquid_from_letsou_stiel(
                    acentric_factor, M, T, Tc, Pc
                );
            }, 
            liquid_sample,
            guess.acentric_factor
        );
        guess.liquid.vapor_pressure = liquid.vapor_pressure.value_or( 
            [M, Tc, Pc](field::StateParameters parameters, double acentric_factor){ 
                return property::estimate_vapor_pressure_as_liquid_from_lee_kesler(
                    acentric_factor, M, T, Tc, Pc
                );
            }, 
            boiling_sample,
            guess.acentric_factor
        );

        guess.molecular_degrees_of_freedom = guess.molecular_degrees_of_freedom.value_or(
            [A](double acentric_factor){
                return property::guess_molecular_degrees_of_freedom(acentric_factor, A);
            },
            guess.acentric_factor
        );
        guess.molecular_degrees_of_freedom = guess.molecular_degrees_of_freedom.value_or(6.0);

        // CALCULATE MISCELLANEOUS PROPERTIES
        guess.liquid.thermal_conductivity = guess.liquid.thermal_conductivity.value_or(
            [](field::StateParameters parameters, si::temperature freezing_point_sample_temperature){
                return property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
                    M, parameters.temperature, freezing_point_sample_temperature
                );
            },
            liquid_sample,
            guess.freezing_point_sample_temperature
        );

        guess.gas.dynamic_viscosity = guess.gas.dynamic_viscosity.value_or(
            [M](si::thermal_conductivity thermal_conductivity_as_gas, si::specific_heat_capacity heat_capacity_as_gas){ 
                return property::estimate_viscosity_as_gas_from_eucken(thermal_conductivity_as_gas, M, heat_capacity_as_gas);
            },
            guess.gas.thermal_conductivity, 
            guess.gas.heat_capacity
        );

        guess.gas.thermal_conductivity = guess.gas.thermal_conductivity.value_or( 
            [M](si::dynamic_viscosity dynamic_viscosity_as_gas, si::specific_heat_capacity heat_capacity_as_gas){ 
                return property::estimate_thermal_conductivity_as_gas_from_eucken(dynamic_viscosity_as_gas, M, heat_capacity_as_gas);
            }
            guess.gas.dynamic_viscosity, 
            guess.gas.heat_capacity
        );

        guess.molecular_absorption_cross_section = guess.molecular_absorption_cross_section.value_or(
            [M, k, Pc, Tc, Zc](field::SpectralParameters sample, si::density density_as_solid, double reflectance, double refractive_index){
                si::length wavelength = 2.0/(sample.nlo+sample.nhi);
                double attenuation_coefficient_as_solid = property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(reflectance, refractive_index, 1.0, wavelength);
                si::number_density number_density_as_solid = density_as_solid/M;
                return property::get_cross_section_from_attenuation_coefficient(attenuation_coefficient_as_solid, number_density_as_solid/si::molecule);
            },
            guess.solids[i].density,
            guess.solids[i].spectral_reflectance,
            guess.solids[i].refractive_index
        );
*/
        /*
        NOTE: the following is not currently very useful, but could be useful if we discover other correlations in the future
        guess.solids[i].spectral_reflectance = guess.solids[i].spectral_reflectance.value_or(
            [k, Pc, Tc, Zc](field::StateParameters sample, si::density density_as_solid, double refractive_index_as_solid, double molecular_absorption_cross_section){
                // find number density of sample
                double Z = property::estimate_compressibility_factor(sample.pressure, sample.temperature, Pc, Tc, Zc);
                si::number_density number_density_as_gas = property::get_number_density_from_compressibility(sample.pressure, sample.temperature, Z);
                si::number_density number_density_as_solid = density_as_solid/M;
                si::area attenuation_coefficient_as_solid = property::get_attenuation_coefficient_from_cross_section(molecular_absorption_cross_section, number_density_as_solid/si::molecule);
                return property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(attenuation_coefficient_as_solid, refractive_index_as_solid, 1.0, wavelength);
            },
            guess.solids[i].density,
            guess.solids[i].refractive_index,
            guess.molecular_absorption_cross_section
        );
        */
    }




    /*
    Return a `PartlyKnownCompound` that has the properties of `known` where present, 
    in addition to properties that can derived from the properties of `known`. The function is idempotent.
    The function can be thought of as the traversal of the category defined within the `property` namespace.
    */
    PartlyKnownCompound speculate(const PartlyKnownCompound& known){
        PartlyKnownCompound guess = known;
        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i].density = guess.solids[i].density.value_or( 
                [](si::density density_as_liquid){ 
                    return property::guess_density_as_solid_from_density_as_liquid(density_as_liquid); 
                },
                guess.liquid.density
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.liquid.density = guess.liquid.density.value_or( 
                [](si::density density_as_solid){ 
                    return property::guess_density_as_liquid_from_density_as_solid(density_as_solid); 
                },
                guess.solids[0].density
            );
        }

        return guess;
    }


}
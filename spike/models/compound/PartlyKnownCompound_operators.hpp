#pragma once

// C libraries
#include <cmath>

#include "models/compound/property/published.hpp"
#include "models/compound/property/speculative.hpp"
// #include "models/compound/property/color.hpp"

#include "field/EmptyParameters_OptionalSpectralParameters.hpp"
#include "field/EmptyParameters_OptionalStateParameters.hpp"
#include "field/OptionalStateParameters_OptionalSpectralParameters.hpp"

// #include "phase/gas/PartlyKnownGas_operators.hpp"
// #include "phase/liquid/PartlyKnownLiquid_operators.hpp"
#include "phase/solid/PartlyKnownSolid_operators.hpp"

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

        StateParameterSamples samples(guess);

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
                samples.liquid,
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
                samples.liquid,
                guess.solids[0].density,
                guess.triple_point_temperature
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.triple_point_temperature = guess.triple_point_temperature.value_or(
                [M](field::StateParameters parameters, si::density density_as_liquid, si::density density_as_solid){ 
                    auto estimate = property::estimate_triple_point_temperature_from_goodman(M / density_as_liquid, M / density_as_solid, parameters.temperature);
                    return estimate > 0.0 * si::kelvin? 
                        field::OptionalConstantField<si::temperature>(estimate) : 
                        field::OptionalConstantField<si::temperature>();
                },
                samples.liquid,
                guess.liquid.density,
                guess.solids[0].density
            );
        }

        // CALCULATE POSSIBLE ROUTES TO ACENTRIC FACTOR
        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i].vapor_pressure = guess.solids[i].vapor_pressure.value_or( 
                [M](field::StateParameters parameters, 
                   si::specific_energy latent_heat_of_vaporization, si::specific_energy latent_heat_of_fusion, 
                   si::pressure triple_point_pressure, si::temperature triple_point_temperature){
                    return property::estimate_vapor_pressure_as_solid_from_clapeyron(
                        latent_heat_of_vaporization + latent_heat_of_fusion,
                        M, 
                        parameters.temperature, triple_point_temperature,
                        triple_point_pressure
                    );
                },
                samples.solid,
                guess.latent_heat_of_vaporization,
                guess.latent_heat_of_fusion,
                guess.triple_point_pressure,
                guess.triple_point_temperature
            );
        }

        if (guess.solids.size() > 0)
        {
            guess.latent_heat_of_vaporization = guess.latent_heat_of_vaporization.value_or(
                [M](field::StateParameters parameters,
                    si::pressure vapor_pressure, 
                    si::temperature triple_point_temperature, 
                    si::pressure triple_point_pressure, 
                    si::specific_energy latent_heat_of_fusion){
                    si::specific_energy estimate = property::estimate_latent_heat_of_sublimation_from_clapeyron(
                        vapor_pressure,
                        M, 
                        parameters.temperature, 
                        triple_point_temperature, 
                        triple_point_pressure
                    ) - latent_heat_of_fusion;
                    bool is_valid = si::specific_energy(0.0) < estimate && !std::isinf(estimate/si::specific_energy(1.0));
                    return is_valid? field::OptionalConstantField<si::specific_energy>(estimate) : field::OptionalConstantField<si::specific_energy>();
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
                    si::pressure vapor_pressure, 
                    si::temperature triple_point_temperature, 
                    si::pressure triple_point_pressure, 
                    si::specific_energy latent_heat_of_vaporization){ 
                    si::specific_energy estimate = property::estimate_latent_heat_of_sublimation_from_clapeyron(
                        vapor_pressure,
                        M, 
                        parameters.temperature, 
                        triple_point_temperature, 
                        triple_point_pressure
                    ) - latent_heat_of_vaporization;
                    bool is_valid = si::specific_energy(0.0) < estimate && !std::isinf(estimate/si::specific_energy(1.0));
                    return is_valid? field::OptionalConstantField<si::specific_energy>(estimate) : field::OptionalConstantField<si::specific_energy>();
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
            [M, Tc](field::StateParameters parameters, si::specific_energy latent_heat_of_vaporization){ 
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
            [M, pc, Tc](field::StateParameters parameters, si::dynamic_viscosity dynamic_viscosity_as_liquid){ 
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
            [pc, Tc](field::StateParameters parameters, si::pressure vapor_pressure_as_liquid){ 
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
                si::specific_energy estimate = property::estimate_latent_heat_of_vaporization_from_pitzer(
                    acentric_factor, M, parameters.temperature, Tc
                );
                bool is_valid = si::specific_energy(0.0) < estimate && !std::isinf(estimate/si::specific_energy(1.0));
                return is_valid? field::OptionalConstantField<si::specific_energy>(estimate) : field::OptionalConstantField<si::specific_energy>();
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
            samples.liquid,
            guess.acentric_factor
        );
        guess.liquid.vapor_pressure = guess.liquid.vapor_pressure.value_or( 
            [Tc, pc](field::StateParameters parameters, double acentric_factor){ 
                return property::estimate_vapor_pressure_as_liquid_from_lee_kesler(
                    acentric_factor, parameters.temperature, Tc, pc
                );
            }, 
            samples.boiling,
            guess.acentric_factor
        );

        // CALCULATE MISCELLANEOUS PROPERTIES
        guess.liquid.thermal_conductivity = guess.liquid.thermal_conductivity.value_or(
            [M](field::StateParameters parameters, si::temperature freezing_point_sample_temperature){
                return property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
                    M, parameters.temperature, freezing_point_sample_temperature
                );
            },
            samples.liquid,
            guess.freezing_point_sample_temperature
        );

        guess.gas.dynamic_viscosity = guess.gas.dynamic_viscosity.value_or(
            [M](si::specific_heat_capacity heat_capacity_as_gas, si::thermal_conductivity thermal_conductivity_as_gas){ 
                return property::estimate_viscosity_as_gas_from_eucken(heat_capacity_as_gas, M, thermal_conductivity_as_gas);
            },
            guess.gas.specific_heat_capacity, 
            guess.gas.thermal_conductivity
        );

        guess.gas.thermal_conductivity = guess.gas.thermal_conductivity.value_or( 
            [M](si::dynamic_viscosity dynamic_viscosity_as_gas, si::specific_heat_capacity heat_capacity_as_gas){ 
                return property::estimate_thermal_conductivity_as_gas_from_eucken(dynamic_viscosity_as_gas, M, heat_capacity_as_gas);
            },
            guess.gas.dynamic_viscosity, 
            guess.gas.specific_heat_capacity
        );


        /*
        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.molecular_absorption_cross_section = guess.molecular_absorption_cross_section.value_or(
                [M, k, pc, Tc, Zc](field::SpectralParameters sample, si::density density_as_solid, double reflectance, double refractive_index){
                    si::length wavelength = 2.0/(sample.nlo+sample.nhi);
                    si::attenuation attenuation_coefficient_as_solid = property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(reflectance, refractive_index, 1.0, wavelength);
                    si::molar_density molar_density_as_solid = density_as_solid/M;
                    return property::get_cross_section_from_attenuation_coefficient(attenuation_coefficient_as_solid, molar_density_as_solid/si::molecule);
                },
                field::SpectralParameters(1.0/(500.0*si::nanometer), 1.0/(500.0*si::nanometer), samples.solid.pressure, samples.solid.temperature),
                guess.solids[i].density,
                guess.solids[i].spectral_reflectance,
                guess.solids[i].refractive_index
            );
        }
        */

        /*
        NOTE: the following is not currently very useful, but could be useful if we discover other correlations in the future
        guess.solids[i].spectral_reflectance = guess.solids[i].spectral_reflectance.value_or(
            [k, pc, Tc, Zc](field::StateParameters sample, si::density density_as_solid, double refractive_index_as_solid, double molecular_absorption_cross_section){
                // find number density of sample
                double Z = property::estimate_compressibility_factor(sample.pressure, sample.temperature, Pc, Tc, Zc);
                si::number_density number_density_as_gas = property::get_number_density_from_compressibility(sample.pressure, sample.temperature, Z);
                si::molar_density molar_density_as_solid = density_as_solid/M;
                si::attenuation attenuation_coefficient_as_solid = property::get_attenuation_coefficient_from_cross_section(molecular_absorption_cross_section, molar_density_as_solid/si::molecule);
                return property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(attenuation_coefficient_as_solid, refractive_index_as_solid, 1.0, wavelength);
            },
            guess.solids[i].density,
            guess.solids[i].refractive_index,
            guess.molecular_absorption_cross_section
        );
        */

        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i] = compound::phase::infer(guess.solids[i]);
        }
        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            for (std::size_t j = 0; j < guess.solids.size(); j++)
            {
                if (i!=j)
                {
                    guess.solids[j] = guess.solids[j].value_or(guess.solids[i]);
                }
            }
        }
        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i] = compound::phase::infer(guess.solids[i]);
        }

        return guess;
    }




    /*
    Return a `PartlyKnownCompound` that has the properties of `known` where present, 
    in addition to properties that can derived from the properties of `known`. The function is idempotent.
    The function can be thought of as the traversal of the category defined within the `property` namespace.
    */
    PartlyKnownCompound speculate(const PartlyKnownCompound& known){
        PartlyKnownCompound guess = known;
        int A = known.atoms_per_molecule;

        // StateParameterSamples samples(guess);

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

        for (std::size_t i = 0; i < guess.solids.size(); i++)
        {
            guess.solids[i].dynamic_viscosity = guess.solids[i].dynamic_viscosity.value_or( 
                [](si::dynamic_viscosity dynamic_viscosity_as_liquid){ 
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


}
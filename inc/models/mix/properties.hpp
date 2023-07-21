#pragma once

// in-house libraries
#include "rules.hpp"
#include "fractions.hpp"

namespace mix{


    template<typename TProperty>
    si::molar_mass<double> molar_mass(const std::vector<si::molar_mass<double>>& molar_masses, const Tproperty& molar_property){
        return linear_rule(molar_masses, molar_fractions(molar_property));
    }

    template<typename TProperty>
    si::length<double> molecular_diameter(const std::vector<si::length<double>>& molecular_diameter, const Tproperty& molar_property){
        return linear_rule(molecular_diameter, molar_fractions(molar_property));
    }

    // "Amgat's Law"
    template<typename TProperty>
    si::molar_volume<double> molar_volume_as_liquid(const std::vector<si::molar_volume<double>>& volumes, const Tproperty& molar_property){
        return linear_rule(volumes, molar_fractions(molar_property));
    }



    // "Kay's Rule"
    template<typename TProperty>
    si::temperature<double> critical_temperature(const std::vector<si::temperature<double>>& critical_point_temperatures, const Tproperty& molar_property){
        return linear_rule(critical_point_temperatures, molar_fractions(molar_property));
    }

    // mole weighted arithmetic mean
    template<typename TProperty>
    double critical_compressibility(const std::vector<double>& critical_point_compressibilities, const Tproperty& molar_property){
        return linear_rule(critical_point_compressibilities, molar_fractions(molar_property));
    }

    // Yorizane
    double critical_compressibility(const double acentric_factor){
        return 0.291 - 0.08*acentric_factor;
    }

    // "Amgat's Law"
    template<typename TProperty>
    si::molar_volume<double> critical_volume(const std::vector<si::molar_volume<double>>& volumes, const Tproperty& molar_property){
        return linear_rule(volumes, molar_fractions(molar_property));
    }

    // "Prausnitz-Gunn"
    template<typename TProperty>
    si::pressure<double> critical_pressure(const si::temperature<double> critical_temperature, const si::molar_volume<double> critical_volume, const double critical_compressibility){
        return si::universal_gas_constant * critical_temperature * critical_compressibility / critical_volume;
    }



    // "Amgat's Law"
    template<typename TProperty>
    double density_as_gas(
        const si::molar_mass<double>  molar_mass,
        const si::pressure<double>    critical_pressure,
        const si::temperature<double> critical_temperature,
        const double                  critical_compressibility
    ){
        return GasDensityStateRelation(
            molar_mass,
            critical_pressure,
            critical_temperature,
            critical_compressibility
        );
    }

    // mass weighted arithmetic mean for specific volume
    template<typename TProperty>
    auto density_as_liquid(const std::vector<LiquidDensityTemperatureRelation>& densities, const Tproperty& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::density<double>>(
            [=](si::temperature<double> T) { 
                si::specific_volume<double> specific_volume(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                    specific_volume += fractions[i] / densities[i];
                }
                return 1.0 / specific_volume;
            }
        );
    }

    // mass weighted arithmetic mean for specific volume
    template<typename TProperty>
    auto density_as_solid(const std::vector<SolidDensityTemperatureRelation>& densities, const Tproperty& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::density<double>>(
            [=](si::temperature<double> T) { 
                si::specific_volume<double> specific_volume(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                    specific_volume += fractions[i] / densities[i];
                }
                return 1.0 / specific_volume;
            }
        );
    }



    // mole weighted arithmetic mean
    template<typename TProperty>
    double acentric_factor(const std::vector<double>& acentric_factors, const Tproperty& molar_property){
        return linear_rule(acentric_factors, molar_fractions(molar_property));
    }



    // mass weighted arithmetic mean
    template<typename TProperty>
    si::specific_energy<double> latent_heat_of_vaporization(const std::vector<si::specific_energy<double>>& latent_heat_of_vaporization, const Tproperty& mass_property){
        return linear_rule(latent_heat_of_vaporization, mass_fractions(mass_property));
    }

    // mass weighted arithmetic mean
    template<typename TProperty>
    si::specific_energy<double> latent_heat_of_fusion(const std::vector<si::specific_energy<double>>& latent_heat_of_fusion, const Tproperty& mass_property){
        return linear_rule(latent_heat_of_fusion, mass_fractions(mass_property));
    }

    // mass weighted arithmetic mean
    template<typename TProperty>
    si::specific_energy<double> latent_heat_of_sublimation(const std::vector<si::specific_energy<double>>& latent_heat_of_sublimation, const Tproperty& mass_property){
        return linear_rule(latent_heat_of_sublimation, mass_fractions(mass_property));
    }



    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_gas(const std::vector<GasHeatCapacityStateRelation>& specific_heat_capacities_as_gas, const Tproperty& mass_property){
        return linear_rule(specific_heat_capacities_as_gas, mass_fractions(mass_property));
    }

    // "Kopp's Law"
    template<typename TProperty>
    double molar_heat_capacity_as_gas(const std::vector<GasPropertyStateRelation<si::molar_heat_capacity<double>>>& molar_heat_capacities_as_gas, const Tproperty& molar_property){
        return linear_rule(specific_heat_capacities_as_gas, molar_fractions(molar_property));
    }

    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_liquid(const std::vector<LiquidHeatCapacityTemperatureRelation>& specific_heat_capacities_as_liquid, const Tproperty& mass_property){
        return linear_rule(specific_heat_capacities_as_liquid, mass_fractions(mass_property));
    }

    // "Kopp's Law"
    template<typename TProperty>
    double molar_heat_capacity_as_liquid(const std::vector<LiquidMolarHeatCapacityTemperatureRelation>& molar_heat_capacities_as_liquid, const Tproperty& molar_property){
        return linear_rule(specific_heat_capacities_as_liquid, molar_fractions(molar_property));
    }

    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_solid(const std::vector<SolidHeatCapacityTemperatureRelation>& specific_heat_capacities_as_solid, const Tproperty& mass_property){
        return linear_rule(specific_heat_capacities_as_solid, mass_fractions(mass_property));
    }

    // "Kopp's Law"
    template<typename TProperty>
    auto molar_heat_capacity_as_solid(const std::vector<SolidMolarHeatCapacityTemperatureRelation>& molar_heat_capacities_as_solid, const Tproperty& molar_property){
        return linear_rule(specific_heat_capacities_as_solid, molar_fractions(molar_property));
    }



    // "Herning-Zipperer"
    template<typename TProperty>
    auto dynamic_viscosity_as_gas(const std::vector<GasDynamicViscosityStateRelation>& dynamic_viscosity_as_gas, const Tproperty& molar_property, const std::vector<si::molar_mass<double>>& molar_masses){
        std::vector<double> fractions = molar_fractions(molar_property);
        GasDynamicViscosityStateRelation numerator(0);
        double denominator(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            numerator   += fractions[i] * std::sqrt(molar_masses[i]/(si::kilogram/si::mole)) * dynamic_viscosity_as_gas[i];
            denominator += fractions[i] * std::sqrt(molar_masses[i]/(si::kilogram/si::mole));
        }
        return numerator/denominator;
    }

    // mass weigted logarithmic mean
    template<typename TProperty>
    auto dynamic_viscosity_as_liquid(const std::vector<LiquidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_liquid, const Tproperty& mass_property){
        return linear_rule(dynamic_viscosity_as_liquid, log(mass_fractions(molar_property)));
    }

    // mass weigted logarithmic mean
    template<typename TProperty>
    auto dynamic_viscosity_as_solid(const std::vector<SolidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_solid, const Tproperty& mass_property){
        return linear_rule(dynamic_viscosity_as_solid, log(mass_fractions(molar_property)));
    }



    // "Raoult's Law"
    template<typename TProperty>
    auto vapor_pressure_as_liquid(const std::vector<LiquidMolarHeatCapacityTemperatureRelation>& vapor_pressures_as_liquid, const Tproperty& molar_property){
        return linear_rule(specific_heat_capacities_as_solid, molar_fractions(molar_property));
    }

    // "Raoult's Law", see Goldfarb 2008 for sample of validity
    template<typename TProperty>
    auto vapor_pressure_as_solid(const std::vector<SolidMolarHeatCapacityTemperatureRelation>& vapor_pressures_as_solid, const Tproperty& molar_property){
        return linear_rule(vapor_pressures_as_solid, molar_fraction(volume_property));
    }



    // Winterfeld-Scriven-Davis
    template<typename TProperty>
    auto surface_tension_as_liquid(const std::vector<LiquidSurfaceTensionTemperatureRelation>& surface_tension_as_liquid, const Tproperty& molar_property, const std::vector<si::molar_volume<double>> molar_volumes){
        std::vector<double> fractions = molar_fractions(molar_property);
        auto total_molar_volume = molar_volume(molar_volumes, molar_property);
        auto total_molar_volume2 = total_molar_volume * total_molar_volume;
        return GenericRelation<si::temperature<double>, si::surface_tension<double>>(
            [=](si::temperature<double> T) { 
                auto sigma = surface_tension_as_liquid[0](T);
                si::surface_energy<double> result(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                for (std::size_t j=0; j<fractions.size(); j++) {
                    result += (fractions[i] * molar_volumes[i]) * (fractions[j] * molar_volumes[j]) * si::sqrt(surface_tension_as_liquid[i]*surface_tension_as_liquid[j]) / total_molar_volume2;
                }}
                return result;
            }
        );
    }



    // Wassiljewa-Herning-Zipperer
    template<typename TProperty>
    auto thermal_conductivity_as_gas(const std::vector<GasThermalConductivityStateRelation>& thermal_conductivity_as_gas, const Tproperty& mass_property, const std::vector<si::molar_mass<double>>& molar_masses){
        std::vector<double> fractions = mass_fractions(mass_property);
        GasThermalConductivityStateRelation numerator;
        for (std::size_t i=0; i<fractions.size(); i++) {
            double denominator(0);
            for (std::size_t j=0; j<fractions.size(); j++) {
                denominator += fractions[i] * std::sqrt(molar_masses[j]/molar_masses[i]);
            }
            numerator += fractions[i] * thermal_conductivity_as_gas[i];
        }
        return numerator / denominator;
    }

    // DIPPR 9H
    template<typename TProperty>
    auto thermal_conductivity_as_liquid(const std::vector<LiquidThermalConductivityTemperatureRelation>& thermal_conductivity_as_liquid, const Tproperty& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::thermal_conductivity<double>>(
            [=](si::temperature<double> T) { 
                auto lambda = thermal_conductivity_as_liquid[0](T);
                auto result += fractions[0] / (lambda*lambda);
                for (std::size_t i=1; i<fractions.size(); i++) {
                    lambda = thermal_conductivity_as_liquid[i](T);
                    result += fractions[i] / (lambda*lambda);
                }
                return std::sqrt(result);
            }
        );
    }

    // volume weighted parallel mean
    template<typename TProperty>
    auto thermal_conductivity_as_solid(const std::vector<SolidThermalConductivityTemperatureRelation>& thermal_conductivity_as_solid, const Tproperty& volume_property){
        return parallel_rule(thermal_conductivity_as_solid, volume_fractions(volume_property));
    }



    template<typename TProperty>
    auto tensile_modulus_as_solid(const std::vector<SolidTensileModulusTemperatureRelation>& tensile_modulus_as_solid, const Tproperty& volume_property){
        return parallel_rule(tensile_modulus_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto shear_modulus_as_solid(const std::vector<SolidShearModulusTemperatureRelation>& shear_modulus_as_solid, const Tproperty& volume_property){
        return parallel_rule(shear_modulus_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto bulk_modulus_as_solid(const std::vector<SolidShearModulusTemperatureRelation>& bulk_modulus_as_solid, const Tproperty& volume_property){
        return parallel_rule(bulk_modulus_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto pwave_modulus_as_solid(const std::vector<SolidPwaveModulusTemperatureRelation>& pwave_modulus_as_solid, const Tproperty& volume_property){
        return parallel_rule(pwave_modulus_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto lame_parameter_as_solid(const std::vector<SolidLameParameterTemperatureRelation>& lame_parameter_as_solid, const Tproperty& volume_property){
        return parallel_rule(lame_parameter_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto poisson_ratio_as_solid(const std::vector<SolidPoissonRatioTemperatureRelation>& poisson_ratio_as_solid, const Tproperty& volume_property){
        return parallel_rule(poisson_ratio_as_solid, volume_fractions(volume_property));
    }



    template<typename TProperty>
    auto tensile_yield_strength_as_solid(const std::vector<SolidTensileYieldStrengthTemperatureRelation>& tensile_yield_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(tensile_yield_strength_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto shear_yield_strength_as_solid(const std::vector<SolidShearYieldStrengthTemperatureRelation>& shear_yield_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(shear_yield_strength_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto compressive_yield_strength_as_solid(const std::vector<SolidCompressiveYieldStrengthTemperatureRelation>& compressive_yield_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(compressive_yield_strength_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto tensile_fracture_strength_as_solid(const std::vector<SolidTensileFractureStrengthTemperatureRelation>& tensile_fracture_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(tensile_fracture_strength_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto shear_fracture_strength_as_solid(const std::vector<SolidShearFractureStrengthTemperatureRelation>& shear_fracture_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(shear_fracture_strength_as_solid, volume_fractions(volume_property));
    }

    template<typename TProperty>
    auto compressive_fracture_strength_as_solid(const std::vector<SolidCompressiveFractureStrengthTemperatureRelation>& compressive_fracture_strength_as_solid, const Tproperty& volume_property){
        return parallel_rule(compressive_fracture_strength_as_solid, volume_fractions(volume_property));
    }



    // check these to make sure they match the signature style of the functions here:
    // const std::vector<SolidRefractiveIndexWavenumberRelation> refractive_index;
    // const std::vector<SolidExtinctionCoefficientWavenumberRelation> extinction_coefficient;
    // const std::vector<SolidAbsorptionCoefficientWavenumberRelation> absorption_coefficient;

}

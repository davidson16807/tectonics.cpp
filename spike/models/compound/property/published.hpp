#pragma once

// C libraries
#include <math.h>       // log10 

// in-house libraries
#include <units/si.hpp>  // estimate_compressibility_factor

#include "compressibility.hpp"  // estimate_compressibility_factor

namespace compound {
namespace property {

    /*
    NOTE: 
    `published.hpp` describes mappings within the "property" category which are published in peer reviewed literature. 
    More speculative correlations within the catch_objc are are included in a separate file, `speculative.hpp`,
    so that we can clearly delineatly what is or is not peer-reviewed,
    */

    // taken from https://eng.libretexts.org/Bookshelves/Chemical_Engineering/Book%3A_Distillation_Science_(Coleman)/03%3A_Critical_Properties_and_Acentric_Factor
    constexpr double get_critical_compressibility(const si::pressure<double> critical_pressure, const si::temperature<double> critical_temperature, const si::molar_volume<double> critical_volume)
    {
        return (critical_pressure * critical_volume / (si::universal_gas_constant * critical_temperature));
    }
    constexpr si::molar_volume<double> get_critical_volume(const si::pressure<double> critical_pressure, const si::temperature<double> critical_temperature, const double critical_compressibility)
    {
        return critical_compressibility * si::universal_gas_constant * critical_temperature / critical_pressure;
    }
    constexpr si::pressure<double> get_critical_pressure(const si::temperature<double> critical_temperature, const si::molar_volume<double> critical_volume, const double critical_compressibility)
    {
        return critical_compressibility * si::universal_gas_constant * critical_temperature / critical_volume;
    }
    constexpr si::temperature<double> get_critical_temperature(const si::pressure<double> critical_pressure, const si::molar_volume<double> critical_volume, const double critical_compressibility)
    {
        return critical_pressure * critical_volume/(critical_compressibility*si::universal_gas_constant);
    }
    // from definition of the acentric factor: https://en.wikipedia.org/wiki/Acentric_factor
    constexpr double get_acentric_factor(const si::pressure<double> liquid_saturated_vapor_pressure_at_reduced_temperature_of_0_7, const si::pressure<double> critical_pressure)
    {
        return -log10((liquid_saturated_vapor_pressure_at_reduced_temperature_of_0_7/critical_pressure)) - 1.0;
    }

    // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
    constexpr si::temperature<double> estimate_critical_temperature_from_klincewicz(const si::molar_mass<double> molar_mass, const si::temperature<double> normal_boiling_point)
    {
        double molar_mass_in_grams = (molar_mass / (si::gram/si::mole));
        double normal_boiling_point_in_kelvin = (normal_boiling_point / si::kelvin);
        double temperature_in_kelvin = 50.2 - 0.16 * molar_mass_in_grams + 1.41 * normal_boiling_point_in_kelvin;
        return temperature_in_kelvin * si::kelvin; 
    }
    // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
    constexpr si::pressure<double> estimate_critical_pressure_from_klincewicz(const si::molar_mass<double> molar_mass, const unsigned int atom_count)
    {
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double Y = 0.335 + 0.009 * molar_mass_in_grams + 0.019 * atom_count;
        double critical_pressure_in_bars = molar_mass_in_grams/Y;
        return critical_pressure_in_bars * si::bar;
    }
    // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
    constexpr si::molar_volume<double> estimate_critical_molar_volume_from_klincewicz(const si::molar_mass<double> molar_mass, const unsigned int atom_count)
    {
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double critical_molar_volume_in_cm3 = 20.1 + 0.88 * molar_mass_in_grams + 13.4 * atom_count;
        return critical_molar_volume_in_cm3 * (si::centimeter3/si::mole);
    }

    // Ihmels (2010)
    constexpr si::temperature<double> estimate_critical_temperature_from_ihmels(const si::pressure<double> critical_pressure, const si::molar_volume<double> critical_molar_volume)
    {
        double critical_volume_in_m3_per_mole = (critical_molar_volume / (si::meter3/si::mole));
        double critical_pressure_in_pascal = (critical_pressure / si::pascal);
        double critical_temperature_in_kelvin = (critical_pressure_in_pascal + 0.025f) * critical_volume_in_m3_per_mole / 2.215f;
        return critical_temperature_in_kelvin * si::kelvin;
    }
    // Ihmels (2010)
    constexpr si::molar_volume<double> estimate_critical_molar_volume_from_ihmels(const si::temperature<double> critical_temperature, const si::pressure<double> critical_pressure)
    {
        double critical_pressure_in_pascal = (critical_pressure / si::pascal);
        double critical_temperature_in_kelvin = (critical_temperature / si::kelvin);
        double critical_volume_in_m3_per_mole = critical_temperature_in_kelvin * 2.215 / (critical_pressure_in_pascal + 0.025f);
        return critical_volume_in_m3_per_mole * (si::meter3/si::mole);
    }
    // Ihmels (2010)
    constexpr si::pressure<double> estimate_critical_pressure_from_ihmels(const si::temperature<double> critical_temperature, const si::molar_volume<double> critical_molar_volume)
    {
        double critical_temperature_in_kelvin = (critical_temperature / si::kelvin);
        double critical_volume_in_m3_per_mole = (critical_molar_volume / (si::meter3/si::mole));
        double critical_pressure_in_pascal = 2.215 * critical_temperature_in_kelvin / critical_volume_in_m3_per_mole - 0.025f;
        return critical_pressure_in_pascal * si::pascal;
    }

    // Sheffy Johnson: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    constexpr si::thermal_conductivity<double> estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
        const si::molar_mass<double> molar_mass,
        const si::temperature<double> temperature,
        const si::temperature<double> normal_melting_point
    ){
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double temperature_in_kelvin = (temperature / si::kelvin);
        double standard_melting_point_in_kelvin = (normal_melting_point / si::kelvin);
        double liquid_thermal_conductivity_in_watts_per_meter_kelvin = 
            1.951 * (1.0 - 0.00126 * (temperature_in_kelvin - standard_melting_point_in_kelvin)) / 
                     (std::pow(standard_melting_point_in_kelvin, 0.216)*std::pow(molar_mass_in_grams, 0.3));
        return std::max(liquid_thermal_conductivity_in_watts_per_meter_kelvin, 0.01) * si::watt / (si::meter*si::kelvin); // We assume thermal conductivity must be positive and nonzero, so we cap it to the lowest order of magnitude ever observed
    }

    // Sato-Riedel method: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    constexpr si::thermal_conductivity<double> estimate_thermal_conductivity_as_liquid_from_sato_riedel(
        const si::molar_mass<double> molar_mass,  
        const si::temperature<double> temperature, 
        const si::temperature<double> normal_boiling_point, 
        const si::temperature<double> critical_temperature 
    ){
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double liquid_thermal_conductivity_in_watts_per_meter_kelvin = 
            (1.1053 / sqrt(molar_mass_in_grams)) * (3 + 20.0 * pow(1.0 - (         temperature / critical_temperature), 2.0/3.0)) / 
                                                    (3 + 20.0 * pow(1.0 - (normal_boiling_point / critical_temperature), 2.0/3.0));
        return liquid_thermal_conductivity_in_watts_per_meter_kelvin * si::watt / (si::meter*si::kelvin);
    }

    constexpr si::molar_volume<double> get_molar_volume_as_ideal_gas(const si::temperature<double> temperature, const si::pressure<double> pressure)
    {
        return si::universal_gas_constant * temperature / pressure;
    }

    constexpr si::specific_heat_capacity<double> get_constant_pressure_heat_capacity_as_gas(
        si::molar_mass<double> molar_mass,
        double degrees_of_freedom
    ){
        return degrees_of_freedom * (si::universal_gas_constant / molar_mass);   
    }

    // Rowlinson-Poling: https://chemicals.readthedocs.io/chemicals.heat_capacity.html?highlight=rowlinson#chemicals.heat_capacity.Rowlinson_Poling
    constexpr si::molar_heat_capacity<double> estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(
        const si::temperature<double> critical_temperature,
        const si::temperature<double> temperature,
        const double accentric_factor,
        const si::molar_heat_capacity<double> constant_pressure_heat_capacity_as_gas
    ){
        double reduced_temperature = (temperature / critical_temperature);
        double heat_capacity_phase_difference_versus_gas_constant = 1.586 + 
            0.49f/(1.0-reduced_temperature) + 
            accentric_factor * (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
        return heat_capacity_phase_difference_versus_gas_constant * si::universal_gas_constant + constant_pressure_heat_capacity_as_gas;
    }

    // Rowlinson-Poling: https://chemicals.readthedocs.io/chemicals.heat_capacity.html?highlight=rowlinson#chemicals.heat_capacity.Rowlinson_Poling
    constexpr si::molar_heat_capacity<double> estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(
        const si::temperature<double> critical_temperature,
        const si::temperature<double> temperature,
        const double accentric_factor,
        const si::molar_heat_capacity<double> constant_pressure_heat_capacity_as_liquid
    ){
        double reduced_temperature = (temperature / critical_temperature);
        double heat_capacity_phase_difference_versus_gas_constant = 1.586 + 
            0.49f/(1.0-reduced_temperature) + 
            accentric_factor * (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
        return constant_pressure_heat_capacity_as_liquid - heat_capacity_phase_difference_versus_gas_constant * si::universal_gas_constant;
    }

    constexpr double estimate_acentric_factor_from_rowlinson_poling(
        const si::temperature<double> critical_temperature,
        const si::temperature<double> temperature,
        const si::molar_heat_capacity<double> constant_pressure_heat_capacity_as_liquid,
        const si::molar_heat_capacity<double> constant_pressure_heat_capacity_as_gas
    ){
        double reduced_temperature = (temperature / critical_temperature);
        double heat_capacity_phase_difference_versus_gas_constant = ((constant_pressure_heat_capacity_as_liquid - constant_pressure_heat_capacity_as_gas) / si::universal_gas_constant);
        return (heat_capacity_phase_difference_versus_gas_constant - 1.586 - 0.49f/(1.0-reduced_temperature)) / (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
    }

    // Pitzer model: https://chemicals.readthedocs.io/chemicals.phase_change.html#heat-of-vaporization-at-tb-correlations
    constexpr double estimate_accentric_factor_from_pitzer(
        const si::specific_energy<double> latent_heat_of_vaporization,
        const si::molar_mass<double> molar_mass,  
        const si::temperature<double> temperature,
        const si::temperature<double> critical_temperature
    ){
        double reduced_temperature = (temperature / critical_temperature);
        return ((latent_heat_of_vaporization * molar_mass / (si::universal_gas_constant * critical_temperature)) - 7.08 * pow(1.0 - reduced_temperature, 0.354)) / (10.95 * pow(1.0 - reduced_temperature, 0.456));
    }
    // Pitzer model: https://chemicals.readthedocs.io/chemicals.phase_change.html#heat-of-vaporization-at-tb-correlations
    constexpr si::specific_energy<double> estimate_latent_heat_of_vaporization_from_pitzer(
        const double accentric_factor,
        const si::molar_mass<double> molar_mass,  
        const si::temperature<double> temperature,
        const si::temperature<double> critical_temperature
    ){
        double reduced_temperature = (temperature / critical_temperature);
        return (7.08 * pow(1.0 - reduced_temperature, 0.354) + 10.95 * accentric_factor * pow(1.0 - reduced_temperature, 0.456)) * si::universal_gas_constant * critical_temperature / molar_mass;
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    constexpr si::dynamic_viscosity<double> estimate_viscosity_as_liquid_from_letsou_stiel(
        const double acentric_factor,
        const si::molar_mass<double> molar_mass,  
        const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double reduced_temperature = (temperature / critical_temperature);
        double critical_temperature_in_kelvin = (critical_temperature / si::kelvin);
        double critical_pressure_in_pascal =  (critical_pressure / si::pascal);
        double eta_zeta_0 = (1.5174 - 2.135 * reduced_temperature + 0.75 * reduced_temperature * reduced_temperature) * 1e-5;
        double eta_zeta_1 = (4.2552 - 7.674 * reduced_temperature + 3.40 * reduced_temperature * reduced_temperature) * 1e-5;
        double zeta = 2173.424 * pow(critical_temperature_in_kelvin, 1.0/6.0)  /  (sqrt(molar_mass_in_grams) * pow(critical_pressure_in_pascal, 2.0/3.0));
        double eta_zeta = eta_zeta_0 + acentric_factor * eta_zeta_1;
        double eta = eta_zeta / zeta;
        return std::max(eta, 1e-6) * si::pascal * si::second; // clamped not to fall below the lowest viscosity reported of any compound, helium
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    constexpr double estimate_acentric_factor_from_letsou_stiel(
        const si::dynamic_viscosity<double> viscosity_as_liquid,
        const si::molar_mass<double> molar_mass,  
        const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        double molar_mass_in_grams = (molar_mass / (si::gram / si::mole));
        double reduced_temperature = (temperature / critical_temperature);
        double critical_temperature_in_kelvin = (critical_temperature / si::kelvin);
        double critical_pressure_in_atm =  (critical_pressure / si::pascal);
        double eta_zeta_0 = (1.5174 - 2.135 * reduced_temperature + 0.75 * reduced_temperature * reduced_temperature) * 1e-5;
        double eta_zeta_1 = (4.2552 - 7.674 * reduced_temperature + 3.40 * reduced_temperature * reduced_temperature) * 1e-5;
        double zeta = 2173.424 * pow(critical_temperature_in_kelvin, 1.0/6.0)  /  (sqrt(molar_mass_in_grams) * pow(critical_pressure_in_atm, 2.0/3.0));
        return ((viscosity_as_liquid * zeta / (si::pascal * si::second)) - eta_zeta_0) / eta_zeta_1;
    }

    constexpr si::length<double> estimate_molecular_diameter_from_tee_gotoh_steward(
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        double critical_pressure_in_atm = (critical_pressure / si::standard_pressure);
        double critical_temperature_in_kelvin = (critical_temperature / si::kelvin);
        double molecular_diameter_in_angstrom = 2.3647 * pow(critical_temperature_in_kelvin / critical_pressure_in_atm, 1.0/3.0);
        return molecular_diameter_in_angstrom * si::angstrom;
    }
    
    

    namespace {
        /*
        NOTE: 
        this namespace describes subfunctions of the Lee-Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
        */
        constexpr double f0(const double reduced_temperature) {
            return 5.92714 - 6.09648f/reduced_temperature - 1.28862f*log(reduced_temperature) + 0.169347f*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature;
        }
        constexpr double f1(const double reduced_temperature) {
            return 15.2518 - 15.6875f/reduced_temperature - 13.4721f*log(reduced_temperature) + 0.43577 *reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature;
        }
    }

    // Lee Kesler method (1975): https://chemicals.readthedocs.io/chemicals.acentric.html
    constexpr double estimate_accentric_factor_from_lee_kesler(
        const si::pressure<double> liquid_saturated_vapor_pressure, 
        const si::temperature<double> normal_melting_point_temperature, 
        const si::temperature<double> critical_temperature,
        const si::temperature<double> temperature, 
        const si::pressure<double> critical_pressure
    ){
        double reduced_pressure    = (liquid_saturated_vapor_pressure / critical_pressure);
        double reduced_temperature = (temperature / critical_temperature);
        return (log(reduced_pressure) - f0(reduced_temperature)) / f1(reduced_temperature);
    }
    // Lee Kesler method (1975): https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
    constexpr double estimate_accentric_factor_from_lee_kesler(
        const si::pressure<double> liquid_saturated_vapor_pressure, 
        const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature,
        const si::pressure<double> critical_pressure
    ){
        double reduced_pressure    = (liquid_saturated_vapor_pressure / critical_pressure);
        double reduced_temperature = (temperature / critical_temperature);
        return (log(reduced_pressure) - f0(reduced_temperature)) / f1(reduced_temperature);
    }
    // Lee Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
    constexpr si::pressure<double> estimate_vapor_pressure_as_liquid_from_lee_kesler(
        const double acentric_factor, 
        const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        double reduced_temperature = (temperature / critical_temperature);
        double reduced_pressure    = exp(acentric_factor * f1(reduced_temperature) + f0(reduced_temperature));
        return reduced_pressure * critical_pressure;
    }

    // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/chemicals.lennard_jones.html#molecular-diameter-correlations
    constexpr si::length<double> estimate_molecular_diameter_from_bird_steward_lightfoot_1(const si::molar_volume<double> critical_molar_volume)
    {
        double critical_molar_volume_in_meter3_per_mole = (critical_molar_volume / (si::milliliter/si::mole));
        double molecular_diameter_in_angstrom = 0.841 * pow(critical_molar_volume_in_meter3_per_mole, 1.0/3.0);
        return molecular_diameter_in_angstrom * si::angstrom;
    }

    // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/chemicals.lennard_jones.html#molecular-diameter-correlations
    constexpr si::molar_volume<double> estimate_critical_molar_volume_from_bird_steward_lightfoot_1(const si::length<double> molecular_diameter)
    {
        double molecular_diameter_in_angstrom = (molecular_diameter / si::angstrom);
        double critical_molar_volume_in_meter3_per_mole = pow(molecular_diameter_in_angstrom / 0.841f, 3.0);
        return critical_molar_volume_in_meter3_per_mole * (si::milliliter/si::mole);
    }

    // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/chemicals.lennard_jones.html#molecular-diameter-correlations
    constexpr si::length<double> estimate_molecular_diameter_from_bird_steward_lightfoot_2(const si::molar_volume<double> liquid_molar_volume_at_melting_point)
    {
        double liquid_molar_volume_in_meter3_per_mole = (liquid_molar_volume_at_melting_point / (si::milliliter/si::mole));
        double molecular_diameter_in_angstrom = 1.222 * pow(liquid_molar_volume_in_meter3_per_mole, 1.0/3.0);
        return molecular_diameter_in_angstrom * si::angstrom;
    }

    // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/chemicals.lennard_jones.html#molecular-diameter-correlations
    constexpr si::molar_volume<double> estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(const si::length<double> molecular_diameter)
    {
        double molecular_diameter_in_angstrom = (molecular_diameter / si::angstrom);
        double liquid_molar_volume_in_meter3_per_mole = pow(molecular_diameter_in_angstrom / 1.222f, 3.0);
        return liquid_molar_volume_in_meter3_per_mole * (si::milliliter/si::mole);
    }

    // Goodman: https://chemicals.readthedocs.io/chemicals.volume.html#pure-solid-correlations
    constexpr si::molar_volume<double> estimate_molar_volume_as_solid_from_goodman(
        const si::molar_volume<double> liquid_molar_volume_at_melting_point, 
        const si::temperature<double> temperature_as_liquid, 
        const si::temperature<double> triple_point_temperature
    ){
        double temperature_in_kelvin = (temperature_as_liquid / si::kelvin);
        double triple_point_temperature_in_kelvin = (triple_point_temperature / si::kelvin);
        double liquid_molar_volume_in_meter3_per_mole = (liquid_molar_volume_at_melting_point / (si::meter3/si::mole));
        double solid_molar_volume_in_meter3_per_mole = liquid_molar_volume_in_meter3_per_mole / (1.28 - 0.16 * temperature_in_kelvin / triple_point_temperature_in_kelvin);
        return solid_molar_volume_in_meter3_per_mole * (si::meter3/si::mole);
    }

    // Goodman: https://chemicals.readthedocs.io/chemicals.volume.html#pure-solid-correlations
    constexpr si::molar_volume<double> estimate_molar_volume_as_liquid_from_goodman(
        const si::molar_volume<double> solid_molar_volume, 
        const si::temperature<double> temperature_as_liquid, 
        const si::temperature<double> triple_point_temperature
    ){
        double temperature_in_kelvin = (temperature_as_liquid / si::kelvin);
        double triple_point_temperature_in_kelvin = (triple_point_temperature / si::kelvin);
        double solid_molar_volume_in_meter3_per_mole = (solid_molar_volume / (si::meter3/si::mole));
        double liquid_molar_volume_in_meter3_per_mole = solid_molar_volume_in_meter3_per_mole * (1.28 - 0.16 * temperature_in_kelvin / triple_point_temperature_in_kelvin);
        return liquid_molar_volume_in_meter3_per_mole * (si::meter3/si::mole);
    }

    // Goodman: https://chemicals.readthedocs.io/chemicals.volume.html#pure-solid-correlations
    constexpr si::temperature<double> estimate_triple_point_temperature_from_goodman(
        const si::molar_volume<double> liquid_molar_volume_at_melting_point, 
        const si::molar_volume<double> solid_molar_volume, 
        const si::temperature<double> temperature_as_liquid 
    ){
        double temperature_in_kelvin = (temperature_as_liquid / si::kelvin);
        double solid_molar_volume_in_meter3_per_mole = (solid_molar_volume / (si::meter3/si::mole));
        double liquid_molar_volume_in_meter3_per_mole = (liquid_molar_volume_at_melting_point / (si::meter3/si::mole));
        double triple_point_temperature_in_kelvin = (-0.16 * temperature_in_kelvin) / (liquid_molar_volume_in_meter3_per_mole / solid_molar_volume_in_meter3_per_mole - 1.28);
        return triple_point_temperature_in_kelvin * si::kelvin;
    }

    // Rackett equation
    constexpr si::molar_volume<double> estimate_molar_volume_as_liquid_from_rackett(const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure, 
        const double critical_compressibility
    ){
        double reduced_temperature = (temperature / critical_temperature);
        return si::universal_gas_constant * critical_temperature / critical_pressure * pow(critical_compressibility, 1.0 + pow(1.0 - reduced_temperature, 2.0/7.0));
    }

    constexpr si::molar_volume<double> estimate_molar_volume_as_gas(
        const si::pressure<double> pressure, 
        const si::pressure<double> critical_pressure, 
        const si::temperature<double> temperature, 
        const si::temperature<double> critical_temperature, 
        const double critical_compressibility
    ){
        double compressibility = estimate_compressibility_factor(pressure, temperature, critical_pressure, critical_temperature, critical_compressibility);
        return compressibility * si::universal_gas_constant * temperature / pressure;
    }

    constexpr si::density<double> get_density_from_molar_volume(const si::molar_volume<double> molar_volume, const si::molar_mass<double> molar_mass)
    {
        return molar_mass/molar_volume;
    }

    // Eucken: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    constexpr si::thermal_conductivity<double> estimate_thermal_conductivity_as_gas_from_eucken(
        const si::dynamic_viscosity<double> viscosity_as_gas,
        const si::molar_mass<double> molar_mass,
        const si::specific_heat_capacity<double> constant_volume_specific_heat_capacity_as_gas
    ){
        si::molar_heat_capacity<double> constant_molar_volume_heat_capacity_as_gas = constant_volume_specific_heat_capacity_as_gas * molar_mass;
        return (1.0 + (9.0/4.0) / (constant_molar_volume_heat_capacity_as_gas/si::universal_gas_constant))
              * viscosity_as_gas * constant_molar_volume_heat_capacity_as_gas / molar_mass;
    }

    // Eucken: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    constexpr si::dynamic_viscosity<double> estimate_viscosity_as_gas_from_eucken(
        const si::specific_heat_capacity<double> constant_volume_specific_heat_capacity_as_gas,
        const si::molar_mass<double> molar_mass,
        const si::thermal_conductivity<double> thermal_conductivity_as_gas
    ){
        si::molar_heat_capacity<double> constant_molar_volume_heat_capacity_as_gas = constant_volume_specific_heat_capacity_as_gas * molar_mass;
        return thermal_conductivity_as_gas *molar_mass / ((1.0 + (9.0/4.0) / (constant_molar_volume_heat_capacity_as_gas/si::universal_gas_constant))*constant_molar_volume_heat_capacity_as_gas);
    }

    // Clapeyron: https://chemicals.readthedocs.io/chemicals.vapor_pressure.html#sublimation-pressure-estimation-correlations
    constexpr si::pressure<double> estimate_vapor_pressure_as_solid_from_clapeyron(
        const si::specific_energy<double> latent_heat_of_sublimation,
        const si::molar_mass<double> molar_mass,
        const si::temperature<double> temperature,
        const si::temperature<double> triple_point_temperature,
        const si::pressure<double> triple_point_pressure
    ){
        return triple_point_pressure * exp(-((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
    }

    constexpr si::specific_energy<double> estimate_latent_heat_of_sublimation_from_clapeyron(
        const si::pressure<double> vapor_pressure_as_solid,
        const si::molar_mass<double> molar_mass,
        const si::temperature<double> temperature,
        const si::temperature<double> triple_point_temperature,
        const si::pressure<double> triple_point_pressure
    ){

        return -si::universal_gas_constant * (vapor_pressure_as_solid / triple_point_pressure) / (1.0/temperature - 1.0/triple_point_temperature) / molar_mass;
    }
}}
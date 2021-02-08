#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    struct CompletedSolid
    {
        field::CompletedStateField<si::specific_heat_capacity> specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::CompletedStateField<si::density> density;
        field::CompletedStateField<si::pressure> vapor_pressure;
        field::CompletedSpectralField<double> refractive_index;
        field::CompletedSpectralField<double> spectral_reflectance;

        field::CompletedSpectralField<si::pressure> bulk_modulus;     // K   bulk modulus
        field::CompletedSpectralField<si::pressure> tensile_modulus;  // E   tensile Modulus, Young's modulus, modulus of elasticity
        field::CompletedSpectralField<si::pressure> shear_modulus;    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
        field::CompletedSpectralField<si::pressure> pwave_modulus;    // M   P-wave modulus
        field::CompletedSpectralField<si::pressure> lame_parameter;   // λ   Lamé's first parameter
        field::CompletedSpectralField<double> poisson_ratio;          // ν   Poisson's ratio
        
        field::CompletedStateField<si::pressure> compressive_fracture_strength;
        field::CompletedStateField<si::pressure> tensile_fracture_strength;
        field::CompletedStateField<si::pressure> shear_fracture_strength;
        field::CompletedStateField<si::pressure> compressive_yield_strength;
        field::CompletedStateField<si::pressure> tensile_yield_strength;
        field::CompletedStateField<si::pressure> shear_yield_strength;
        
        double chemical_susceptibility_estimate;

        CompletedSolid(
            const field::CompletedStateField<si::specific_heat_capacity> specific_heat_capacity,
            const field::CompletedStateField<si::thermal_conductivity> thermal_conductivity,
            const field::CompletedStateField<si::dynamic_viscosity> dynamic_viscosity,
            const field::CompletedStateField<si::density> density,
            const field::CompletedStateField<si::pressure> vapor_pressure,
            const field::CompletedSpectralField<double> refractive_index,
            const field::CompletedSpectralField<double> spectral_reflectance,

            const field::CompletedSpectralField<si::pressure> bulk_modulus,     // K   bulk modulus
            const field::CompletedSpectralField<si::pressure> tensile_modulus,  // E   tensile Modulus, Young's modulus, modulus of elasticity
            const field::CompletedSpectralField<si::pressure> shear_modulus,    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
            const field::CompletedSpectralField<si::pressure> pwave_modulus,    // M   P-wave modulus
            const field::CompletedSpectralField<si::pressure> lame_parameter,   // λ   Lamé's first parameter
            const field::CompletedSpectralField<double> poisson_ratio,          // ν   Poisson's ratio
            
            const field::CompletedStateField<si::pressure> compressive_fracture_strength,
            const field::CompletedStateField<si::pressure> tensile_fracture_strength,
            const field::CompletedStateField<si::pressure> shear_fracture_strength,
            const field::CompletedStateField<si::pressure> compressive_yield_strength,
            const field::CompletedStateField<si::pressure> tensile_yield_strength,
            const field::CompletedStateField<si::pressure> shear_yield_strength,
            
            const double chemical_susceptibility_estimate
        ):
            specific_heat_capacity(specific_heat_capacity),
            thermal_conductivity(thermal_conductivity),
            dynamic_viscosity(dynamic_viscosity),
            density(density),
            vapor_pressure(vapor_pressure),
            refractive_index(refractive_index),
            spectral_reflectance(spectral_reflectance),

            bulk_modulus(bulk_modulus),     // K   bulk modulus
            tensile_modulus(tensile_modulus),  // E   tensile Modulus, Young's modulus, modulus of elasticity
            shear_modulus(shear_modulus),    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
            pwave_modulus(pwave_modulus),    // M   P-wave modulus
            lame_parameter(lame_parameter),   // λ   Lamé's first parameter
            poisson_ratio(poisson_ratio),          // ν   Poisson's ratio
            
            compressive_fracture_strength(compressive_fracture_strength),
            tensile_fracture_strength(tensile_fracture_strength),
            shear_fracture_strength(shear_fracture_strength),
            compressive_yield_strength(compressive_yield_strength),
            tensile_yield_strength(tensile_yield_strength),
            shear_yield_strength(shear_yield_strength),
            
            chemical_susceptibility_estimate(chemical_susceptibility_estimate)
        {

        }

    };
}}
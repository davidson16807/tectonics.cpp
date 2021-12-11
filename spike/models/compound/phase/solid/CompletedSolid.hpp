#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    struct CompletedSolid
    {
        field::CompletedStateField<si::specific_heat_capacity> isobaric_specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::CompletedStateField<si::density> density;
        field::CompletedStateField<si::pressure> vapor_pressure;
        field::CompletedSpectralField<double> refractive_index;
        field::CompletedSpectralField<double> extinction_coefficient;
        field::CompletedSpectralField<si::attenuation> absorption_coefficient;

        field::CompletedStateField<si::pressure> bulk_modulus;     // K   bulk modulus
        field::CompletedStateField<si::pressure> tensile_modulus;  // E   tensile Modulus, Young's modulus, modulus of elasticity
        field::CompletedStateField<si::pressure> shear_modulus;    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
        field::CompletedStateField<si::pressure> pwave_modulus;    // M   P-wave modulus
        field::CompletedStateField<si::pressure> lame_parameter;   // λ   Lamé's first parameter
        field::CompletedStateField<double> poisson_ratio;          // ν   Poisson's ratio
        
        field::CompletedStateField<si::pressure> compressive_fracture_strength;
        field::CompletedStateField<si::pressure> tensile_fracture_strength;
        field::CompletedStateField<si::pressure> shear_fracture_strength;
        field::CompletedStateField<si::pressure> compressive_yield_strength;
        field::CompletedStateField<si::pressure> tensile_yield_strength;
        field::CompletedStateField<si::pressure> shear_yield_strength;
        
        double chemical_susceptibility_estimate;


    };
}}
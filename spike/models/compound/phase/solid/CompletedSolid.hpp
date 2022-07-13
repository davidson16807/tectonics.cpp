#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

#include <models/compound/relation/PolynomialRailyardRelation.hpp>

namespace compound { 
namespace phase { 
    struct CompletedSolid
    {
        field::CompletedStateField<si::specific_heat_capacity<double>> isobaric_specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity<double>> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity<double>> dynamic_viscosity;
        field::CompletedStateField<si::density<double>> density;
        field::CompletedStateField<si::pressure<double>> vapor_pressure;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1> refractive_index;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1> extinction_coefficient;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3> absorption_coefficient;

        field::CompletedStateField<si::pressure<double>> bulk_modulus;     // K   bulk modulus
        field::CompletedStateField<si::pressure<double>> tensile_modulus;  // E   tensile Modulus, Young's modulus, modulus of elasticity
        field::CompletedStateField<si::pressure<double>> shear_modulus;    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
        field::CompletedStateField<si::pressure<double>> pwave_modulus;    // M   P-wave modulus
        field::CompletedStateField<si::pressure<double>> lame_parameter;   // λ   Lamé's first parameter
        field::CompletedStateField<double> poisson_ratio;          // ν   Poisson's ratio
        
        field::CompletedStateField<si::pressure<double>> compressive_fracture_strength;
        field::CompletedStateField<si::pressure<double>> tensile_fracture_strength;
        field::CompletedStateField<si::pressure<double>> shear_fracture_strength;
        field::CompletedStateField<si::pressure<double>> compressive_yield_strength;
        field::CompletedStateField<si::pressure<double>> tensile_yield_strength;
        field::CompletedStateField<si::pressure<double>> shear_yield_strength;
        
        double chemical_susceptibility_estimate;


    };
}}
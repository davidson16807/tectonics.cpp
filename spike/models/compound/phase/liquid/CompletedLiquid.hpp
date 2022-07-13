#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

#include <models/compound/relation/PolynomialRailyardRelation.hpp>

namespace compound { 
namespace phase { 
    struct CompletedLiquid
    {
        field::CompletedStateField<si::specific_heat_capacity<double>> isobaric_specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity<double>> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity<double>> dynamic_viscosity;
        field::CompletedStateField<si::density<double>> density;
        field::CompletedStateField<si::pressure<double>> vapor_pressure;
        field::CompletedStateField<si::surface_energy<double>> surface_tension;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1> refractive_index;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1> extinction_coefficient;
    };
}}
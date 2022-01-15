#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

#include <models/compound/relation/state/GasPropertyStateRelation.hpp>

namespace compound { 
namespace phase { 
    struct CompletedGas
    {
        relation::GasPropertyStateRelation<si::specific_heat_capacity> isobaric_specific_heat_capacity;
        relation::GasPropertyStateRelation<si::thermal_conductivity> thermal_conductivity;
        relation::GasPropertyStateRelation<si::dynamic_viscosity> dynamic_viscosity;
        field::CompletedStateField<si::density> density;
        field::CompletedSpectralField<double> refractive_index;
    };
}}
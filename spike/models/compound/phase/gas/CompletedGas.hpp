#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    struct CompletedGas
    {
        field::CompletedStateField<si::specific_heat_capacity> isobaric_specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::CompletedStateField<si::density> density;
        field::CompletedSpectralField<double> refractive_index;
    };
}}
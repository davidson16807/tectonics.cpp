#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    struct CompletedLiquid
    {
        field::CompletedStateField<si::specific_heat_capacity> specific_heat_capacity;
        field::CompletedStateField<si::thermal_conductivity> thermal_conductivity;
        field::CompletedStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::CompletedStateField<si::density> density;
        field::CompletedStateField<si::pressure> vapor_pressure;
        field::CompletedStateField<si::surface_energy> surface_tension;
        field::CompletedSpectralField<double> refractive_index;
    };
}}
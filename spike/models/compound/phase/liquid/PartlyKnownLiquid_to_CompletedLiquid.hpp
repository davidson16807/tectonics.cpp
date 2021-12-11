#pragma once

// in-house libraries
#include "PartlyKnownLiquid.hpp"
#include "CompletedLiquid.hpp"

#include <models/compound/field/state/OptionalStateField_to_CompletedStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    /*
    Populate the missing `*_function` attributes with functions returning representative values if they are available.
    The function is idempotent.
    */
    CompletedLiquid complete(const PartlyKnownLiquid& known, const CompletedLiquid& fallback)
    {
        return CompletedLiquid{
            field::complete(known.isobaric_specific_heat_capacity,  fallback.isobaric_specific_heat_capacity  ),
            field::complete(known.thermal_conductivity,             fallback.thermal_conductivity             ),
            field::complete(known.dynamic_viscosity,                fallback.dynamic_viscosity                ),
            field::complete(known.density,                          fallback.density                          ),
            field::complete(known.vapor_pressure,                   fallback.vapor_pressure                   ),
            field::complete(known.surface_tension,                  fallback.surface_tension                  ),
            field::complete(known.refractive_index,                 fallback.refractive_index                 ),
            field::complete(known.extinction_coefficient,           fallback.extinction_coefficient           )
        };
    }
}}
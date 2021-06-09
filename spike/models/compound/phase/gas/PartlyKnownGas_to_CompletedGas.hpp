#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField_to_CompletedStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_CompletedSpectralField.hpp>

#include "PartlyKnownGas.hpp"
#include "CompletedGas.hpp"


namespace compound { 
namespace phase { 
    /*
    Populate the missing `*_function` attributes with functions returning representative values if they are available.
    The function is idempotent.
    */
    CompletedGas complete(const PartlyKnownGas& known, const CompletedGas& fallback)
    {
        return CompletedGas(
            field::complete(known.specific_heat_capacity,           fallback.specific_heat_capacity           ),
            field::complete(known.thermal_conductivity,             fallback.thermal_conductivity             ),
            field::complete(known.dynamic_viscosity,                fallback.dynamic_viscosity                ),
            field::complete(known.density,                          fallback.density                          ),
            field::complete(known.refractive_index,                 fallback.refractive_index                 )
        );
    }
}}
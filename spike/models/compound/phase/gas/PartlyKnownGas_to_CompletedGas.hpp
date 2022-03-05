#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField_to_CompletedStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_CompletedSpectralField.hpp>

#include <models/compound/relation/OptionalStateRelation.hpp>

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
        return CompletedGas{
            known.isobaric_specific_heat_capacity .complete(fallback.isobaric_specific_heat_capacity  ),
            known.thermal_conductivity            .complete(fallback.thermal_conductivity             ),
            known.dynamic_viscosity               .complete(fallback.dynamic_viscosity                ),
            field::complete(known.density,                          fallback.density                          ),
            known.refractive_index                .complete(fallback.refractive_index                 )
        };
    }
}}
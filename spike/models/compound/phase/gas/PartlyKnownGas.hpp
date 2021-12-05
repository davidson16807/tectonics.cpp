#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownGas
    {
        field::OptionalStateField<si::specific_heat_capacity> specific_heat_capacity;
        field::OptionalStateField<si::thermal_conductivity> thermal_conductivity;
        field::OptionalStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        field::OptionalSpectralField<double> refractive_index;

        /*
        Return a `PartlyKnownGas` that has the properties of `known` where present, otherwise substitute with properties of `base`
        */
        PartlyKnownGas value_or(const PartlyKnownGas& fallback)  const {
            // copy what you do know
            PartlyKnownGas guess = *this;

            guess.specific_heat_capacity = specific_heat_capacity .value_or(fallback.specific_heat_capacity);
            guess.thermal_conductivity   = thermal_conductivity   .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity      = dynamic_viscosity      .value_or(fallback.dynamic_viscosity);
            guess.density                = density                .value_or(fallback.density);
            guess.refractive_index       = refractive_index       .value_or(fallback.refractive_index);

            return guess;
        }

    };
    /*
    Synonymous to `value_or()`, analogous with similar functions for CompletedGas
    */
    PartlyKnownGas value_or(const PartlyKnownGas& known, const PartlyKnownGas& fallback)
    {
        return known.value_or(fallback);
    }
}}
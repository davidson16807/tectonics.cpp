#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownLiquid
    {
        field::OptionalStateField<si::specific_heat_capacity> specific_heat_capacity;
        field::OptionalStateField<si::thermal_conductivity> thermal_conductivity;
        field::OptionalStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        field::OptionalStateField<si::pressure> vapor_pressure;
        field::OptionalStateField<si::surface_energy> surface_tension;
        field::OptionalSpectralField<double> refractive_index;

        /*
        Return a `PartlyKnownLiquid` that has the properties of `known` where present, otherwise substitute with properties of `base`
        */
        PartlyKnownLiquid value_or(const PartlyKnownLiquid& fallback)  const {
            // copy what you do know
            PartlyKnownLiquid guess = *this;

            guess.specific_heat_capacity = specific_heat_capacity .value_or(fallback.specific_heat_capacity);
            guess.vapor_pressure         = vapor_pressure         .value_or(fallback.vapor_pressure);
            guess.surface_tension        = surface_tension        .value_or(fallback.surface_tension);
            guess.thermal_conductivity   = thermal_conductivity   .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity      = dynamic_viscosity      .value_or(fallback.dynamic_viscosity);
            guess.density                = density                .value_or(fallback.density);
            guess.refractive_index       = refractive_index       .value_or(fallback.refractive_index);

            return guess;
        }

        /*
        Return a `PartlyKnownLiquid` that for each property copies whichever observation offers the most information from the two arguments available.
        If arguments offer different observations with the same amount of information, default to the observation from the first argument.
        */
        PartlyKnownLiquid fuse(const PartlyKnownLiquid& other)  const {
            // copy what you do know
            PartlyKnownLiquid guess = *this;

            guess.specific_heat_capacity = specific_heat_capacity .compare(other.specific_heat_capacity);
            guess.vapor_pressure         = vapor_pressure         .compare(other.vapor_pressure);
            guess.surface_tension        = surface_tension        .compare(other.surface_tension);
            guess.thermal_conductivity   = thermal_conductivity   .compare(other.thermal_conductivity);
            guess.dynamic_viscosity      = dynamic_viscosity      .compare(other.dynamic_viscosity);
            guess.density                = density                .compare(other.density);
            guess.refractive_index       = refractive_index       .compare(other.refractive_index);

            return guess;
        }


    };
    /*
    Synonymous to `value_or()`, analogous with similar functions for CompletedLiquid
    */
    PartlyKnownLiquid value_or(const PartlyKnownLiquid& known, const PartlyKnownLiquid& fallback)
    {
        return known.value_or(fallback);
    }
}}
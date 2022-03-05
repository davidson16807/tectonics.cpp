#pragma once

// std libraries

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

#include <models/compound/relation/OptionalStateRelation.hpp>
#include <models/compound/relation/GasPropertyStateRelation.hpp>
#include <models/compound/relation/SplineRelation.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownGas
    {
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::specific_heat_capacity>> isobaric_specific_heat_capacity;
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::thermal_conductivity>> thermal_conductivity;
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::dynamic_viscosity>> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        relation::OptionalStateRelation<relation::SplineRelation<si::wavenumber,float,0,1>> refractive_index;

        /*
        Return a `PartlyKnownGas` that has the properties of `known` where present, otherwise substitute with properties of `base`
        */
        PartlyKnownGas value_or(const PartlyKnownGas& fallback)  const {
            // copy what you do know
            PartlyKnownGas guess = *this;

            guess.isobaric_specific_heat_capacity = isobaric_specific_heat_capacity .value_or(fallback.isobaric_specific_heat_capacity);
            guess.thermal_conductivity            = thermal_conductivity            .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity               = dynamic_viscosity               .value_or(fallback.dynamic_viscosity);
            guess.density                         = density                         .value_or(fallback.density);
            guess.refractive_index                = refractive_index                .value_or(fallback.refractive_index);

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
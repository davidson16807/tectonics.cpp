#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

#include <models/compound/relation/OptionalStateRelation.hpp>
#include <models/compound/relation/SplineRelation.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownLiquid
    {
        field::OptionalStateField<si::specific_heat_capacity> isobaric_specific_heat_capacity;
        field::OptionalStateField<si::thermal_conductivity> thermal_conductivity;
        field::OptionalStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        field::OptionalStateField<si::pressure> vapor_pressure;
        field::OptionalStateField<si::surface_energy> surface_tension;
        relation::OptionalStateRelation<relation::SplineRelation<si::wavenumber,float,0,1>> refractive_index;
        relation::OptionalStateRelation<relation::SplineRelation<si::wavenumber,float,0,1>> extinction_coefficient;

        /*
        Return a `PartlyKnownLiquid` that has the properties of `known` where present, otherwise substitute with properties of `base`
        */
        PartlyKnownLiquid value_or(const PartlyKnownLiquid& fallback)  const {
            // copy what you do know
            PartlyKnownLiquid guess = *this;

            guess.isobaric_specific_heat_capacity = isobaric_specific_heat_capacity .value_or(fallback.isobaric_specific_heat_capacity);
            guess.vapor_pressure                  = vapor_pressure                  .value_or(fallback.vapor_pressure);
            guess.surface_tension                 = surface_tension                 .value_or(fallback.surface_tension);
            guess.thermal_conductivity            = thermal_conductivity            .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity               = dynamic_viscosity               .value_or(fallback.dynamic_viscosity);
            guess.density                         = density                         .value_or(fallback.density);
            guess.refractive_index                = refractive_index                .value_or(fallback.refractive_index);
            guess.extinction_coefficient          = extinction_coefficient          .value_or(fallback.extinction_coefficient);

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
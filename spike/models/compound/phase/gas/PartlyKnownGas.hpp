#pragma once

// std libraries

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

#include <models/compound/relation/OptionalStateRelation.hpp>
#include <models/compound/relation/GasPropertyStateRelation.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownGas
    {
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::specific_heat_capacity>> isobaric_specific_heat_capacity;
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::thermal_conductivity>> thermal_conductivity;
        relation::OptionalStateRelation<relation::GasPropertyStateRelation<si::dynamic_viscosity>> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        relation::OptionalStateRelation<relation::PolynomialRailyardRelation<si::wavenumber,double,0,1>> refractive_index;

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
    /*
    `distance` is a distance metric that is designed to test whether two objects are equal 
    up to some user defined threshold based around the precision of underlying types.
    It calculates a distance metric for each attribute separately, 
    casting booleans and integers as floats, and returns the maximum distance calculated.
    */
    float distance(const PartlyKnownGas& first, const PartlyKnownGas& second)
    {
        float out(0.0f);
        field::StateParameters lo(0.01*si::standard_pressure, 3.0*si::kelvin);
        field::StateParameters hi(10.0*si::standard_pressure, 1000.0*si::kelvin);
        out = std::max(out, relation::distance(first.isobaric_specific_heat_capacity, second.isobaric_specific_heat_capacity, lo, hi));
        out = std::max(out, relation::distance(first.thermal_conductivity, second.thermal_conductivity, lo, hi));
        out = std::max(out, relation::distance(first.dynamic_viscosity, second.dynamic_viscosity, lo, hi));
        // out = std::max(out, relation::distance(first.density, second.density)); // don't care, fix it later
        out = std::max(out, relation::distance(first.refractive_index, second.refractive_index, 
            1.0/si::centimeter, 100e3/si::centimeter));
        return out;
    }
}}
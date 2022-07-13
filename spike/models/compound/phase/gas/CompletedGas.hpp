#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/field/state/CompletedStateField.hpp>
#include <models/compound/field/spectral/CompletedSpectralField.hpp>

#include <models/compound/relation/GasPropertyStateRelation.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>

namespace compound { 
namespace phase { 
    struct CompletedGas
    {
        relation::GasPropertyStateRelation<si::specific_heat_capacity<double>> isobaric_specific_heat_capacity;
        relation::GasPropertyStateRelation<si::thermal_conductivity<double>> thermal_conductivity;
        relation::GasPropertyStateRelation<si::dynamic_viscosity<double>> dynamic_viscosity;
        field::CompletedStateField<si::density<double>> density;
        relation::PolynomialRailyardRelation<si::wavenumber<double>,double,0,1> refractive_index;
    };

    /*
    `distance` is a distance metric that is designed to test whether two objects are equal 
    up to some user defined threshold based around the precision of underlying types.
    It calculates a distance metric for each attribute separately, 
    casting booleans and integers as floats, and returns the maximum distance calculated.
    */
    float distance(const CompletedGas& first, const CompletedGas& second)
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
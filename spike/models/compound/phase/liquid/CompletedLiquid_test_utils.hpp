#pragma once

#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>

#include "CompletedLiquid.hpp"

compound::phase::CompletedLiquid known_liquid_nitrogen {
    /*isobaric_specific_heat_capacity*/            2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
    /*thermal_conductivity*/              0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
    /*dynamic_viscosity*/                 157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
    /*density*/                           0807.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    
        compound::field::StateFunction<si::pressure<double>>([](si::pressure<double> p, si::temperature<double> T){ 
            return 3.720822*si::standard_pressure - (293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
        }), // Friedman (1950)
    /*surface_tension*/                   11.8000 * si::millinewton / si::meter,
    /*refractive_index*/                  1.19876,
    /*extinction_coefficient*/            0.0
};
compound::phase::CompletedLiquid known_dummy_liquid {
    /*isobaric_specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*surface_tension*/                   6.0 * si::newton / si::meter,
    /*refractive_index*/                  1.3,
    /*extinction_coefficient*/            0.3
};

namespace compound {
namespace phase {

    bool operator==(const CompletedLiquid& first, const CompletedLiquid& second)
    {
        return 
            first.isobaric_specific_heat_capacity== second.isobaric_specific_heat_capacity&&
            first.thermal_conductivity           == second.thermal_conductivity           &&
            first.dynamic_viscosity              == second.dynamic_viscosity              &&
            first.density                        == second.density                        &&
            first.vapor_pressure                 == second.vapor_pressure                 &&
            first.surface_tension                == second.surface_tension                &&
            relation::distance(first.refractive_index, second.refractive_index, 1.0/si::centimeter, 100e3/si::centimeter) < 1e-4 && 
            relation::distance(first.extinction_coefficient, second.extinction_coefficient, 1.0/si::centimeter, 100e3/si::centimeter) < 1e-4;
    }
    bool operator!=(const CompletedLiquid& first, const CompletedLiquid& second)
    {
        return !(first==second);
    }

}}
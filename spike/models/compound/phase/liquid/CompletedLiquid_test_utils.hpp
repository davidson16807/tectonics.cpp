#pragma once

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>
#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include "CompletedLiquid.hpp"

compound::phase::CompletedLiquid known_liquid_nitrogen (
    /*specific_heat_capacity*/            2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
    /*thermal_conductivity*/              0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
    /*dynamic_viscosity*/                 157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
    /*density*/                           0807.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    
        compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
            return 3.720822*si::standard_pressure - (293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
        }), // Friedman (1950)
    /*surface_tension*/                   11.8000 * si::millinewton / si::meter,
    /*refractive_index*/                  1.19876
);
compound::phase::CompletedLiquid known_dummy_liquid (
    /*specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*surface_tension*/                   6.0 * si::newton / si::meter,
    /*refractive_index*/                  1.3
);

bool operator==(const compound::phase::CompletedLiquid& first, const compound::phase::CompletedLiquid& second)
{
    return 
        first.specific_heat_capacity == second.specific_heat_capacity &&
        first.thermal_conductivity   == second.thermal_conductivity   &&
        first.dynamic_viscosity      == second.dynamic_viscosity      &&
        first.density                == second.density                &&
        first.vapor_pressure         == second.vapor_pressure         &&
        first.surface_tension        == second.surface_tension        &&
        first.refractive_index       == second.refractive_index;
}
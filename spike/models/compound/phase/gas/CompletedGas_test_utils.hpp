#pragma once

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>

#include <models/compound/field/state/StateParameters.hpp>

#include "CompletedGas.hpp"

compound::phase::CompletedGas known_steam {
    /*isobaric_specific_heat_capacity*/   2.080 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.016 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.24e-5 * si::pascal * si::second, // engineering toolbox, at 100 C
    /*density*/                           0.590 *  si::kilogram/si::meter3,
    /*refractive_index*/                  1.000261, // engineering toolbox
};
compound::phase::CompletedGas known_nitrogen {
    /*isobaric_specific_heat_capacity*/   1.040 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.0234 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.76e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
    /*density*/                           1.17 * si::kilogram/si::meter3,
    /*refractive_index*/                  1.0002990, // engineering toolbox
};
compound::phase::CompletedGas known_dummy_gas  {
    /*isobaric_specific_heat_capacity*/   1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*refractive_index*/                  1.3 // engineering toolbox
};

namespace compound {
namespace phase {

    bool operator==(const CompletedGas& first, const CompletedGas& second)
    {
        field::StateParameters lo(1.0*si::pascal, 3.0*si::kelvin);
        field::StateParameters hi(100000000.0*si::pascal, si::solar_temperature);
        return 
            relation::distance(first.isobaric_specific_heat_capacity, second.isobaric_specific_heat_capacity, lo, hi) < 1e-4 &&
            relation::distance(first.thermal_conductivity,            second.thermal_conductivity,            lo, hi) < 1e-4 &&
            relation::distance(first.dynamic_viscosity,               second.dynamic_viscosity,               lo, hi) < 1e-4 &&
            first.density == second.density &&
            relation::distance(first.refractive_index, second.refractive_index, 1.0/si::centimeter, 100e3/si::centimeter) < 1e-4; 
    }

    bool operator!=(const CompletedGas& first, const CompletedGas& second)
    {
        return !(first==second);
    }

}}
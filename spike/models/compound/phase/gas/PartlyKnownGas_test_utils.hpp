#pragma once

#include <units/si.hpp>

#include <models/compound/field/constant/OptionalConstantField_test_utils.hpp>
#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include <models/compound/relation/PolynomialRailyardRelation.hpp>

#include "PartlyKnownGas.hpp"

compound::phase::PartlyKnownGas unknown_gas  {
    /*isobaric_specific_heat_capacity*/   std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*refractive_index*/                  std::monostate()
};
compound::phase::PartlyKnownGas steam {
    /*isobaric_specific_heat_capacity*/   2.080 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.016 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.24e-5 * si::pascal * si::second, // engineering toolbox, at 100 C
    /*density*/                           std::monostate(),
    /*refractive_index*/                  1.000261f // engineering toolbox
};
compound::phase::PartlyKnownGas nitrogen_gas {
    /*isobaric_specific_heat_capacity*/   1.040 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.0234 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.76e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
    /*density*/                           std::monostate(),
    /*refractive_index*/                  1.0002990f
};
compound::phase::PartlyKnownGas dummy_gas  {
    /*isobaric_specific_heat_capacity*/   1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*refractive_index*/                  1.3f
};
compound::phase::PartlyKnownGas perflouromethane_gas {
    /*isobaric_specific_heat_capacity*/   std::monostate(),
    /*thermal_conductivity*/              16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*refractive_index*/                  1.0004823f
};

int PartlyKnownGas_attribute_known_count(const compound::phase::PartlyKnownGas& liquid)
{
    return
        liquid.isobaric_specific_heat_capacity .has_value() +
        liquid.thermal_conductivity            .has_value() +
        liquid.dynamic_viscosity               .has_value() +
        liquid.density                         .has_value() +
        liquid.refractive_index                .has_value();
}

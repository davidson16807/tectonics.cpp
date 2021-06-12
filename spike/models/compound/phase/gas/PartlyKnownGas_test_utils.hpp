#pragma once

#include <units/si.hpp>

#include <models/compound/field/constant/OptionalConstantField_test_utils.hpp>
#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include "PartlyKnownGas.hpp"

compound::phase::PartlyKnownGas unknown_gas  {
    /*specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*refractive_index*/                  std::monostate()
};
compound::phase::PartlyKnownGas steam {
    /*specific_heat_capacity*/            2.080 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.016 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.24e-5 * si::pascal * si::second, // engineering toolbox, at 100 C
    /*density*/                           std::monostate(),
    /*refractive_index*/                  1.000261, // engineering toolbox
};
compound::phase::PartlyKnownGas nitrogen_gas {
    /*specific_heat_capacity*/            1.040 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              0.0234 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1.76e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
    /*density*/                           std::monostate(),
    /*refractive_index*/                  //1.0002990,
        compound::field::SpectralFunction<double>([](const si::wavenumber nhi, const si::wavenumber nlo, const si::pressure p, const si::temperature T) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0 + 6.8552e-5 + 3.243157e-2 / (144.0 - invl2);
        })
};
compound::phase::PartlyKnownGas dummy_gas  {
    /*specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*refractive_index*/                  1.3
};
compound::phase::PartlyKnownGas perflouromethane_gas {
    /*specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*refractive_index*/                  1.0004823
};

int PartlyKnownGas_attribute_index_sum(const compound::phase::PartlyKnownGas& liquid)
{
    return
        liquid.specific_heat_capacity          .index() +
        liquid.thermal_conductivity            .index() +
        liquid.dynamic_viscosity               .index() +
        liquid.density                         .index() +
        liquid.refractive_index                .index();
}

int PartlyKnownGas_attribute_known_count(const compound::phase::PartlyKnownGas& liquid)
{
    return
        liquid.specific_heat_capacity          .has_value() +
        liquid.thermal_conductivity            .has_value() +
        liquid.dynamic_viscosity               .has_value() +
        liquid.density                         .has_value() +
        liquid.refractive_index                .has_value();
}

namespace compound{
namespace phase{
    bool operator==(const PartlyKnownGas& first, const PartlyKnownGas& second)
    {
        return 
            first.specific_heat_capacity == second.specific_heat_capacity &&
            first.thermal_conductivity   == second.thermal_conductivity   &&
            first.dynamic_viscosity      == second.dynamic_viscosity      &&
            first.density                == second.density                &&
            first.refractive_index       == second.refractive_index;
    }
    bool operator!=(const PartlyKnownGas& first, const PartlyKnownGas& second)
    {
        return !(first==second);
    }
}}
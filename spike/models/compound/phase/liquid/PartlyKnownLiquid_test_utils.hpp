#pragma once

#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include "PartlyKnownLiquid.hpp"

compound::phase::PartlyKnownLiquid unknown {
    /*specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  std::monostate()
};
compound::phase::PartlyKnownLiquid molten_silica {
    /*specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 exp(10.0) * si::poise, // Doremus (2002), at 1400 C
    /*density*/                           2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  std::monostate()
};
compound::phase::PartlyKnownLiquid liquid_nitrogen {
    /*specific_heat_capacity*/            2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
    /*thermal_conductivity*/              0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
    /*dynamic_viscosity*/                 157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
    /*density*/                           0807.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    
        compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
            return 3.720822*si::standard_pressure - (293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
        }), // Friedman (1950)
    /*refractive_index*/                  1.19876
};
compound::phase::PartlyKnownLiquid dummy {
    /*specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*refractive_index*/                  1.3
};

compound::phase::PartlyKnownLiquid liquid_ammonia {
    /*specific_heat_capacity*/            4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  1.3944
};

int PartlyKnownLiquid_attribute_index_sum(const compound::phase::PartlyKnownLiquid& liquid)
{
    return
        liquid.specific_heat_capacity          .index() +
        liquid.thermal_conductivity            .index() +
        liquid.dynamic_viscosity               .index() +
        liquid.density                         .index() +
        liquid.vapor_pressure                  .index() +
        liquid.refractive_index                .index();
}
int PartlyKnownLiquid_attribute_known_count(const compound::phase::PartlyKnownLiquid& liquid)
{
    return
        liquid.specific_heat_capacity          .has_value() +
        liquid.thermal_conductivity            .has_value() +
        liquid.dynamic_viscosity               .has_value() +
        liquid.density                         .has_value() +
        liquid.vapor_pressure                  .has_value() +
        liquid.refractive_index                .has_value();
}
bool operator==(const compound::phase::PartlyKnownLiquid& first, const compound::phase::PartlyKnownLiquid& second)
{
    return 
        first.specific_heat_capacity == second.specific_heat_capacity &&
        first.thermal_conductivity   == second.thermal_conductivity   &&
        first.dynamic_viscosity      == second.dynamic_viscosity      &&
        first.density                == second.density                &&
        first.vapor_pressure         == second.vapor_pressure         &&
        first.refractive_index       == second.refractive_index;
}
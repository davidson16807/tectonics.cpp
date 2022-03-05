#pragma once

#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>

#include "PartlyKnownLiquid.hpp"

compound::phase::PartlyKnownLiquid unknown_liquid {
    /*isobaric_specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*vapor_pressure*/                    std::monostate(),
    /*surface_tension*/                   std::monostate(),
    /*refractive_index*/                  std::monostate(),
    /*extinction_coefficient*/            std::monostate()
};
compound::phase::PartlyKnownLiquid molten_silica {
    /*isobaric_specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 exp(10.0) * si::poise, // Doremus (2002), at 1400 C
    /*density*/                           2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
    /*vapor_pressure*/                    std::monostate(),
    /*surface_tension*/                   250.0 * si::dyne/si::centimeter,
    /*refractive_index*/                  std::monostate(),
    /*extinction_coefficient*/            std::monostate()
};
compound::phase::PartlyKnownLiquid liquid_nitrogen {
    /*isobaric_specific_heat_capacity*/            2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
    /*thermal_conductivity*/              0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
    /*dynamic_viscosity*/                 157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
    /*density*/                           0807.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    
        compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
            return 3.720822*si::standard_pressure - (293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
        }), // Friedman (1950)
    /*surface_tension*/                   11.8000 * si::millinewton / si::meter,
    /*refractive_index*/                  compound::relation::spline_constant<si::wavenumber,float,0,1>(1.19876),
    /*extinction_coefficient*/            compound::relation::spline_constant<si::wavenumber,float,0,1>(0.0)
};
compound::phase::PartlyKnownLiquid dummy_liquid {
    /*isobaric_specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*surface_tension*/                   6.0 * si::millinewton / si::meter,
    /*refractive_index*/                  compound::relation::spline_constant<si::wavenumber,float,0,1>(1.3),
    /*extinction_coefficient*/            compound::relation::spline_constant<si::wavenumber,float,0,1>(0.3)
};

compound::phase::PartlyKnownLiquid liquid_ammonia {
    /*isobaric_specific_heat_capacity*/            4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
    /*vapor_pressure*/                    std::monostate(),
    /*surface_tension*/                   std::monostate(),
    /*refractive_index*/                  compound::relation::spline_constant<si::wavenumber,float,0,1>(1.3944),
    /*extinction_coefficient*/            compound::relation::spline_constant<si::wavenumber,float,0,1>(0.0)
};

int PartlyKnownLiquid_attribute_known_count(const compound::phase::PartlyKnownLiquid& liquid)
{
    return
        liquid.isobaric_specific_heat_capacity .has_value() +
        liquid.thermal_conductivity            .has_value() +
        liquid.dynamic_viscosity               .has_value() +
        liquid.density                         .has_value() +
        liquid.vapor_pressure                  .has_value() +
        liquid.surface_tension                 .has_value() +
        liquid.refractive_index                .has_value() +
        liquid.extinction_coefficient          .has_value();
}


namespace compound {
namespace phase {

    bool operator==(const PartlyKnownLiquid& first, const PartlyKnownLiquid& second)
    {
        return 
            first.isobaric_specific_heat_capacity == second.isobaric_specific_heat_capacity &&
            first.thermal_conductivity            == second.thermal_conductivity            &&
            first.dynamic_viscosity               == second.dynamic_viscosity               &&
            first.density                         == second.density                         &&
            first.vapor_pressure                  == second.vapor_pressure                  &&
            first.surface_tension                 == second.surface_tension                 &&
            relation::distance(first.refractive_index, second.refractive_index, 1.0/si::centimeter, 100e3/si::centimeter) < 1e-4 && 
            relation::distance(first.extinction_coefficient, second.extinction_coefficient, 1.0/si::centimeter, 100e3/si::centimeter) < 1e-4;
    }

    bool operator!=(const PartlyKnownLiquid& first, const PartlyKnownLiquid& second)
    {
        return !(first==second);
    }

}}
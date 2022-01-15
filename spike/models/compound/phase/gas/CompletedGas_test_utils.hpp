#pragma once

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>

#include <models/compound/relation/state/GasPropertyStateRelation_test_utils.hpp>

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
    /*refractive_index*/                  //1.0002990,
        compound::field::SpectralFunction<double>([](const si::wavenumber nhi, const si::wavenumber nlo, const si::pressure p, const si::temperature T) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0 + 6.8552e-5 + 3.243157e-2 / (144.0 - invl2);
        })
};
compound::phase::CompletedGas known_dummy_gas  {
    /*isobaric_specific_heat_capacity*/   1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*refractive_index*/                  1.3
};

namespace compound {
namespace phase {
    bool operator==(const CompletedGas& first, const CompletedGas& second)
    {
        return 
            first.isobaric_specific_heat_capacity == second.isobaric_specific_heat_capacity &&
            first.thermal_conductivity            == second.thermal_conductivity            &&
            first.dynamic_viscosity               == second.dynamic_viscosity               &&
            first.density                         == second.density                         &&
            first.refractive_index                == second.refractive_index;
    }
    bool operator!=(const CompletedGas& first, const CompletedGas& second)
    {
        return !(first==second);
    }
}}

#pragma once

#include "PartlyKnownSolid.hpp"
#include "CompletedSolid.hpp"

#include <models/compound/field/state/CompletedStateField_test_utils.hpp>
#include <models/compound/field/spectral/CompletedSpectralField_test_utils.hpp>
#include <models/compound/field/state/OptionalStateField_test_utils.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_test_utils.hpp>


compound::phase::PartlyKnownSolid unknown_solid {
    /*isobaric_specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  std::monostate(),
    /*extinction_coefficient*/            std::monostate(),
    /*absorption_coefficient*/            std::monostate(),

    /*bulk_modulus*/                      std::monostate(),
    /*tensile_modulus*/                   std::monostate(),
    /*shear_modulus*/                     std::monostate(),
    /*pwave_modulus*/                     std::monostate(),
    /*lame_parameter*/                    std::monostate(),
    /*poisson_ratio*/                     std::monostate(),

    /*compressive_fracture_strength*/     std::monostate(),
    /*tensile_fracture_strength*/         std::monostate(),
    /*shear_fracture_strength*/           std::monostate(),
    /*compressive_yield_strength*/        std::monostate(),
    /*tensile_yield_strength*/            std::monostate(),
    /*shear_yield_strength*/              std::monostate(),

    /*chemical_susceptibility_estimate*/  std::monostate()
};
compound::phase::PartlyKnownSolid ice {
    /*isobaric_specific_heat_capacity*/   2.05 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1e13 * si::poise, // reference by Carey (1953)
    /*density*/                           0916.9 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    138.268 * si::megapascal,
    /*refractive_index*/                  1.3098,
    /*extinction_coefficient*/            0.03,
    /*absorption_coefficient*/            25.0 / si::centimeter,

    /*bulk_modulus*/                      8.899 * si::gigapascal, // gammon (1983)
    /*tensile_modulus*/                   9.332 * si::gigapascal, // gammon (1983)
    /*shear_modulus*/                     3.521 * si::gigapascal, // gammon (1983)
    /*pwave_modulus*/                     13.59 * si::gigapascal, // gammon (1983)
    /*lame_parameter*/                    6.552 * si::gigapascal, // gammon (1983)
    /*poisson_ratio*/                     0.3252, // gammon (1983)

    /*compressive_fracture_strength*/     6.0 * si::megapascal, // engineering toolbox
    /*tensile_fracture_strength*/         1.0 * si::megapascal, // engineering toolbox
    /*shear_fracture_strength*/           1.1 * si::megapascal, // Frederking (1989)
    /*compressive_yield_strength*/        6.0 * si::megapascal, // brittle, effectively the same as fracture strength
    /*tensile_yield_strength*/            1.0 * si::megapascal, // brittle, effectively the same as fracture strength
    /*shear_yield_strength*/              1.1 * si::megapascal, // brittle, effectively the same as fracture strength

    /*chemical_susceptibility_estimate*/  false
};
compound::phase::PartlyKnownSolid quartz {
    /*isobaric_specific_heat_capacity*/   0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
    /*thermal_conductivity*/              1.36 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           2650.0 * si::kilogram/si::meter3, // alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  1.4585,
    /*extinction_coefficient*/            0.00014657, 
    /*absorption_coefficient*/            1.0 / si::centimeter,

    /*bulk_modulus*/                      37.0 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
    /*tensile_modulus*/                   72.0 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
    /*shear_modulus*/                     31.0 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
    /*pwave_modulus*/                     std::monostate(),
    /*lame_parameter*/                    8.0 * si::gigapascal, // https://www.subsurfwiki.org/wiki/1st_Lam%C3%A9_parameter,
    /*poisson_ratio*/                     0.17, // https://www.momentive.com/en-us/categories/quartz/mechanical-properties

    /*compressive_fracture_strength*/     1.1 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
    /*tensile_fracture_strength*/         48.0 * si::megapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
    /*shear_fracture_strength*/           std::monostate(),
    /*compressive_yield_strength*/        1.1 * si::gigapascal, // brittle, effectively the same as fracture strength
    /*tensile_yield_strength*/            48.0 * si::megapascal, // brittle, effectively the same as fracture strength
    /*shear_yield_strength*/              std::monostate(), // brittle, effectively the same as fracture strength

    /*chemical_susceptibility_estimate*/  0.0
};

compound::phase::PartlyKnownSolid copper{

    /*isobaric_specific_heat_capacity*/   0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           8960.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  1.01,
    /*extinction_coefficient*/            13.430, 
    /*absorption_coefficient*/            std::monostate(),

    /*bulk_modulus*/                      130.0 * si::gigapascal,
    /*tensile_modulus*/                   117.0 * si::gigapascal,
    /*shear_modulus*/                     44.7 * si::gigapascal,
    /*pwave_modulus*/                     161.5 * si::gigapascal,
    /*lame_parameter*/                    72.14 * si::gigapascal,
    /*poisson_ratio*/                     0.33,

    /*compressive_fracture_strength*/     45.0 * si::megapascal,
    /*tensile_fracture_strength*/         220.0 * si::megapascal, // engineering toolbox
    /*shear_fracture_strength*/           172.0 * si::megapascal,
    /*compressive_yield_strength*/        33.3 * si::megapascal,
    /*tensile_yield_strength*/            70.0 * si::megapascal, // engineering toolbox
    /*shear_yield_strength*/              std::monostate(),

    /*chemical_susceptibility_estimate*/  std::monostate(),
}; 
compound::phase::PartlyKnownSolid dummy_solid {
    /*isobaric_specific_heat_capacity*/   1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*refractive_index*/                  1.3,
    /*extinction_coefficient*/            0.03, 
    /*absorption_coefficient*/            1.0 / si::centimeter,

    /*bulk_modulus*/                      6.0 * si::gigapascal, 
    /*tensile_modulus*/                   7.0 * si::gigapascal, 
    /*shear_modulus*/                     8.0 * si::gigapascal, 
    /*pwave_modulus*/                     9.0 * si::gigapascal, 
    /*lame_parameter*/                    10.0 * si::gigapascal,
    /*poisson_ratio*/                     0.3, 

    /*compressive_fracture_strength*/     11.0 * si::megapascal,
    /*tensile_fracture_strength*/         12.0 * si::megapascal,
    /*shear_fracture_strength*/           13.1 * si::megapascal,
    /*compressive_yield_strength*/        14.0 * si::megapascal,
    /*tensile_yield_strength*/            15.0 * si::megapascal,
    /*shear_yield_strength*/              16.1 * si::megapascal,

    /*chemical_susceptibility_estimate*/  false
};

int PartlyKnownSolid_attribute_known_count(const compound::phase::PartlyKnownSolid& solid)
{
    return
        solid.isobaric_specific_heat_capacity .has_value() +
        solid.thermal_conductivity            .has_value() +
        solid.dynamic_viscosity               .has_value() +
        solid.density                         .has_value() +
        solid.vapor_pressure                  .has_value() +
        solid.refractive_index                .has_value() +
        solid.extinction_coefficient          .has_value() +
        solid.absorption_coefficient          .has_value() +

        solid.bulk_modulus                    .has_value() +
        solid.tensile_modulus                 .has_value() +
        solid.shear_modulus                   .has_value() +
        solid.pwave_modulus                   .has_value() +
        solid.lame_parameter                  .has_value() +
        solid.poisson_ratio                   .has_value() +

        solid.compressive_fracture_strength   .has_value() +
        solid.tensile_fracture_strength       .has_value() +
        solid.shear_fracture_strength         .has_value() +
        solid.compressive_yield_strength      .has_value() +
        solid.tensile_yield_strength          .has_value() +
        solid.shear_yield_strength            .has_value() +

        solid.chemical_susceptibility_estimate.has_value();
}

namespace compound {
namespace phase {

    bool operator==(const PartlyKnownSolid first, const PartlyKnownSolid second)
    {
        return 
            first.isobaric_specific_heat_capacity == second.isobaric_specific_heat_capacity &&
            first.vapor_pressure                  == second.vapor_pressure                  &&
            first.thermal_conductivity            == second.thermal_conductivity            &&
            first.dynamic_viscosity               == second.dynamic_viscosity               &&
            first.density                         == second.density                         &&
            relation::distance(first.refractive_index, second.refractive_index, 1.0f/si::centimeter, 100e3f/si::centimeter) < 1e-4 && 
            relation::distance(first.extinction_coefficient, second.extinction_coefficient, 1.0f/si::centimeter, 100e3f/si::centimeter) < 1e-4 && 
            relation::distance(first.absorption_coefficient, second.absorption_coefficient, 1.0f/si::centimeter, 100e3f/si::centimeter) < 1e-4 && 

            first.bulk_modulus           == second.bulk_modulus           &&
            first.tensile_modulus        == second.tensile_modulus        &&
            first.shear_modulus          == second.shear_modulus          &&
            first.pwave_modulus          == second.pwave_modulus          &&
            first.lame_parameter         == second.lame_parameter         &&
            first.poisson_ratio          == second.poisson_ratio          &&

            first.compressive_fracture_strength == second.compressive_fracture_strength &&
            first.tensile_fracture_strength     == second.tensile_fracture_strength     &&
            first.shear_fracture_strength       == second.shear_fracture_strength       &&
            first.compressive_yield_strength    == second.compressive_yield_strength    &&
            first.tensile_yield_strength        == second.tensile_yield_strength        &&
            first.shear_yield_strength          == second.shear_yield_strength          &&

            first.chemical_susceptibility_estimate == second.chemical_susceptibility_estimate;
    }

    bool operator!=(const PartlyKnownSolid first, const PartlyKnownSolid second)
    {
        return !(first==second);
    }

}}

#pragma once

#include "PartlyKnownSolid.hpp"
#include "CompletedSolid.hpp"

compound::phase::CompletedSolid known_ice (
    /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1e13 * si::poise, // reference by Carey (1953)
    /*density*/                           0916.9 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    138.268 * si::megapascal,
    /*refractive_index*/                  1.3098,
    /*spectral_reflectance*/              0.9,

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

    /*chemical_susceptibility_estimate*/  0.0
);
compound::phase::CompletedSolid known_dummy (
    /*specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*refractive_index*/                  1.3,
    /*spectral_reflectance*/              0.5,

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

    /*chemical_susceptibility_estimate*/  0.0
);

#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::StateFunction<si::density<double>>> density_as_liquid {
        { ids::water,                
                relation::StateFunction<si::density<double>>([](const si::pressure<double> p, const si::temperature<double> T) {
                    // Perry equation 119, specialized for water
                    // valid for 273.16-647.096K
                    double Tc = 647.096;
                    double t = std::clamp(T/si::kelvin, 273.16, Tc);
                    double tau = 1.0 - (t/Tc);
                    double rho = 17.874 
                        + 35.618*pow(tau,0.33) 
                        + 19.655*pow(tau,0.66)
                        - 9.1306*pow(tau, 5.0/3.0) 
                        - 31.367*pow(tau, 16.0/3.0) 
                        - 813.56*pow(tau,43.0/3.0) 
                        - 17421000.0*pow(tau,110.0/3.0);
                    return rho * 18.015 * si::gram / si::decimeter3;
                }),
            },
        { ids::nitrogen,        // relation::state_invariant(0.4314*si::gram/si::centimeter3), // Johnson (1960), @ si::atmosphere, 125.01*si::kelvin
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 28.013  * si::gram/si::decimeter3,
                     3.2091, 0.2861, 126.2, 0.2966,
                     63.15, 126.2), // 63.15-126.2K
            },
        { ids::oxygen,                // 1.141 * si::gram/si::milliliter,
                // relation::StateSample<si::density<double>>(0.6779*si::gram/si::centimeter3, si::atmosphere, 149.8*si::kelvin), // Johnson (1960)
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 31.9988 * si::gram/si::decimeter3,
                     3.9143, 0.28772, 154.58, 0.2924,
                     54.35, 154.58), // 54.35-154.58K
            },
        { ids::carbon_dioxide,                // 1101.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 44.01 * si::gram/si::decimeter3,
                     2.768, 0.26212, 304.21, 0.2908,
                     216.58, 304.21), // 216.58-304.21K
            },
        { ids::methane,                
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 16.043 * si::gram/si::decimeter3,
                     2.9214, 0.28976, 190.56, 0.28881,
                     90.69, 190.56), // 90.69-190.56K
            },
        { ids::argon,                
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 39.948 * si::gram/si::decimeter3,
                     3.8469, 0.2881, 150.86, 0.29783,
                     3.78, 150.86), // 83.78, 150.86K
            },
        { ids::helium,                
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 4.0026 * si::gram/si::decimeter3,
                     7.2475, 0.41865, 5.2, 0.24096,
                     2.2, 5.2), // 2.2-5.2K
            },
        { ids::hydrogen,                // 0.0708 * si::gram/si::milliliter,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 2.016 * si::gram/si::decimeter3,
                     5.414, 0.34893, 33.19, 0.2706,
                     13.95, 33.19), // 13.95-33.19K
            },
        { ids::ammonia,                // 681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 17.031 * si::gram/si::decimeter3,
                     3.5383, 0.25443, 405.65, 0.2888,
                     195.41, 405.65), // 195.41-405.65K
            },
        { ids::ozone,                // 1349.0 * si::kilogram / si::meter3, //encyclopedia.airliquide.com
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 47.998 * si::gram/si::decimeter3,
                     3.3592, 0.29884, 261.0, 0.28523,
                     80.15, 261.0), // 80.15-261.0K
            },
        { ids::nitrous_oxide,                // 1230.458 * si::kilogram / si::meter3, 
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 44.012 * si::gram/si::decimeter3,
                     2.781, 0.27244, 309.57, 0.2882,
                     182.3, 309.57), // 182.3-309.57K
            },
        { ids::sulfur_dioxide,                // 389.06 * si::kilogram / si::meter3, // encyclopedia.airliquide.com
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 64.064 * si::gram/si::decimeter3,
                     2.106, 0.25842, 430.75, 0.2895,
                     197.67, 430.75), // 197.67-430.75K
            },
        { ids::nitric_oxide,                // 1230.458 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 30.006 * si::gram/si::decimeter3,
                     5.246, 0.3044, 180.15, 0.242,
                     109.5, 180.15), // 109.5-180.15K
            },
        { ids::carbon_monoxide,                
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 28.010 * si::gram/si::decimeter3,
                     2.897, 0.27532, 132.92, 0.2813,
                     68.15, 132.92), // 68.15-132.92K
            },
        { ids::ethane,                // 545.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 30.070 * si::gram/si::decimeter3,
                     1.9122, 0.27937, 305.32, 0.29187,
                     90.35, 305.32), // 90.35-305.32K
            },
        { ids::hydrogen_cyanide,                // 687.6 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 27.026 * si::gram/si::decimeter3,
                     1.3413, 0.18589, 456.65, 0.28206,
                     259.83, 456.65), // 259.83-456.65K
            },
        { ids::ethanol,                // 0789.3 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 46.068 * si::gram/si::decimeter3,
                     1.6288, 0.27469, 514.0, 0.23178,
                     159.05, 514), // 159.05-514K
            },
        { ids::formaldehyde,                // 0.8153 * si::kilogram/si::meter3,  // wikipedia
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 30.026 * si::gram/si::decimeter3,
                     3.897011, 0.331636, 420.0, 0.28571,
                     155.15, 420.0), // 155.15-420.0K
            },
        { ids::formic_acid,                // 1220.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 46.026 * si::gram/si::decimeter3,
                     1.938, 0.24225, 588.0, 0.24435,
                     281.45, 588.0), // 281.45-588.0K
            },
        { ids::perflouromethane,                // 1890.0 * si::kilogram/si::meter3, //pubchem
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 88.0  * si::gram/si::decimeter3,
                     1.955, 0.27884, 227.51, 0.28571,
                     89.56, 227.51), // 89.56-227.51K
            },
        { ids::benzene,                // 879.0 * si::kilogram/si::meter3, // Perry
                relation::get_dippr_temperature_function_105
                    (si::kelvin, 79.102 * si::gram/si::decimeter3,
                     1.0259, 0.266666, 562.05, 0.28394,
                     278.68, 562.05), // 278.68-562.05K
            },
        { ids::pyrimidine,      relation::state_invariant(1.016 * si::gram/si::centimeter3), // wikipedia
            },
        { ids::quartz,          relation::state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
            },
        { ids::halite,          relation::state_invariant(1549.0 * si::kilogram/si::meter3),  // wikipedia data page
            },
        // { ids::corundum,     },
        // { ids::apatite,      },
        { ids::carbon,          relation::state_invariant(1.6*si::gram/si::centimeter3), // Bundy, referenced in Steinbeck (1990)
            },
        // { ids::calcite,      },
        { ids::orthoclase,      relation::state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
            },
        { ids::andesine,        relation::state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
            },
        { ids::augite,          relation::state_invariant(2800.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for basaltic  magma
            },
        // { ids::forsterite,   },
        // { ids::goethite,     },
        // { ids::pyrite,       },
        // { ids::hematite,     },
        { ids::gold,            relation::state_invariant(17310.0 * si::kilogram/si::meter3), 
            },
        { ids::silver,          relation::state_invariant(9320.0 * si::kilogram/si::meter3), 
            },
        { ids::copper,          relation::state_invariant(8020.0 * si::kilogram/si::meter3), 
            },
        // { ids::magnetite,    },
        // { ids::chalcocite,   },
        // { ids::chalcopyrite, },
    };


}}
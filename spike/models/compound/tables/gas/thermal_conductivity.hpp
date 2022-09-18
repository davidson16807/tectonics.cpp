#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/GasPropertyStateRelation.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::GasPropertyStateRelation<si::thermal_conductivity<double>>> thermal_conductivity_as_gas {
        {ids::water,   
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00054, 1.09614, 0.00000, 0.00000, 0.09827, 691.90362, 883.95160, 0.08323,
                    300.0, 1273.2, 0.0, 10.0, 0.097), 
                    // water, mean error: 2.5%, max error: 9.7%, range: 300-1273.2K, 0-10MPa, stp estimate: 0.018
            },
        {ids::nitrogen,   // 0.0234 * si::watt / (si::meter * si::kelvin),                    // wikipedia
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00026, 1.21142, 0.00037, 0.75287, -0.00165,
                     113.15, 2013.2,  0.0, 3.0, 0.050), 
                    // nitrogen, mean error: 0.8%, max error: 5.0%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 0.024
            },
        {ids::oxygen,   
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00023, 1.29450, 0.00018, 0.88149, -0.00130,
                     104.36, 1004.4,  0.0, 3.0, 0.042), 
                    // oxygen, mean error: 0.8%, max error: 4.2%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 0.025
            },
        {ids::carbon_dioxide,   // 0.01662 * si::watt / ( si::meter * si::kelvin ),                 // wikipedia
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00032, 1.14102, 0.00027, 0.83072, -0.01434,
                     266.59, 1116.6,  0.0, 3.0, 0.027), 
                    // carbon dioxide, mean error: 0.8%, max error: 2.7%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.015
            },
        {ids::methane,   // 34.4 * si::milliwatt / ( si::meter * si::kelvin ),               // Huber & Harvey         
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00081, 1.07534, 0.00000, 0.00000, 0.28162, 818.25156, 1044.83566, 0.22399,
                    140.69, 640.69, 0.0, 3.0, 0.032), 
                    // methane, mean error: 1.1%, max error: 3.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 0.031
            },
        {ids::argon,   // 0.016 * si::watt / ( si::meter * si::kelvin ),                   // wikipedia
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00034, 1.11244, 0.00029, 0.73962, -0.00193,
                     133.81, 683.81,  0.0, 3.0, 0.027), 
                    // argon, mean error: 1.2%, max error: 2.7%, range: 133.81-683.81K, 0-3MPa, stp estimate: 0.016
            },
        {ids::helium,   // 155.7 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    -0.15319, 1.77283, 0.00284, 0.70030, 0.00337,
                     52.177, 1502.2,  0.0, 0.1, 0.041), 
                    // helium, mean error: 0.8%, max error: 4.1%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 0.145
            },
        {ids::hydrogen,   // 186.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00197, 1.33064, 0.00154, 0.84231, -0.00399,
                     63.957, 1014,  0.0, 1.0, 0.025), 
                    // hydrogen, mean error: 1.3%, max error: 2.5%, range: 63.957-1014K, 0-1MPa, stp estimate: 0.169
            },
        {ids::ammonia,   // 25.1 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00076, 1.10036, 0.00000, 0.00000, 0.07387, 491.21103, 544.10090, 0.05855,
                    245.5, 695.5, 0.0, 10.0, 0.069), 
                    // ammonia, mean error: 2.5%, max error: 6.9%, range: 245.5-695.5K, 0-10MPa, stp estimate: 0.023
            },
        {ids::ozone,   
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     0.0043147, 0.47999, 700.09, 0.0,
                     161.85, 1000.0), // 161.85-1000K
            },
        {ids::nitrous_oxide,   // 17.4 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     0.001096, 0.667, 540.0, 0.0,
                     182.3, 1000.0), // 182.3-1000K
            },
        {ids::sulfur_dioxide,   // 9.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     10.527, -0.7732, -1333.0, 1506400.0,
                     250.0, 900.0), // 250-900K
            },
        {ids::nitric_oxide,   // 25.9 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     0.0004096, 0.7509, 45.6, 0.0,
                     121.38, 750.0), // 121.38-750K
            },
        {ids::carbon_monoxide,   // 25.0 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00045, 0.43412, 0.00055, 0.70174, -0.00365,
                     118.16, 518.16,  0.0, 3.0, 0.025), 
                    // carbon monoxide, mean error: 0.8%, max error: 2.5%, range: 118.16-518.16K, 0-3MPa, stp estimate: 0.025
            },
        {ids::ethane,   // 21.2 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00064, 1.08467, 0.00000, 0.00000, 0.08892, 442.87962, 573.08449, 0.06794,
                    140.35, 640.35, 0.0, 3.0, 0.038), 
                    // ethane, mean error: 1.1%, max error: 3.8%, range: 140.35-640.35K, 0-3MPa, stp estimate: 0.018
            },
        {ids::hydrogen_cyanide,   
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     4.6496e-6, 1.3669, -210.76, 58295.0,
                     273.15, 673.15), // 273.15-673.15K
            },
        {ids::ethanol,   // 14.4 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     -0.010109, 0.6475, -7332.0, -268e3,
                     293.15, 1000.0), // 293.15-1000K
            },
        {ids::formaldehyde,   
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     5.2201e-06, 1.417, 0.0, 0.0,
                     253.85, 1000.0), // 253.85-1000K
            },
        {ids::perflouromethane,   // 16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00019, 1.49795, 0.00010, 0.95277, -0.00704,
                     148.94, 598.94,  0.0, 3.0, 0.084), 
                    // tetrafluoromethane, mean error: 3.3%, max error: 8.4%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.015
            },
        {ids::benzene,   
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::watt / (si::meter * si::kelvin), 
                     0.00001652, 1.3117, 491, 0.0,
                     339.15, 1000.0), // 339.15-1000K
            },
        // {ids::pyrimidine,    },
        // {ids::halite,        },
        // {ids::corundum,      },
        // {ids::apatite,       },
        // {ids::carbon,        },
        // {ids::calcite,       },
        // {ids::orthoclase,    },
        // {ids::andesine,      },
        // {ids::augite,        },
        // {ids::forsterite,    },
        // {ids::goethite,      },
        // {ids::pyrite,        },
        // {ids::hematite,      },
        // {ids::gold,          },
        // {ids::silver,        },
        // {ids::copper,        },
        // {ids::magnetite,     },
        // {ids::chalcocite,    },
        // {ids::chalcopyrite,  },
    };

}}
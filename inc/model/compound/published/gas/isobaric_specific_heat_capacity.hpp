#pragma once

// in-house libraries
#include <model/compound/ids.hpp>
#include <model/compound/relation/gas/GasPropertyStateRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using GasHeatCapacityStateRelation = relation::GasPropertyStateRelation<si::specific_heat_capacity<double>>;
    table::PartialTable<GasHeatCapacityStateRelation> isobaric_specific_heat_capacity_as_gas {
        { compounds::water, 
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.01766, 0.80539, 0.00707, 0.69586, 1.42782,
                     300.0, 1273.2,  0.0, 10.0, 0.034),
                    // water, mean error: 0.8%, max error: 3.4%, range: 300-1273.2K, 0-10MPa, stp estimate: 1.781
            },
        { compounds::nitrogen, // 1.341 * si::joule / (si::gram * si::kelvin),
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.00115, 0.62179, 0.00000, 0.00000, 2.54371, 2268.64874, -2637.49785, -0.99334,
                    163.15, 2013.2, 0.0, 3.0, 0.054), 
                    // nitrogen, mean error: 0.8%, max error: 5.4%, range: 163.15-2013.2K, 0-3MPa, stp estimate: 1.013
            },
        { compounds::oxygen,        
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    -0.00022, 0.00063, 0.00000, 0.00000, 1.11067, 1268.80242, -836.03510, 0.17582,
                    204.36, 1004.4, 0.0, 3.0, 0.033), 
                    // oxygen, mean error: 0.7%, max error: 3.3%, range: 204.36-1004.4K, 0-3MPa, stp estimate: 0.907
            },
        { compounds::carbon_dioxide, // 36.61 * si::joule / (44.01 * si::gram * si::kelvin),             // wikipedia
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.00034, 3.89562, 0.00000, 0.00000, 3.24445, 600.78481, -562.26128, -1.80573,
                    266.59, 1116.6, 0.0, 3.0, 0.028), 
                    // carbon dioxide, mean error: 0.8%, max error: 2.8%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.828
            },
        { compounds::methane, // 2.218 * si::joule / (si::gram * si::kelvin),                     
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.02624, 1.04690, 0.00000, 0.00000, 1.04105, 264.87732, 492.16481, 2.86235,
                    140.69, 640.69, 0.0, 3.0, 0.021), 
                    // methane, mean error: 0.7%, max error: 2.1%, range: 140.69-640.69K, 0-3MPa, stp estimate: 2.201
            },
        { compounds::argon,  // 0.570 * si::joule / (si::gram * si::kelvin),        
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::kilojoule/(si::kilogram*si::kelvin),
                     -1e-8, 0.98, 2.75e-4, 1.02, 5.6e-1,
                     90.0, 1000.0,  0.1, 10.0, 0.0),             
            },
        { compounds::helium, // 9.78 * si::joule / (si::gram * si::kelvin), 
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::kilojoule/(si::kilogram*si::kelvin),
                     -1e-6, 0.95, 1.86e-2, 1.02, 5.7e-1,
                     20.0, 1000.0,  0.1, 10.0, 0.0),             
            },
        { compounds::hydrogen, 
                relation::get_sigmoid_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                     0.03602, 3.74271, 0.00000, 1.70014, 2.72519, 120.41496, 97.80116, 11.80351,
                     63.957, 1014.0, 0.0, 1.0, 0.08), 
                     // hydrogen, mean error: 0.3%, max error: 1.4%, range: 63.957-1014K, 0-1MPa, stp estimate: 14.133
            },
        { compounds::ammonia, // 35.06 * si::joule / (17.031 * si::gram * si::kelvin), // wikipedia data page
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.04652, 0.81959, 0.02155, 0.67889, 1.04112,
                     245.5, 695.5,  0.0, 10.0, 0.043), 
                    // ammonia, mean error: 1.2%, max error: 4.3%, range: 245.5-695.5K, 0-10MPa, stp estimate: 2.020
            },
        // { compounds::ozone,            },
        // { compounds::nitrous_oxide,    },
        { compounds::sulfur_dioxide, 
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.03124, 1.38702, 0.00214, 0.79655, 0.42765,
                     247.7, 547.7,  0.0, 1.0, 0.020), 
                    // sulfur dioxide, mean error: 0.7%, max error: 2.0%, range: 247.7-547.7K, 0-1MPa, stp estimate: 0.615
            },
        { compounds::nitric_oxide, 
                relation::get_perry_temperature_gas_relation
                    (si::kelvin, si::calorie/(30.006 * si::gram*si::kelvin), 
                      8.05, 0.000233, -156300.0, 0.0,
                      300.0, 5000.0),
            },
        { compounds::carbon_monoxide, // 29.0 * si::joule / (28.010 * si::gram * si::kelvin), // wikipedia data page
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.00728, 0.92688, 0.00010, 0.97052, 1.01709,
                     168.16, 518.16,  0.0, 3.0, 0.008), 
                    // carbon monoxide, mean error: 0.3%, max error: 0.8%, range: 168.16-518.16K, 0-3MPa, stp estimate: 1.041
            },
        { compounds::ethane, // 52.49 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.05856, 0.85039, 0.00090, 1.21385, 0.86559,
                     140.35, 640.35,  0.0, 3.0, 0.033), 
                    // ethane, mean error: 1.3%, max error: 3.3%, range: 140.35-640.35K, 0-3MPa, stp estimate: 1.687
            },
        // { compounds::hydrogen_cyanide, },
        { compounds::ethanol, 78.28 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
            },
        // { compounds::formaldehyde,     },
        { compounds::formic_acid, 45.68 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
            },
        { compounds::perflouromethane, 
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.00845, 1.30975, 0.00914, 0.70892, 0.16266,
                     148.94, 598.94,  0.0, 3.0, 0.053), 
                    // tetrafluoromethane, mean error: 2.6%, max error: 5.3%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.651
            },
        { compounds::benzene, // 82.44 * si::joule / (79.109 * si::gram * si::kelvin),
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.11074, 0.98216, 0.00048, 1.26428, 0.48272,
                     300, 628.7,  0.0, 1.0, 0.071), 
                    // benzene, mean error: 2.6%, max error: 7.1%, range: 300-628.7K, 0-1MPa, stp estimate: 1.069
            },
        // { compounds::pyrimidine,       },
        // { compounds::halite,          },
        // { compounds::corundum,        },
        // { compounds::apatite,         },
        // { compounds::carbon,           },
        // { compounds::calcite,         },
        // { compounds::orthoclase,      },
        // { compounds::andesine,         },
        // { compounds::augite,           },
        // { compounds::forsterite,       },
        // { compounds::goethite,        },
        // { compounds::pyrite,          },
        // { compounds::hematite,         },
        // { compounds::gold,            },
        // { compounds::silver,          },
        // { compounds::copper,          },
        // { compounds::magnetite,       },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,    },
    };

}}
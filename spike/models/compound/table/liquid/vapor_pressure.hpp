#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace table { 

    std::map<int, relation::StateFunction<si::pressure<double>> vapor_pressure_as_liquid {
        { ids::water,         
                // get_antoine_vapor_pressure_function(
                //     si::celcius, si::millimeter_mercury, 
                //     7.94917, 1657.462, 1474.68, 213.69), // Physical and Chemical Equilibrium for Chemical Engineers, Second Edition. 
                // get_dippr_temperature_function_101
                //     (si::kelvin, si::pascal,
                //      73.649, -7258.2, -7.3037, 4.1653e-6,
                //      273.16, 647.1),//273.16-647.1K
                relation::StateFunction<si::pressure<double>>([](const si::pressure<double> p, const si::temperature<double> T) {
                    // Buck equation
                    double C = T/si::celcius;
                    return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
                }),
            },
        { ids::nitrogen,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     58.282, -1084.1, -8.3144, 0.044127, 1.0,
                      63.15, 126.2), // 63.15-126.2K
            },
        { ids::oxygen,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     51.245, -1200.2, -6.4361, 0.028405, 1.0,
                      54.36, 154.58), // 54.36-154.58K
            },
        { ids::carbon_dioxide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     47.0169, -2839.0, -3.86388, 2.81e-16, 6.0,
                      216.58, 304.21), // 216.58-304.21K
            },
        { ids::methane,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     39.205, -1324.4, -3.4366, 0.000031019, 2.0,
                      90.69, 190.56), // 90.69-190.56K
            },
        { ids::argon,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     42.127, -1093.1, -4.1425, 0.000057254, 2.0,
                      83.78, 150.86), // 83.78-150.86K
            },
        { ids::helium,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     11.533, -8.99, 0.6724, 0.2743, 1.0,
                      1.76, 5.2), // 1.76-5.2K
            },
        { ids::hydrogen,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     12.69, -94.896, 1.1125, 0.00032915, 2.0,
                      13.95, 33.19), // 13.95-33.19K
            },
        { ids::ammonia,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     90.483, -4669.7, -11.607, 0.017194, 1.0,
                      195.41, 405.65), // 195.41-405.65K
            },
        { ids::ozone,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     40.067, -2204.8, -2.9351, 7.75e-16, 6.0,
                      80.15, 261), // 80.15-261K
            },
        { ids::nitrous_oxide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     96.512, -4045, -12.277, 0.00002886, 2.0,
                      182.3, 309.57),// 182.3-309.57K
            },
        { ids::sulfur_dioxide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     47.365, -4084.5, -3.6469, 1.80e-17, 6.0,
                     197.67, 430.75),//197.67-430.75K
            },
        { ids::nitric_oxide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     72.974, -2650.0, -8.261, 9.7e-15, 6.0,
                      109.5, 180.15),// 109.5-180.15K
            },
        { ids::carbon_monoxide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     45.698, -1076.6, -4.8814, 0.000075673, 2.0,
                      68.15, 132.92), // 68.15-132.92K
            },
        { ids::ethane,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     51.857,-2598.7, -5.1283, 0.000014913, 2.0,
                      90.35, 305.32), // 90.35-305.32K
            },
        { ids::hydrogen_cyanide,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     36.75, -3927.1, -2.1245, 3.89e-17, 6.0,
                     259.83, 456.65),
            },
        { ids::ethanol,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     73.304, -7122.3, -7.1424, 2.8853e-6, 2.0,
                     159.05, 514.0), 
            },
        { ids::formaldehyde,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     49.3632, -3847.87, -4.09834, 4.64e-17, 6.0,
                      155.15, 420), // 155.15-420K
            },
        { ids::formic_acid,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     43.8066, -5131.03, -3.18777, 2.37819e-6, 2.0,
                      281.45, 588), // 281.45-588K
            },
        { ids::perflouromethane,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     61.89, -2296.3, -7.086, 0.000034687, 2.0,
                      89.56, 227.51), // 89.56-227.51K
            },
        { ids::benzene,         
                get_dippr_temperature_function_101
                    (si::kelvin, si::pascal,
                     83.107, -6486.2, -9.2194, 6.9844e-06, 2.0,
                      278.68, 562.05), // 278.68-562.05K
            },
        // { ids::pyrimidine,   },
        { ids::quartz,         
                    get_interpolated_temperature_function
                        (si::celcius, si::pascal,
                         std::vector<double>{1966.0,     2149.0,     2368.0}, 
                         std::vector<double>{1.0 ,       10.0,       100.0  }),
            },
        { ids::halite,         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                      std::vector<double>{835.0,     987.0,     1461.0}, 
                     std::vector<double>{100.0 ,     1e3,       100e3}),
            },
        // { ids::corundum,     },
        // { ids::apatite,      },
        { ids::carbon,         
                    get_interpolated_temperature_function
                        (si::celcius, si::pascal,
                         std::vector<double>{2566.0,     3016.0,     3635.0}, 
                         std::vector<double>{1.0 ,       1e3,        100e3}),
                                                                             // TOOD: autocomplete vapor pressure for solids/liquids if function is present for other phase
            },
        // { ids::calcite,      },
        // { ids::orthoclase,   },
        // { ids::andesine,     },
        // { ids::augite,       },
        // { ids::forsterite,   },
        // { ids::goethite,     },
        // { ids::pyrite,       },
        // { ids::hematite,     },
        { ids::gold,         
                    get_interpolated_temperature_function
                        (si::celcius, si::pascal,
                         std::vector<double>{1373.0,     2008.0,     2805.0}, 
                         std::vector<double>{1.0 ,       1e3,        100e3}),
            },
        { ids::silver,         
                    get_interpolated_temperature_function
                        (si::celcius, si::pascal,
                         std::vector<double>{1010.0,     1509.0,     2160.0}, 
                         std::vector<double>{1.0 ,       1e3,        100e3}),
            },
        { ids::copper,         
                    get_interpolated_temperature_function
                        (si::celcius, si::pascal,
                         std::vector<double>{1236.0,     1816.0,     2563.0}, 
                         std::vector<double>{1.0 ,       1e3,        100e3}),
            },
        // { ids::magnetite,    },
        // { ids::chalcocite,   },
        // { ids::chalcopyrite, },
    };

}}
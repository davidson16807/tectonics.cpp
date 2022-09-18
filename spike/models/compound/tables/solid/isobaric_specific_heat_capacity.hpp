#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::StateFunction<si::specific_heat_capacity<double>>> isobaric_specific_heat_capacity_as_solid {
        { ids::water,   
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/66e3, 15.0,
                     si::kilojoule / (si::kilogram * si::kelvin), 
                     2.05, 1.0/186.0, 0.0, -1.0/80e3,
                     173.15, 273.15), // Johnson (1960) and Engineering Toolbox, custom fit
            },
        { ids::nitrogen,                   
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                      std::vector<double>{ 39.0, 60.0, 62.5},
                      std::vector<double>{37.39,45.64,46.97}), // Manzhelii (1997)
            },
        { ids::oxygen,   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{45.90, 54.39},
                      std::vector<double>{11.02, 11.06}), // Johnson (1960)
            },
        { ids::carbon_dioxide,            
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,   3.0,  10.0, 40.0, 70.0, 200.0, 215.0}, 
                     std::vector<double>{0.0,0.0151,0.5883,19.64,33.38, 56.77, 61.90}), // Manzhelii (1997)
            },
        { ids::methane, 
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (16.043*si::gram * si::kelvin),
                      std::vector<double>{21.35, 61.40, 87.20},
                      std::vector<double>{4.325, 8.673, 10.19}), // Johnson (1960)
            },
        { ids::argon,   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0, 90.68},
                      std::vector<double>{0.0, 0.197}), // Johnson (1960)
            },
        // { ids::helium,           },
        { ids::hydrogen,   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0,    3.04,    5.95,   9.87},
                      std::vector<double>{0.0, 0.02584, 0.06349, 0.2763}), // Johnson (1960)
            },
        // { ids::ammonia,          },
        // { ids::ozone,            },
        { ids::nitrous_oxide,            
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,   3.0,  10.0, 40.0, 60.0, 150.0, 180.0}, 
                     std::vector<double>{0.0,0.0189, 0.822,21.65,31.47, 50.70, 58.28}), // Manzhelii (1997)
            },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,            
                get_interpolated_temperature_function
                    (si::kelvin, si::joule/(28.010*si::gram*si::kelvin),
                     std::vector<double>{ 63.0,  64.0,  65.0,  66.0,  67.0}, 
                     std::vector<double>{50.10, 50.58, 51.08, 51.58, 52.29}), // Manzhelii (1997)
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        { ids::ethanol,   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule/(46.068*si::gram*si::kelvin),
                     std::vector<double>{0.0, 159.2}, 
                     std::vector<double>{0.0,111.46}), // wikipedia data page
            },
        // { ids::formaldehyde,     },
        { ids::formic_acid,   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule / (46.026*si::gram*si::kelvin),
                     std::vector<double>{0.0, 281.5}, 
                     std::vector<double>{0.0,  74.5}), // wikipedia data page
            },
        // { ids::perflouromethane, },
        { ids::benzene,   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule / (79.109*si::gram*si::kelvin),
                     std::vector<double>{0.0, 278.7}, 
                     std::vector<double>{0.0, 118.4}), // wikipedia data page
            },
        // { ids::pyrimidine,       },
        { ids::quartz,            // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     10.87, 0.008712, 241200.0, 0.0, 
                     273.0, 848.0), // Johnson (1960) and Perry
            },
        { ids::halite,   // 50.0 * si::joule / (90.442 * si::gram * si::kelvin), // wikipedia data page
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(80.088 * si::gram*si::kelvin), 
                     10.79, 0.00420, 0.0, 0.0,
                      273.0, 1074.0),
            },
        { ids::corundum,   // 750.0*si::joule/(si::kilogram* si::kelvin)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(101.96 * si::gram*si::kelvin), 
                     22.08, 0.008971, -522500.0, 0.0,
                     273.0, 1973.0), 
            },
        { ids::apatite,   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,   0.7}), // Schön (2015)
            },
        { ids::carbon,   // 0.710 * si::joule / (si::gram * si::kelvin), // wikipedia, Diamond is 0.5091
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     1e-5, 1.0/970e3, 10.0,
                     si::calorie/(12.011 * si::gram*si::kelvin),
                     2.673, 0.002617, -116900.0, 0.0,
                     273.0, 1373.0), // Johnson (1960) and Perry
            },
        { ids::calcite,   // 0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(100.087 * si::gram*si::kelvin), 
                     19.68, 0.01189, -307600.0, 0.0, 
                     273.0, 1033.0), 
            },
        { ids::orthoclase,   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(278.33 * si::gram*si::kelvin), 
                     69.26, 0.00821, -2331000.0, 0.0, 
                     273.0, 1373.0),
            },
        { ids::andesine,   // 66.0 * si::joule / (268.6 * si::gram * si::kelvin), // Richet (1984)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(268.6 * si::gram*si::kelvin), 
                     63.13, 0.01500, -1537000.0, 0.0, 
                     273.0, 1673.0), // for anorthite
            },
        { ids::augite,   // 0.7 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988), representative of pyroxenes
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(236.4 * si::gram*si::kelvin), 
                     23.35, 0.008062, -558800.0, 0.0, 
                     273.0, 773.0), // for maganese pyroxenes 
            },
        { ids::forsterite,   // 0.79 * si::joule / (si::gram * si::kelvin), // Cermak (1988), for fayalite/forsterite mix
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(153.31 * si::gram*si::kelvin), 
                     33.57, 0.01907, -879700.0, 0.0, 
                     273.0, 1161.0), // for fayalite
            },
        // { ids::goethite,         },
        { ids::pyrite,   // 0.5 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(119.98 * si::gram*si::kelvin), 
                     10.7, 0.01336, 0.0, 0.0, 
                     273.0, 773.0),
            },
        { ids::hematite,   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.69 * si::gram*si::kelvin), 
                     24.72, 0.01604, -423400.0, 0.0, 
                     273.0, 1097.0),
            },
        { ids::gold,   // 0.129 * si::joule / (si::gram * si::kelvin), // wikipedia, room temperature
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     3.75e-6, 9.86/(165.0*165.0*165.0), 15.0,
                     si::calorie/(196.967 * si::gram*si::kelvin), 
                     5.61, 0.00144, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            },
        { ids::silver,   // 0.233 * si::joule / (si::gram * si::kelvin), // wikipedia
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/600e3, 30.0,
                     si::calorie/(107.868 * si::gram*si::kelvin), 
                     5.6, 0.00150, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            },
        { ids::copper,            // 0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     10.8e-6, 30.6/(344.5*344.5*344.5), 10.0,
                     si::calorie/(63.546 * si::gram*si::kelvin), 
                     5.44, 0.001462, 0.0, 0.0, 
                     273.0, 1357.0), // Johnson (1960) and Perry
            },
        { ids::magnetite,   // 0.6 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(231.53 * si::gram*si::kelvin), 
                     41.17, 0.01882, -979500.0, 0.0, 
                     273.0, 1065.0),
            },
        { ids::chalcocite,            
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.16 * si::gram*si::kelvin), 
                     9.38, 0.0312, 0.0, 0.0, 
                     273.0, 376.0),
            },
        { ids::chalcopyrite,   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,  0.54}), // Cermak (1988), for chalcopyrite
            },
    };


}}
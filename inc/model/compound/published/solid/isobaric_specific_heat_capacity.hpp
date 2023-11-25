#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidMolarHeatCapacityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::molar_heat_capacity<double>, -2,3>;
    using SolidHeatCapacityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::specific_heat_capacity<double>, -2,3>;
    table::PartialTable<SolidHeatCapacityTemperatureRelation> isobaric_specific_heat_capacity_as_solid {

        { polymorphs::water_ice_1h,   
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/66e3, 15.0,
                     si::kilojoule / (si::kilogram * si::kelvin), 
                     2.05, 1.0/186.0, 0.0, -1.0/80e3,
                     173.15, 273.15), // Johnson (1960) and Engineering Toolbox, custom fit
            },
        // { polymorphs::water_ice_1c,              2500.0*si::joule/(si::kilogram*si::kelvin)          // Tchijov (2008)
        //     },
        // { polymorphs::water_ice_2,               },
        // { polymorphs::water_ice_3,               },
        // { polymorphs::water_ice_4,               },
        // { polymorphs::water_ice_5,               2500.0*si::joule/(si::kilogram*si::kelvin)          // Tchijov (2008)
        //     },
        // { polymorphs::water_ice_6,               2600.0*si::joule/(si::kilogram*si::kelvin)          // Tchijov (2008)
        //     },
        // { polymorphs::water_ice_7,               },
        // { polymorphs::water_ice_8,               },
        // { polymorphs::water_ice_9,               },
        // { polymorphs::water_ice_10,              },
        // { polymorphs::water_ice_11h,             },
        // { polymorphs::water_ice_11o,             },
        // { polymorphs::water_ice_12,              },
        // { polymorphs::water_ice_13,              },
        // { polymorphs::water_ice_14,              },
        // { polymorphs::water_ice_15,              },
        // { polymorphs::water_ice_16,              },
        // { polymorphs::water_ice_17,              },
        // { polymorphs::water_ice_18,              },
        // { polymorphs::water_ice_19,              },
        { polymorphs::nitrogen_ice_beta,                   
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                      std::vector<double>{0.0,  39.0, 60.0, 62.5},
                      std::vector<double>{0.0, 37.39,45.64,46.97}), // Manzhelii (1997)
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                      std::vector<double>{0.0,  2.0,  6.0, 10.0, 30.0, 35.3},
                      std::vector<double>{0.0,0.024,0.907,4.832,34.16,45.53}), // Manzhelli (1997)
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,   
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{0.0, 45.90, 54.39},
                      std::vector<double>{0.0, 11.02, 11.06}), // Johnson (1960)
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{0.0, 25.02, 42.21},
                      std::vector<double>{0.0,  5.42, 10.73}), // Johnson (1960)
            },
        { polymorphs::oxygen_ice_alpha,            
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{0.0,          2.0, 12.97, 22.24},
                      std::vector<double>{0.0, 11.7/31.9988,  1.10,  4.40}), // Johnson (1960)
            },
        { polymorphs::carbon_dioxide_ice,            
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,    3.0,  10.0, 40.0, 70.0, 200.0, 215.0}, 
                     std::vector<double>{0.0, 0.0151,0.5883,19.64,33.38, 56.77, 61.90}), // Manzhelii (1997)
            },
        { polymorphs::methane_ice_alpha, 
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (16.043*si::gram * si::kelvin),
                      std::vector<double>{0.0, 21.35, 61.40, 87.20},
                      std::vector<double>{0.0, 4.325, 8.673, 10.19}), // Johnson (1960)
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (16.043*si::gram * si::kelvin),
                      std::vector<double>{0.0, 10.33, 15.48, 19.94},
                      std::vector<double>{0.0, 0.954, 2.774, 10.91}), // Johnson (1960)
            },
        { polymorphs::argon_ice,   
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0, 90.68},
                      std::vector<double>{0.0, 0.197}), // Johnson (1960)
            },
        // { polymorphs::helium_ice,           },
        { polymorphs::hydrogen_ice,   
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0,    3.04,    5.95,   9.87},
                      std::vector<double>{0.0, 0.02584, 0.06349, 0.2763}), // Johnson (1960)
            },
        // { polymorphs::ammonia_ice,          },
        // { polymorphs::ozone_ice,            },
        { polymorphs::nitrous_oxide_ice,            
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,   3.0,  10.0, 40.0, 60.0, 150.0, 180.0}, 
                     std::vector<double>{0.0,0.0189, 0.822,21.65,31.47, 50.70, 58.28}), // Manzhelii (1997)
            },
        // { polymorphs::sulfur_dioxide_ice,   },
        // { polymorphs::nitric_oxide_ice,     },
        { polymorphs::carbon_monoxide_ice_beta,            
                relation::get_linear_interpolation_function
                    (si::kelvin, si::joule/(28.010*si::gram*si::kelvin),
                     std::vector<double>{0.0,  63.0,  64.0,  65.0,  66.0,  67.0}, 
                     std::vector<double>{0.0, 50.10, 50.58, 51.08, 51.58, 52.29}), // Manzhelii (1997)
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::joule/(28.010*si::gram*si::kelvin),
                     std::vector<double>{  0.0,   6.0,  12.0,  18.0,  24.0,  30.0,  36.0,  42.0,  48.0,  54.0,  61.0}, 
                     std::vector<double>{  0.0, 0.467, 4.375, 11.77, 18.92, 25.04, 30.73, 35.92, 41.45, 48.53, 67.83}), // Manzhelii (1997)
            },
        // { polymorphs::ethane_ice,           },
        // { polymorphs::hydrogen_cyanide_ice, },
        { polymorphs::ethanol_ice,   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::joule/(46.068*si::gram*si::kelvin),
                     std::vector<double>{0.0, 159.2}, 
                     std::vector<double>{0.0,111.46}), // wikipedia data page
            },
        // { polymorphs::formaldehyde_ice,     },
        { polymorphs::formic_acid_ice,   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::joule / (46.026*si::gram*si::kelvin),
                     std::vector<double>{0.0, 281.5}, 
                     std::vector<double>{0.0,  74.5}), // wikipedia data page
            },
        // { polymorphs::perflouromethane_ice, },
        { polymorphs::benzene_ice,   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::joule / (79.109*si::gram*si::kelvin),
                     std::vector<double>{0.0, 278.7}, 
                     std::vector<double>{0.0, 118.4}), // wikipedia data page
            },
        // { polymorphs::pyrimidine_ice,       },
        { polymorphs::quartz_alpha,            // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     10.87, 0.008712, 241200.0, 0.0, 
                     273.0, 848.0), // Johnson (1960) and Perry
            },
        { polymorphs::quartz_beta,             // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     10.95, 0.00550, 0.0, 0.0, 
                     848.0, 1873.0), // Johnson (1960) and Perry
            },
        { polymorphs::crystoballite_alpha,       // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     3.65, 0.0240, 0.0, 0.0,
                     273.0, 523.0), // Johnson (1960) and Perry
            },
        { polymorphs::crystoballite_beta,        // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     17.09, 0.000454, -897200.0, 0.0,
                     523.0, 1973.0), // Johnson (1960) and Perry
            },
        { polymorphs::halite,   // 50.0 * si::joule / (90.442 * si::gram * si::kelvin), // wikipedia data page
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(80.088 * si::gram*si::kelvin), 
                     10.79, 0.00420, 0.0, 0.0,
                      273.0, 1074.0),
            },
        { polymorphs::corundum,   // 750.0*si::joule/(si::kilogram* si::kelvin)
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(101.96 * si::gram*si::kelvin), 
                     22.08, 0.008971, -522500.0, 0.0,
                     273.0, 1973.0), 
            },
        { polymorphs::apatite,   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,   0.7}), // Schön (2015)
            },
        { polymorphs::graphite,   // 0.710 * si::joule / (si::gram * si::kelvin), // wikipedia, Diamond is 0.5091
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     1e-5, 1.0/970e3, 10.0,
                     si::calorie/(12.011 * si::gram*si::kelvin),
                     2.673, 0.002617, -116900.0, 0.0,
                     273.0, 1373.0), // Johnson (1960) and Perry
            },
        { polymorphs::diamond,  // 0.5091* si::joule / (si::gram * si::kelvin), // wikipedia 
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     3e-6, 1.0/67e6, 30.0,
                     si::calorie/(12.011 * si::gram*si::kelvin),
                     2.162, 0.003059, -130300.0, 0.0, 
                     273.0, 1313.0), // Johnson (1960) and Perry
            },
        { polymorphs::calcite,   // 0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(100.087 * si::gram*si::kelvin), 
                     19.68, 0.01189, -307600.0, 0.0, 
                     273.0, 1033.0), 
            },
        { polymorphs::aragonite,   // 0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(100.087 * si::gram*si::kelvin), 
                     19.68, 0.01189, -307600.0, 0.0, 
                     273.0, 1033.0), 
            },
        { polymorphs::orthoclase,   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(278.33 * si::gram*si::kelvin), 
                     69.26, 0.00821, -2331000.0, 0.0, 
                     273.0, 1373.0),
            },
        { polymorphs::andesine,   // 66.0 * si::joule / (268.6 * si::gram * si::kelvin), // Richet (1984)
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(268.6 * si::gram*si::kelvin), 
                     63.13, 0.01500, -1537000.0, 0.0, 
                     273.0, 1673.0), // for anorthite
            },
        { polymorphs::augite,   // 0.7 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988), representative of pyroxenes
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(236.4 * si::gram*si::kelvin), 
                     23.35, 0.008062, -558800.0, 0.0, 
                     273.0, 773.0), // for maganese pyroxenes 
            },
        { polymorphs::forsterite,   // 0.79 * si::joule / (si::gram * si::kelvin), // Cermak (1988), for fayalite/forsterite mix
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(153.31 * si::gram*si::kelvin), 
                     33.57, 0.01907, -879700.0, 0.0, 
                     273.0, 1161.0), // for fayalite
            },
        // { polymorphs::goethite,         },
        { polymorphs::pyrite,   // 0.5 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(119.98 * si::gram*si::kelvin), 
                     10.7, 0.01336, 0.0, 0.0, 
                     273.0, 773.0),
            },
        { polymorphs::hematite,   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.69 * si::gram*si::kelvin), 
                     24.72, 0.01604, -423400.0, 0.0, 
                     273.0, 1097.0),
            },
        { polymorphs::gold,   // 0.129 * si::joule / (si::gram * si::kelvin), // wikipedia, room temperature
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     3.75e-6, 9.86/(165.0*165.0*165.0), 15.0,
                     si::calorie/(196.967 * si::gram*si::kelvin), 
                     5.61, 0.00144, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            },
        { polymorphs::silver,   // 0.233 * si::joule / (si::gram * si::kelvin), // wikipedia
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/600e3, 30.0,
                     si::calorie/(107.868 * si::gram*si::kelvin), 
                     5.6, 0.00150, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            },
        { polymorphs::copper,            // 0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     10.8e-6, 30.6/(344.5*344.5*344.5), 10.0,
                     si::calorie/(63.546 * si::gram*si::kelvin), 
                     5.44, 0.001462, 0.0, 0.0, 
                     273.0, 1357.0), // Johnson (1960) and Perry
            },
        { polymorphs::magnetite,   // 0.6 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(231.53 * si::gram*si::kelvin), 
                     41.17, 0.01882, -979500.0, 0.0, 
                     273.0, 1065.0),
            },
        { polymorphs::chalcocite_alpha,            
                relation::get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.16 * si::gram*si::kelvin), 
                     9.38, 0.0312, 0.0, 0.0, 
                     273.0, 376.0),
            },
        { polymorphs::chalcopyrite,   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,  0.54}), // Cermak (1988), for chalcopyrite
            },

    };


}}

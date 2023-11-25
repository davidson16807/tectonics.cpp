#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidThermalConductivityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::thermal_conductivity<double>, 0,1>;
    table::PartialTable<SolidThermalConductivityTemperatureRelation> thermal_conductivity_as_solid {

        { polymorphs::water_ice_1h,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt/(si::meter*si::kelvin),
                     std::vector<double>{ 2.0,  4.0,  8.0, 10.0, 30.0, 100.0, 173.0, 193.0, 233.0, 253.0, 273.15 }, 
                     std::vector<double>{21.0, 57.0, 87.0, 81.0, 22.0,   6.0,  3.48,  3.19,  2.63,  2.39,   2.22 }), // Ahmad (1994) and Engineering Toolbox
            },
        // { polymorphs::water_ice_1c,              },
        // { polymorphs::water_ice_2,               },
        // { polymorphs::water_ice_3,               },
        // { polymorphs::water_ice_4,               },
        // { polymorphs::water_ice_5,               },
        // { polymorphs::water_ice_6,               },
                // get_interpolated_pressure_function
                //     (si::gigapascal, si::watt/(si::meter*si::kelvin),
                //      std::vector<double>{ 0.96, 2.17}, 
                //      std::vector<double>{ 1.69, 1.79}), // Carretero (2018)
        // { polymorphs::water_ice_7,               },
                // get_interpolated_pressure_function
                //     (si::gigapascal, si::watt/(si::meter*si::kelvin),
                //      std::vector<double>{7.52,  9.97,  19.75 }, 
                //      std::vector<double>{ 4.6,   5.7,   9.85 }), // Carretero (2018)
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
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 45.0, 57.5}, 
                     std::vector<double>{ 2.21, 2.09}), // Manzhelii (1997)
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{  1.1,  1.5,  2.0,  3.3,   3.9,   4.5,   5.0,   6.0,   7.0,   8.0,   9.0,  14.0,  17.0,  24.0,  30.0,  34.0 }, 
                     std::vector<double>{17.08,40.27, 84.9,237.1, 238.5, 199.1, 156.3, 87.82, 50.56, 32.55, 22.73,  7.56,  5.26,  3.33,  2.81,  2.66 }), // Manzhelii (1997)
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          0.17 * si::watt / (si::centimeter * si::kelvin),  // Jezowski (1993)
            },
        // { polymorphs::oxygen_ice_beta,           },
        // { polymorphs::oxygen_ice_alpha,          },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 25.0, 40.0, 50.0, 80.0, 120.0, 210.0}, 
                     std::vector<double>{59.17,27.32,20.24,11.68,  7.79,  4.19}), // Manzhelii (1997)
            },
        { polymorphs::methane_ice_alpha,         // 0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
                relation::get_linear_interpolation_function
                    (si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{18.0, 20.0, 20.4, 21.0},              
                     std::vector<double>{0.7,  2.5,  11.0,  0.7}), // Johnson (1960)
            },
        // { polymorphs::methane_ice_beta,          },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::milliwatt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.0,  8.0, 50.0},              
                     std::vector<double>{4.0, 40.0,  5.0}),  // Johnson (1960)
            },
        { polymorphs::helium_ice,                // 0.1 * si::watt / (si::centimeter * si::kelvin), // Webb (1952)
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.39, 1.70, 2.12, 4.05}, 
                     std::vector<double>{0.32, 0.63, 0.40, 0.01}), // Johnson (1960)
            },
        { polymorphs::hydrogen_ice,              1.819 * si::watt / ( si::meter * si::kelvin ), // wikipedia
            },
        // { polymorphs::ammonia_ice,               },
        { polymorphs::ozone_ice,                 5.21e-4 * si::calorie / (si::second*si::centimeter2*si::kelvin/si::centimeter), // Streng (1961)
            },
        { polymorphs::nitrous_oxide_ice,         
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 23.0, 30.0, 40.0, 60.0, 120.0, 180.0}, 
                     std::vector<double>{17.08,40.27, 10.8, 7.89,  4.96,  3.38}), // Manzhelii (1997)
            },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{22.5, 25.0, 35.0, 45.0, 58.0}, 
                     std::vector<double>{6.37, 5.69, 4.16, 3.34, 2.62}), // Manzhelii (1997)
            },
        // { polymorphs::carbon_monoxide_ice_alpha, },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,    20.0,  80.0, 273.15}, 
                     std::vector<double>{1200.0, 480.0,  30.0, 7.69}), // Perry, with Schön (2015) for standard temperature 
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    5.55 * si::watt / ( si::meter * si::kelvin ), // Wilkens (2011)
            },
        { polymorphs::corundum,                  // field::StateSample<si::thermal_conductivity<double>>(37.0*si::watt/(si::meter * si::kelvin), si::atmosphere, 20.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0, 60.0,  300.0, 400.0, 1400.0}, 
                     std::vector<double>{7.0,  174.0,  36.0,  26.0,    6.0}), // Perry
            },
        { polymorphs::apatite,                   1.38 * si::watt / (si::meter*si::kelvin), // Schön (2015)
            },
        { polymorphs::graphite,                  // 247.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia (middle range value)
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{  7.0, 10.0, 100.0,  300.0, 1400.0}, 
                     std::vector<double>{ 15.0, 81.0,4980.0, 2000.0,  370.0}), // Johnson (1960) and Perry, perpendicular to basal plane
            },
        { polymorphs::diamond,                   2200.0 * si::watt / ( si::meter * si::kelvin ), //wikipedia 
            },
        { polymorphs::calcite,                   3.59 * si::watt / (si::meter * si::kelvin), // Schön (2015)
            },
        { polymorphs::aragonite,                 2.24 * si::watt / (si::meter * si::kelvin), // Schön (2015)
            },
        { polymorphs::orthoclase,          2.31 * si::watt/(si::meter*si::kelvin), // Schön (2015)
            },
        { polymorphs::andesine,            1.69 * si::watt / (si::centimeter * si::kelvin), // Schön (2015), for anorthite
            },
        { polymorphs::augite,              4.66 * si::watt / (si::centimeter * si::kelvin), // Schön (2015)
            },
        { polymorphs::forsterite,          7.69 * si::watt / (si::centimeter * si::kelvin), // Schön (2015)
            },
        { polymorphs::goethite,            2.91 * si::watt / (si::meter * si::kelvin), // Cermak (1988)
            },
        { polymorphs::pyrite,              19.21 * si::watt / (si::meter * si::kelvin), // Schön (2015)
            },
        { polymorphs::hematite,            11.28 * si::watt / (si::meter * si::kelvin), // Schön (2015)
            },
        { polymorphs::gold,              // 314.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,    60.0,  300.0, 1200.0}, 
                     std::vector<double>{1600.0, 2800.0, 380.0,  315.0,  262.0}), // Johnson (1960) and Perry
            },
        { polymorphs::silver,              // 427.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{16500.0, 630.0,  424.0,  358.0}), // Perry
            },
        { polymorphs::copper,              // 401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
                relation::get_linear_interpolation_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{7000.0, 19000.0, 850.0,  398.0,  342.0}), // Johnson (1960)  and Perry
            },
        { polymorphs::magnetite,           5.1 * si::watt / (si::centimeter * si::kelvin), // Schön (2015)
            },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        { polymorphs::chalcopyrite,        8.19 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), for chalcopyrite
            },

    };


}}
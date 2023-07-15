#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidDensityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::density<double>, 0,1>;
    table::PartialTable<SolidDensityTemperatureRelation> density_as_solid {

        { polymorphs::water_ice_1h,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilogram/si::meter3,
                     std::vector<double>{-100.0, -80.0, -60.0, -40.0, -20.0,   0.0}, 
                     std::vector<double>{ 925.7, 924.1, 922.4, 920.8, 919.4, 916.2}), // https://www.engineeringtoolbox.com/ice-thermal-properties-d_576.html
            },
        { polymorphs::water_ice_1c,              0.964 * si::gram/si::centimeter3,                    // Tchijov (2008)
            },
        // { polymorphs::water_ice_2,               },
        // { polymorphs::water_ice_3,               },
        { polymorphs::water_ice_4,               1.314 * si::gram/si::centimeter3,                    // Tchijov (2008)
            },
        // { polymorphs::water_ice_5,               },
        // { polymorphs::water_ice_6,               },
        // { polymorphs::water_ice_7,               },
        { polymorphs::water_ice_8,               1.674 * si::gram/si::centimeter3,                    // Tchijov (2008)
            },
        { polymorphs::water_ice_9,               1.210 * si::gram/si::centimeter3,                    // Tchijov (2008)
            },
        // { polymorphs::water_ice_10,              },
        // { polymorphs::water_ice_11h,             },
        // { polymorphs::water_ice_11o,             },
        { polymorphs::water_ice_12,              1.314 * si::gram/si::centimeter3,                    // Tchijov (2008)
            },
        // { polymorphs::water_ice_13,              },
        // { polymorphs::water_ice_14,              },
        // { polymorphs::water_ice_15,              },
        { polymorphs::water_ice_16,              0.81*si::gram/si::centimeter3, //wikipedia
            },
        // { polymorphs::water_ice_17,              },
        // { polymorphs::water_ice_18,              },
        // { polymorphs::water_ice_19,              },
        { polymorphs::nitrogen_ice_beta,         
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{0.9955,0.9899,0.9862,0.9845,0.9748,0.9669,0.9586}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{1.0296,1.0284,1.0271,1.0255,1.0237,1.0216,1.0192,1.0166,1.0134,1.0095}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{1.396, 1.390, 1.383, 1.377, 1.370}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{1.521, 1.513, 1.510, 1.505, 1.501, 1.496, 1.490, 1.485, 1.475}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_alpha,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{1.537, 1.536, 1.535, 1.535, 1.534, 1.532, 1.531, 1.528, 1.525}), // Prokhvatilov
            },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{1.7056,1.7036,1.6963,1.6852,1.6718,1.6568,1.6403,1.6223,1.6022,1.5789}), // Prokhvatilov
            },
        { polymorphs::methane_ice_alpha,         0.5245*si::gram/si::centimeter3, // Prokhvatilov, @ si::atmosphere, 14.4*si::kelvin
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{0.5228,0.5278,0.5136,0.5087,0.5031,0.4969}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{1.7705,1.7641,1.7518,1.7349,1.7148,1.6919,1.6657,1.6349}), // Prokhvatilov
            },
        { polymorphs::helium_ice,                
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram / si::centimeter3,
                     std::vector<double>{1.0,    4.0,    26.0}, 
                     std::vector<double>{0.1891, 0.2305,  0.3963}), // Johnson (1960)
            },
        { polymorphs::hydrogen_ice,              // 86.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{.08740,.08740,.08738,.08732,.08718,.08706,.08656}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.855, 0.853, 0.848, 0.841, 0.834, 0.826, 0.821}), // Prokhvatilov
            },
        { polymorphs::ozone_ice,                 1354.0  * si::kilogram / si::meter3, //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
            },
        { polymorphs::nitrous_oxide_ice,         1.263 * si::gram/si::centimeter3, // Hudson (2020)
            },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.9378,0.9326,0.9269,0.9238}), // Prokhvatilov
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{1.0293,1.0273,1.0232,1.0202,1.0159,1.0110,1.0054,0.9909,0.9812,0.9689}), // Prokhvatilov
            },
        { polymorphs::ethane_ice,                0.739 * si::gram / si::centimeter3, // Hudson (2020)
            },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        { polymorphs::formic_acid_ice,           0.979 * si::gram / si::centimeter3, // Hudson (2020)
            },
        { polymorphs::perflouromethane_ice,      1980.0 * si::kilogram/si::meter3, // pubchem
            },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              2650.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { polymorphs::quartz_beta,               2533.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { polymorphs::crystoballite_alpha,       2334.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { polymorphs::crystoballite_beta,        2334.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { polymorphs::halite,                    2170.0 * si::kilogram/si::meter3,
            },
        { polymorphs::corundum,                  3970.0 * si::kilogram/si::meter3,
            },
        { polymorphs::apatite,                   3180.0 * si::kilogram/si::meter3,
            },
        { polymorphs::graphite,                  2260.0 * si::kilogram/si::meter3,
            },
        { polymorphs::diamond,                   3513.0  * si::kilogram/si::meter3,
            },
        { polymorphs::calcite,                   2710.0 * si::kilogram/si::meter3,
            },
        { polymorphs::aragonite,                 2930.0 * si::kilogram/si::meter3, // Perry
            },
        { polymorphs::orthoclase,                2560.0 * si::kilogram/si::meter3,
            },
        { polymorphs::andesine,                  2670.0 * si::kilogram/si::meter3,
            },
        { polymorphs::augite,                    3380.0 * si::kilogram/si::meter3,
            },
        { polymorphs::forsterite,                3810.0 * si::kilogram/si::meter3,
            },
        { polymorphs::goethite,                  4300.0 * si::kilogram/si::meter3,
            },
        { polymorphs::pyrite,                    5020.0 * si::kilogram/si::meter3,
            },
        { polymorphs::hematite,                  5250.0 * si::kilogram/si::meter3,
            },
        { polymorphs::gold,                      19300.0 * si::kilogram/si::meter3,
            },
        { polymorphs::silver,                    10500.0 * si::kilogram/si::meter3,
            },
        { polymorphs::copper,                    8960.0 * si::kilogram/si::meter3,
            },
        { polymorphs::magnetite,                 5170.0 * si::kilogram/si::meter3,
            },
        { polymorphs::chalcocite_alpha,          5600.0 * si::kilogram/si::meter3,
            },
        // { polymorphs::chalcocite_beta,           },
        { polymorphs::chalcopyrite,              4200.0 *  si::kilogram/si::meter3, //wikipedia
            },

    };

}}
#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/GenericRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidTensileModulusTemperatureRelation = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
    table::PartialTable<SolidTensileModulusTemperatureRelation> tensile_modulus_as_solid {

        { polymorphs::water_ice_1h,              9.332 * si::gigapascal,                           // gammon (1983)
            },
        // { polymorphs::water_ice_1c,              },
        // { polymorphs::water_ice_2,               },
        // { polymorphs::water_ice_3,               },
        // { polymorphs::water_ice_4,               },
        // { polymorphs::water_ice_5,               },
        // { polymorphs::water_ice_6,               },
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
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{18.18, 17.95, 17.81, 17.66, 17.31, 16.94, 16.54}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{21.48, 21.45, 21.37, 21.29, 21.18, 21.06, 20.90, 20.70, 20.38, 19.98}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 7.55,  7.48,  7.44,  7.37,  7.30}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{15.84, 18.33, 19.62, 20.85, 21.92, 23.04, 23.79, 24.34, 23.41}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_alpha,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{34.54, 33.47, 32.38, 31.44, 30.50, 29.32, 27.77, 25.66, 21.85, 15.14}), // Prokhvatilov
            },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{111.7,111.45,110.66,109.09,106.22,102.60, 97.72, 91.48, 84.14, 75.37}), // Prokhvatilov
            },
        { polymorphs::methane_ice_alpha,         30.83 * si::gigapascal, // Manzhelii (2003), @ si::standard_pressure, 30.0*si::kelvin
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 25.74, 24.51, 23.31, 22.38, 20.85, 19.39}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{39.11, 38.97, 37.07, 35.22, 33.46, 30.08, 27.08, 23.70}), // Prokhvatilov
            },
        { polymorphs::helium_ice,                
                relation::get_linear_interpolation_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 2.43,   2.32,  2.17}), // Prokhvatilov 
            },
        { polymorphs::hydrogen_ice,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{3.135, 3.115, 3.089, 3.056, 3.015, 2.966, 2.924}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        // { polymorphs::ammonia_ice,               },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{11.27, 10.88, 10.49, 10.33}), // Prokhvatilov
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{37.69, 37.30, 36.83, 36.33, 35.73, 34.85, 33.71, 30.15, 27.68, 24.73}), // Prokhvatilov
            },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              0.8680e11*si::pascal, 
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        // { polymorphs::halite,                    0.4947e11*si::pascal, // values from Perry produce negative bulk modulus so they are commented out
        // { polymorphs::corundum,                  },
        { polymorphs::apatite,                   1.667e11*si::pascal,
            },
        // { polymorphs::graphite,                  },
        { polymorphs::diamond,                   1050.0 * si::gigapascal, // wikipedia, for diamond
            },
        // { polymorphs::calcite,                   },
        // { polymorphs::aragonite,                 },
        { polymorphs::forsterite,          3.2848e11*si::pascal,
            },
        // { polymorphs::goethite,         },
        { polymorphs::pyrite,              3.818e11*si::pascal,
            },
        { polymorphs::hematite,            2.4243e11*si::pascal, 
            },
        { polymorphs::gold,                ((76.0+81.0)/2.0)*si::gigapascal, // https://www.azom.com/properties.aspx?ArticleID=59(1.9244e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            },
        { polymorphs::silver,              1.2399e11*si::pascal,
            },
        { polymorphs::copper,              1.683e11*si::pascal,
            },
        { polymorphs::magnetite,           2.730e11*si::pascal,
            },
        // { polymorphs::chalcocite,       },
        // { polymorphs::chalcopyrite,     },
    };

}}
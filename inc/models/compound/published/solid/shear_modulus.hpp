#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/relation/GenericRelation.hpp>
#include <models/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidShearModulusTemperatureRelation = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
    table::PartialTable<SolidShearModulusTemperatureRelation> shear_modulus_as_solid {

        { polymorphs::water_ice_1h,              3.521 * si::gigapascal,                           // gammon (1983)
            },
        // { polymorphs::water_ice_1c,              },
        { polymorphs::water_ice_2,               6.2e4 * si::bar,                                     // Gagnon (1990)
            },
        { polymorphs::water_ice_3,               4.6e4 * si::bar,                                     // Gagnon (1990)
            },
        // { polymorphs::water_ice_4,               },
        { polymorphs::water_ice_5,               6.1e4 * si::bar,                                     // Gagnon (1990)
            },
        { polymorphs::water_ice_6,               7.5e4 * si::bar,                                     // Gagnon (1990)
            },
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
                     std::vector<double>{7.008, 6.935, 6.893, 6.893, 6.472, 6.645, 6.540}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{8.046, 8.036, 8.008, 7.978, 7.946, 7.911, 7.857, 7.783, 7.688, 7.553}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 2.61,  2.58,  2.57,  2.55,  2.52}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{ 5.57,  6.51,  7.00,  7.48,  7.91,  8.37,  8.68,  8.92,  8.56}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_alpha,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{ 12.9, 12.45, 12.01, 11.63, 11.25, 10.77, 10.16,  9.32,  7.84,  5.31}), // Prokhvatilov
            },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{44.10, 44.05, 43.81, 43.25, 42.26, 40.84, 38.80, 36.31, 33.41, 29.89}), // Prokhvatilov
            },
        { polymorphs::methane_ice_alpha,         12.4*si::kilobar, // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{  9.96,  9.44,  8.95,  8.60,  7.99,  7.39}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{15.64, 15.59, 14.83, 14.06, 13.28, 11.94, 10.66,  9.44}), // Prokhvatilov
            },
        { polymorphs::helium_ice,                
                relation::get_linear_interpolation_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 1.16,   1.05,  1.00}), // Prokhvatilov 
            },
        { polymorphs::hydrogen_ice,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.285, 1.278, 1.267, 1.253, 1.237, 1.216, 1.199}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{43.28, 43.14, 42.70, 41.67, 40.18, 49.64, 47.83}), // Prokhvatilov
            },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{ 4.08,  3.94,  3.80,  3.74}), // Prokhvatilov
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{15.47, 15.32, 15.13, 14.94, 14.70, 14.36, 13.95, 12.60, 11.59, 10.34}), // Prokhvatilov
            },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              0.5820e11*si::pascal, 
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    14.9 * si::gigapascal, // 0.1287e11*si::pascal, // values from Perry produce negative bulk modulus so they are commented out
            },
        { polymorphs::corundum,                  162.1 * si::gigapascal, // Mavko (2009)
            },
        // { polymorphs::apatite,                   },
        // { polymorphs::graphite,                  },
        // { polymorphs::diamond,                   },
        { polymorphs::calcite,                   32.0 * si::gigapascal, // Schön (2015) 
            },
        { polymorphs::aragonite,                 38.5 * si::gigapascal, // Schön (2015) 
            },
        { polymorphs::orthoclase,          27.3 * si::gigapascal, // Schön (2015)
            },
        { polymorphs::andesine,            40.0 * si::gigapascal, // Schön (2015), for anorthite
            },
        { polymorphs::augite,              57.0*si::gigapascal, // Schön (2015)
            },
        { polymorphs::forsterite,          0.6515e11*si::pascal,
            },
        // { polymorphs::goethite,         },
        { polymorphs::pyrite,              1.094e11*si::pascal,
            },
        { polymorphs::hematite,            0.8569e11*si::pascal, 
            },
        { polymorphs::gold,                ((26.0+30.0)/2.0*si::gigapascal), // https://www.azom.com/properties.aspx?ArticleID=598 0.4200e11*si::pascal,    // values from Perry produce negative bulk modulus so they are commented out
            },
        { polymorphs::silver,              0.4612e11*si::pascal,
            },
        { polymorphs::copper,              0.757e11*si::pascal,
            },
        { polymorphs::magnetite,           0.971e11*si::pascal,
            },
        // { polymorphs::chalcocite,       },
        // { polymorphs::chalcopyrite,     },

    };

}}
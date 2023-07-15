#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/GenericRelation.hpp>
#include <models/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidBulkModulusTemperatureRelation = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
    table::PartialTable<SolidBulkModulusTemperatureRelation> bulk_modulus_as_solid {

        { polymorphs::water_ice_1h,              8.899 * si::gigapascal, // gammon (1983)
            },
        // { polymorphs::water_ice_1c,              },
        { polymorphs::water_ice_2,               14.39e4 * si::bar, // Gagnon (1990)
            },
        { polymorphs::water_ice_3,               9.6e4 * si::bar // Gagnon (1990)
            },
        // { polymorphs::water_ice_4,               },
        { polymorphs::water_ice_5,               13.86e4 * si::bar // Gagnon (1990)
            },
        { polymorphs::water_ice_6,               18.48e4 * si::bar // Gagnon (1990)
            },
        { polymorphs::water_ice_7,               23.9*si::gigapascal //Fei (1993)
            },
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
                     std::vector<double>{14.91, 14.54, 14.26, 14.26, 13.26, 12.52, 11.71}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{21.71, 21.65, 21.54, 21.38, 21.15, 20.81, 20.42, 20.01, 19.42, 18.76}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{24.76, 24.27, 23.81, 23.33, 22.84}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{33.67, 32.93, 32.78, 32.42, 31.92, 31.06, 30.70, 29.97, 29.22}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_alpha,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{35.75, 35.73, 35.56, 35.47, 35.29, 35.17, 35.01, 34.74, 34.41, 34.19}), // Prokhvatilov
            },
        { polymorphs::carbon_dioxide_ice,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{79.73, 79.06, 78.53, 76.16, 72.73, 70.12, 67.67, 63.46, 58.25, 52.45}), // Prokhvatilov
            },
        { polymorphs::methane_ice_alpha,         19.69 * si::kilobar, // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 20.72, 20.30, 19.66, 18.82, 17.85, 17.11}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{26.07, 25.98, 24.72, 24.61, 23.24, 20.89, 19.62, 17.38}), // Prokhvatilov
            },
        // { polymorphs::helium_ice,                },
        { polymorphs::hydrogen_ice,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.861, 1.849, 1.834, 1.815, 1.790, 1.761, 1.737}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{61.24, 61.02, 59.16, 56.23, 53.20, 49.64, 47.83}), // Prokhvatilov
            },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{15.65, 15.23, 14.80, 14.55}), // Prokhvatilov
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{22.29, 22.04, 21.70, 21.34, 20.91, 20.24, 19.24, 16.52, 15.07, 13.56}), // Prokhvatilov
            },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              37.0 * si::gigapascal, // Schön (2015)
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    24.8 * si::gigapascal, // Mavko (2009)
            },
        { polymorphs::corundum,                  252.9 * si::gigapascal, // Mavko (2009)
            },
        // { polymorphs::apatite,                   },
        // { polymorphs::graphite,                  },
        { polymorphs::diamond,                   443.0 * si::gigapascal,  // wikipedia, for diamond
            },
        { polymorphs::calcite,                   73.0 * si::gigapascal, // Schön (2015)
            },
        { polymorphs::aragonite,                 47.0 * si::gigapascal, // Schön (2015)
            },
        { polymorphs::orthoclase,                46.8 * si::gigapascal, // Schön (2015)
            },
        { polymorphs::andesine,                  84.0 * si::gigapascal, // Schön (2015), for anorthite
            },
        { polymorphs::augite,                    94.1 * si::gigapascal, // Schön (2015)
            },
        { polymorphs::forsterite,                129.6 * si::gigapascal, // Schön (2015)
            },
        // { polymorphs::goethite,                  },
        { polymorphs::pyrite,                    143.0 * si::gigapascal, // Schön (2015)
            },
        // { polymorphs::hematite,                  },
        // { polymorphs::gold,                      },
        // { polymorphs::silver,                    },
        // { polymorphs::copper,                    },
        // { polymorphs::magnetite,                 },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        // { polymorphs::chalcopyrite,              },

    };

}}
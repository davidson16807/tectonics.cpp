#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidBulkModulusTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidBulkModulusTemperatureRelation> bulk_modulus_as_solid {
        { ids::water,              8.899 * si::gigapascal,                           // gammon (1983)
            },
        { ids::nitrogen,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{14.91, 14.54, 14.26, 14.26, 13.26, 12.52, 11.71}), // Prokhvatilov
            },
        { ids::oxygen,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{24.76, 24.27, 23.81, 23.33, 22.84}), // Prokhvatilov
            },
        { ids::carbon_dioxide,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{79.73, 79.06, 78.53, 76.16, 72.73, 70.12, 67.67, 63.46, 58.25, 52.45}), // Prokhvatilov
            },
        { ids::methane,              19.69 * si::kilobar, // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { ids::argon,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{26.07, 25.98, 24.72, 24.61, 23.24, 20.89, 19.62, 17.38}), // Prokhvatilov
            },
        // { ids::helium,           },
        { ids::hydrogen,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.861, 1.849, 1.834, 1.815, 1.790, 1.761, 1.737}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { ids::ammonia,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{61.24, 61.02, 59.16, 56.23, 53.20, 49.64, 47.83}), // Prokhvatilov
            },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                      
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{15.65, 15.23, 14.80, 14.55}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              37.0 * si::gigapascal, // Schön (2015)
            },
        { ids::halite,              24.8 * si::gigapascal, // Mavko (2009)
            },
        { ids::corundum,            252.9 * si::gigapascal, // Mavko (2009)
            },
        // { ids::apatite,          },
        // { ids::carbon,           },
        { ids::calcite,             73.0 * si::gigapascal, // Schön (2015)
            },
        { ids::orthoclase,          46.8 * si::gigapascal, // Schön (2015)
            },
        { ids::andesine,            84.0 * si::gigapascal, // Schön (2015), for anorthite
            },
        { ids::augite,              94.1 * si::gigapascal, // Schön (2015)
            },
        { ids::forsterite,          129.6 * si::gigapascal, // Schön (2015)
            },
        // { ids::goethite,         },
        { ids::pyrite,              143.0 * si::gigapascal, // Schön (2015)
            },
        // { ids::hematite,         },
        // { ids::gold,             },
        // { ids::silver,           },
        // { ids::copper,           },
        // { ids::magnetite,        },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
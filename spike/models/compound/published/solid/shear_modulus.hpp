#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidShearModulusTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,0,1>;
    std::map<int, SolidShearModulusTemperatureRelation> shear_modulus_as_solid {
        { ids::water,               3.521 * si::gigapascal,                           // gammon (1983)
            },
        { ids::nitrogen,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{7.008, 6.935, 6.893, 6.893, 6.472, 6.645, 6.540}), // Prokhvatilov
            },
        { ids::oxygen,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 2.61,  2.58,  2.57,  2.55,  2.52}), // Prokhvatilov
            },
        { ids::carbon_dioxide,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{44.10, 44.05, 43.81, 43.25, 42.26, 40.84, 38.80, 36.31, 33.41, 29.89}), // Prokhvatilov
            },
        { ids::methane,             12.4*si::kilobar, // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { ids::argon,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{15.64, 15.59, 14.83, 14.06, 13.28, 11.94, 10.66,  9.44}), // Prokhvatilov
            },
        { ids::helium,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 1.16,   1.05,  1.00}), // Prokhvatilov 
            },
        { ids::hydrogen,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.285, 1.278, 1.267, 1.253, 1.237, 1.216, 1.199}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { ids::ammonia,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{43.28, 43.14, 42.70, 41.67, 40.18, 49.64, 47.83}), // Prokhvatilov
            },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                     
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{ 4.08,  3.94,  3.80,  3.74}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              0.5820e11*si::pascal, 
            },
        { ids::halite,              14.9 * si::gigapascal, // 0.1287e11*si::pascal, // values from Perry produce negative bulk modulus so they are commented out
            },
        { ids::corundum,            162.1 * si::gigapascal, // Mavko (2009)
            },
        // { ids::apatite,          },
        // { ids::carbon,           },
        { ids::calcite,             32.0 * si::gigapascal, // Schön (2015) 
            },
        { ids::orthoclase,          27.3 * si::gigapascal, // Schön (2015)
            },
        { ids::andesine,            40.0 * si::gigapascal, // Schön (2015), for anorthite
            },
        { ids::augite,              57.0*si::gigapascal, // Schön (2015)
            },
        { ids::forsterite,          0.6515e11*si::pascal,
            },
        // { ids::goethite,         },
        { ids::pyrite,              1.094e11*si::pascal,
            },
        { ids::hematite,            0.8569e11*si::pascal, 
            },
        { ids::gold,                ((26.0+30.0)/2.0*si::gigapascal), // https://www.azom.com/properties.aspx?ArticleID=598 0.4200e11*si::pascal,    // values from Perry produce negative bulk modulus so they are commented out
            },
        { ids::silver,              0.4612e11*si::pascal,
            },
        { ids::copper,              0.757e11*si::pascal,
            },
        { ids::magnetite,           0.971e11*si::pascal,
            },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/relation/GenericRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidTensileModulusTemperatureRelation = relation::GenericRelation<si::temperature<double>,si::pressure<double>>;
    table::PartialTable<SolidTensileModulusTemperatureRelation> tensile_modulus_as_solid {
        { compounds::water,               9.332 * si::gigapascal,                           // gammon (1983)
            },
        { compounds::nitrogen,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{18.18, 17.95, 17.81, 17.66, 17.31, 16.94, 16.54}), // Prokhvatilov
            },
        { compounds::oxygen,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 7.55,  7.48,  7.44,  7.37,  7.30}), // Prokhvatilov
            },
        { compounds::carbon_dioxide,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{111.7,111.45,110.66,109.09,106.22,102.60, 97.72, 91.48, 84.14, 75.37}), // Prokhvatilov
            },
        { compounds::methane,             30.83 * si::gigapascal, // Manzhelii (2003), @ si::standard_pressure, 30.0*si::kelvin
            },
        { compounds::argon,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{39.11, 38.97, 37.07, 35.22, 33.46, 30.08, 27.08, 23.70}), // Prokhvatilov
            },
        { compounds::helium,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 2.43,   2.32,  2.17}), // Prokhvatilov 
            },
        { compounds::hydrogen,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{3.135, 3.115, 3.089, 3.056, 3.015, 2.966, 2.924}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        // { compounds::ammonia,          },
        // { compounds::ozone,            },
        // { compounds::nitrous_oxide,    },
        // { compounds::sulfur_dioxide,   },
        // { compounds::nitric_oxide,     },
        { compounds::carbon_monoxide,                   
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{11.27, 10.88, 10.49, 10.33}), // Prokhvatilov
            },
        // { compounds::ethane,           },
        // { compounds::hydrogen_cyanide, },
        // { compounds::ethanol,          },
        // { compounds::formaldehyde,     },
        // { compounds::formic_acid,      },
        // { compounds::perflouromethane, },
        // { compounds::benzene,          },
        // { compounds::pyrimidine,       },
        { compounds::quartz,              0.8680e11*si::pascal, 
            },
        // { compounds::halite,           0.4947e11*si::pascal, // values from Perry produce negative bulk modulus so they are commented out
            // }
        // { compounds::corundum,         },
        { compounds::apatite,             1.667e11*si::pascal,
            },
        // { compounds::carbon,           },
        // { compounds::calcite,          },
        // { compounds::orthoclase,       },
        // { compounds::andesine,         },
        // { compounds::augite,           },
        { compounds::forsterite,          3.2848e11*si::pascal,
            },
        // { compounds::goethite,         },
        { compounds::pyrite,              3.818e11*si::pascal,
            },
        { compounds::hematite,            2.4243e11*si::pascal, 
            },
        { compounds::gold,                ((76.0+81.0)/2.0)*si::gigapascal, // https://www.azom.com/properties.aspx?ArticleID=59(1.9244e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            },
        { compounds::silver,              1.2399e11*si::pascal,
            },
        { compounds::copper,              1.683e11*si::pascal,
            },
        { compounds::magnetite,           2.730e11*si::pascal,
            },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };

}}
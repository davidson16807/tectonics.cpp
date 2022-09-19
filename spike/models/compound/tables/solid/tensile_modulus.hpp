#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::StateFunction<si::pressure<double>>> tensile_modulus_as_solid {
        { ids::water,                   relation::state_invariant(9.332 * si::gigapascal),                           // gammon (1983)
            },
        { ids::nitrogen,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{18.18, 17.95, 17.81, 17.66, 17.31, 16.94, 16.54}), // Prokhvatilov
            },
        { ids::oxygen,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 7.55,  7.48,  7.44,  7.37,  7.30}), // Prokhvatilov
            },
        { ids::carbon_dioxide,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{111.7,111.45,110.66,109.09,106.22,102.60, 97.72, 91.48, 84.14, 75.37}), // Prokhvatilov
            },
        { ids::methane,                   relation::state_invariant(30.83 * si::gigapascal), // Manzhelii (2003), @ si::standard_pressure, 30.0*si::kelvin
            },
        { ids::argon,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{39.11, 38.97, 37.07, 35.22, 33.46, 30.08, 27.08, 23.70}), // Prokhvatilov
            },
        { ids::helium,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 2.43,   2.32,  2.17}), // Prokhvatilov 
            },
        { ids::hydrogen,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{3.135, 3.115, 3.089, 3.056, 3.015, 2.966, 2.924}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        // { ids::ammonia,          },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                   
                relation::get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{11.27, 10.88, 10.49, 10.33}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,                   relation::state_invariant(0.8680e11*si::pascal), 
            },
        // { ids::halite,           // relation::state_invariant(0.4947e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            // }
        // { ids::corundum,         },
        { ids::apatite,                   relation::state_invariant(1.667e11*si::pascal),
            },
        // { ids::carbon,           },
        // { ids::calcite,          },
        // { ids::orthoclase,       },
        // { ids::andesine,         },
        // { ids::augite,           },
        { ids::forsterite,                   relation::state_invariant(3.2848e11*si::pascal),
            },
        // { ids::goethite,         },
        { ids::pyrite,                   relation::state_invariant(3.818e11*si::pascal),
            },
        { ids::hematite,                   relation::state_invariant(2.4243e11*si::pascal), 
            },
        { ids::gold,                   relation::state_invariant((76.0+81.0)/2.0*si::gigapascal), // https://www.azom.com/properties.aspx?ArticleID=598 // relation::state_invariant(1.9244e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            },
        { ids::silver,                   relation::state_invariant(1.2399e11*si::pascal),
            },
        { ids::copper,                   relation::state_invariant(1.683e11*si::pascal),
            },
        { ids::magnetite,                   relation::state_invariant(2.730e11*si::pascal),
            },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
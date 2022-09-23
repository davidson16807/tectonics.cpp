#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidTensileYieldStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,0,1>;
    std::map<int, SolidTensileYieldStrengthTemperatureRelation> tensile_yield_strength_as_solid {
        { ids::water,               1.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
            },
        { ids::nitrogen,            
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{175.0, 130.0, 100.0,  80.0,  78.0,  76.0,  70.0}), // Prokhvatilov
            },
        // { ids::oxygen,           },
        // { ids::carbon_dioxide,   },
        { ids::methane,            
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{178.0, 159.0, 145.0, 137.0, 135.0, 117.0}), // Prokhvatilov
            },
        { ids::argon,            
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  20.0,  30.0,  40.0,  50.0,  60.0,  75.0}, 
                     std::vector<double>{151.0, 107.0,  82.0,  61.0,  49.0,  43.0,  20.0}), // Prokhvatilov
            },
        // { ids::helium,           },
        { ids::hydrogen,            
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  1.4,   2.0,   3.0,   4.2,   6.0,   8.0,  10.0,  12.0,  13.0}, 
                     std::vector<double>{  8.4,   7.6,   7.0,   6.6,   5.5,   4.3,   3.8,   3.0,   1.6}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        // { ids::ammonia,          },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        // { ids::carbon_monoxide,  },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              48.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        { ids::halite,              1.65 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { ids::corundum,         },
        // { ids::apatite,          },
        { ids::carbon,              14.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { ids::calcite,          },
        // { ids::orthoclase,       },
        // { ids::andesine,         },
        // { ids::augite,           },
        // { ids::forsterite,       },
        // { ids::goethite,         },
        // { ids::pyrite,           },
        // { ids::hematite,         },
        { ids::gold,                300.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { ids::silver,           },
        { ids::copper,              70.0 * si::megapascal, // engineering toolbox
            },
        // { ids::magnetite,        },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
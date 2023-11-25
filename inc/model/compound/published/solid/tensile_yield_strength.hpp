#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidTensileYieldStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidTensileYieldStrengthTemperatureRelation> tensile_yield_strength_as_solid {

        { polymorphs::water_ice_1h,              1.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
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
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{175.0, 130.0, 100.0,  80.0,  78.0,  76.0,  70.0}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{175.0, 130.0, 100.0,  80.0,  78.0,  76.0,  70.0}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        // { polymorphs::oxygen_ice_gamma,          },
        // { polymorphs::oxygen_ice_beta,           },
        // { polymorphs::oxygen_ice_alpha,          },
        // { polymorphs::carbon_dioxide_ice,        },
        { polymorphs::methane_ice_alpha,         
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{178.0, 159.0, 145.0, 137.0, 135.0, 117.0}), // Prokhvatilov
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{178.0, 159.0, 145.0, 137.0, 135.0, 117.0}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  20.0,  30.0,  40.0,  50.0,  60.0,  75.0}, 
                     std::vector<double>{151.0, 107.0,  82.0,  61.0,  49.0,  43.0,  20.0}), // Prokhvatilov
            },
        // { polymorphs::helium_ice,                },
        { polymorphs::hydrogen_ice,              
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  1.4,   2.0,   3.0,   4.2,   6.0,   8.0,  10.0,  12.0,  13.0}, 
                     std::vector<double>{  8.4,   7.6,   7.0,   6.6,   5.5,   4.3,   3.8,   3.0,   1.6}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        // { polymorphs::ammonia_ice,               },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        // { polymorphs::carbon_monoxide_ice_beta,  },
        // { polymorphs::carbon_monoxide_ice_alpha, },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              48.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    1.65 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { polymorphs::corundum,                  },
        // { polymorphs::apatite,                   },
        { polymorphs::graphite,                  14.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { polymorphs::diamond,                   },
        // { polymorphs::calcite,          },
        // { polymorphs::orthoclase,       },
        // { polymorphs::andesine,         },
        // { polymorphs::augite,           },
        // { polymorphs::forsterite,       },
        // { polymorphs::goethite,         },
        // { polymorphs::pyrite,           },
        // { polymorphs::hematite,         },
        { polymorphs::gold,                300.0 * si::megapascal, // https://material-properties.org/strength-of-materials-tensile-yield/
            },
        // { polymorphs::silver,           },
        { polymorphs::copper,              70.0 * si::megapascal, // engineering toolbox
            },
        // { polymorphs::magnetite,                 },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        // { polymorphs::chalcopyrite,              },
    };

}}
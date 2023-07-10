#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidCompressiveYieldStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidCompressiveYieldStrengthTemperatureRelation> compressive_yield_strength_as_solid {
        { ids::water,              6.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
            },
        // { ids::nitrogen,         },
        // { ids::oxygen,           },
        // { ids::carbon_dioxide,   },
        // { ids::methane,          },
        // { ids::argon,            },
        // { ids::helium,           },
        // { ids::hydrogen,         },
        { ids::ammonia,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0}, 
                     std::vector<double>{580.0, 480.0, 230.0, 150.0, 110.0, 200.0}), // Prokhvatilov
            },
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
        // { ids::quartz,           },
        { ids::halite,              15.0 * si::megapascal, // Bauer (2019)
            },
        // { ids::corundum,         },
        // { ids::apatite,          },
        // { ids::carbon,           },
        // { ids::calcite,          },
        // { ids::orthoclase,       },
        // { ids::andesine,         },
        // { ids::augite,           },
        // { ids::forsterite,       },
        // { ids::goethite,         },
        // { ids::pyrite,           },
        // { ids::hematite,         },
        // { ids::gold,             },
        // { ids::silver,           },
        // { ids::copper,           },
        // { ids::magnetite,        },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
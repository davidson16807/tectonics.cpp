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
        { compounds::water,              6.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
            },
        // { compounds::nitrogen,         },
        // { compounds::oxygen,           },
        // { compounds::carbon_dioxide,   },
        // { compounds::methane,          },
        // { compounds::argon,            },
        // { compounds::helium,           },
        // { compounds::hydrogen,         },
        { compounds::ammonia,        
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0}, 
                     std::vector<double>{580.0, 480.0, 230.0, 150.0, 110.0, 200.0}), // Prokhvatilov
            },
        // { compounds::ozone,            },
        // { compounds::nitrous_oxide,    },
        // { compounds::sulfur_dioxide,   },
        // { compounds::nitric_oxide,     },
        // { compounds::carbon_monoxide,  },
        // { compounds::ethane,           },
        // { compounds::hydrogen_cyanide, },
        // { compounds::ethanol,          },
        // { compounds::formaldehyde,     },
        // { compounds::formic_acid,      },
        // { compounds::perflouromethane, },
        // { compounds::benzene,          },
        // { compounds::pyrimidine,       },
        // { compounds::quartz,           },
        { compounds::halite,              15.0 * si::megapascal, // Bauer (2019)
            },
        // { compounds::corundum,         },
        // { compounds::apatite,          },
        // { compounds::carbon,           },
        // { compounds::calcite,          },
        // { compounds::orthoclase,       },
        // { compounds::andesine,         },
        // { compounds::augite,           },
        // { compounds::forsterite,       },
        // { compounds::goethite,         },
        // { compounds::pyrite,           },
        // { compounds::hematite,         },
        // { compounds::gold,             },
        // { compounds::silver,           },
        // { compounds::copper,           },
        // { compounds::magnetite,        },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };

}}
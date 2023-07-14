#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidShearYieldStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidShearYieldStrengthTemperatureRelation> shear_yield_strength_as_solid {
        { compounds::water,               1.1 * si::megapascal, // brittle, effectively the same as fracture strength
            },
        // { compounds::nitrogen,         },
        // { compounds::oxygen,           },
        // { compounds::carbon_dioxide,   },
        // { compounds::methane,          },
        // { compounds::argon,            },
        // { compounds::helium,           },
        // { compounds::hydrogen,         },
        // { compounds::ammonia,          },
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
        // { compounds::halite,           },
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
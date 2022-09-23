#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <models/compound/relation/Relation.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidTensileFractureStrengthTemperatureRelation = relation::Relation<si::temperature<double>,si::pressure<double>, math::PolynomialRailyard<float,0,1>>;
    std::map<int, SolidTensileFractureStrengthTemperatureRelation> tensile_fracture_strength_as_solid {
        { ids::water,                1.1 * si::megapascal,                             // Frederking (1989)
            },
        // { ids::nitrogen,          },
        { ids::oxygen,               0.31 * si::megapascal, // Bates 1955, @ 1atm, 45.0K
            },
        // { ids::carbon_dioxide,    },
        // { ids::methane,           },
        // { ids::argon,             },
        // { ids::helium,            },
        // { ids::hydrogen,          },
        // { ids::ammonia,           },
        // { ids::ozone,             },
        // { ids::nitrous_oxide,     },
        // { ids::sulfur_dioxide,    },
        // { ids::nitric_oxide,      },
        // { ids::carbon_monoxide,   },
        // { ids::ethane,            },
        // { ids::hydrogen_cyanide,  },
        // { ids::ethanol,           },
        // { ids::formaldehyde,      },
        // { ids::formic_acid,       },
        // { ids::perflouromethane,  },
        // { ids::benzene,           },
        // { ids::pyrimidine,        },
        // { ids::quartz,            },
        // { ids::halite,            },
        // { ids::corundum,          },
        // { ids::apatite,           },
        // { ids::carbon,            },
        // { ids::calcite,           },
        // { ids::orthoclase,        },
        // { ids::andesine,          },
        // { ids::augite,            },
        // { ids::forsterite,        },
        // { ids::goethite,          },
        // { ids::pyrite,            },
        // { ids::hematite,          },
        // { ids::gold,              },
        // { ids::silver,            },
        // { ids::copper,            },
        // { ids::magnetite,         },
        // { ids::chalcocite,        },
        // { ids::chalcopyrite,      },
    };

}}
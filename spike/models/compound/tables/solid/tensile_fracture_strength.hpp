#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::StateFunction<si::pressure<double>>> tensile_fracture_strength_as_solid {
        { ids::water,           state_invariant(1.1 * si::megapascal),                             // Frederking (1989)
            },
        // { ids::nitrogen,          },
        { ids::oxygen,           state_invariant(0.31 * si::megapascal), // Bates 1955, @ 1atm, 45.0K
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
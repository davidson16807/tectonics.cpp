#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace table { 

    std::map<int, relation::StateFunction<si::dynamic_viscosity<double>>> dynamic_viscosity_as_solid {
        { ids::water,                 
                state_invariant(1e13 * si::poise),                                 // reference by Carey (1953)
            },
        // { ids::nitrogen,              
                // relation::StateFunction<si::dynamic_viscosity<double>>([](si::pressure<double> p, si::temperature<double> T){ 
                //     return math::mix(2.5e9, 0.6e9, math::linearstep(45.0, 56.0, T/si::kelvin))*si::pascal*si::second;
                // }), // Yamashita 2010
            // },
        // { ids::oxygen,           },
        { ids::carbon_dioxide,      state_invariant(1e14 * si::pascal*si::second), // Yamashita (1997) @1 bar, 180K
            },
        { ids::methane,             state_invariant(1e11 * si::pascal*si::second), // Yamashita (1997), @ 0.1*si::megapascal, 77.0*si::kelvin
            },
        // { ids::argon,            },
        // { ids::helium,           },
        // { ids::hydrogen,         },
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
        // { ids::quartz,           },
        { ids::halite,              state_invariant(1e17 * si::poise), // various sources, Carey (1953) cites this number from Weinberg (1927), and Mukherjee (2010), provides a literature review and findings from salt diapirs. Science is weird.
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
        // { ids::magnetite,        // 3e8 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for venusian lava flows, middle of range on log scale
            // },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };


}}
#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/ExponentiatedPolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidDynamicViscosityTemperatureRelation = relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,si::dynamic_viscosity<double>, 0,1>;
    table::PartialTable<SolidDynamicViscosityTemperatureRelation> dynamic_viscosity_as_solid {
        { compounds::water,               1e13 * si::poise,                                 // reference by Carey (1953)
            },
        // { compounds::nitrogen,              
                // relation::StateFunction<si::dynamic_viscosity<double>>([](si::pressure<double> p, si::temperature<double> T){ 
                //     return math::mix(2.5e9, 0.6e9, math::linearstep(45.0, 56.0, T/si::kelvin))*si::pascal*si::second;
                // }), // Yamashita 2010
            // },
        // { compounds::oxygen,           },
        { compounds::carbon_dioxide,      1e14 * si::pascal*si::second, // Yamashita (1997) @1 bar, 180K
            },
        { compounds::methane,             1e11 * si::pascal*si::second, // Yamashita (1997), @ 0.1*si::megapascal, 77.0*si::kelvin
            },
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
        { compounds::halite,              1e17 * si::poise, // various sources, Carey (1953) cites this number from Weinberg (1927), and Mukherjee (2010), provides a literature review and findings from salt diapirs. Science is weird.
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
        // { compounds::magnetite,        // 3e8 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for venusian lava flows, middle of range on log scale
            // },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };


}}
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

    using LiquidExtinctionCoefficientWavenumberRelation = relation::Relation<si::wavenumber<double>,double, math::PolynomialRailyard<float,0,1>>;
    std::map<int, LiquidExtinctionCoefficientWavenumberRelation> extinction_coefficient_as_liquid {

        { ids::water, 
                    relation::get_spectral_linear_interpolation_function_of_wavelength
                        (si::micrometer, 1.0,
                         std::vector<double>{0.2,     0.3,     0.425,   0.55,     0.675,    0.8,      0.925,    1.4,      2.8,   3.0,   3.3,    3.6,     4.1,     4.6,    5.6,    6.1,      6.6,    9.2,   10.5,  13.0,  15.5,  18.0,  32.0,  60.0, 200.0},
                         std::vector<double>{1.1e-07, 1.6e-08, 1.3e-09, 1.96e-09, 2.23e-08, 1.25e-07, 1.06e-06, 0.000138, 0.115, 0.272, 0.0368, 0.00515, 0.00562, 0.0147, 0.0142, 0.131, 0.0356, 0.0415, 0.0662, 0.305, 0.414, 0.426, 0.324, 0.587, 0.504})
                        // Hale (1973)
            },
        // { ids::nitrogen,         },
        // { ids::oxygen,           },
        // { ids::carbon_dioxide,   },
        // { ids::methane,          },
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
        // { ids::halite,           },
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
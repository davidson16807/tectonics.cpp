#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/ids.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidExtinctionCoefficientWavenumberRelation = relation::PolynomialRailyardRelation<si::spatial_frequency<double>,double, 0,1>;
    table::PartialTable<LiquidExtinctionCoefficientWavenumberRelation> extinction_coefficient_as_liquid {

        { compounds::water, 
                    relation::get_spectral_linear_interpolation_function_of_wavelength
                        (si::micrometer, 1.0,
                         std::vector<double>{0.2,     0.3,     0.425,   0.55,     0.675,    0.8,      0.925,    1.4,      2.8,   3.0,   3.3,    3.6,     4.1,     4.6,    5.6,    6.1,      6.6,    9.2,   10.5,  13.0,  15.5,  18.0,  32.0,  60.0, 200.0},
                         std::vector<double>{1.1e-07, 1.6e-08, 1.3e-09, 1.96e-09, 2.23e-08, 1.25e-07, 1.06e-06, 0.000138, 0.115, 0.272, 0.0368, 0.00515, 0.00562, 0.0147, 0.0142, 0.131, 0.0356, 0.0415, 0.0662, 0.305, 0.414, 0.426, 0.324, 0.587, 0.504})
                        // Hale (1973)
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
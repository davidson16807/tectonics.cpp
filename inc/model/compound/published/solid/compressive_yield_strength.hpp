#pragma once

// in-house libraries
#include <unit/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidCompressiveYieldStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidCompressiveYieldStrengthTemperatureRelation> compressive_yield_strength_as_solid {

        { polymorphs::water_ice_1h,              6.0 * si::megapascal // brittle, effectively the same as fracture strength
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
        // { polymorphs::nitrogen_ice_beta,         },
        // { polymorphs::nitrogen_ice_alpha,        },
        // { polymorphs::nitrogen_ice_gamma,        },
        // { polymorphs::oxygen_ice_gamma,          },
        // { polymorphs::oxygen_ice_beta,           },
        // { polymorphs::oxygen_ice_alpha,          },
        // { polymorphs::carbon_dioxide_ice,        },
        // { polymorphs::methane_ice_alpha,         },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::argon_ice,                 },
        // { polymorphs::helium_ice,                },
        // { polymorphs::hydrogen_ice,              },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0}, 
                     std::vector<double>{580.0, 480.0, 230.0, 150.0, 110.0, 200.0}), // Prokhvatilov
            },
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
        // { polymorphs::quartz_alpha,              },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    15.0 * si::megapascal, // Bauer (2019)
            },
        // { polymorphs::corundum,                  },
        // { polymorphs::apatite,                   },
        // { polymorphs::graphite,                  },
        // { polymorphs::diamond,                   },
        // { polymorphs::calcite,                   },
        // { polymorphs::aragonite,                 },
        // { polymorphs::orthoclase,                },
        // { polymorphs::andesine,                  },
        // { polymorphs::augite,                    },
        // { polymorphs::goethite,                  },
        // { polymorphs::pyrite,                    },
        // { polymorphs::hematite,                  },
        // { polymorphs::gold,                      },
        // { polymorphs::silver,                    },
        // { polymorphs::copper,                    },
        // { polymorphs::magnetite,                 },
        // { polymorphs::chalcocite_alpha,          },
        // { polymorphs::chalcocite_beta,           },
        // { polymorphs::chalcopyrite,              },

    };

}}
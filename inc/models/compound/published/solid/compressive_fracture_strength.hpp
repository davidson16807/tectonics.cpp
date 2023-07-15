#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidCompressiveFractureStrengthTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::pressure<double>, 0,1>;
    table::PartialTable<SolidCompressiveFractureStrengthTemperatureRelation> compressive_fracture_strength_as_solid {

        { polymorphs::water_ice_1h,              6.0 * si::megapascal, //engineering toolbox
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
        { polymorphs::methane_ice_alpha,         8.0 * si::megapascal, // Yamashita (2010)
            },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::methane_ice_beta,          },
        // { polymorphs::argon_ice,                 },
        // { polymorphs::helium_ice,                },
        // { polymorphs::hydrogen_ice,              },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0, 160.0}, 
                     std::vector<double>{580.0, 620.0, 540.0, 450.0, 360.0, 270.0, 190.0}), // Prokhvatilov
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
        { polymorphs::quartz_alpha,              1.1 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    20.0 * si::megapascal, // Bauer (2019)
            },
        { polymorphs::corundum,                  2265.0*si::megapascal, //azom.com/article.aspx?ArticleId=1948
            },
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

        // { compounds::nitrogen,     
        //                             relation::StateFunction<si::pressure<double>>([](si::pressure<double> p, si::temperature<double> T){ 
        //                                 return math::mix(0.24, 6.00, math::linearstep(58.0, 5.0, T/si::kelvin))*si::megapascal;
        //                             }), // wikipedia, and Yamashita (2010)
        //                         },

    };

}}

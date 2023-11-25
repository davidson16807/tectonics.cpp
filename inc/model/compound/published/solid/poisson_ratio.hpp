#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/GenericRelation.hpp>
#include <model/compound/ids.hpp>

#include "polymorphs.hpp"

namespace compound { 
namespace published { 

    using SolidPoissonRatioTemperatureRelation = relation::GenericRelation<si::temperature<double>,double>;
    table::PartialTable<SolidPoissonRatioTemperatureRelation> poisson_ratio_as_solid {

        { polymorphs::water_ice_1h,              0.3252,                                           // gammon (1983)
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
        { polymorphs::nitrogen_ice_beta,         
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{ 0.30,  0.29,  0.29,  0.29,  0.28,  0.28,  0.27}), // Prokhvatilov
            },
        { polymorphs::nitrogen_ice_alpha,        
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{ 0.34,  0.34,  0.34,  0.33,  0.33,  0.33,  0.33,  0.33,  0.33,  0.32}), // Prokhvatilov
            },
        // { polymorphs::nitrogen_ice_gamma,        },
        { polymorphs::oxygen_ice_gamma,          
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{0.449, 0.449, 0.448, 0.447, 0.447}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_beta,           
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{0.422, 0.407, 0.400, 0.393, 0.385, 0.376, 0.371, 0.365, 0.366}), // Prokhvatilov
            },
        { polymorphs::oxygen_ice_alpha,          
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{0.345, 0.344, 0.348, 0.352, 0.356, 0.361, 0.368, 0.377, 0.394, 0.426}), // Prokhvatilov
            },
        // { polymorphs::carbon_dioxide_ice,        },
        { polymorphs::methane_ice_alpha,         0.39, // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { polymorphs::methane_ice_beta,          
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 0.293, 0.299, 0.302, 0.302, 0.305, 0.311}), // Prokhvatilov
            },
        { polymorphs::argon_ice,                 
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 0.25,  0.25,  0.25,  0.26,  0.26,  0.27,  0.27,  0.27}), // Prokhvatilov
            },
        // { polymorphs::helium_ice,                },
        { polymorphs::hydrogen_ice,              
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  2.0,  13.5}, 
                     std::vector<double>{0.219, 0.219}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { polymorphs::ammonia_ice,               
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.214, 0.214, 0.209, 0.203, 0.198, 0.195, 0.196}), // Prokhvatilov
            },
        // { polymorphs::ozone_ice,                 },
        // { polymorphs::nitrous_oxide_ice,         },
        // { polymorphs::sulfur_dioxide_ice,        },
        // { polymorphs::nitric_oxide_ice,          },
        { polymorphs::carbon_monoxide_ice_beta,  
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.380, 0.381, 0.382, 0.382}), // Prokhvatilov
            },
        { polymorphs::carbon_monoxide_ice_alpha, 
                relation::get_linear_interpolation_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{0.218, 0.218, 0.217, 0.216, 0.215, 0.213, 0.208, 0.196, 0.194, 0.196}), // Prokhvatilov
            },
        // { polymorphs::ethane_ice,                },
        // { polymorphs::hydrogen_cyanide_ice,      },
        // { polymorphs::ethanol_ice,               },
        // { polymorphs::formaldehyde_ice,          },
        // { polymorphs::formic_acid_ice,           },
        // { polymorphs::perflouromethane_ice,      },
        // { polymorphs::benzene_ice,               },
        // { polymorphs::pyrimidine_ice,            },
        { polymorphs::quartz_alpha,              0.08, // Schön (2015)
            },
        // { polymorphs::quartz_beta,               },
        // { polymorphs::crystoballite_alpha,       },
        // { polymorphs::crystoballite_beta,        },
        { polymorphs::halite,                    0.25, // Mavko (2009)
            },
        { polymorphs::corundum,                  0.24, // Mavko (2009)
            },
        { polymorphs::apatite,                   0.26, // Schön (2015)
            },
        // { polymorphs::graphite,                  },
        // { polymorphs::diamond,                   },
        { polymorphs::calcite,                   0.32, // Schön (2015)
            },
        // { polymorphs::aragonite,                 },
        // { polymorphs::orthoclase,                },
        { polymorphs::andesine,                  0.29, // Schön (2015), for anorthite
            },
        { polymorphs::augite,                    0.25, // Schön (2015)
            },
        { polymorphs::forsterite,          0.24,
            },
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
        { compounds::forsterite,          0.24,
            },
        // { compounds::goethite,         },
        { compounds::pyrite,              0.16, // Schön (2015)
            },
        // { compounds::hematite,         },
        { compounds::gold,                0.43, // wikipedia
            },
        // { compounds::silver,           },
        { compounds::copper,              0.33,
            },
        // { compounds::magnetite,        },
        // { compounds::chalcocite,       },
        // { compounds::chalcopyrite,     },
    };

}}
#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidDensityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::density<double>, 0,1>;
    table::PartialTable<SolidDensityTemperatureRelation> density_as_solid {

        // { polymorphs::water_ice_1h,              },
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
        // { polymorphs::ammonia_ice,               },
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
        // { polymorphs::halite,                    },
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

        { compounds::water,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilogram/si::meter3,
                     std::vector<double>{-100.0, -80.0, -60.0, -40.0, -20.0,   0.0}, 
                     std::vector<double>{ 925.7, 924.1, 922.4, 920.8, 919.4, 916.2}), // https://www.engineeringtoolbox.com/ice-thermal-properties-d_576.html
            },
        { compounds::nitrogen,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{0.9955,0.9899,0.9862,0.9845,0.9748,0.9669,0.9586}), // Prokhvatilov
            },
        { compounds::oxygen,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{1.396, 1.390, 1.383, 1.377, 1.370}), // Prokhvatilov
            },
        { compounds::carbon_dioxide,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{1.7056,1.7036,1.6963,1.6852,1.6718,1.6568,1.6403,1.6223,1.6022,1.5789}), // Prokhvatilov
            },
        { compounds::methane,             0.5245*si::gram/si::centimeter3, // Prokhvatilov, @ si::atmosphere, 14.4*si::kelvin
            },
        { compounds::argon,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{1.7705,1.7641,1.7518,1.7349,1.7148,1.6919,1.6657,1.6349}), // Prokhvatilov
            },
        { compounds::helium,                           // 0187.0 * si::kilogram/si::meter3,
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram / si::centimeter3,
                     std::vector<double>{1.0,    4.0,    26.0}, 
                     std::vector<double>{0.1891, 0.2305,  0.3963}), // Johnson (1960)
            },
        { compounds::hydrogen,                           // 86.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{.08740,.08740,.08738,.08732,.08718,.08706,.08656}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { compounds::ammonia,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.855, 0.853, 0.848, 0.841, 0.834, 0.826, 0.821}), // Prokhvatilov
            },
        { compounds::ozone,               1354.0  * si::kilogram / si::meter3, //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
            },
        { compounds::nitrous_oxide,       1.263 * si::gram/si::centimeter3, // Hudson (2020)
            },
        // { compounds::sulfur_dioxide,   },
        // { compounds::nitric_oxide,     },
        { compounds::carbon_monoxide,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.9378,0.9326,0.9269,0.9238}), // Prokhvatilov
            },
        // { compounds::ethane,           },
        // { compounds::hydrogen_cyanide, },
        { compounds::ethanol,             0.739 * si::gram / si::centimeter3, // Hudson (2020)
            },
        // { compounds::formaldehyde,     },
        { compounds::formic_acid,         0.979 * si::gram / si::centimeter3, // Hudson (2020)
            },
        { compounds::perflouromethane,    1980.0 * si::kilogram/si::meter3, // pubchem
            },
        // { compounds::benzene,          },
        // { compounds::pyrimidine,       },
        { compounds::quartz,              2650.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { compounds::halite,              2170.0 * si::kilogram/si::meter3,
            },
        { compounds::corundum,            3970.0 * si::kilogram/si::meter3,
            },
        { compounds::apatite,             3180.0 * si::kilogram/si::meter3,
            },
        { compounds::carbon,              2260.0 * si::kilogram/si::meter3, 
            },
        { compounds::calcite,             2710.0 * si::kilogram/si::meter3,
            },
        { compounds::orthoclase,          2560.0 * si::kilogram/si::meter3,
            },
        { compounds::andesine,            2670.0 * si::kilogram/si::meter3,
            },
        { compounds::augite,              3380.0 * si::kilogram/si::meter3,
            },
        { compounds::forsterite,          3810.0 * si::kilogram/si::meter3,
            },
        { compounds::goethite,            4300.0 * si::kilogram/si::meter3,
            },
        { compounds::pyrite,              5020.0 * si::kilogram/si::meter3,
            },
        { compounds::hematite,            5250.0 * si::kilogram/si::meter3,
            },
        { compounds::gold,                19300.0 * si::kilogram/si::meter3,
            },
        { compounds::silver,              10500.0 * si::kilogram/si::meter3,
            },
        { compounds::copper,              8960.0 * si::kilogram/si::meter3,
            },
        { compounds::magnetite,           5170.0 * si::kilogram/si::meter3,
            },
        { compounds::chalcocite,          5600.0 * si::kilogram/si::meter3,
            },
        { compounds::chalcopyrite,        4200.0 *  si::kilogram/si::meter3, //wikipedia
            },
    };

}}
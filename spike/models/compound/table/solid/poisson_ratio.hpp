#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace table { 

    std::std::map<int, relation::StateFunction<double>> poisson_ratio_as_solid {
        { ids::water,                     state_invariant(0.3252),                                           // gammon (1983)
            },
        { ids::nitrogen,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{ 0.30,  0.29,  0.29,  0.29,  0.28,  0.28,  0.27}), // Prokhvatilov
            },
        { ids::oxygen,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{0.449, 0.449, 0.448, 0.447, 0.447}), // Prokhvatilov
            },
        // { ids::carbon_dioxide,   },
        { ids::methane,                     state_invariant(0.39), // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            },
        { ids::argon,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 0.25,  0.25,  0.25,  0.26,  0.26,  0.27,  0.27,  0.27}), // Prokhvatilov
            },
        // { ids::helium,           },
        { ids::hydrogen,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  2.0,  13.5}, 
                     std::vector<double>{0.219, 0.219}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { ids::ammonia,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.214, 0.214, 0.209, 0.203, 0.198, 0.195, 0.196}), // Prokhvatilov
            },
        // { ids::ozone,            },
        // { ids::nitrous_oxide,    },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.380, 0.381, 0.382, 0.382}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,                     state_invariant(0.08), // Schön (2015)
            },
        { ids::halite,                     state_invariant(0.25), // Mavko (2009)
            },
        { ids::corundum,                     0.24, // Mavko (2009)
            },
        { ids::apatite,                     state_invariant(0.26), // Schön (2015)
            },
        // { ids::carbon,           },
        { ids::calcite,                     state_invariant(0.32), // Schön (2015)
            },
        // { ids::orthoclase,       },
        { ids::andesine,                     state_invariant(0.29), // Schön (2015), for anorthite
            },
        { ids::augite,                     state_invariant(0.25), // Schön (2015)
            },
        { ids::forsterite,                     state_invariant(0.24),
            },
        // { ids::goethite,         },
        { ids::pyrite,                     state_invariant(0.16), // Schön (2015)
            },
        // { ids::hematite,         },
        { ids::gold,                     state_invariant(0.43), // wikipedia
            },
        // { ids::silver,           },
        { ids::copper,                     state_invariant(0.33),
            },
        // { ids::magnetite,        },
        // { ids::chalcocite,       },
        // { ids::chalcopyrite,     },
    };

}}
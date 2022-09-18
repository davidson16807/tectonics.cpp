#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, relation::StateFunction<si::density<double>>> density_as_solid {
        { ids::water,                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    0.9228, 0.00728, 0.00075), // Gagnon (1990)
            },
        { ids::nitrogen,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{0.9955,0.9899,0.9862,0.9845,0.9748,0.9669,0.9586}), // Prokhvatilov
            },
        { ids::oxygen,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{1.396, 1.390, 1.383, 1.377, 1.370}), // Prokhvatilov
            },
        { ids::carbon_dioxide,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{1.7056,1.7036,1.6963,1.6852,1.6718,1.6568,1.6403,1.6223,1.6022,1.5789}), // Prokhvatilov
            },
        { ids::methane,                           state_invariant(0.5245*si::gram/si::centimeter3), // Prokhvatilov, @ si::atmosphere, 14.4*si::kelvin
            },
        { ids::argon,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{1.7705,1.7641,1.7518,1.7349,1.7148,1.6919,1.6657,1.6349}), // Prokhvatilov
            },
        { ids::helium,                           // 0187.0 * si::kilogram/si::meter3,
                get_interpolated_temperature_function
                    (si::kelvin, si::gram / si::centimeter3,
                     std::vector<double>{1.0,    4.0,    26.0}, 
                     std::vector<double>{0.1891, 0.2305,  0.3963}), // Johnson (1960)
            },
        { ids::hydrogen,                           // 86.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{.08740,.08740,.08738,.08732,.08718,.08706,.08656}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { ids::ammonia,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.855, 0.853, 0.848, 0.841, 0.834, 0.826, 0.821}), // Prokhvatilov
            },
        { ids::ozone,                           state_invariant(1354.0  * si::kilogram / si::meter3), //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
            },
        { ids::nitrous_oxide,                           state_invariant(1.263 * si::gram/si::centimeter3), // Hudson (2020)
            },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.9378,0.9326,0.9269,0.9238}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        { ids::ethanol,                           state_invariant(0.739 * si::gram / si::centimeter3), // Hudson (2020)
            },
        // { ids::formaldehyde,     },
        { ids::formic_acid,                           state_invariant(0.979 * si::gram / si::centimeter3), // Hudson (2020)
            },
        { ids::perflouromethane,                           state_invariant(1980.0 * si::kilogram/si::meter3), // pubchem
            },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,                           state_invariant(2650.0 *  si::kilogram/si::meter3), // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { ids::halite,                           state_invariant(2170.0 * si::kilogram/si::meter3),
            },
        { ids::corundum,                           state_invariant(3970.0 * si::kilogram/si::meter3),
            },
        { ids::apatite,                           state_invariant(3180.0 * si::kilogram/si::meter3),
            },
        { ids::carbon,                           state_invariant(2260.0 * si::kilogram/si::meter3), 
            },
        { ids::calcite,                           state_invariant(2710.0 * si::kilogram/si::meter3),
            },
        { ids::orthoclase,                           state_invariant(2560.0 * si::kilogram/si::meter3),
            },
        { ids::andesine,                           state_invariant(2670.0 * si::kilogram/si::meter3),
            },
        { ids::augite,                           state_invariant(3380.0 * si::kilogram/si::meter3),
            },
        { ids::forsterite,                           state_invariant(3810.0 * si::kilogram/si::meter3),
            },
        { ids::goethite,                           state_invariant(4300.0 * si::kilogram/si::meter3),
            },
        { ids::pyrite,                           state_invariant(5020.0 * si::kilogram/si::meter3),
            },
        { ids::hematite,                           state_invariant(5250.0 * si::kilogram/si::meter3),
            },
        { ids::gold,                           state_invariant(19300.0 * si::kilogram/si::meter3),
            },
        { ids::silver,                           state_invariant(10500.0 * si::kilogram/si::meter3),
            },
        { ids::copper,                           8960.0 * si::kilogram/si::meter3,
            },
        { ids::magnetite,                           state_invariant(5170.0 * si::kilogram/si::meter3),
            },
        { ids::chalcocite,                           state_invariant(5600.0 * si::kilogram/si::meter3),
            },
        { ids::chalcopyrite,                           state_invariant(4200.0 *  si::kilogram/si::meter3), //wikipedia
            },
    };

}}
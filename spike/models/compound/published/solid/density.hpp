#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    using SolidDensityTemperatureRelation = relation::PolynomialRailyardRelation<si::temperature<double>,si::density<double>,0,1>;
    std::map<int, SolidDensityTemperatureRelation> density_as_solid {
        { ids::water,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::kilogram/si::meter3,
                     std::vector<double>{-100.0, -80.0, -60.0, -40.0, -20.0,   0.0}, 
                     std::vector<double>{ 925.7, 924.1, 922.4, 920.8, 919.4, 916.2}), // https://www.engineeringtoolbox.com/ice-thermal-properties-d_576.html
            },
        { ids::nitrogen,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{0.9955,0.9899,0.9862,0.9845,0.9748,0.9669,0.9586}), // Prokhvatilov
            },
        { ids::oxygen,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{1.396, 1.390, 1.383, 1.377, 1.370}), // Prokhvatilov
            },
        { ids::carbon_dioxide,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{1.7056,1.7036,1.6963,1.6852,1.6718,1.6568,1.6403,1.6223,1.6022,1.5789}), // Prokhvatilov
            },
        { ids::methane,             0.5245*si::gram/si::centimeter3, // Prokhvatilov, @ si::atmosphere, 14.4*si::kelvin
            },
        { ids::argon,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{1.7705,1.7641,1.7518,1.7349,1.7148,1.6919,1.6657,1.6349}), // Prokhvatilov
            },
        { ids::helium,                           // 0187.0 * si::kilogram/si::meter3,
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram / si::centimeter3,
                     std::vector<double>{1.0,    4.0,    26.0}, 
                     std::vector<double>{0.1891, 0.2305,  0.3963}), // Johnson (1960)
            },
        { ids::hydrogen,                           // 86.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{.08740,.08740,.08738,.08732,.08718,.08706,.08656}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            },
        { ids::ammonia,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.855, 0.853, 0.848, 0.841, 0.834, 0.826, 0.821}), // Prokhvatilov
            },
        { ids::ozone,               1354.0  * si::kilogram / si::meter3, //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
            },
        { ids::nitrous_oxide,       1.263 * si::gram/si::centimeter3, // Hudson (2020)
            },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,                           
                relation::get_linear_interpolation_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.9378,0.9326,0.9269,0.9238}), // Prokhvatilov
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        { ids::ethanol,             0.739 * si::gram / si::centimeter3, // Hudson (2020)
            },
        // { ids::formaldehyde,     },
        { ids::formic_acid,         0.979 * si::gram / si::centimeter3, // Hudson (2020)
            },
        { ids::perflouromethane,    1980.0 * si::kilogram/si::meter3, // pubchem
            },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              2650.0 *  si::kilogram/si::meter3, // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            },
        { ids::halite,              2170.0 * si::kilogram/si::meter3,
            },
        { ids::corundum,            3970.0 * si::kilogram/si::meter3,
            },
        { ids::apatite,             3180.0 * si::kilogram/si::meter3,
            },
        { ids::carbon,              2260.0 * si::kilogram/si::meter3, 
            },
        { ids::calcite,             2710.0 * si::kilogram/si::meter3,
            },
        { ids::orthoclase,          2560.0 * si::kilogram/si::meter3,
            },
        { ids::andesine,            2670.0 * si::kilogram/si::meter3,
            },
        { ids::augite,              3380.0 * si::kilogram/si::meter3,
            },
        { ids::forsterite,          3810.0 * si::kilogram/si::meter3,
            },
        { ids::goethite,            4300.0 * si::kilogram/si::meter3,
            },
        { ids::pyrite,              5020.0 * si::kilogram/si::meter3,
            },
        { ids::hematite,            5250.0 * si::kilogram/si::meter3,
            },
        { ids::gold,                19300.0 * si::kilogram/si::meter3,
            },
        { ids::silver,              10500.0 * si::kilogram/si::meter3,
            },
        { ids::copper,              8960.0 * si::kilogram/si::meter3,
            },
        { ids::magnetite,           5170.0 * si::kilogram/si::meter3,
            },
        { ids::chalcocite,          5600.0 * si::kilogram/si::meter3,
            },
        { ids::chalcopyrite,        4200.0 *  si::kilogram/si::meter3, //wikipedia
            },
    };

}}
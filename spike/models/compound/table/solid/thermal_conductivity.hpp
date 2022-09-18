#pragma once

// std libraries
#include <map>

// in-house libraries
#include <models/compound/relation/StateFunction.hpp>
#include "../ids.hpp"

namespace compound { 
namespace table { 

    std::std::map<int, relation::StateFunction<si::thermal_conductivity<double>>> thermal_conductivity_as_solid {
        { ids::water,              
                get_interpolated_temperature_function
                    (si::kelvin, si::watt/(si::meter*si::kelvin),
                     std::vector<double>{ 2.0,  4.0,  8.0, 10.0, 30.0, 100.0, 173.0, 193.0, 233.0, 253.0, 273.15 }, 
                     std::vector<double>{21.0, 57.0, 87.0, 81.0, 22.0,   6.0,  3.48,  3.19,  2.63,  2.39,   2.22 }), // Ahmad (1994) and Engineering Toolbox
            },
        { ids::nitrogen,              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 45.0, 57.5}, 
                     std::vector<double>{ 2.21, 2.09}), // Manzhelii (1997)
            },
        { ids::oxygen,              state_invariant(0.17 * si::watt / (si::centimeter * si::kelvin)),  // Jezowski (1993)
            },
        { ids::carbon_dioxide,              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 25.0, 40.0, 50.0, 80.0, 120.0, 210.0}, 
                     std::vector<double>{59.17,27.32,20.24,11.68,  7.79,  4.19}), // Manzhelii (1997)
            },
        { ids::methane,              // 0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
                get_interpolated_temperature_function
                    (si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{18.0, 20.0, 20.4, 21.0},              
                     std::vector<double>{0.7,  2.5,  11.0,  0.7}), // Johnson (1960)
            },
        { ids::argon,              
                get_interpolated_temperature_function
                    (si::kelvin, si::milliwatt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.0,  8.0, 50.0},              
                     std::vector<double>{4.0, 40.0,  5.0}),  // Johnson (1960)
            },
        { ids::helium,              // 0.1 * si::watt / (si::centimeter * si::kelvin), // Webb (1952)
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.39, 1.70, 2.12, 4.05}, 
                     std::vector<double>{0.32, 0.63, 0.40, 0.01}), // Johnson (1960)
            },
        { ids::hydrogen,              state_invariant(1.819 * si::watt / ( si::meter * si::kelvin )), // wikipedia
            },
        // { ids::ammonia,          },
        { ids::ozone,              state_invariant(5.21e-4 * si::calorie / (si::second*si::centimeter2*si::kelvin/si::centimeter)), // Streng (1961)
            },
        { ids::nitrous_oxide,              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 23.0, 30.0, 40.0, 60.0, 120.0, 180.0}, 
                     std::vector<double>{17.08,40.27, 10.8, 7.89,  4.96,  3.38}), // Manzhelii (1997)
            },
        // { ids::sulfur_dioxide,   },
        // { ids::nitric_oxide,     },
        { ids::carbon_monoxide,              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{22.5, 25.0, 35.0, 45.0, 58.0}, 
                     std::vector<double>{6.37, 5.69, 4.16, 3.34, 2.62}), // Manzhelii (1997)
            },
        // { ids::ethane,           },
        // { ids::hydrogen_cyanide, },
        // { ids::ethanol,          },
        // { ids::formaldehyde,     },
        // { ids::formic_acid,      },
        // { ids::perflouromethane, },
        // { ids::benzene,          },
        // { ids::pyrimidine,       },
        { ids::quartz,              
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,    20.0,  80.0, 273.15}, 
                     std::vector<double>{1200.0, 480.0,  30.0, 7.69}), // Perry, with Schön (2015) for standard temperature 
            },
        { ids::halite,              state_invariant(5.55 * si::watt / ( si::meter * si::kelvin )), // Wilkens (2011)
            },
        { ids::corundum,              // field::StateSample<si::thermal_conductivity<double>>(37.0*si::watt/(si::meter * si::kelvin), si::atmosphere, 20.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0, 60.0,  300.0, 400.0, 1400.0}, 
                     std::vector<double>{7.0,  174.0,  36.0,  26.0,    6.0}), // Perry
            },
        { ids::apatite,              state_invariant(1.38 * si::watt / (si::meter*si::kelvin)), // Schön (2015)
            },
        { ids::carbon,              // 247.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia (middle range value)
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{  7.0, 10.0, 100.0,  300.0, 1400.0}, 
                     std::vector<double>{ 15.0, 81.0,4980.0, 2000.0,  370.0}), // Johnson (1960) and Perry, perpendicular to basal plane
            },
        { ids::calcite,              state_invariant(3.59 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            },
        { ids::orthoclase,              state_invariant(2.31 * si::watt/(si::meter*si::kelvin)), // Schön (2015)
            },
        { ids::andesine,              state_invariant(1.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015), for anorthite
            },
        { ids::augite,              state_invariant(4.66 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            },
        { ids::forsterite,              state_invariant(7.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            },
        { ids::goethite,              state_invariant(2.91 * si::watt / (si::meter * si::kelvin)), // Cermak (1988)
            },
        { ids::pyrite,              state_invariant(19.21 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            },
        { ids::hematite,              state_invariant(11.28 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            },
        { ids::gold,              // 314.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,    60.0,  300.0, 1200.0}, 
                     std::vector<double>{1600.0, 2800.0, 380.0,  315.0,  262.0}), // Johnson (1960) and Perry
            },
        { ids::silver,              // 427.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{16500.0, 630.0,  424.0,  358.0}), // Perry
            },
        { ids::copper,              // 401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{7000.0, 19000.0, 850.0,  398.0,  342.0}), // Johnson (1960)  and Perry
            },
        { ids::magnetite,              state_invariant(5.1 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            },
        // { ids::chalcocite,       },
        { ids::chalcopyrite,              state_invariant(8.19 * si::watt / (si::centimeter * si::kelvin)), // Cermak (1988), for chalcopyrite
            },
    };


}}
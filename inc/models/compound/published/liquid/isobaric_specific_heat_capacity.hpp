#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/ids.hpp>
#include <models/compound/relation/RationalRailyardRelation.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidMolarHeatCapacityTemperatureRelation = relation::RationalRailyardRelation<si::temperature<double>, si::molar_heat_capacity<double>, 0,6,0,1>;
    using LiquidHeatCapacityTemperatureRelation = relation::RationalRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, 0,6,0,1>;
    table::PartialTable<LiquidHeatCapacityTemperatureRelation> isobaric_specific_heat_capacity_as_liquid {
        { compounds::water, 
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (18.01528 * si::kilogram * si::kelvin), 
                    276370.0, -2090.1, 8.125, -0.014116, 9.3701e-6,
                    273.16, 533.15), 
            },
        { compounds::nitrogen, // 2.042 * si::joule / (si::gram * si::kelvin),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (28.0134 * si::kilogram * si::kelvin), 
                    281970.0, -12281.0, 248.0, -2.2182, 0.0074902,
                    63.15, 112.0), 
            },
        { compounds::oxygen, // 1.699 * si::joule / (si::gram * si::kelvin),             
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (31.9988 * si::kilogram * si::kelvin), 
                    175430.0, -6152.3, 113.92, -0.92382, 0.0027963,
                    4.36, 142.0), 
            },
        { compounds::carbon_dioxide, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (44.0095 * si::kilogram * si::kelvin), 
                    -8304300.0, 104370.0, -433.33, 0.60052, 0.0,
                    220, 290), 
            },
        { compounds::methane, // 3.481 * si::joule / (si::gram * si::kelvin),              
                relation::get_dippr_liquid_heat_capacity_temperature_relation_114
                    (190.56 * si::kelvin, si::joule / (16.0425 * si::kilogram * si::kelvin), 
                    65.708, 38883.0, -257.95, 614.07, 0.0,
                    90.69, 190.0), 
            },
        { compounds::argon, //1.078 * si::joule / (si::gram * si::kelvin),              
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (39.948 * si::kilogram * si::kelvin), 
                    134390.0, -1989.4, 11.043, 0.0, 0.0,
                    83.78, 135.0), 
            },
        { compounds::helium, // 4.545 * si::joule / (si::gram * si::kelvin), 
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(si::gram * si::kelvin),
                      std::vector<double>{3.0,3.5, 4.0, 4.5, 5.0  },
                      std::vector<double>{2.6,3.41,4.52,6.78,20.24}),
            },
        { compounds::hydrogen, // 9.668 * si::joule / (si::gram * si::kelvin), 
                // NOTE: this estimate produces bogus results outside its range, we may need to clamp this
                relation::get_dippr_liquid_heat_capacity_temperature_relation_114
                    (32.97 * si::kelvin, si::joule / (2.01588 * si::kilogram * si::kelvin), 
                    66.653, 6765.9, -123.63, 478.27, 0.0,
                    13.95, 32.0), 
            },
        { compounds::ammonia, // 4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
                relation::get_dippr_liquid_heat_capacity_temperature_relation_114
                    (405.56 * si::kelvin, si::joule / (17.031 * si::kilogram * si::kelvin), 
                    61.289, 80925.0, 799.4, -2651.0, 0.0,
                    203.15, 401.15),
            },
        { compounds::ozone, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (47.9982 * si::kilogram * si::kelvin), 
                    60046.0, 281.16, 0.0, 0.0, 0.0,
                    90.0, 150.0), 
            },
        { compounds::nitrous_oxide, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (44.0128 * si::kilogram * si::kelvin), 
                    67556.0, 54.373, 0.0, 0.0, 0.0,
                    182.3, 200.0), 
            },
        { compounds::sulfur_dioxide, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (64.0638 * si::kilogram * si::kelvin), 
                    85743.0, 5.7443, 0.0, 0.0, 0.0,
                    197.67, 350.0), 
            },
        { compounds::nitric_oxide, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (30.0061 * si::kilogram * si::kelvin), 
                    -2979600.0, 76602.0, -652.59, 1.8879, 0.0,
                    109.5, 150.0), 
            },
        { compounds::carbon_monoxide, // 60.351 * si::joule / (28.010 * si::gram * si::kelvin), // pubchem
                // NOTE: this function is not producing physically sensible values, we may need to remove it if it can't be fixed
                // relation::get_dippr_liquid_heat_capacity_temperature_relation_114
                //     (132.86 * si::kelvin, si::joule / (28.0101* si::kilogram * si::kelvin), 
                //     65.429, 28723.0, -847.39, 1959.6, 0.0,
                //     68.15, 132.0
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(si::gram * si::kelvin),
                      std::vector<double>{68.16, 80.0,100.0,110.0,120.0,130.0},
                      std::vector<double>{2.157,2.142,2.306,2.558,3.202,8.070}),
            },
        { compounds::ethane, // 68.5 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
                relation::get_dippr_liquid_heat_capacity_temperature_relation_114
                    (305.36 * si::kelvin, si::joule / (30.069 * si::kilogram * si::kelvin), 
                    44.009, 89718.0, 918.77, -1886.0, 0.0,
                    92.0, 290.0), 
                // relation::get_linear_interpolation_function
                //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
                //       std::vector<double>{90.4,120.0,160.0,240.0,280.0,300.0},
                //       std::vector<double>{2.33,2.280,2.357,2.847,3.987,10.02}),
            },
        { compounds::hydrogen_cyanide, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (27.02534 * si::kilogram * si::kelvin), 
                    95398.0, -197.52, 0.3883, 0.0, 0.0,
                    259.83, 298.85), 
            },
        { compounds::ethanol, // 112.4 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (46.06844 * si::kilogram * si::kelvin), 
                    102640.0, -139.63, -0.030341, 0.0020386, 0.0,
                    159, 390), 
                // relation::get_linear_interpolation_function
                //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
                //       std::vector<double>{250.0,280.0,300.0,440.0,480.0,500.0},
                //       std::vector<double>{2.032,2.351,2.597,4.109,5.091,7.255}),
            },
        { compounds::formaldehyde, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (30.02598* si::kilogram * si::kelvin), 
                    70077.0, -661.79, 5.9749, -0.01813, 0.00001983,
                    155.15, 253.85), 
            },
        { compounds::formic_acid, // 101.3 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (46.0257* si::kilogram * si::kelvin), 
                    78060.0, 71.54, 0.0, 0.0, 0.0,
                    281.45, 380.0), 
            },
        { compounds::perflouromethane, // missing(),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::joule / (88.0043* si::kilogram * si::kelvin), 
                    104600.0, -500.6, 2.2851, 0.0, 0.0,
                    89.56, 145.1), 
            },
        { compounds::benzene, // 134.8 * si::joule / (79.109 * si::gram * si::kelvin),
                relation::get_linear_interpolation_function
                    ( si::kelvin, si::joule/(si::gram * si::kelvin),
                      std::vector<double>{278.7,300.0,320.0,450.0,500.0,550.0},
                      std::vector<double>{1.708,1.706,1.766,2.279,2.637,4.765}),
            },
        // { compounds::pyrimidine,   },
        // { compounds::quartz,       },
        { compounds::halite,          15.9 * si::calorie/(80.088 * si::gram*si::kelvin), // Perry, 1073-1205K
            },
        // { compounds::corundum,     },
        // { compounds::apatite,      },
        { compounds::carbon,          26.0 * si::joule / (12.011 * si::gram * si::kelvin), // Steinbeck (1990)
                // NOTE: the following equation is reported, but fails dimensionality check
                // field::StateFunction<si::isobaric_specific_heat_capacity>([](const si::pressure<double> p, const si::temperature<double> T){
                //     const si::energy<double> fermi_energy = 16.9 * si::electronvolt;
                //     const double pi = 3.1415926;
                //     return 3.0*si::universal_gas_constant + pi*pi*si::universal_gas_constant*si::boltzmann_constant*T/(2.0*fermi_energy);
                // }), // Steinbeck (1990)
            },
        // { compounds::calcite,      },
        // { compounds::orthoclase,   },
        // { compounds::andesine,     },
        { compounds::augite,          1.08 * si::kilojoule / (si::kilogram*si::kelvin), // Schön (2015), for basaltic lava
            },
        // { compounds::forsterite,   },
        // { compounds::goethite,     },
        // { compounds::pyrite,       },
        // { compounds::hematite,     },
        { compounds::gold,            7.0 * si::calorie/(196.967 * si::gram*si::kelvin), // Perry, 1336-1573K
            },
        { compounds::silver,          8.2 * si::calorie/(107.868 * si::gram*si::kelvin), // Perry, 1234-1573K
            },
        { compounds::copper, 
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::calorie/(63.546 * si::gram*si::kelvin), 
                     5.44, 0.001462, 0.0, 0.0, 0.0,
                     1357.0, 1573.0),
            },
        // { compounds::magnetite,    },
        // { compounds::chalcocite,   },
        // { compounds::chalcopyrite, },
    };

}}
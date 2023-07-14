#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/ids.hpp>
#include <models/compound/relation/PolynomialRailyardRelation.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidThermalConductivityTemperatureRelation = 
        relation::PolynomialRailyardRelation<si::temperature<double>,si::thermal_conductivity<double>, 0,4>;
    table::PartialTable<LiquidThermalConductivityTemperatureRelation> thermal_conductivity_as_liquid {
        { compounds::water,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     -0.432, 0.0057255, -0.000008078, 1.861e-9, 0.0,
                      273.15, 633.15), 
            },
        { compounds::nitrogen,                  
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.2654, -0.001677,  0.0,  0.0,  0.0,
                      63.15, 124), 
            },
        { compounds::oxygen,   // 0.1514 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.2741, -0.00138, 0.0,  0.0,  0.0,
                      60, 150), // 60-150K
            },
        { compounds::carbon_dioxide,   // 0.087 * si::watt / (si::meter * si::kelvin),                     // wikipedia
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.4406, -0.0012175, 0.0,  0.0,  0.0,
                      216.58, 300), 
            },
        { compounds::methane,   // 0.1931 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.41768, -0.0024528, 3.5588e-6,  0.0,  0.0,
                      90.69, 180), 
            },
        { compounds::argon,   // 0.1232 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.1819, -0.0003176, -0.00000411, 0.0, 0.0,
                      83.78, 150), 
            },
        { compounds::helium,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     -0.013833, 0.022913, -0.0054872, 0.0004585, 0.0,
                      2.2, 4.8), 
            },
        { compounds::hydrogen,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     -0.0917, 0.017678, -0.000382, -3.3324e-6, 1.0266e-7,
                      13.95, 31), 
            },
        { compounds::ammonia,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     1.169, -0.002314, 0.0, 0.0,  0.0,
                      195.41, 400.05), 
            },
        { compounds::ozone,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.17483, 0.00075288, -2.5228e-6, 0.0, 0.0,
                      77.35, 161.85), 
            },
        { compounds::nitrous_oxide,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.10112, 0.0, 0.0, 0.0,  0.0,
                      277.59, 277.59), 
            },
        { compounds::sulfur_dioxide,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.38218, -0.0006254, 0.0, 0.0, 0.0,
                      197.67, 400), 
            },
        { compounds::nitric_oxide,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.1878, 0.0010293, -0.00000943,  0.0, 0.0,
                      110, 176.4), 
            },
        { compounds::carbon_monoxide,   // 0.1428 * si::watt / (si::meter * si::kelvin), // pubchem
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.2855, -0.001784, 0.0,  0.0,  0.0,
                      68.15, 125), 
            },
        { compounds::ethane,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.35758, -0.0011458, 6.1866e-7, 0.0, 0.0,
                      90.35, 300), 
            },
        { compounds::hydrogen_cyanide,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.43454, -0.0007008,  0.0,  0.0,  0.0,
                      259.83, 298.85), 
            },
        { compounds::ethanol,   // 0.167 * si::watt / ( si::meter * si::kelvin ),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.2468, -0.000264,  0.0,  0.0,  0.0,
                      159.05, 353.15), 
            },
        { compounds::formaldehyde,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.336003243, -0.00054, 0.0, 0.0, 0.0,
                      155.15, 253.85), 
            },
        { compounds::formic_acid,   // 0.267 * si::watt / (si::meter * si::kelvin),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.302, -0.00018, 0.0,  0.0,  0.0,
                      281.45, 373.71), 
            },
        { compounds::perflouromethane,   
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.20771, -0.00078883,  0.0,  0.0,  0.0,
                      89.56, 145.1), 
            },
        { compounds::benzene,   // 0.1411 * si::watt / ( si::meter * si::kelvin ),
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.23444, -0.00030572, 0.0,  0.0,  0.0,
                      278.68, 413.1), 
            },
        // { compounds::pyrimidine,   },
        // { compounds::quartz,       },
        { compounds::halite,          0.45 * si::watt / (si::meter * si::kelvin)  // Passerini (2010)
            },
        // { compounds::corundum,     // dynamic_viscosity = 0.035 * si::pascal * si::second, // Blomquist (1978)
            // },
        // { compounds::apatite,      },
        { compounds::carbon,   // 2.9 * si::watt / (si::centimeter*si::kelvin), // Steinbeck (1990)
                relation::get_dippr_quartic_temperature_relation_100
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     0.0, 
                     3.1415926*3.1415926*si::boltzmann_constant*si::boltzmann_constant * si::kelvin /
                        (3.0*si::elementary_charge_constant*si::elementary_charge_constant*39.5*si::microohm*si::centimeter) / 
                        (si::watt / (si::meter * si::kelvin)), 
                     0.0,  0.0,  0.0, 
                     278.68, 413.1), 
            }, // Steinbeck (1990)
        // { compounds::calcite,      },
        // { compounds::orthoclase,   },
        // { compounds::andesine,     },
        { compounds::augite,          2.5 * si::watt / (si::meter*si::kelvin), // Schön (2015), for basaltic lava
            },
        // { compounds::forsterite,   },
        // { compounds::goethite,     },
        // { compounds::pyrite,       },
        // { compounds::hematite,     },
        { compounds::gold,            105.0 * si::watt / (si::meter * si::kelvin), // Mills (1996)
            },
        { compounds::silver,          180.0 * si::watt / (si::meter * si::kelvin), // Mills (1996)
            },
        { compounds::copper,          160.0 * si::watt / (si::meter * si::kelvin), // Mills (1996)
            },
        // { compounds::magnetite,    },
        // { compounds::chalcocite,   },
        // { compounds::chalcopyrite, },
    };

}}
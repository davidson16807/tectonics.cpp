#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/ids.hpp>
#include <models/compound/relation/AnonymousTemperatureRelation.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<relation::AnonymousTemperatureRelation<si::dynamic_viscosity<double>>> dynamic_viscosity_as_liquid {
        { ids::water,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -52.843, 3703.6, 5.866, -5.879e-29, 10,
                      273.16, 646.15), // 273.16-646.15K
            },
        { ids::nitrogen,      // 157.9 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     16.004, -181.61, -5.1551, 0.0, 0,
                      63.15, 124.0), // 63.15-124K
            },
        { ids::oxygen,      // 188.0 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -4.1476, 94.04, -1.207, 0.0, 0,
                      54.36, 150.0), // 54.36-150K
            },
        { ids::carbon_dioxide,      // 0.0712 * si::millipascal*si::second,                             // wikipedia data page
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     18.775, -402.92, -4.6854, -6.9171e-26, 10,
                      216.58, 303.15), // 216.58-303.15K
            },
        { ids::methane,      // 118.6 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -6.1572, 178.15, -0.95239, -9.0606e-24, 10,
                      90.69, 188.0), // 90.69-188K
            },
        { ids::argon,      // 252.1 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal * si::second, 
                     -8.8685, 204.29, -0.38305, -1.294e-22, 10,
                      83.78, 150.0), // 83.78-150.0K
            },
        { ids::helium,      // 3.57 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -9.6312, -3.841, -1.458, -1.065e-8, 10,
                      2.2, 5.1), // 2.2-5.1K
            },
        { ids::hydrogen,      // 13.06 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -11.661, 24.7, -0.261, -4.10e-16, 10,
                      13.95, 33.0), // 13.95-33K
            },
        { ids::ammonia,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -6.743,598.3,-0.7341,-3.690e-27,10,
                      195.41, 393.15), // 195.41-393.15K
            },
        { ids::ozone,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -10.94, 415.96, 0.0, 0.0, 0,
                      75.55, 208.8), // 75.55-208.8K
            },
        { ids::nitrous_oxide,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     19.329, -381.68, -4.8618, 0.0, 0,
                      210.0, 283.09), // 210-283.09K
            },
        { ids::sulfur_dioxide,      // 0.368 * si::millipascal*si::second, // pubchem
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     46.223, -1378.0, -8.7475, 0.0, 0,
                      225.0, 400.0), // 225-400K
            },
        { ids::nitric_oxide,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -246.65, 3150.3, 49.98, -0.22541, 1.0,
                      109.5, 180.05), // 109.5-180.05K
            },
        { ids::carbon_monoxide,      // 0.170 * si::millipascal * si::second, // Johnson (1960)
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -4.9735, 97.67, -1.1088, 0.0, 0,
                      68.15, 131.37), // 68.15-131.37K
            },
        { ids::ethane,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -7.0046, 276.38, -0.6087, -3.11e-18, 7,
                      90.35, 300.0), // 90.35-300K
            },
        { ids::hydrogen_cyanide,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -21.927, 1266.5, 1.5927, 0.0, 0,
                      259.83, 298.85), // 259.83-298.85K
            },
        { ids::ethanol,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     7.875, 781.98, -3.0418, 0.0, 0,
                      200.0, 440.0), // 200-440K
            },
        { ids::formaldehyde,      // 0.1421 * si::millipascal * si::second, //pubchem
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -7.6591, 603.36, -0.53378, 0.0, 0,
                      155.15, 253.85), // 155.15-253.85K
            },
        { ids::formic_acid,      // 1.607 * si::millipascal*si::second,
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -48.529, 3394.7, 5.3903, 0.0, 0,
                      281.45, 373.71), // 281.45-373.71K
            },
        { ids::perflouromethane,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -9.9212, 300.5, 0.0, 0.0, 0,
                      89.56, 145.1), // 89.56-145.1K
            },
        { ids::benzene,      // 0.601 * si::millipascal * si::second, // engineering toolbox, at 300K
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     7.5117, 294.68, -2.794, 0.0, 0,
                      278.68, 545.0), // 278.68-545K
            },
        // { ids::pyrimidine,   },
        { ids::quartz,          exp(10.0) * si::poise, // Doremus (2002), at 1400 C
            },
        { ids::halite,      
                relation::get_exponential_linear_dynamic_viscosity_temperature_relation
                    (si::kelvin, si::millipascal*si::second,
                     1073.15,  1270.15, 
                     1.57,     0.708), // Dortmund data bank
            },
        { ids::corundum,      
                relation::get_dippr_liquid_dynamic_viscosity_temperature_relation_101
                    (si::kelvin, si::pascal* si::second, 
                     -8.2734, 11448.0, 0.0, 0.0, 0,
                      2400.0, 2750.0), // Blomquist (1978)
            },
        // { ids::apatite,      },
        // { ids::carbon,       },
        // { ids::calcite,      },
        { ids::orthoclase,      1e8 * si::poise, // observed by Bowen (1934) for molten Orthoclase, seems large compared to other silicates, but continental crust has been observed to have higher viscosity than oceanic (Itô 1979, "Rheology of the crust...", I can only get a hold of the abstract)
            },
        { ids::andesine,        1.38e2 * si::poise, // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
            // .dynamic_viscosity = exp(5.0) * si::poise, // Doremus (2002) for Albite, at 1400 C
            // .dynamic_viscosity = 4e4 * si::poise, // observed by Bowen (1934) for Albite at 1400C
            // .dynamic_viscosity = 3.8e1 * si::poise, // observed by McCaffery for molten anorthite at 1550C, referenced by Bowen (1934)
            // .dynamic_viscosity = 1.38e2 * si::poise, // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
            // .dynamic_viscosity = 3.0e11 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for andesite lava flow, middle value on log scale
            },
        // { ids::augite,       },
        { ids::forsterite,      1.27e2 * si::poise, // 1.36-1.19 poises, observed by Kani for olivine basaltic magma at 1400C, referenced by Bowen (1934)
            // .dynamic_viscosity = exp(1.5) * si::poise, // Doremus (2002) for Olivine, at 1400 C
            },
        // { ids::goethite,     },
        // { ids::pyrite,       },
        // { ids::hematite,     },
        // { ids::gold,      
        //         relation::get_exponential_interpolated_inverse_dynamic_viscosity_temperature_function
        //             (si::kelvin, si::centipoise,
        //              0.6, 0.725, 
        //              4.0, 5.3  ), // Chapman (1966)
        //     },
        // { ids::silver,      
        //         relation::get_exponential_interpolated_inverse_dynamic_viscosity_temperature_function
        //             (si::kelvin, si::centipoise,
        //              0.6, 0.8,
        //              2.4, 4.0) // Chapman (1966)
        //     },
        // { ids::copper,      
        //         relation::get_exponential_interpolated_inverse_dynamic_viscosity_temperature_function
        //             (si::kelvin, si::centipoise,
        //              0.6, 0.725, 
        //              2.4, 5.0  ), // Chapman (1966)
        //     }
        // { ids::magnetite,    },
        // { ids::chalcocite,   },
        // { ids::chalcopyrite, },
    };

}}
#pragma once

// in-house libraries
#include <model/compound/ids.hpp>
#include <model/compound/relation/gas/GasPropertyStateRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using GasDynamicViscosityStateRelation = relation::GasPropertyStateRelation<si::dynamic_viscosity<double>>;
    table::PartialTable<GasDynamicViscosityStateRelation> dynamic_viscosity_as_gas {
        { compounds::water,      
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.00019, 3.33694, 0.02183, 1.08016, -0.58257,
                     300.0, 1273.2,  0.0, 10.0, 0.035), 
                    // water, mean error: 1.2%, max error: 3.5%, range: 300-1273.2K, 0-10MPa, stp estimate: 8.765
            },
        { compounds::nitrogen,      // 1.76e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.04908, 2.30670, 0.50611, 0.64605, -2.56297,
                     113.15, 2013.2,  0.0, 3.0, 0.051), 
                    // nitrogen, mean error: 1.5%, max error: 5.1%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 16.416
            },
        { compounds::oxygen,      
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    -0.08909, 0.00000, 0.39475, 0.70840, -2.08620,
                     104.36, 1004.4,  0.0, 3.0, 0.081), 
                    // oxygen, mean error: 2.1%, max error: 8.1%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 18.827
            },
        { compounds::carbon_dioxide,      // 1.47e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.05967, 1.75501, 0.18576, 0.79011, -1.65245,
                     266.59, 1116.6,  0.0, 3.0, 0.032), 
                    // carbon dioxide, mean error: 1.5%, max error: 3.2%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 13.980
            },
        { compounds::methane,      // 1.10e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.12223, 1.53060, 0.09521, 0.83600, -0.21904,
                     140.69, 640.69,  0.0, 3.0, 0.052), 
                    // methane, mean error: 1.9%, max error: 5.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 10.148
            },
        { compounds::argon,      // 2.23e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.41072, 0.79782, 0.22045, 0.81080, -0.33331,
                     133.81, 683.81,  0.0, 3.0, 0.042), 
                    // argon, mean error: 1.7%, max error: 4.2%, range: 133.81-683.81K, 0-3MPa, stp estimate: 20.565
            },
        { compounds::helium,      // 1.96e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    -0.19093, 1.50359, 0.28815, 0.73057, 1.34631,
                     52.177, 1502.2,  0.0, 0.1, 0.007), 
                    // helium, mean error: 0.1%, max error: 0.7%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 18.702
            },
        { compounds::hydrogen,      // 0.88e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.05907, 1.95272, 0.20949, 0.66373, -0.28287,
                     63.957, 1014,  0.0, 1.0, 0.009), 
                    // hydrogen, mean error: 0.2%, max error: 0.9%, range: 63.957-1014K, 0-1MPa, stp estimate: 8.393
            },
        { compounds::ammonia,      // 0.99e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.00986, 1.88109, 0.02443, 1.06310, -0.30191,
                     245.5, 695.5,  0.0, 10.0, 0.035), 
                    // ammonia, mean error: 0.7%, max error: 3.5%, range: 245.5-695.5K, 0-10MPa, stp estimate: 9.204
            },
        { compounds::ozone,      
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    1.196e-7, 0.84797, 0.0, 0.0,
                    80.15, 1000.0), // 80.15-1000K
            },
        { compounds::nitrous_oxide,      // 1.47e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    2.1150e-6, 0.4642, 305.7, 0.0,
                    182.3, 1000.0), // 182.3-1000 K
            },
        { compounds::sulfur_dioxide,      // 1.26e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    6.8630e-7, 0.6112, 217.0, 0.0,
                    197.67, 1000.0), // 197.67-1000K
            },
        { compounds::nitric_oxide,      // 0.0188 * si::millipascal * si::second, //pubchem
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    1.4670e-6, 0.5123, 125.4, 0.0,
                    110.0, 1500.0), // 110-1500K
            },
        { compounds::carbon_monoxide,      // 1.74e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.26687, 1.09457, 0.33802, 0.70825, -1.67961,
                     118.16, 518.16,  0.0, 3.0, 0.029), 
                    // carbon monoxide, mean error: 1.2%, max error: 2.9%, range: 118.16-518.16K, 0-3MPa, stp estimate: 16.311
            },
        { compounds::ethane,      // 9.4 * si::micropascal*si::second,
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.07538, 2.19443, 0.07385, 0.85870, -0.57044,
                     140.35, 640.35,  0.0, 3.0, 0.029), 
                    // ethane, mean error: 1.2%, max error: 2.9%, range: 140.35-640.35K, 0-3MPa, stp estimate: 8.560
            },
        { compounds::hydrogen_cyanide,      
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    1.2780e-8, 1.0631, 340.0, 0.0,
                    183.65, 1000.0), // 183.65-1000K
            },
        { compounds::ethanol,      
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    1.0613e-7, 0.8066, 52.7, 0.0,
                    200.0, 1000.0), // 200-1000K
            },
        { compounds::formaldehyde,      
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    1.5948e-5, 0.21516, 1151.1, 0.0,
                    167.55, 1000.0), // 167.55-1000K
            },
        { compounds::formic_acid,      
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    5.0702e-8, 0.9114, 0.0, 0.0,
                    281.45, 1000.0), // 281.45-1000K
            },
        { compounds::perflouromethane,      
                relation::get_exponent_pressure_temperature_relation
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.22721, 1.71531, 0.16433, 0.82188, -0.84952,
                     148.94, 598.94,  0.0, 3.0, 0.049), 
                    // tetrafluoromethane, mean error: 2.0%, max error: 4.9%, range: 148.94-598.94K, 0-3MPa, stp estimate: 15.680
            },
        { compounds::benzene,      // 0.75e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
                relation::get_dippr_temperature_relation_102
                    (si::kelvin, si::pascal * si::second,
                    3.1340e-8, 0.9676, 7.9, 0.0,
                    278.68, 1000.0), // 278.68-1000K
            },
        // {compounds::pyrimidine,    }
        // {compounds::halite,       }
        // {compounds::corundum,     }
        // {compounds::apatite,      }
        // {compounds::carbon,        }
        // {compounds::calcite,      }
        // {compounds::orthoclase,   }
        // {compounds::andesine,      }
        // {compounds::augite,        }
        // {compounds::forsterite,    }
        // {compounds::goethite,     }
        // {compounds::pyrite,       }
        // {compounds::hematite,      }
        // {compounds::gold,         }
        // {compounds::silver,       }
        // {compounds::copper,       }
        // {compounds::magnetite,    }
        // {compounds::chalcocite,    }
        // {compounds::chalcopyrite, }
    };

}}
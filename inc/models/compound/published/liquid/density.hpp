#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/ids.hpp>
#include <models/compound/relation/liquid/LiquidDensityTemperatureRelation.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LiquidDensityTemperatureRelation = relation::LiquidDensityTemperatureRelation;
    table::PartialTable<LiquidDensityTemperatureRelation> density_as_liquid {
        { compounds::water,                
                // Perry equation 119, specialized for water
                relation::LiquidDensityTemperatureRelation({}, 
                    {
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(+35.618f,     647.096f, 0.33f)),      
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(+19.655f,     647.096f, 0.66f)),     
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(-9.1306f,     647.096f, 5.0f/3.0f)),      
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(-31.367f,     647.096f, 16.0f/3.0f)),      
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(-813.56f,     647.096f, 43.0f/3.0f)),      
                        relation::ClampedDippr119Term(273.16f, 647.096f, dippr::Dippr119Term(-17421000.0f, 647.096f, 110.0f/3.0f)),
                    }, si::kelvin, 18.015 * si::gram / si::decimeter3, 17.874f, 0.0f),
            },
        { compounds::nitrogen,        // 0.4314*si::gram/si::centimeter3, // Johnson (1960), @ si::atmosphere, 125.01*si::kelvin
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 28.013  * si::gram/si::decimeter3,
                     3.2091, 0.2861, 126.2, 0.2966,
                     63.15, 126.2), // 63.15-126.2K
            },
        { compounds::oxygen,                // 1.141 * si::gram/si::milliliter,
                // relation::StateSample<si::density<double>>(0.6779*si::gram/si::centimeter3, si::atmosphere, 149.8*si::kelvin), // Johnson (1960)
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 31.9988 * si::gram/si::decimeter3,
                     3.9143, 0.28772, 154.58, 0.2924,
                     54.35, 154.58), // 54.35-154.58K
            },
        { compounds::carbon_dioxide,                // 1101.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 44.01 * si::gram/si::decimeter3,
                     2.768, 0.26212, 304.21, 0.2908,
                     216.58, 304.21), // 216.58-304.21K
            },
        { compounds::methane,                
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 16.043 * si::gram/si::decimeter3,
                     2.9214, 0.28976, 190.56, 0.28881,
                     90.69, 190.56), // 90.69-190.56K
            },
        { compounds::argon,                
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 39.948 * si::gram/si::decimeter3,
                     3.8469, 0.2881, 150.86, 0.29783,
                     3.78, 150.86), // 83.78, 150.86K
            },
        { compounds::helium,                
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 4.0026 * si::gram/si::decimeter3,
                     7.2475, 0.41865, 5.2, 0.24096,
                     2.2, 5.2), // 2.2-5.2K
            },
        { compounds::hydrogen,                // 0.0708 * si::gram/si::milliliter,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 2.016 * si::gram/si::decimeter3,
                     5.414, 0.34893, 33.19, 0.2706,
                     13.95, 33.19), // 13.95-33.19K
            },
        { compounds::ammonia,                // 681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 17.031 * si::gram/si::decimeter3,
                     3.5383, 0.25443, 405.65, 0.2888,
                     195.41, 405.65), // 195.41-405.65K
            },
        { compounds::ozone,                // 1349.0 * si::kilogram / si::meter3, //encyclopedia.airliquide.com
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 47.998 * si::gram/si::decimeter3,
                     3.3592, 0.29884, 261.0, 0.28523,
                     80.15, 261.0), // 80.15-261.0K
            },
        { compounds::nitrous_oxide,                // 1230.458 * si::kilogram / si::meter3, 
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 44.012 * si::gram/si::decimeter3,
                     2.781, 0.27244, 309.57, 0.2882,
                     182.3, 309.57), // 182.3-309.57K
            },
        { compounds::sulfur_dioxide,                // 389.06 * si::kilogram / si::meter3, // encyclopedia.airliquide.com
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 64.064 * si::gram/si::decimeter3,
                     2.106, 0.25842, 430.75, 0.2895,
                     197.67, 430.75), // 197.67-430.75K
            },
        { compounds::nitric_oxide,                // 1230.458 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 30.006 * si::gram/si::decimeter3,
                     5.246, 0.3044, 180.15, 0.242,
                     109.5, 180.15), // 109.5-180.15K
            },
        { compounds::carbon_monoxide,                
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 28.010 * si::gram/si::decimeter3,
                     2.897, 0.27532, 132.92, 0.2813,
                     68.15, 132.92), // 68.15-132.92K
            },
        { compounds::ethane,                // 545.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 30.070 * si::gram/si::decimeter3,
                     1.9122, 0.27937, 305.32, 0.29187,
                     90.35, 305.32), // 90.35-305.32K
            },
        { compounds::hydrogen_cyanide,                // 687.6 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 27.026 * si::gram/si::decimeter3,
                     1.3413, 0.18589, 456.65, 0.28206,
                     259.83, 456.65), // 259.83-456.65K
            },
        { compounds::ethanol,                // 0789.3 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 46.068 * si::gram/si::decimeter3,
                     1.6288, 0.27469, 514.0, 0.23178,
                     159.05, 514), // 159.05-514K
            },
        { compounds::formaldehyde,                // 0.8153 * si::kilogram/si::meter3,  // wikipedia
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 30.026 * si::gram/si::decimeter3,
                     3.897011, 0.331636, 420.0, 0.28571,
                     155.15, 420.0), // 155.15-420.0K
            },
        { compounds::formic_acid,                // 1220.0 * si::kilogram/si::meter3,
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 46.026 * si::gram/si::decimeter3,
                     1.938, 0.24225, 588.0, 0.24435,
                     281.45, 588.0), // 281.45-588.0K
            },
        { compounds::perflouromethane,                // 1890.0 * si::kilogram/si::meter3, //pubchem
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 88.0  * si::gram/si::decimeter3,
                     1.955, 0.27884, 227.51, 0.28571,
                     89.56, 227.51), // 89.56-227.51K
            },
        { compounds::benzene,                // 879.0 * si::kilogram/si::meter3, // Perry
                relation::get_dippr_temperature_relation_105
                    (si::kelvin, 79.102 * si::gram/si::decimeter3,
                     1.0259, 0.266666, 562.05, 0.28394,
                     278.68, 562.05), // 278.68-562.05K
            },
        { compounds::pyrimidine,      1.016 * si::gram/si::centimeter3, // wikipedia
            },
        { compounds::quartz,          2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
            },
        { compounds::halite,          1549.0 * si::kilogram/si::meter3,  // wikipedia data page
            },
        // { compounds::corundum,     },
        // { compounds::apatite,      },
        { compounds::carbon,          1.6*si::gram/si::centimeter3, // Bundy, referenced in Steinbeck (1990)
            },
        // { compounds::calcite,      },
        { compounds::orthoclase,      2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
            },
        { compounds::andesine,        2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
            },
        { compounds::augite,          2800.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for basaltic  magma
            },
        // { compounds::forsterite,   },
        // { compounds::goethite,     },
        // { compounds::pyrite,       },
        // { compounds::hematite,     },
        { compounds::gold,            17310.0 * si::kilogram/si::meter3, 
            },
        { compounds::silver,          9320.0 * si::kilogram/si::meter3, 
            },
        { compounds::copper,          8020.0 * si::kilogram/si::meter3, 
            },
        // { compounds::magnetite,    },
        // { compounds::chalcocite,   },
        // { compounds::chalcopyrite, },
    };


}}
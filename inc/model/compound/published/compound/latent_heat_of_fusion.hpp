#pragma once

// in-house libraries
#include <unit/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/relation/compound/LatentHeatTemperatureRelation.hpp>

#include "latent_heat_of_vaporization.hpp"

namespace compound { 
namespace published { 

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_fusion {
        {compounds::water,            6.01*si::kilojoule/(18.015*si::gram)},
        {compounds::nitrogen,         0.71*si::kilojoule/(28.013*si::gram)},
        {compounds::oxygen,           0.44*si::kilojoule/(31.9988*si::gram)},
        {compounds::carbon_dioxide,   9.02*si::kilojoule/(44.01*si::gram)},
        {compounds::methane,          0.94*si::kilojoule/(16.043*si::gram)},
        {compounds::argon,            5.66*si::kilojoule/(39.948*si::gram)},
        {compounds::helium,           0.0138*si::kilojoule/(4.0026  * si::gram)}, // periodic-table.org
        {compounds::hydrogen,         0.12*si::kilojoule/(2.016*si::gram)},
        {compounds::ammonia,          5.66*si::kilojoule/(17.031*si::gram)},
        // {compounds::ozone,            },
        {compounds::nitrous_oxide,    6.54*si::kilojoule/(44.012*si::gram)},
        {compounds::sulfur_dioxide,   1769.0 * si::calorie/(64.064 * si::gram)}, // Perry
        {compounds::nitric_oxide,     2.3*si::kilojoule/(30.006*si::gram)},
        {compounds::carbon_monoxide,  0.833*si::kilojoule/(28.010*si::gram)},
        {compounds::ethane,           2.72*si::kilojoule/(30.070*si::gram)},
        {compounds::hydrogen_cyanide, 2009.0*si::calorie/(27.026 * si::gram)}, // Perry
        {compounds::ethanol,          4.931*si::kilojoule/(46.068*si::gram)},
        {compounds::formaldehyde,     7.53 * si::kilojoule/(30.026*si::gram)},// Vasil'ev et al. (1998)
        {compounds::formic_acid,      12.68*si::kilojoule/(46.026*si::gram)},
        {compounds::perflouromethane, 0.704*si::kilojoule/(88.0*si::gram)},
        {compounds::benzene,          9.87*si::kilojoule/(79.102*si::gram)},
        // {compounds::pyrimidine,       },
        {compounds::quartz,           9.76*si::kilojoule/(60.08*si::gram)}, // cristobalite
        {compounds::halite,           28.16*si::kilojoule/(90.442*si::gram)},
        {compounds::corundum,         900.0*si::kilojoule / si::kilogram},
        // {compounds::apatite,          },
        {compounds::carbon,           117.4*si::kilojoule/(12.011*si::gram)},
        // {compounds::calcite,          },
        {compounds::orthoclase,       100.0 * si::calorie/si::gram}, // Perry
        {compounds::andesine,         100.0 * si::calorie/si::gram}, // Perry
        // {compounds::augite,           },
        {compounds::forsterite,       130.0 * si::calorie/si::gram}, // Perry, for Olivine (85 for fayalite)
        // {compounds::goethite,         },
        // {compounds::pyrite,           },
        // {compounds::hematite,         },
        {compounds::gold,             12.55*si::kilojoule/(196.967*si::gram)},
        {compounds::silver,           11.3*si::kilojoule/(107.868*si::gram)},
        {compounds::copper,           13.26*si::kilojoule/(63.546*si::gram)},
        // {compounds::magnetite,        },
        // {compounds::chalcocite,       },
        // {compounds::chalcopyrite,     },
    };

}}
#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    std::map<int, si::specific_energy<double>> latent_heat_of_fusion {
        {ids::water,            6.01*si::kilojoule/(18.015*si::gram)},
        {ids::nitrogen,         0.71*si::kilojoule/(28.013*si::gram)},
        {ids::oxygen,           0.44*si::kilojoule/(31.9988*si::gram)},
        {ids::carbon_dioxide,   9.02*si::kilojoule/(44.01*si::gram)},
        {ids::methane,          0.94*si::kilojoule/(16.043*si::gram)},
        {ids::argon,            5.66*si::kilojoule/(39.948*si::gram)},
        // {ids::helium,           },
        {ids::hydrogen,         0.12*si::kilojoule/(2.016*si::gram)},
        {ids::ammonia,          5.66*si::kilojoule/(17.031*si::gram)},
        // {ids::ozone,            },
        {ids::nitrous_oxide,    6.54*si::kilojoule/(44.012*si::gram)},
        {ids::sulfur_dioxide,   1769.0 * si::calorie/(64.064 * si::gram)}, // Perry
        {ids::nitric_oxide,     2.3*si::kilojoule/(30.006*si::gram)},
        {ids::carbon_monoxide,  0.833*si::kilojoule/(28.010*si::gram)},
        {ids::ethane,           2.72*si::kilojoule/(30.070*si::gram)},
        {ids::hydrogen_cyanide, 2009.0*si::calorie/(27.026 * si::gram)}, // Perry
        {ids::ethanol,          4.931*si::kilojoule/(46.068*si::gram)},
        {ids::formaldehyde,     7.53 * si::kilojoule/(30.026*si::gram)},// Vasil'ev et al. (1998)
        {ids::formic_acid,      12.68*si::kilojoule/(46.026*si::gram)},
        {ids::perflouromethane, 0.704*si::kilojoule/(88.0*si::gram)},
        {ids::benzene,          9.87*si::kilojoule/(79.102*si::gram)},
        // {ids::pyrimidine,       },
        {ids::quartz,           9.76*si::kilojoule/(60.08*si::gram)}, // cristobalite
        {ids::halite,           28.16*si::kilojoule/(90.442*si::gram)},
        {ids::corundum,         900.0*si::kilojoule / si::kilogram},
        // {ids::apatite,          },
        {ids::carbon,           117.4*si::kilojoule/(12.011*si::gram)},
        // {ids::calcite,          },
        {ids::orthoclase,       100.0 * si::calorie/si::gram}, // Perry
        {ids::andesine,         100.0 * si::calorie/si::gram}, // Perry
        // {ids::augite,           },
        {ids::forsterite,       130.0 * si::calorie/si::gram}, // Perry, for Olivine (85 for fayalite)
        // {ids::goethite,         },
        // {ids::pyrite,           },
        // {ids::hematite,         },
        {ids::gold,             12.55*si::kilojoule/(196.967*si::gram)},
        {ids::silver,           11.3*si::kilojoule/(107.868*si::gram)},
        {ids::copper,           13.26*si::kilojoule/(63.546*si::gram)},
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}
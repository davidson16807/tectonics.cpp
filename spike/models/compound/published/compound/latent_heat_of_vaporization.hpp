#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    std::map<int, si::specific_energy<double>> latent_heat_of_vaporization {
        {ids::water,            22.6e5 * si::joule/si::kilogram}, 
        {ids::nitrogen,         198.8 * si::joule/si::gram},
        {ids::oxygen,           213.1 * si::joule/si::gram},
        {ids::carbon_dioxide,   205.0 * si::joule / si::gram},
        {ids::methane,          510.83 * si::joule/si::gram},
        {ids::argon,            161.0 * si::joule/si::gram},
        {ids::helium,           20.7 * si::joule / si::gram},
        {ids::hydrogen,         445.0 * si::joule/si::gram},
        {ids::ammonia,          23.33*si::kilojoule/(17.031*si::gram)},
        {ids::ozone,            288.49 * si::kilojoule / si::kilogram},
        {ids::nitrous_oxide,    374.286 * si::kilojoule / si::kilogram}, //encyclopedia.airliquide.com
        {ids::sulfur_dioxide,   24.94 *si::kilojoule/(64.064*si::gram)},
        {ids::nitric_oxide,     13.83 * si::kilojoule/(30.006*si::gram)},
        {ids::carbon_monoxide,  6.04 * si::kilojoule/(28.010*si::gram)},
        {ids::ethane,           14.69 * si::kilojoule/(30.070*si::gram)},
        {ids::hydrogen_cyanide, 6027.0*si::calorie/(27.026 * si::gram)}, // Perry
        {ids::ethanol,          42.32 * si::kilojoule/(46.068*si::gram)}, 
        {ids::formaldehyde,     23.3 * si::kilojoule/(30.026*si::gram)},//pubchem
        {ids::formic_acid,      20.10 * si::kilojoule/(46.026*si::gram)},
        {ids::perflouromethane, 137000.0 * si::joule / si::kilogram},
        {ids::benzene,          33.83 * si::kilojoule / (79.109 * si::gram)}, 
        {ids::pyrimidine,       49.79*79 * si::kilojoule / (80.088*si::gram)},
        {ids::quartz,           11770e3 * si::joule / si::kilogram},
        {ids::halite,           40810.0 * si::calorie/(80.088 * si::gram)}, // Perry
        // {ids::corundum,         },
        // {ids::apatite,          },
        {ids::carbon,           355.8*si::kilojoule/(12.011*si::gram)},
        // {ids::calcite,          },
        // {ids::orthoclase,       },
        // {ids::andesine,         },
        // {ids::augite,           },
        // {ids::forsterite,       },
        // {ids::goethite,         },
        // {ids::pyrite,           },
        // {ids::hematite,         },
        {ids::gold,             324.0 * si::kilojoule/(196.967*si::gram)},
        {ids::silver,           60720.0 *  si::calorie/(107.868 * si::gram)}, // Perry
        {ids::copper,           204.8 * si::joule/si::gram},
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}
#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    std::map<int, double> acentric_factor {
        {ids::water,            0.345},
        {ids::nitrogen,         0.040},
        {ids::oxygen,           0.022},
        {ids::carbon_dioxide,   0.228},
        {ids::methane,          0.011},
        {ids::argon,            0.001},
        {ids::helium,           -0.39},
        {ids::hydrogen,         -0.215},
        {ids::ammonia,          0.252},
        {ids::ozone,            0.227},
        {ids::nitrous_oxide,    0.142},
        {ids::sulfur_dioxide,   0.245},
        {ids::nitric_oxide,     0.585},
        {ids::carbon_monoxide,  0.066},
        {ids::ethane,           0.099},
        {ids::hydrogen_cyanide, 0.407},
        {ids::ethanol,          0.637},
        {ids::formaldehyde,     0.282},
        {ids::formic_acid,      0.473},
        {ids::perflouromethane, 0.186},
        {ids::benzene,          0.211},
        // {ids::pyrimidine,       },
        // {ids::quartz,           },
        // {ids::halite,           },
        // {ids::corundum,         },
        // {ids::apatite,          },
        // {ids::carbon,           },
        // {ids::calcite,          },
        // {ids::orthoclase,       },
        // {ids::andesine,         },
        // {ids::augite,           },
        // {ids::forsterite,       },
        // {ids::goethite,         },
        // {ids::pyrite,           },
        // {ids::hematite,         },
        // {ids::gold,             },
        // {ids::silver,           },
        // {ids::copper,           },
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}
#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/table/PartialTable.hpp>
#include <model/compound/ids.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<double> acentric_factor {
        {compounds::water,            0.345},
        {compounds::nitrogen,         0.040},
        {compounds::oxygen,           0.022},
        {compounds::carbon_dioxide,   0.228},
        {compounds::methane,          0.011},
        {compounds::argon,            0.001},
        {compounds::helium,           -0.39},
        {compounds::hydrogen,         -0.215},
        {compounds::ammonia,          0.252},
        {compounds::ozone,            0.227},
        {compounds::nitrous_oxide,    0.142},
        {compounds::sulfur_dioxide,   0.245},
        {compounds::nitric_oxide,     0.585},
        {compounds::carbon_monoxide,  0.066},
        {compounds::ethane,           0.099},
        {compounds::hydrogen_cyanide, 0.407},
        {compounds::ethanol,          0.637},
        {compounds::formaldehyde,     0.282},
        {compounds::formic_acid,      0.473},
        {compounds::perflouromethane, 0.186},
        {compounds::benzene,          0.211},
        // {compounds::pyrimidine,       },
        // {compounds::quartz,           },
        // {compounds::halite,           },
        // {compounds::corundum,         },
        // {compounds::apatite,          },
        // {compounds::carbon,           },
        // {compounds::calcite,          },
        // {compounds::orthoclase,       },
        // {compounds::andesine,         },
        // {compounds::augite,           },
        // {compounds::forsterite,       },
        // {compounds::goethite,         },
        // {compounds::pyrite,           },
        // {compounds::hematite,         },
        // {compounds::gold,             },
        // {compounds::silver,           },
        // {compounds::copper,           },
        // {compounds::magnetite,        },
        // {compounds::chalcocite,       },
        // {compounds::chalcopyrite,     },
    };

}}
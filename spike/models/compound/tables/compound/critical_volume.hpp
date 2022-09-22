#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, si::molar_volume<double>> critical_volume {
        {ids::water,            56.0 *  si::centimeter3/si::mole},
        {ids::nitrogen,         90.0 * si::centimeter3/si::mole},
        {ids::oxygen,           73.0 * si::centimeter3/si::mole},
        {ids::carbon_dioxide,   94.0 * si::centimeter3/si::mole},
        {ids::methane,          99.0 * si::centimeter3/si::mole},
        {ids::argon,            75.0 * si::centimeter3/si::mole},
        {ids::helium,           57.0 * si::centimeter3/si::mole},
        {ids::hydrogen,         65.0 * si::centimeter3/si::mole},
        {ids::ammonia,          69.8*si::centimeter3/si::mole},
        {ids::ozone,            89.0 * si::centimeter3/si::mole},
        // {ids::nitrous_oxide,    }
        {ids::sulfur_dioxide,   122.0 * si::centimeter3/si::mole},
        {ids::nitric_oxide,     58.0 *  si::centimeter3/si::mole},
        {ids::carbon_monoxide,  93.0 * si::centimeter3/si::mole},
        {ids::ethane,           146.0 * si::centimeter3/si::mole},
        // {ids::hydrogen_cyanide, }
        {ids::ethanol,          169.0 * si::centimeter3/si::mole},
        // {ids::formaldehyde,     }
        {ids::formic_acid,      115.9*9*si::centimeter3/si::mole},
        {ids::perflouromethane, 140.0 * si::centimeter3/si::mole},
        {ids::benzene,          257.0*si::centimeter3/si::mole},
        // {ids::pyrimidine,       }
        // {ids::quartz,           }
        // {ids::halite,           }
        // {ids::corundum,         }
        // {ids::apatite,          }
        // {ids::carbon,           }
        // {ids::calcite,          }
        // {ids::orthoclase,       }
        // {ids::andesine,         }
        // {ids::augite,           }
        // {ids::forsterite,       }
        // {ids::goethite,         }
        // {ids::pyrite,           }
        // {ids::hematite,         }
        // {ids::gold,             }
        // {ids::silver,           }
        // {ids::copper,           }
        // {ids::magnetite,        }
        // {ids::chalcocite,       }
        // {ids::chalcopyrite,     }
    };

}}
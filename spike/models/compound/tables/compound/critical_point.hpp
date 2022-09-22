#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"
#include "../state.hpp"

namespace compound { 
namespace tables { 

    std::map<int, state<double>> critical_point {
        {ids::water,            state<double>(22.06 *  si::megapascal, 647.01 * si::kelvin )},
        {ids::nitrogen,         state<double>(3.39 * si::megapascal,   126.21 * si::kelvin )},
        {ids::oxygen,           state<double>(5.043 * si::megapascal,  154.59 * si::kelvin )},
        {ids::carbon_dioxide,   state<double>(7.375 * si::megapascal,  304.13 * si::kelvin )},
        {ids::methane,          state<double>(4.60 * si::megapascal,   190.56 * si::kelvin )},
        {ids::argon,            state<double>(4.898* si::megapascal,   150.87 * si::kelvin )},
        {ids::helium,           state<double>(0.227 * si::megapascal,  5.19 * si::kelvin   )},
        {ids::hydrogen,         state<double>(1.293 *  si::megapascal, 32.97 * si::kelvin  )},
        {ids::ammonia,          state<double>(11.357*si::megapascal,   405.56 * si::kelvin )},
        {ids::ozone,            state<double>(5.57 *  si::megapascal,  261.1 * si::kelvin  )},
        {ids::nitrous_oxide,    state<double>(7.245 *  si::megapascal, 309.52 * si::kelvin )},
        {ids::sulfur_dioxide,   state<double>(7.884 * si::megapascal,  430.64 * si::kelvin )},
        {ids::nitric_oxide,     state<double>(6.48 *  si::megapascal,  180.0 * si::kelvin  )},
        {ids::carbon_monoxide,  state<double>(3.494 *  si::megapascal, 132.86 * si::kelvin )},
        {ids::ethane,           state<double>(4.88 * si::megapascal,   305.36 * si::kelvin )},
        {ids::hydrogen_cyanide, state<double>(5.4 * si::megapascal,    456.65 * si::kelvin )},
        {ids::ethanol,          state<double>(6.25 * si::megapascal,   351.44 * si::kelvin )},
        {ids::formaldehyde,     state<double>(6.788 * si::megapascal,  410.3 * si::kelvin  )},
        // {ids::formic_acid,      ,                     588.0 * si::kelvin  },
        {ids::perflouromethane, state<double>(3.73 * si::megapascal,   227.54 * si::kelvin )},
        {ids::benzene,          state<double>(4.9 * si::megapascal,    562.0 * si::kelvin  )},
        // {ids::pyrimidine,       ,                                         },
        {ids::quartz,           state<double>(1.7e8 * si::pascal,      5300.0* si::kelvin  )},
        {ids::halite,           state<double>(26.0 * si::megapascal,   3900.0 * si::kelvin )}, // wikipedia data page
        // {ids::corundum,         ,                                         },
        // {ids::apatite,          ,                                         },
        {ids::carbon,           state<double>(2200.0 * si::atmosphere, 6810.0 * si::kelvin )},     // Leider (1973)
        // {ids::calcite,          ,                                         },
        // {ids::orthoclase,       ,                                         },
        // {ids::andesine,         ,                                         },
        // {ids::augite,           ,                                         },
        // {ids::forsterite,       ,                                         },
        // {ids::goethite,         ,                                         },
        // {ids::pyrite,           ,                                         },
        // {ids::hematite,         ,                                         },
        {ids::gold,             state<double>(510e3 * si::kilopascal,  7250.0 * si::kelvin )}, // wow!
        // {ids::silver,           ,                     6410.0 * si::kelvin },
        // {ids::copper,           ,                                         },
        // {ids::magnetite,        ,                                         },
        // {ids::chalcocite,       ,                                         },
        // {ids::chalcopyrite,     ,                                         },
    };

}}
#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"
#include "../state.hpp"

namespace compound { 
namespace tables { 

    std::map<int, state<double>> triple_point {

        {ids::water,            state<double>(0.6116e3*si::pascal,      273.15 * si::kelvin)},
        {ids::nitrogen,         state<double>(12.463 * si::kilopascal,  63.15 * si::kelvin)},
        {ids::oxygen,           state<double>(0.14633 * si::kilopascal, 54.35 * si::kelvin)},
        {ids::carbon_dioxide,   state<double>(517e3 * si::pascal,       216.56 * si::kelvin)},
        {ids::methane,          state<double>(11.696 * si::kilopascal,  90.694 * si::kelvin)},
        {ids::argon,            state<double>(68.95 * si::kilopascal,   83.8058 * si::kelvin)},
        {ids::helium,           state<double>(5.048e3 * si::pascal,     2.1768 * si::kelvin)},
        {ids::hydrogen,         state<double>(7.042 * si::kilopascal,   13.8 * si::kelvin)},
        {ids::ammonia,          state<double>(6.060e3 * si::pascal,     195.30 * si::kelvin)},
        {ids::ozone,            state<double>(7.346e-6 * si::bar,       -193.0 * si::celcius)}, //encyclopedia.airliquide.com
        {ids::nitrous_oxide,    state<double>(87.85 * si::kilopascal,   -90.82 * si::celcius)},   // wikipedia
        {ids::sulfur_dioxide,   state<double>(1.67e3 * si::pascal,      197.69 * si::kelvin)},
        {ids::nitric_oxide,     state<double>(87.85e3 * si::pascal,     182.34 * si::kelvin)},
        {ids::carbon_monoxide,  state<double>(15.35 * si::kilopascal,   68.15 * si::kelvin)},     // PubChem
        {ids::ethane,           state<double>(1.4 * si::pascal,         90.35 * si::kelvin)},
        {ids::hydrogen_cyanide, state<double>(0.153e3 * si::pascal,     259.7 * si::kelvin)},
        {ids::ethanol,          state<double>(0.00043 * si::pascal,     150.0 * si::kelvin)},  // wikipedia data page
        // {ids::formaldehyde,     155.10 * si::kelvin}, 
        {ids::formic_acid,      state<double>(2.2 * si::kilopascal,     281.4 * si::kelvin)},
        {ids::perflouromethane, state<double>(0.1012e3 * si::pascal,    89.54 * si::kelvin)},
        {ids::benzene,          state<double>(4.83 * si::kilopascal,    278.5 * si::kelvin)}, // wikipedia data page
        // {ids::pyrimidine,       155.1 * si::kelvin}, // NIST
        {ids::quartz,           state<double>(0.0003 * si::pascal,      1983.0 * si::kelvin)},
        {ids::halite,           state<double>(30.0 * si::pascal,        1074.0 * si::kelvin)}, // wikipedia data page
        // {ids::corundum,         },
        // {ids::apatite,          },
        {ids::carbon,           state<double>(107.0 * si::atmosphere,   4765.0 * si::kelvin)},       // Leider (1973)
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
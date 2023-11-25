#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/point.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<point<double>> triple_point {

        {compounds::water,            point<double>(0.6116e3*si::pascal,      273.15 * si::kelvin)},
        {compounds::nitrogen,         point<double>(12.463 * si::kilopascal,  63.15 * si::kelvin)},
        {compounds::oxygen,           point<double>(0.14633 * si::kilopascal, 54.35 * si::kelvin)},
        {compounds::carbon_dioxide,   point<double>(517e3 * si::pascal,       216.56 * si::kelvin)},
        {compounds::methane,          point<double>(11.696 * si::kilopascal,  90.694 * si::kelvin)},
        {compounds::argon,            point<double>(68.95 * si::kilopascal,   83.8058 * si::kelvin)},
        {compounds::helium,           point<double>(5.048e3 * si::pascal,     2.1768 * si::kelvin)},
        {compounds::hydrogen,         point<double>(7.042 * si::kilopascal,   13.8 * si::kelvin)},
        {compounds::ammonia,          point<double>(6.060e3 * si::pascal,     195.30 * si::kelvin)},
        {compounds::ozone,            point<double>(7.346e-6 * si::bar,       -193.0 * si::celcius)}, //encyclopedia.airliquide.com
        {compounds::nitrous_oxide,    point<double>(87.85 * si::kilopascal,   -90.82 * si::celcius)},   // wikipedia
        {compounds::sulfur_dioxide,   point<double>(1.67e3 * si::pascal,      197.69 * si::kelvin)},
        {compounds::nitric_oxide,     point<double>(87.85e3 * si::pascal,     182.34 * si::kelvin)},
        {compounds::carbon_monoxide,  point<double>(15.35 * si::kilopascal,   68.15 * si::kelvin)},     // PubChem
        {compounds::ethane,           point<double>(1.4 * si::pascal,         90.35 * si::kelvin)},
        {compounds::hydrogen_cyanide, point<double>(0.153e3 * si::pascal,     259.7 * si::kelvin)},
        {compounds::ethanol,          point<double>(0.00043 * si::pascal,     150.0 * si::kelvin)},  // wikipedia data page
        // {compounds::formaldehyde,     155.10 * si::kelvin}, 
        {compounds::formic_acid,      point<double>(2.2 * si::kilopascal,     281.4 * si::kelvin)},
        {compounds::perflouromethane, point<double>(0.1012e3 * si::pascal,    89.54 * si::kelvin)},
        {compounds::benzene,          point<double>(4.83 * si::kilopascal,    278.5 * si::kelvin)}, // wikipedia data page
        // {compounds::pyrimidine,       155.1 * si::kelvin}, // NIST
        {compounds::quartz,           point<double>(0.0003 * si::pascal,      1983.0 * si::kelvin)},
        {compounds::halite,           point<double>(30.0 * si::pascal,        1074.0 * si::kelvin)}, // wikipedia data page
        // {compounds::corundum,         },
        // {compounds::apatite,          },
        {compounds::carbon,           point<double>(107.0 * si::atmosphere,   4765.0 * si::kelvin)},       // Leider (1973)
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
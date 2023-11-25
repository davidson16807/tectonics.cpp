#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<double> molecular_degrees_of_freedom {
        {compounds::water,            6.8},
        {compounds::nitrogen,         5.0},
        {compounds::oxygen,           5.06},
        {compounds::carbon_dioxide,   6.96},
        {compounds::methane,          5.1},
        {compounds::argon,            3.0},
        {compounds::helium,           3.0},
        {compounds::hydrogen,         4.96},
        {compounds::ammonia,          6.0},
        {compounds::ozone,            6.0},
        {compounds::nitrous_oxide,    7.32},
        {compounds::sulfur_dioxide,   6.0},
        // {compounds::nitric_oxide,     },
        {compounds::carbon_monoxide,  5.0},
        {compounds::ethane,           6.8},
        // {compounds::hydrogen_cyanide, },
        {compounds::ethanol,          6.0},
        {compounds::formaldehyde,     6.0},
        {compounds::formic_acid,      6.0},
        // {compounds::perflouromethane, },
        {compounds::benzene,          6.0}, // wikipedia
        {compounds::pyrimidine,       6.0}, // wikipedia
        {compounds::quartz,           6.0},
        // {compounds::halite,           },
        {compounds::corundum,         3.0},
        {compounds::apatite,          3.0},
        {compounds::carbon,           3.0},
        {compounds::calcite,          6.0},
        {compounds::orthoclase,       6.0},
        {compounds::andesine,         6.0},
        {compounds::augite,           6.0},
        {compounds::forsterite,       6.0},
        {compounds::goethite,         6.0},
        {compounds::pyrite,           6.0},
        {compounds::hematite,         6.0},
        {compounds::gold,             3.0},
        {compounds::silver,           3.0},
        {compounds::copper,           3.0},
        {compounds::magnetite,        6.0},
        {compounds::chalcocite,       6.0},
        {compounds::chalcopyrite,     6.0},
    };

}}
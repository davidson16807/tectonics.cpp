#pragma once

// std libraries
#include <map>

// in-house libraries
#include <units/si.hpp>

#include "../ids.hpp"

namespace compound { 
namespace tables { 

    std::map<int, double> molecular_degrees_of_freedom {
        {ids::water,            6.8},
        {ids::nitrogen,         5.0},
        {ids::oxygen,           5.06},
        {ids::carbon_dioxide,   6.96},
        {ids::methane,          5.1},
        {ids::argon,            3.0},
        {ids::helium,           3.0},
        {ids::hydrogen,         4.96},
        {ids::ammonia,          6.0},
        {ids::ozone,            6.0},
        {ids::nitrous_oxide,    7.32},
        {ids::sulfur_dioxide,   6.0},
        // {ids::nitric_oxide,     },
        {ids::carbon_monoxide,  5.0},
        {ids::ethane,           6.8},
        // {ids::hydrogen_cyanide, },
        {ids::ethanol,          6.0},
        {ids::formaldehyde,     6.0},
        {ids::formic_acid,      6.0},
        // {ids::perflouromethane, },
        {ids::benzene,          6.0}, // wikipedia
        {ids::pyrimidine,       6.0}, // wikipedia
        {ids::quartz,           6.0},
        // {ids::halite,           },
        {ids::corundum,         3.0},
        {ids::apatite,          3.0},
        {ids::carbon,           3.0},
        {ids::calcite,          6.0},
        {ids::orthoclase,       6.0},
        {ids::andesine,         6.0},
        {ids::augite,           6.0},
        {ids::forsterite,       6.0},
        {ids::goethite,         6.0},
        {ids::pyrite,           6.0},
        {ids::hematite,         6.0},
        {ids::gold,             3.0},
        {ids::silver,           3.0},
        {ids::copper,           3.0},
        {ids::magnetite,        6.0},
        {ids::chalcocite,       6.0},
        {ids::chalcopyrite,     6.0},
    };

}}
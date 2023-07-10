#pragma once

// in-house libraries
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<double> simon_glatzel_exponent({
        {ids::water,            60.0},
        {ids::nitrogen,         1.791},
        {ids::oxygen,           1.742594},
        {ids::argon,            1.593},
        {ids::helium,           1.5602},
        {ids::hydrogen,         1.74407},
        {ids::carbon_dioxide,   2.6},
        {ids::formic_acid,      5.2},
        {ids::ammonia,          4.3},
        {ids::hydrogen_cyanide, 3.6},
        {ids::ethanol,          1.61},
        {ids::benzene,          2.6},
        {ids::silver,           7.6},
    });
}}

#pragma once

// in-house libraries
#include <model/compound/table/PartialTable.hpp>
#include <model/compound/ids.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<double> simon_glatzel_exponent({
        {compounds::water,            60.0},
        {compounds::nitrogen,         1.791},
        {compounds::oxygen,           1.742594},
        {compounds::argon,            1.593},
        {compounds::helium,           1.5602},
        {compounds::hydrogen,         1.74407},
        {compounds::carbon_dioxide,   2.6},
        {compounds::formic_acid,      5.2},
        {compounds::ammonia,          4.3},
        {compounds::hydrogen_cyanide, 3.6},
        {compounds::ethanol,          1.61},
        {compounds::benzene,          2.6},
        {compounds::silver,           7.6},
    });
}}

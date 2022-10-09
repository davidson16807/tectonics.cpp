#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<si::pressure<double>> simon_glatzel_scale({
        {ids::water,            620.0 * si::bar},
        {ids::nitrogen,         1606.56 * si::bar},
        {ids::oxygen,           2732.95 * si::bar},
        {ids::argon,            2114.0 * si::bar},
        {ids::helium,           50.96 * si::bar},
        {ids::hydrogen,         274.22 * si::bar},
        {ids::carbon_dioxide,   4000.0 * si::bar},
        {ids::ammonia,          5270.0 * si::bar},
        {ids::formic_acid,      4100.0 * si::bar},
        {ids::hydrogen_cyanide, 3080.0 * si::bar},
        {ids::ethanol,          10600.0 * si::bar},
        {ids::benzene,          3600.0 * si::bar},
        {ids::silver,           9000.0 * si::bar},
    });

}}
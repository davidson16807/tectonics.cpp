#pragma once

// in-house libraries
#include <units/si.hpp>
#include <models/compound/table/PartialTable.hpp>
#include <models/compound/ids.hpp>

namespace compound { 
namespace published { 

    table::PartialTable<si::pressure<double>> simon_glatzel_scale({
        {compounds::water,            620.0 * si::bar},
        {compounds::nitrogen,         1606.56 * si::bar},
        {compounds::oxygen,           2732.95 * si::bar},
        {compounds::argon,            2114.0 * si::bar},
        {compounds::helium,           50.96 * si::bar},
        {compounds::hydrogen,         274.22 * si::bar},
        {compounds::carbon_dioxide,   4000.0 * si::bar},
        {compounds::ammonia,          5270.0 * si::bar},
        {compounds::formic_acid,      4100.0 * si::bar},
        {compounds::hydrogen_cyanide, 3080.0 * si::bar},
        {compounds::ethanol,          10600.0 * si::bar},
        {compounds::benzene,          3600.0 * si::bar},
        {compounds::silver,           9000.0 * si::bar},
    });

}}
#pragma once

// C libraries
#include <cmath>

// std libraries
#include <array>

// in-house libraries
#include "Stratum.hpp"
#include "StratumMassPool_operators.hpp"

namespace strata
{
    
    // OPERATORS, regular functions of the form: Stratum x Stratum -> Stratum
    static void scale(const Stratum& a, float scalar, Stratum& output)
    {
        output = a;
        for (std::size_t i=0; i<stratum_mass_pool_count; i++)
        {
            output.mass_pools[i].mass = a.mass_pools[i].mass * scalar;
        }
    }

    static void combine(
        const Stratum& a, 
        const Stratum& b, 
        Stratum& output
    ) {
        for (std::size_t i=0; i<stratum_mass_pool_count; i++)
        {
            combine(a.mass_pools[i], b.mass_pools[i], output.mass_pools[i]);
        }

        output.max_temperature_received = std::max(a.max_temperature_received, b.max_temperature_received);
        output.max_pressure_received = std::max(a.max_pressure_received, b.max_pressure_received);
        output.age_of_world_when_deposited = std::max(a.age_of_world_when_deposited, b.age_of_world_when_deposited);
    }

}

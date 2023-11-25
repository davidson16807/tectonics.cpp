#pragma once

// C libraries
#include <cmath>

// std libraries
#include <array>

// in-house libraries
#include <model/mineral/Mineral_operators.hpp>
#include "Stratum.hpp"

namespace stratum
{
    
    // OPERATORS, regular functions of the form: Stratum x Stratum -> Stratum
    template<std::size_t M>
    static void scale(const Stratum<M>& a, float scalar, Stratum<M>& output)
    {
        output = a;
        for (std::size_t i=0; i<M; i++)
        {
            output.minerals[i].mass = a.minerals[i].mass * scalar;
        }
    }

    template<std::size_t M>
    static void combine(
        const Stratum<M>& a, 
        const Stratum<M>& b, 
        Stratum<M>& output
    ) {
        for (std::size_t i=0; i<M; i++)
        {
            combine(a.minerals[i], b.minerals[i], output.minerals[i]);
        }

        output.max_temperature_received = std::max(a.max_temperature_received, b.max_temperature_received);
        output.max_pressure_received = std::max(a.max_pressure_received, b.max_pressure_received);
        output.age_of_world_when_deposited = std::max(a.age_of_world_when_deposited, b.age_of_world_when_deposited);
    }

}

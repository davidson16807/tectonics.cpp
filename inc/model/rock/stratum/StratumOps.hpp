#pragma once

// C libraries
#include <cmath>

// std libraries
#include <array>

// in-house libraries
#include <model/rock/mineral/Mineral.hpp>
#include <model/rock/mineral/MineralOps.hpp>

#include "Stratum.hpp"

namespace rock
{

    template<std::size_t M>
    class StratumOps
    {
        const MineralOps minerals;

    public:
        StratumOps(const MineralOps& minerals):
            minerals(minerals)
        {}

        // OPERATORS, regular functions of the form: Stratum x Stratum -> Stratum
        void scale(const Stratum<M>& a, float scalar, Stratum<M>& output) const
        {
            output = a;
            for (std::size_t i=0; i<M; i++)
            {
                output.minerals[i].mass = a.minerals[i].mass * scalar;
            }
            output.age_of_world_when_first_deposited = a.age_of_world_when_first_deposited;
            output.age_of_world_when_last_deposited = a.age_of_world_when_last_deposited;
        }

        void combine(
            const Stratum<M>& a, 
            const Stratum<M>& b, 
            Stratum<M>& output
        ) const {
            for (std::size_t i=0; i<M; i++)
            {
                minerals.combine(a.minerals[i], b.minerals[i], output.minerals[i]);
            }

            output.age_of_world_when_first_deposited = std::min(a.age_of_world_when_first_deposited, b.age_of_world_when_first_deposited);
            output.age_of_world_when_last_deposited = std::max(a.age_of_world_when_last_deposited, b.age_of_world_when_last_deposited);
        }

    };

}


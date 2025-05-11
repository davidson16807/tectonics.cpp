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

    // NOTE: `M` is mineral count
    template<std::size_t M>
    class StratumOps
    {
        using time = si::time<double>;

        static constexpr double oo = std::numeric_limits<double>::max();

        const MineralOps minerals;

    public:
        StratumOps():
            minerals()
        {}

        Stratum<M> scale(const Stratum<M>& a, float scalar) const
        {
            Stratum<M> output;
            for (std::size_t i=0; i<M; i++)
            {
                output.minerals[i].mass = a.minerals[i].mass * scalar;
            }
            output.age_of_world_when_first_deposited = a.age_of_world_when_first_deposited;
            output.age_of_world_when_last_deposited = a.age_of_world_when_last_deposited;
            return output;
        }

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

        void combine(
            const StratumStore<M>& a, 
            const StratumStore<M>& b, 
            StratumStore<M>& output
        ) const {
            Mineral unpacked1, unpacked2, combined;
            for (std::size_t i=0; i<M; i++)
            {
                a[i].unpack(unpacked1);
                b[i].unpack(unpacked2);
                minerals.combine(unpacked1, unpacked2, combined);
                output[i].pack(combined);
            }
            auto a_start = a.age_of_world_when_first_deposited();
            auto b_start = b.age_of_world_when_first_deposited();
            auto a_stop = a.age_of_world_when_last_deposited();
            auto b_stop = b.age_of_world_when_last_deposited();
            output.age_of_world_when_first_deposited(std::min(a_start, b_start));
            output.age_of_world_when_last_deposited (std::max(a_stop, b_stop));
        }

    };

}


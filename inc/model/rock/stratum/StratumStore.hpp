#pragma once

// C libraries
#include <cmath>
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/mineral/MineralStore.hpp>
#include "Stratum.hpp"

namespace rock
{
    /*
    `StratumStore` is a memory efficient variant of the 
    `Stratum` data structure. Isn't it adorable!?
    It would take ridiculous amounts of memory to store a `Stratum` 
    for every stratum within a raster, so we store each stratum in a raster
    as a `StratumStore`, then convert back to `Stratum` when
    we want to perform some operation on it. 

    The interpretation of attributes within `StratumStore` is error prone,
    so to prevent users from doing so we encapsulate the class.
    */
    // NOTE: `M` is mineral count
    template <int M>
    class StratumStore
    {
        using uint16 = std::uint16_t;

        std::array<rock::MineralStore, M> minerals;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        uint16 age_of_world_when_first_deposited_in_megayears;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        // Also used for cache alignment
        uint16 age_of_world_when_last_deposited_in_megayears;

        static constexpr uint16 oo = std::numeric_limits<uint16>::max()-1;

    public:

        ~StratumStore()
        {
        }
        StratumStore(const Stratum<M>& output)
        {
            pack(output);
        }
        StratumStore():
            age_of_world_when_first_deposited_in_megayears(0.0f),
            age_of_world_when_last_deposited_in_megayears(0.0f)
        {
            minerals.fill(rock::MineralStore());
        }

        void unpack(Stratum<M>& output) const
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].unpack(output.minerals[i]);
            }
            output.age_of_world_when_first_deposited = age_of_world_when_first_deposited_in_megayears * si::megayear;
            output.age_of_world_when_last_deposited = age_of_world_when_last_deposited_in_megayears * si::megayear;
        }

        void pack(const Stratum<M>& input)
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].pack(input.minerals[i]);
            }
            age_of_world_when_first_deposited_in_megayears = input.age_of_world_when_first_deposited / si::megayear;
            age_of_world_when_last_deposited_in_megayears = input.age_of_world_when_last_deposited / si::megayear;
        }

        inline auto age_of_world_when_first_deposited () const 
        {
            return age_of_world_when_first_deposited_in_megayears * si::megayear;
        }

        inline auto age_of_world_when_last_deposited () const 
        {
            return age_of_world_when_last_deposited_in_megayears * si::megayear;
        }

        inline auto age_of_world_when_first_deposited (const si::time<double> value) 
        {
            age_of_world_when_first_deposited_in_megayears = std::max(0.0, value / si::megayear);
        }

        inline auto age_of_world_when_last_deposited (const si::time<double> value) 
        {
            age_of_world_when_last_deposited_in_megayears = std::min(double(oo), value / si::megayear);
        }

        si::mass<float> mass () const 
        {
            si::mass<float> result;
            for (int i = 0; i < M; ++i)
            {
                result += minerals[i].mass();
            }
            return result;
        }

        bool empty () const 
        {
            for (int i = 0; i < M; ++i)
            {
                if (minerals[i].mass() > 0.0*si::kilogram) { return false; }
            }
            return true;
        }

        inline auto size() const
        {
            return minerals.size();
        }

        inline auto operator[] (const int i) const
        {
            return minerals[i];
        }

        inline auto& operator[] (const int i)
        {
            return minerals[i];
        }
    };
}

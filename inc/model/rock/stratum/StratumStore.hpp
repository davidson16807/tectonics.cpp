#pragma once

// C libraries
#include <cmath>
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>
#include <limits>

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
    we want to perform some heavy operation on it. 
    There are also attributes available if you all you want to convert is a single attribute.
    attributes are exposed via getters/setters in a proper OOP way
    since we want to leave open the ability to experiment with different memory layouts
    without changing downstream code or risk introducing bugs 
    when compressing/decompressing memory efficient data types.

    The interpretation of attributes within `StratumStore` is error prone,
    so to prevent users from doing so we encapsulate the class.
    */
    // NOTE: `M` is mineral count
    template <std::size_t M>
    class StratumStore
    {
        std::array<rock::MineralStore, M> minerals;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        si::time<float> age_of_world_when_first_deposited_;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        // Also used for cache alignment
        si::time<float> age_of_world_when_last_deposited_;

        static constexpr float oo = std::numeric_limits<float>::max();

    public:

        constexpr StratumStore(const Stratum<M>& output)
        {
            pack(output);
        }
        StratumStore():
            age_of_world_when_first_deposited_(oo),
            age_of_world_when_last_deposited_(0.0f)
        {
            minerals.fill(rock::MineralStore());
        }

        void unpack(Stratum<M>& output) const
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].unpack(output.minerals[i]);
            }
            output.age_of_world_when_first_deposited = age_of_world_when_first_deposited_;
            output.age_of_world_when_last_deposited = age_of_world_when_last_deposited_;
        }

        void pack(const Stratum<M>& input)
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].pack(input.minerals[i]);
            }
            age_of_world_when_first_deposited_ = input.age_of_world_when_first_deposited;
            age_of_world_when_last_deposited_ = input.age_of_world_when_last_deposited;
        }

        inline constexpr auto age_of_world_when_first_deposited () const 
        {
            return age_of_world_when_first_deposited_;
        }

        inline constexpr auto age_of_world_when_last_deposited () const 
        {
            return age_of_world_when_last_deposited_;
        }

        inline constexpr auto age_of_world_when_first_deposited (const si::time<double> value) 
        {
            age_of_world_when_first_deposited_ = value;
        }

        inline constexpr auto age_of_world_when_last_deposited (const si::time<double> value) 
        {
            age_of_world_when_last_deposited_ = value;
        }

        inline si::mass<float> mass () const 
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

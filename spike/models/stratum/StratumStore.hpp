#pragma once

// C libraries
#include <cmath>
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include <physics/units.hpp>

#include <models/mineral/MineralStore.hpp>
#include "Stratum.hpp"

namespace stratum
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

    The interpretation of attributes also comes with some performance penalty,
    so to encourage users not to spam calls to getters, 
    we only expose methods to convert to and from `StratumStore`.

    This also grants a certain mathematical purity to the object,
    since the object at high level can be treated strictly by its mappings to other states, 
    as in the spirit of category theory, and in this case they are isomorphic and invertible.
    */
    template <int M>
    class StratumStore
    {
        std::array<mineral::MineralStore, M> minerals;
        // Represent temperatures from 1 to 32768 Kelvin with a precision of 0.02%.
        // This value was chosen to reflect the temperature of Jupiter's core. 
        std::uint16_t stored_max_temperature_received;
        // Represent pressures from 1 to 6e12 Pascals with a precision of 0.02%.
        // This value was chosen to reflect the pressure of Jupiter's core. 
        std::uint16_t stored_max_pressure_received;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        std::uint16_t age_of_world_when_deposited_in_megayears;
        // unused variable, used to simplify cache alignment, 
        // though also allows for future expansion
        std::uint16_t unused;

    private:
        constexpr static float log2_ref_temperature = 15.;
        constexpr static float log2_ref_pressure    = 42.;

        /*
        NOTE: I would prefer these methods to be static variables instead, 
        but exp2 is not constexpr and I can't set a variable as static unless it's constexpr,
        so I settle for using static methods instead.
        */
        static float temperature_max ()
        {
            return exp2(std::numeric_limits<std::uint16_t>::max()/log2_ref_temperature);
        } 
        static float pressure_max    ()
        {
            return exp2(std::numeric_limits<std::uint16_t>::max()/log2_ref_pressure);
        } 

        static float temperature_min ()
        {
            return exp2(0./log2_ref_temperature);
        } 
        static float pressure_min    ()
        {
            return exp2(0./log2_ref_pressure);
        } 


    public:

        void unpack(Stratum<M>& output) const
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].unpack(output.minerals[i]);
            }
            output.max_pressure_received    = exp2( log2_ref_pressure    * float(stored_max_pressure_received)    / float(std::numeric_limits<std::uint16_t>::max()));
            output.max_temperature_received = exp2( log2_ref_temperature * float(stored_max_temperature_received) / float(std::numeric_limits<std::uint16_t>::max()));
            output.age_of_world_when_deposited = age_of_world_when_deposited_in_megayears * units::megayear;
        }

        void pack(const Stratum<M>& input)
        {
            for (std::size_t i=0; i<M; i++)
            {
                minerals[i].pack(input.minerals[i]);
            }
            stored_max_pressure_received    = std::uint16_t(
                std::numeric_limits<std::uint16_t>::max()*std::clamp( log2(input.max_pressure_received)    / log2_ref_pressure,   0., 1.0));
            stored_max_temperature_received = std::uint16_t(
                std::numeric_limits<std::uint16_t>::max()*std::clamp( log2(input.max_temperature_received) / log2_ref_temperature,0., 1.0));
            age_of_world_when_deposited_in_megayears = input.age_of_world_when_deposited / units::megayear;
        }
    };
}

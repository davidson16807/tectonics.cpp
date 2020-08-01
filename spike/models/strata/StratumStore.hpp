#pragma once

// C libraries
#include <cmath>
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include "StratumMassPoolStore.hpp"
#include "Stratum.hpp"

namespace strata
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
    class StratumStore
    {
        std::array<StratumMassPoolStore, Stratum::mass_pool_count> mass_pools;
        // Represent pressures from 1 to 6e12 Pascals with a precision of 0.02%.
        // This value was chosen to reflect the pressure of Jupiter's core. 
        std::uint16_t stored_max_temperature_received;
        // Represent temperatures from 1 to 32768 Kelvin with a precision of 0.02%.
        // This value was chosen to reflect the temperature of Jupiter's core. 
        std::uint16_t stored_max_pressure_received;
        // Represent age of world from 1 to 65.535 billion years to the nearest megayear
        std::uint16_t age_of_world_when_deposited_in_megayears;
        // unused variable, used to simplify cache alignment, 
        // though also allows for future expansion
        std::uint16_t unused;

    private:
        constexpr static float log2_ref_temperature = 42.;
        constexpr static float log2_ref_pressure    = 15.;

        constexpr static float temperature_max = exp2(std::numeric_limits<std::uint16_t>::max()/log2_ref_temperature);
        constexpr static float pressure_max    = exp2(std::numeric_limits<std::uint16_t>::max()/log2_ref_pressure);

        constexpr static float temperature_min = exp2(0./log2_ref_temperature);
        constexpr static float pressure_min    = exp2(0./log2_ref_pressure);

    public:

        void decompress(Stratum& output) const
        {
            for (std::size_t i=0; i<Stratum::mass_pool_count; i++)
            {
                mass_pools[i].decompress(output.mass_pools[i]);
            }
            output.max_pressure_received    = exp2( log2_ref_pressure    * float(stored_max_pressure_received)    / std::numeric_limits<std::uint16_t>::max());
            output.max_temperature_received = exp2( log2_ref_temperature * float(stored_max_temperature_received) / std::numeric_limits<std::uint16_t>::max());
            output.age_of_world_when_deposited = age_of_world_when_deposited_in_megayears * units::megayear
        }
        void compress(const Stratum& input)
        {
            for (std::size_t i=0; i<Stratum::mass_pool_count; i++)
            {
                mass_pools[i].compress(input.mass_pools[i]);
            }

            stored_max_pressure_received    = std::uint8_t(std::numeric_limits<std::uint16_t>::max()*std::clamp( log2(input.max_pressure_received)    / log2_ref_pressure,   0., 1.0));
            stored_max_temperature_received = std::uint8_t(std::numeric_limits<std::uint16_t>::max()*std::clamp( log2(input.max_temperature_received) / log2_ref_temperature,0., 1.0));
            age_of_world_when_deposited_in_megayears = age_of_world_when_deposited / units::megayear;
        }
    };
}

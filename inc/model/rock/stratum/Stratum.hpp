#pragma once

// C libraries
#include <cmath>
#include <cstdint>
#include <cstddef>

// std libraries
#include <limits>
#include <algorithm>
#include <array>

// in-house libraries
#include <unit/si.hpp>
#include <model/rock/mineral/Mineral.hpp>

namespace rock
{
    /*
    "Stratum" describes the composition and texture of a single rock layer
    */
    // NOTE: `M` is mineral count
    template<std::size_t M>
    struct Stratum
    {
        /*
        "particle_size_bin_relative_volume" describes a set of bins.
        Each bin tracks the relative volume occupied by particles that fall within a certain range of diameters.
        Each bin corresponds to 3 gradations of the the Krumbein phi classification to a good approximation, 
        and describes anything from 1m boulders to colloids.
        Particle size is distinct from a mass pool's grain size.
        Grain size is primarily used to indicate distinctions between extrusive and intrusive rocks, like basalt and gabbro.
        Particle size is primarily used to indicate distinctions between things like boulders vs. pebbles vs sand vs. clay
        */
        std::array<rock::Mineral, M>  minerals;
        si::time<double> age_of_world_when_first_deposited;
        si::time<double> age_of_world_when_last_deposited;

        // empty stratum
        Stratum():
            age_of_world_when_first_deposited(std::numeric_limits<double>::max()*si::megayear),
            age_of_world_when_last_deposited(0*si::megayear)
        {
            minerals.fill(rock::Mineral());
        }

        // empty stratum
        Stratum(
            const si::time<double> age_of_world_when_first_deposited,
            const si::time<double> age_of_world_when_last_deposited
        ):
            age_of_world_when_first_deposited(age_of_world_when_first_deposited),
            age_of_world_when_last_deposited(age_of_world_when_last_deposited)
        {
            minerals.fill(rock::Mineral());
        }

        Stratum(
            const si::time<double> age_of_world_when_first_deposited,
            const si::time<double> age_of_world_when_last_deposited,
            const std::array<rock::Mineral,M>& minerals
        ): 
            minerals(minerals),
            age_of_world_when_first_deposited(age_of_world_when_first_deposited),
            age_of_world_when_last_deposited(age_of_world_when_last_deposited)
        {
        }

        inline si::mass<double> mass () const 
        {
            si::mass<double> result;
            for (std::size_t i = 0; i < M; ++i)
            {
                result += minerals[i].mass;
            }
            return result;
        }

    };

}

#pragma once

// std libraries
#include <array>            // array

// in-house libraries
#include "DensityTower.hpp" // Crust

namespace crust
{
    struct LiquidMixture
    {
        unsigned int lightest_liquid_id;
        unsigned int densest_liquid_id;
        LiquidMixture() :
            lightest_liquid_id(0),
            densest_liquid_id(0)
        {

        }
        LiquidMixture(
            unsigned int lightest_liquid_id, 
            unsigned int densest_liquid_id
        ) :
            lightest_liquid_id(lightest_liquid_id),
            densest_liquid_id(densest_liquid_id)
        {

        }

        template <std::size_t M>
        float density(
            const std::array<float, M>& liquid_mass_pools, 
            const std::array<float, M>& liquid_densities) const 
        {
            // sum the mass within the intermixed liquid
            float mixture_mass(0.0f);
            float mixture_volume(0.0f);
            for (std::size_t j = lightest_liquid_id; j < densest_liquid_id && j < M; ++j)
            {
                mixture_mass += liquid_mass_pools[j];
                mixture_volume += liquid_mass_pools[j] / liquid_densities[j];
            }
            return mixture_mass / mixture_volume;
        }
    };
}

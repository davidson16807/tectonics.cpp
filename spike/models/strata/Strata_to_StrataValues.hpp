#pragma once

// in-house libraries
#include "Strata.hpp"

namespace strata
{
    template<int L, int M>
    void get_volumes(
        const Strata<L,M>& strata,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        StrataValues<L>& output
    ) {
        for (std::size_t i=0; i<L; i++)
        {
            output[i] = strata.content[i].volume(age_of_world, mass_pool_densities);
        }
    }
    template<int L, int M>
    void get_densities(
        const Strata<L,M>& strata,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        StrataValues<L>& output
    ) {
        for (std::size_t i=0; i<L; i++)
        {
            output[i] = strata.content[i].density(age_of_world, mass_pool_densities);
        }
    }
    template<int L, int M>
    void get_thermal_conductivities(
        const Strata<L,M>& strata,
        const float age_of_world,
        const std::array<float, M>& mass_pool_densities,
        const std::array<float, M>& mass_pool_thermal_conductivities,
        StrataValues<L>& output
    ) {
        for (std::size_t i=0; i<L; i++)
        {
            output[i] = strata.content[i].thermal_conductivity(
                age_of_world, 
                mass_pool_densities, 
                mass_pool_thermal_conductivities
            );
        }
    }
}
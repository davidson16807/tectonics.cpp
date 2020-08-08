#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "Strata.hpp"
#include "StrataValues.hpp"

namespace strata
{
    template<int L, int M>
    void get_max_pressures_received_by_strata(
        const Strata<L,M>& strata,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].max_pressure_received;
        }
    }
    template<int L, int M>
    void get_max_temperatures_received_by_strata(
        const Strata<L,M>& strata,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].max_temperature_received;
        }
    }
    template<int L, int M>
    void get_volumes_of_strata(
        const Strata<L,M>& strata,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].volume(age_of_world, mass_pool_densities);
        }
    }
    template<int L, int M>
    void get_densities_of_strata(
        const Strata<L,M>& strata,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].density(age_of_world, mass_pool_densities);
        }
    }
    template<int L, int M>
    void get_thermal_conductivities_of_strata(
        const Strata<L,M>& strata,
        const float age_of_world,
        const std::array<float, M>& mass_pool_densities,
        const std::array<float, M>& mass_pool_thermal_conductivities,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].thermal_conductivity(
                age_of_world, 
                mass_pool_densities, 
                mass_pool_thermal_conductivities
            );
        }
    }
}
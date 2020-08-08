#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "Strata.hpp"
#include "StrataValues.hpp"

namespace strata
{
    template<int L, int M>
    void get_max_pressures_received(
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
    void get_max_temperatures_received(
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
    void get_mass(
        const Strata<L,M>& strata,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        for (std::size_t i=0; i<L; i++)
        {
            output.values[i] = strata.content[i].mass();
        }
    }
    template<int L, int M>
    void get_volumes(
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
    void get_densities(
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
    void get_thermal_conductivities(
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

    /*
    `get_overburden_mass()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating pressure.
    Total pressure requires providing too many other variables (such as ocean depth, gravity, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    template<int L, int M>
    void get_overburden_mass(
        const Strata<L,M>& strata,
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        float overburden_mass(0);
        for (int i = 0; i < strata.count; ++i)
        {
            output.values[i] = overburden_mass;
            overburden_mass += strata.content[i].mass();
        }
    }
    /*
    `get_overburden_volume()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating depth.
    Total depth requires providing too many other variables (such as ocean depth, surface area, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    template<int L, int M>
    void get_overburden_volume(
        const Strata<L,M>& strata,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities, 
        StrataValues<float, L>& output
    ) {
        output.count = strata.count;
        float overburden_volume(0);
        for (int i = 0; i < strata.count; ++i)
        {
            output.values[i] = overburden_volume;
            overburden_volume += strata.content[i].volume(age_of_world, mass_pool_densities);
        }
    }
}
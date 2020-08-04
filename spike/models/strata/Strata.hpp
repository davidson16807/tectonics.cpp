#pragma once

#include <array>

// in-house libraries
#include "Stratum.hpp"

namespace strata
{
    const int strata_max_stratum_count = 16;
    struct Strata
    {
        std::array<Stratum, strata_max_stratum_count> content;
        int count;

        ~Strata(){}
        Strata(): content(), count(0){}



        // DERIVED ATTRIBUTES, regular functions of the form: Strata -> T
        float mass_pool(const OxygenPlanetStratumMassPoolTypes type) const {
            float total_mass(0.0);
            for (std::size_t i=0; i<strata_max_stratum_count; i++)
            {
                total_mass += content[i].mass_pools[int(type)].mass;
            }
            return total_mass;
        }
        float mass() const {
            float total_mass(0.0);
            for (std::size_t i=0; i<strata_max_stratum_count; i++)
            {
                total_mass += content[i].mass();
            }
            return total_mass;
        }
        float volume(const std::array<float, stratum_mass_pool_count>& mass_pool_densities) const {
            float total_volume(0.0);
            for (std::size_t i=0; i<strata_max_stratum_count; i++)
            {
                total_volume += content[i].volume(mass_pool_densities);
            }
            return total_volume;
        }
        float density(const std::array<float, stratum_mass_pool_count>& mass_pool_densities) const {
            return mass() / volume(mass_pool_densities);
        }

    };
}

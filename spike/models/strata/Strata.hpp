#pragma once

#include <array>

// in-house libraries
#include "Stratum.hpp"

namespace strata
{

    class Strata
    {
        std::array<Stratum, max_stratum_count> strata;
        int count;

    public:
        ~Strata(){}
        Strata(): strata(), count(0){}

        constexpr static int max_stratum_count = 16;


        // DERIVED ATTRIBUTES, regular functions of the form: Strata -> T
        float mass_pool(const OxygenPlanetStratumMassPoolTypes type) const {
            float total_mass(0.0);
            for (std::size_t i=0; i<max_stratum_count; i++)
            {
                total_mass += strata[i].mass_pools[type];
            }
            return total_mass;
        }
        float mass() const {
            float total_mass(0.0);
            for (std::size_t i=0; i<max_stratum_count; i++)
            {
                total_mass += strata[i].mass();
            }
            return total_mass;
        }
        float volume(const std::array<float, Strata::mass_pool_count>& mass_pool_densities) const {
            float total_volume(0.0);
            for (std::size_t i=0; i<max_stratum_count; i++)
            {
                total_volume += strata[i].volume();
            }
            return total_volume;
        }
        float density(const std::array<float, Strata::mass_pool_count>& mass_pool_densities) const {
            return mass() / volume(mass_pool_densities);
        }

    };
}

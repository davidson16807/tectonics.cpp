#pragma once

// std libraries
#include <array>

// in-house libraries
#include "Stratum.hpp"
#include "StratumMassPool_operators.hpp"

namespace strata
{
    
    // OPERATORS, regular functions of the form: Stratum x Stratum -> Stratum
    static void scale(const Stratum& a, float scalar, Stratum& out)
    {
        for (std::size_t i=0; i<Stratum::mass_pool_count; i++)
        {
            out.mass_pools[i] = a.mass_pools[i] * scalar;
        }
    }
    
    static void add(const Stratum& a, const Stratum& b, Stratum& out)
    {
        for (std::size_t i=0; i<Stratum::mass_pool_count; i++)
        {
            out.mass_pools[i] = a.mass_pools[i] + b.mass_pools[i];
        }
    }

    static void combine(const Stratum& a, const Stratum& b, const std::array<float, Stratum::mass_pool_count>& mass_pool_densities, Stratum& out)
    {
        for (std::size_t i=0; i<Stratum::mass_pool_count; i++)
        {
            StratumMassPool::combine(a.mass_pools[i], b.mass_pools[i], out.mass_pools[i]);
        }
        
        float a_volume = a.volume(mass_pool_densities);
        float b_volume = b.volume(mass_pool_densities);
        float a_sum = a.particle_size_bin_relative_volume_sum();
        float b_sum = b.particle_size_bin_relative_volume_sum();
        for (std::size_t i=0; i<particle_size_bin_count; i++)
        {
            out.particle_size_bin_relative_volume[i] = 
                (a_volume * a.particle_size_bin_relative_volume[i] / a_sum + 
                 b_volume * b.particle_size_bin_relative_volume[i] / b_sum) / (a_volume + b_volume);
        }
    }

}

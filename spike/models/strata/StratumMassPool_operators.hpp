#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "StratumMassPool.hpp"

namespace strata
{
	static void combine(const StratumMassPool& a, const StratumMassPool& b, StratumMassPool& output)
	{
		output.mass = a.mass + b.mass;

        float a_sum = std::accumulate(a.grain_type_bin_fractional_volume.begin(), a.grain_type_bin_fractional_volume.end(), 0);
        float b_sum = std::accumulate(b.grain_type_bin_fractional_volume.begin(), b.grain_type_bin_fractional_volume.end(), 0);
        for (std::size_t i=0; i<particle_size_bin_count; i++)
        {
            output.grain_type_bin_fractional_volume[i] = 
                (a.mass * a.grain_type_bin_fractional_volume[i] / a_sum + 
                 b.mass * b.grain_type_bin_fractional_volume[i] / b_sum) / (a.mass + b.mass);
        }
	}
}

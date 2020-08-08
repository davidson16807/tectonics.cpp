#pragma once

// C libraries
#include <cmath>

// std libraries
#include <numeric>

// in-house libraries
#include "MassPool.hpp"

namespace stratum
{
	static void combine(const MassPool& a, const MassPool& b, MassPool& output)
	{
		output.mass = a.mass + b.mass;

        // clamp lower bound to 1e-4 to prevent division by zero
        float a_sum = std::accumulate(a.grain_type_relative_volume.begin(), a.grain_type_relative_volume.end(), 0.0f);
        float b_sum = std::accumulate(b.grain_type_relative_volume.begin(), b.grain_type_relative_volume.end(), 0.0f);
        for (std::size_t i=0; i<int(GrainType::count); i++)
        {
            output.grain_type_relative_volume[i] = 
                (a.mass * a.grain_type_relative_volume[i] / std::max(a_sum, 1e-4f) + 
                 b.mass * b.grain_type_relative_volume[i] / std::max(b_sum, 1e-4f)) / std::max(a.mass + b.mass, 1e-4f);
        }
	}
}

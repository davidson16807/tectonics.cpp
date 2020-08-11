#pragma once

// C libraries
#include <cmath>

// std libraries
#include <numeric>

// in-house libraries
#include "Mineral.hpp"

namespace mineral
{
	static void combine(const Mineral& a, const Mineral& b, Mineral& output)
	{
		output.mass = a.mass + b.mass;

        // clamp lower bound to 1e-4 to prevent division by zero
        float a_sum = a.grain_type_total_relative_volume();
        float b_sum = b.grain_type_total_relative_volume();
        for (std::size_t i=0; i<int(GrainType::count); i++)
        {
            output.grain_type_relative_volume[i] = 
                (a.mass * a.grain_type_relative_volume[i] / a_sum + 
                 b.mass * b.grain_type_relative_volume[i] / b_sum) / std::max(a.mass + b.mass, 1e-4f);
        }
	}
}

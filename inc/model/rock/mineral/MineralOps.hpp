#pragma once

// C libraries
#include <cmath>
#include <cstdint>
#include <cstddef>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include <unit/si.hpp>
#include "GrainType.hpp"
#include "Mineral.hpp"

namespace rock
{

	struct MineralOps
	{
		MineralOps(){}
		void combine(const Mineral& a, const Mineral& b, Mineral& output) const
		{
			output.mass = a.mass + b.mass;
	        output.phase_id = a.mass > b.mass? a.phase_id : b.phase_id;

	        // clamp lower bound to 1e-4 to prevent division by zero
	        float a_sum = a.grain_type_total_relative_volume();
	        float b_sum = b.grain_type_total_relative_volume();
	        for (std::size_t i=0; i<int(GrainType::count); i++)
	        {
	            output.grain_type_relative_volume[i] = 
	                (a.mass * a.grain_type_relative_volume[i] / a_sum + 
	                 b.mass * b.grain_type_relative_volume[i] / b_sum) / 
	                std::max(a.mass + b.mass, 1e-4f*si::kilogram);
	        }
		}

	};

}

#pragma once

// C libraries
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include <unit/si.hpp>
#include "Mineral.hpp"

namespace rock
{

	/*
	*/
	class MineralStore
	{
		static constexpr float epsilon =1e-4f;

		si::mass<float> mass_; 
		std::uint8_t unweathered_amorphous_part_count  : 4;
		std::uint8_t unweathered_extrusive_part_count  : 4;
		std::uint8_t unweathered_intrusive_part_count  : 4;
		std::uint8_t unweathered_crystalline_part_count: 4;
		std::uint8_t weathered_amorphous_part_count    : 4;
		std::uint8_t weathered_extrusive_part_count    : 4;
		std::uint8_t weathered_intrusive_part_count    : 4;
		std::uint8_t weathered_crystalline_part_count  : 4;

	public:
		// convenience constructor, equivalent to pack()
		explicit MineralStore(Mineral& output)
		{
			pack(output);
		}
		// identity constructor
		constexpr MineralStore():
			mass_(0),
			unweathered_amorphous_part_count(1),           
			unweathered_extrusive_part_count(1),           
			unweathered_intrusive_part_count(1),           
			unweathered_crystalline_part_count(1),           
			weathered_amorphous_part_count(1),  
			weathered_extrusive_part_count(1),  
			weathered_intrusive_part_count(1),  
			weathered_crystalline_part_count(1)
		{
		}

		constexpr inline si::mass<float> mass() const
		{
			return mass_;
		}

		void unpack(Mineral& output) const
		{
		    output.mass = si::mass<double>(mass_);
		    float total_relative_volume(epsilon);
			total_relative_volume += unweathered_amorphous_part_count;
			total_relative_volume += unweathered_extrusive_part_count;
			total_relative_volume += unweathered_intrusive_part_count;
			total_relative_volume += unweathered_crystalline_part_count;
			total_relative_volume += weathered_amorphous_part_count;
			total_relative_volume += weathered_extrusive_part_count;
			total_relative_volume += weathered_intrusive_part_count;
			total_relative_volume += weathered_crystalline_part_count;
			const float scaling_factor(1.0f / total_relative_volume);
            output.grain_type_relative_volume[0] = unweathered_amorphous_part_count   * scaling_factor;
            output.grain_type_relative_volume[1] = unweathered_extrusive_part_count   * scaling_factor;
			output.grain_type_relative_volume[2] = unweathered_intrusive_part_count   * scaling_factor;
			output.grain_type_relative_volume[3] = unweathered_crystalline_part_count * scaling_factor;
			output.grain_type_relative_volume[4] = weathered_amorphous_part_count     * scaling_factor;
			output.grain_type_relative_volume[5] = weathered_extrusive_part_count     * scaling_factor;
			output.grain_type_relative_volume[6] = weathered_intrusive_part_count     * scaling_factor;
			output.grain_type_relative_volume[7] = weathered_crystalline_part_count   * scaling_factor;
		}
		void pack(const Mineral& input)
		{
			mass_ = si::mass<float>(input.mass);
            // rescale bin counts by the new max to fit inside a uint8_t
            float grain_type_relative_volume_max(epsilon);
            for (int i = 0; i < int(GrainType::count); ++i)
            {
	            grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[i]);
            }
			const float max_part_count(15.0f);
			float scaling_factor(max_part_count / grain_type_relative_volume_max);
            unweathered_amorphous_part_count   = std::clamp(std::round(input.grain_type_relative_volume[0] * scaling_factor), 0.0f, max_part_count);
            unweathered_extrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[1] * scaling_factor), 0.0f, max_part_count);
            unweathered_intrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[2] * scaling_factor), 0.0f, max_part_count);
            unweathered_crystalline_part_count = std::clamp(std::round(input.grain_type_relative_volume[3] * scaling_factor), 0.0f, max_part_count);
            weathered_amorphous_part_count     = std::clamp(std::round(input.grain_type_relative_volume[4] * scaling_factor), 0.0f, max_part_count);
            weathered_extrusive_part_count     = std::clamp(std::round(input.grain_type_relative_volume[5] * scaling_factor), 0.0f, max_part_count);
            weathered_intrusive_part_count     = std::clamp(std::round(input.grain_type_relative_volume[6] * scaling_factor), 0.0f, max_part_count);
            weathered_crystalline_part_count   = std::clamp(std::round(input.grain_type_relative_volume[7] * scaling_factor), 0.0f, max_part_count);
  		}
	};
}

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

		float mass; 
		std::uint8_t nonmetamorphic_unweathered_extrusive_part_count : 4;
		std::uint8_t nonmetamorphic_unweathered_intrusive_part_count : 4;
		std::uint8_t nonmetamorphic_weathered_extrusive_part_count   : 4;
		std::uint8_t nonmetamorphic_weathered_intrusive_part_count   : 4;
		std::uint8_t metamorphic_unweathered_extrusive_part_count    : 4;
		std::uint8_t metamorphic_unweathered_intrusive_part_count    : 4;
		std::uint8_t metamorphic_weathered_extrusive_part_count      : 4;
		std::uint8_t metamorphic_weathered_intrusive_part_count      : 4;

	public:
		~MineralStore()
		{
		}
		// convenience constructor, equivalent to pack()
		MineralStore(Mineral& output)
		{
			pack(output);
		}
		// identity constructor
		MineralStore():
			mass(0),
			nonmetamorphic_unweathered_extrusive_part_count(1),           
			nonmetamorphic_unweathered_intrusive_part_count(1),           
			nonmetamorphic_weathered_extrusive_part_count(1),  
			nonmetamorphic_weathered_intrusive_part_count(1),  
			metamorphic_unweathered_extrusive_part_count(1),           
			metamorphic_unweathered_intrusive_part_count(1),           
			metamorphic_weathered_extrusive_part_count(1),  
			metamorphic_weathered_intrusive_part_count(1)
		{
		}
		void unpack(Mineral& output) const
		{
		    output.mass = mass * si::kilogram;
		    float total_relative_volume(epsilon);
			total_relative_volume += nonmetamorphic_unweathered_extrusive_part_count;
			total_relative_volume += nonmetamorphic_unweathered_intrusive_part_count;
			total_relative_volume += nonmetamorphic_weathered_extrusive_part_count;
			total_relative_volume += nonmetamorphic_weathered_intrusive_part_count;
			total_relative_volume += metamorphic_unweathered_extrusive_part_count;
			total_relative_volume += metamorphic_unweathered_intrusive_part_count;
			total_relative_volume += metamorphic_weathered_extrusive_part_count;
			total_relative_volume += metamorphic_weathered_intrusive_part_count;
			const float scaling_factor(1.0f / total_relative_volume);
            output.grain_type_relative_volume[0] = nonmetamorphic_unweathered_extrusive_part_count  * scaling_factor;
			output.grain_type_relative_volume[1] = nonmetamorphic_unweathered_intrusive_part_count  * scaling_factor;
			output.grain_type_relative_volume[2] = nonmetamorphic_weathered_extrusive_part_count    * scaling_factor;
			output.grain_type_relative_volume[3] = nonmetamorphic_weathered_intrusive_part_count    * scaling_factor;
            output.grain_type_relative_volume[4] = metamorphic_unweathered_extrusive_part_count     * scaling_factor;
			output.grain_type_relative_volume[5] = metamorphic_unweathered_intrusive_part_count     * scaling_factor;
			output.grain_type_relative_volume[6] = metamorphic_weathered_extrusive_part_count       * scaling_factor;
			output.grain_type_relative_volume[7] = metamorphic_weathered_intrusive_part_count       * scaling_factor;
		}
		void pack(const Mineral& input)
		{
			mass = input.mass / si::kilogram;
            // rescale bin counts by the new max to fit inside a uint8_t
            float grain_type_relative_volume_max(epsilon);
            for (int i = 0; i < int(GrainType::count); ++i)
            {
	            grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[i]);
            }
			const float max_part_count(15.0f);
			float scaling_factor(max_part_count / grain_type_relative_volume_max);
            nonmetamorphic_unweathered_extrusive_part_count = std::clamp(std::round(input.grain_type_relative_volume[0] * scaling_factor), 0.0f, max_part_count);
            nonmetamorphic_unweathered_intrusive_part_count = std::clamp(std::round(input.grain_type_relative_volume[1] * scaling_factor), 0.0f, max_part_count);
            nonmetamorphic_weathered_extrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[2] * scaling_factor), 0.0f, max_part_count);
            nonmetamorphic_weathered_intrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[3] * scaling_factor), 0.0f, max_part_count);
            metamorphic_unweathered_extrusive_part_count    = std::clamp(std::round(input.grain_type_relative_volume[4] * scaling_factor), 0.0f, max_part_count);
            metamorphic_unweathered_intrusive_part_count    = std::clamp(std::round(input.grain_type_relative_volume[5] * scaling_factor), 0.0f, max_part_count);
            metamorphic_weathered_extrusive_part_count      = std::clamp(std::round(input.grain_type_relative_volume[6] * scaling_factor), 0.0f, max_part_count);
            metamorphic_weathered_intrusive_part_count      = std::clamp(std::round(input.grain_type_relative_volume[7] * scaling_factor), 0.0f, max_part_count);
  		}
		/*
		void unpack(Mineral& output) const
		{
		    output.mass = mass;

		    float total_relative_volume(0);
            for (std::size_t i=0; i<6; i++)
            {
                total_relative_volume += grain_type_relative_volume[i];
            }
            for (std::size_t i=0; i<6; i++)
            {
                output.grain_type_relative_volume[i] = grain_type_relative_volume[i] / total_relative_volume;
            }
		}
		void pack(const Mineral& input)
		{
			mass = input.mass;
            // rescale bin counts by the new max to fit inside a uint8_t
            float grain_type_relative_volume_max(epsilon);
            for (std::size_t i=0; i<6; i++)
            {
                grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[i]);
            }
			const float max_part_count(255.0f);
			float scaling_factor(max_part_count / grain_type_relative_volume_max);
            for (std::size_t i=0; i<6; i++)
            {
                grain_type_relative_volume[i] = 
                    std::clamp(input.grain_type_relative_volume[i] * scaling_factor, 0.0f, max_part_count);
            }
  		}
  		*/
	};
}

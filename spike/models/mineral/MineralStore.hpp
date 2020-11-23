#pragma once

// C libraries
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include "Mineral.hpp"
#include "GrainType.hpp"

namespace mineral
{
	namespace
	{
		const float epsilon(1e-4f);
	}

	/*
	`MineralStore` is a memory efficient variant of the 
	`Mineral` data structure. Isn't it adorable!?
	It would take ridiculous amounts of memory to store a `Mineral` 
	for every mineral within a raster, so we store each mineral in a raster
	as a `MineralStore`, then convert back to `Mineral` when
	we want to perform some operation on it. 

	The interpretation of attributes within `MineralStore` is error prone,
	so to prevent users from doing so we encapsulate the class.

    The interpretation of attributes also comes with some performance penalty,
    so to encourage users not to spam calls to getters, 
    we only expose methods to convert to and from `StratumStore`.
    
    This also grants a certain mathematical purity to the object,
    since in the spirit of category theory the object at high level can be 
    treated strictly by its mappings to other states, 
    which in this case are isomorphic to within precision requirements and therefore invertible.
	*/
	class MineralStore
	{
		float mass; 
		std::uint8_t phase_id                                    : 4;
		std::uint8_t unweathered_extrusive_part_count            : 4;
		std::uint8_t unweathered_intrusive_part_count            : 4;
		std::uint8_t mechanically_weathered_extrusive_part_count : 4;
		std::uint8_t mechanically_weathered_intrusive_part_count : 4;
		std::uint8_t chemically_weathered_extrusive_part_count   : 4;
		std::uint8_t chemically_weathered_intrusive_part_count   : 4;
		std::uint8_t unused                                      : 4;

	public:
		~MineralStore()
		{
		}
		MineralStore(Mineral& output)
		{
			pack(output);
		}
		MineralStore()
		{
		}
		void unpack(Mineral& output) const
		{
		    output.mass = mass;
		    float total_relative_volume(epsilon);
			total_relative_volume += unweathered_extrusive_part_count           ;
			total_relative_volume += unweathered_intrusive_part_count           ;
			total_relative_volume += mechanically_weathered_extrusive_part_count;
			total_relative_volume += mechanically_weathered_intrusive_part_count;
			total_relative_volume += chemically_weathered_extrusive_part_count  ;
			total_relative_volume += chemically_weathered_intrusive_part_count  ;
			const float scaling_factor(1.0f / total_relative_volume);
            output.grain_type_relative_volume[0] = unweathered_extrusive_part_count            * scaling_factor;
			output.grain_type_relative_volume[1] = unweathered_intrusive_part_count            * scaling_factor;
			output.grain_type_relative_volume[2] = mechanically_weathered_extrusive_part_count * scaling_factor;
			output.grain_type_relative_volume[3] = mechanically_weathered_intrusive_part_count * scaling_factor;
			output.grain_type_relative_volume[4] = chemically_weathered_extrusive_part_count   * scaling_factor;
			output.grain_type_relative_volume[5] = chemically_weathered_intrusive_part_count   * scaling_factor;
		}
		void pack(const Mineral& input)
		{
			mass = input.mass;
            // rescale bin counts by the new max to fit inside a uint8_t
            float grain_type_relative_volume_max(epsilon);
            grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[0]);
			grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[1]);
			grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[2]);
			grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[3]);
			grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[4]);
			grain_type_relative_volume_max = std::max(grain_type_relative_volume_max, input.grain_type_relative_volume[5]);
			const float max_part_count(15.0f);
			float scaling_factor(max_part_count / grain_type_relative_volume_max);
            unweathered_extrusive_part_count            = std::clamp(std::round(input.grain_type_relative_volume[0] * scaling_factor), 0.0f, max_part_count);
            unweathered_intrusive_part_count            = std::clamp(std::round(input.grain_type_relative_volume[1] * scaling_factor), 0.0f, max_part_count);
            mechanically_weathered_extrusive_part_count = std::clamp(std::round(input.grain_type_relative_volume[2] * scaling_factor), 0.0f, max_part_count);
            mechanically_weathered_intrusive_part_count = std::clamp(std::round(input.grain_type_relative_volume[3] * scaling_factor), 0.0f, max_part_count);
            chemically_weathered_extrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[4] * scaling_factor), 0.0f, max_part_count);
            chemically_weathered_intrusive_part_count   = std::clamp(std::round(input.grain_type_relative_volume[5] * scaling_factor), 0.0f, max_part_count);
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

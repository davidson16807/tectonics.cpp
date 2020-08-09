#pragma once

// C libraries
#include <cstdint>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include "GrainType.hpp"
#include "Mineral.hpp"

namespace mineral
{

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
    which in this case are isomorphic and invertible.
	*/
	class MineralStore
	{
		float mass; 
		std::array<std::uint8_t, int(GrainType::count)> grain_type_relative_volume;

	public:
		~MineralStore()
		{
		}
		void decompress(Mineral& output) const
		{
		    output.mass = mass;

		    float total_relative_volume(0);
            for (std::size_t i=0; i<int(GrainType::count); i++)
            {
                total_relative_volume += grain_type_relative_volume[i];
            }
            for (std::size_t i=0; i<int(GrainType::count); i++)
            {
                output.grain_type_relative_volume[i] = grain_type_relative_volume[i] / total_relative_volume;
            }
		}
		void compress(const Mineral& input)
		{
			mass = input.mass;
            // rescale bin counts by the new max to fit inside a uint8_t
            float grain_type_relative_bin_count_max = 0;
            for (std::size_t i=0; i<int(GrainType::count); i++)
            {
                grain_type_relative_bin_count_max = std::max(grain_type_relative_bin_count_max, input.grain_type_relative_volume[i]);
            }
            for (std::size_t i=0; i<int(GrainType::count); i++)
            {
                grain_type_relative_volume[i] = 
                    std::clamp(input.grain_type_relative_volume[i] * 255.0f / grain_type_relative_bin_count_max, 0.0f, 255.0f);
            }
  		}
	};
}

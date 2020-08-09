#pragma once

// C libraries
#include <cmath>
#include <cstdint>
#include <cstddef>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include "GrainType.hpp"

namespace mineral
{
    /*
    A "mineral" describes a mass pool composed entirely of a single compound
    Each bin tracks the relative volume occupied by particles or grains that meet certain criteria.
	We lack an accurate model to express how particle size changes in response to weathering, 
	and we do not trust ourselves to invent our own (nor do we have the time to do so),
	so we do not classify by size. 

	We instead classify by history.
	This includes whether the grain was originally from an intrusive or extrusive volcanic rock,
	and whether the rock experienced mechanical weathering or chemical weathering.
	The type of source rock, the type of weathering, and the type of mineral 
	can then be used to guess the size of the rock and the size of the grains for that mineral:

	unweathered extrusive rock                        boulder size   small grains
	unweathered intrusive rock                        boulder size   large grains
	mechanically weathered extrusive rock             gravel size    small grains
	mechanically weathered intrusive rock             sand size      large grains
	resistant, chemically weathered extrusive rock    silt size      small grains
	resistant, chemically weathered intrusive rock    sand size      large grains
	susceptible, chemically weathered rock            clay size      small grains

	We allow for up to 8 grain types but only allocate to 6.
	This is mostly to simplify cache alignment, though it also allows for future expansion.
    */

	struct Mineral
	{
		float mass; 
		std::array<float, int(GrainType::count)> grain_type_relative_volume;

		/*
		Constructs an empty mass pool with minimum possible pressure and temperature,
		min and max grain sizes are undefined.
		*/
		Mineral():
			mass(0)
		{
			grain_type_relative_volume.fill(0.0f);
		}

        // DERIVED ATTRIBUTES, regular functions of the form: Mineral -> primitive
		float grain_type_total_relative_volume() const 
		{
		    float total_relative_volume(0);
            for (std::size_t i=0; i<int(GrainType::count); i++)
            {
                total_relative_volume += grain_type_relative_volume[i];
            }
            return total_relative_volume;
		}
	};
}

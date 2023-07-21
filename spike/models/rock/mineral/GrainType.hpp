#pragma once

namespace mineral
{
    /*
    "Mineral.grain_type_relative_volume" describes a set of bins.
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
	enum struct GrainType
	{
		unweathered_extrusive,
		unweathered_intrusive,
		mechanically_weathered_extrusive,
		mechanically_weathered_intrusive,
		chemically_weathered_extrusive,
		chemically_weathered_intrusive,
		count = 6
	};

}

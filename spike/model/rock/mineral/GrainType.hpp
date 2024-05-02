#pragma once

namespace rock
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

	We would ideally represent the distinction in mechanical/chemical weathering
	but this would complicate our existing weathering model,
	and it would increase the size of model state in a way 
	that makes cache alignment difficult or inefficient.

	We would also ideally want to represent the fraction 
	of all possible phases for the mineral in combination with the attributes above.
	This is useful since metamorphosis is a change in state 
	that persists through weathering and lithification,
	and it permits scenarios that are of interest to the user (e.g. alluvial diamond deposits)
	However this would cost a lot of memory, and the precise number of phases is not known.
	We therefore suffice to represent the fraction of the mineral 
	that has undergone at least some kind of metamorphosis.
    */
	enum struct GrainType
	{
		unweathered_amorphous,
		unweathered_extrusive,
		unweathered_intrusive,
		unweathered_crystalline,
		weathered_amorphous,
		weathered_extrusive,
		weathered_intrusive,
		weathered_crystalline,
		count = 8
	};

}

#pragma once

namespace rasters 
{

	/*
	A "LayerScheme" specifies a way in which the cells of a grid 
	can be subdivided into several "layers" that occur at regular
	intervals along a dimension that's perpendicular to the surface. 
	Layers are stored sequentially in memory, ordered from top to bottom.
	Layers can be accessed by index, in which case layer 0 is topmost.
	*/
	struct LayerScheme
	{
		const float profile_top_height;
		const float profile_bottom_height;
		const float layer_count;
		const float layer_height;
		
		explicit LayerScheme(
			const float profile_top_height, 
			const float profile_bottom_height, 
			const float layer_count
		): 
			profile_top_height(profile_top_height),
			profile_bottom_height(profile_bottom_height),
			layer_count(layer_count),
			layer_height((profile_top_height-profile_bottom_height)/layer_count)
		{}
	};
}


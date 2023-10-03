#pragma once

#include <cmath>

namespace grid 
{

	/*
	A "Layering" specifies a way in which the vertices on a grid 
	can be subdivided into several "layers" that occur at regular
	intervals along a dimension that's perpendicular to the surface. 
	Layers are stored sequentially in memory, ordered from top to bottom.
	Layers can be accessed by index, in which case layer 0 is topmost.
	*/
	template <typename id, typename scalar>
	struct Layering
	{

		const scalar bottom_height;
		const scalar top_height;
		const id layer_count;
		const scalar layer_height;

		explicit Layering(
			const scalar bottom_height, 
			const scalar top_height, 
			const id layer_count
		): 
			bottom_height(bottom_height),
			top_height(top_height),
			layer_count(layer_count),
			layer_height((top_height-bottom_height)/scalar(layer_count))
		{}

		constexpr id height_to_layer(const scalar height) const {
			return id(std::round(std::clamp(height, bottom_height, top_height) / layer_height));
		}

		constexpr scalar layer_to_height(const id layer_id) const {
			return std::clamp(layer_id * layer_height + bottom_height, bottom_height, top_height);
		}
	};
}


#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp
#include <vector>		// vectors
#include <array>		// arrays
#include <limits>       // infinity

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/mat3x3.hpp>        // *mat3

namespace rasters
{

	/*
	An `CollignonTesselation` is a tesselation of tiles based around the "Collignon" projection of a sphere.
	This tesselation has the interesting property that it preserves the topological concept of "closeness":
	points that are close together on the tesselation correspond to points that are also close together on a sphere.
	This property, coupled with the equal area and isolatitude properties of of the collignon projection, 
	allows the construction of a mesh that provides a superset of the guarantees offered by HEALPix:

	* arbitrary subdivision of vertices
	* closeness preservation
	* hierarchical (trivial, since it allows arbitrary subdivision)
	* equal area
	* isolatitude

	The `CollignonTesselation` class exclusively represents data structures and functions 
	that define an isomorphism between points on a sphere and such a tesselation.
	*/

	class CollignonTesselation
	{
		static constexpr float pi = 3.141592652653589793f;
		static constexpr float quadrant_area = pi;
		static constexpr float quadrant_projection_length = std::sqrt(quadrant_area);
		static constexpr float tile_length_world_count = 2.0f;

		glm::vec2 standardize(const glm::vec2 grid_position) const 
		{
			const glm::vec2 tile_id = glm::round(grid_position/tile_length_world_count);
			const float negative_if_tesselation_is_inverted = std::cos(pi*(tile_id.x+tile_id.y));
			const glm::vec2 standardized = 
				(grid_position - tile_id * tile_length_world_count) * negative_if_tesselation_is_inverted;
			return standardized;
		}

		// NOTE: we need a dedicated sign function to simplify code such that an input of 0 returns a nonzero number.
		// Whether it returns 1 or -1 doesn't matter, it just needs to pick a side from which all further decisions can be made
		glm::vec3 get_octant_id(const glm::vec3 v) const {
			return glm::vec3(
				v.x >= 0.0f? 1.0f : -1.0f, 
				v.y >= 0.0f? 1.0f : -1.0f,
				v.z >= 0.0f? 1.0f : -1.0f
			);
		}
		glm::vec3 get_octant_id(const glm::vec2 v) const {
			return glm::vec3(
				v.x >= 0.0f? 1.0f : -1.0f, 
				v.y >= 0.0f? 1.0f : -1.0f,
				std::abs(v.x) + std::abs(v.y) <= 1.0f? 1.0f : -1.0f
			);
		}

	public:
		const CollignonProjection projection;

		~CollignonTesselation()
		{
		}
		explicit CollignonTesselation(CollignonProjection projection) : projection(projection)
		{
		}

		glm::vec2 sphere_to_tesselation(const glm::vec3 sphere_position) const {
			// `octant_id` is the canonical octant on which all subsequent operations concerning octant are based.
			// it is used to prevent edge case errors in which different operations assume different octants due to float imprecision.
			const glm::vec3 octant_id = get_octant_id(sphere_position);
			const float center_longitude = octant_id.z > 0.0f? 0.0f : pi;
			const glm::vec2 projected = projection.hemisphere_to_collignon(sphere_position, center_longitude) / quadrant_projection_length;
			const glm::vec2 rotated = glm::vec2(projected.y, -projected.x) * -octant_id.x * octant_id.y;
			const glm::vec2 translated = rotated + glm::vec2(octant_id.x, octant_id.y); 
			return octant_id.z > 0.0f? projected : translated; 
		}

		glm::vec3 tesselation_to_sphere(const glm::vec2 grid_position) const {
			const glm::vec2 standardized = standardize(grid_position);
			// `octant_id` is the canonical octant on which all subsequent operations concerning octant are based.
			// it is used to prevent edge case errors in which different operations assume different octants due to float imprecision.
			const glm::vec3 octant_id = get_octant_id(standardized);
			const glm::vec2 detranslated = standardized - glm::vec2(octant_id.x, octant_id.y);
			const glm::vec2 derotated = glm::vec2(detranslated.y, -detranslated.x) * octant_id.x * octant_id.y;
			const glm::vec3 sphere_position = octant_id.z > 0.0? 
				projection.collignon_to_hemisphere(quadrant_projection_length * standardized, 0.0f) 
			  : projection.collignon_to_hemisphere(quadrant_projection_length * derotated, pi);
			return sphere_position;
		}

		// int memory_id(const glm::vec2 grid_position) const {
		// 	const glm::vec2 standardized = standardize(glm::round(grid_position) + 0.5f) - 0.5f;
		// 	const glm::ivec2 standard_grid_id = glm::ivec2(standardized) + side_leg_cell_count;
		// 	const glm::ivec2 unique_grid_id = glm::ivec2(standard_grid_id.x % tesselation_leg_cell_count, standard_grid_id.y % tesselation_leg_cell_count);
		// 	const int memory_id = std::clamp(unique_grid_id.x, 0, tesselation_leg_cell_count-1)
		// 		  + std::clamp(unique_grid_id.y, 0, tesselation_leg_cell_count-1) * tesselation_leg_cell_count;
		//     return memory_id;
		// }

		// glm::vec2 grid_position(const int memory_id) const {
		// 	return glm::vec2(memory_id % tesselation_leg_cell_count, memory_id / tesselation_leg_cell_count) - float(side_leg_cell_count);
		// }

	};
}
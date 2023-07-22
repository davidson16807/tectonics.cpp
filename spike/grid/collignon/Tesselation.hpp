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

// in-house libraries
#include "Projection.hpp"

namespace collignon
{

	/*
	An `Tesselation` is a tesselation of tiles based around the "" projection of a unit sphere.
	This tesselation has the interesting property that it preserves the topological concept of "closeness":
	points that are close together on the tesselation correspond to points that are also close together on a sphere.
	This property, coupled with the equal area and isolatitude properties of of the collignon projection, 
	allows the construction of a mesh that provides a superset of the guarantees offered by HEALPix:

	* arbitrary subdivision of vertices
	* closeness preservation
	* hierarchical (trivial, since it allows arbitrary subdivision)
	* equal area
	* isolatitude

	The `Tesselation` class exclusively represents data structures and functions 
	that define an isomorphism between points on a sphere and such a tesselation.
	*/

    template<typename Tfloat=float>
	class Tesselation
	{
		static constexpr Tfloat pi = 3.141592652653589793f;
		static constexpr Tfloat quadrant_area = pi;
		static constexpr Tfloat quadrant_projection_length = std::sqrt(quadrant_area);
		static constexpr Tfloat tile_length_world_count = 2.0f;


		// NOTE: we need a dedicated sign function to simplify code such that an input of 0 returns a nonzero number.
		// Whether it returns 1 or -1 doesn't matter, it just needs to pick a side from which all further decisions can be made
		glm::vec<3,Tfloat,glm::defaultp> get_octant_id(const glm::vec<3,Tfloat,glm::defaultp> v) const {
			return glm::vec<3,Tfloat,glm::defaultp>(
				v.x >= 0.0f? 1.0f : -1.0f, 
				v.y >= 0.0f? 1.0f : -1.0f,
				v.z >= 0.0f? 1.0f : -1.0f
			);
		}
		glm::vec<3,Tfloat,glm::defaultp> get_octant_id(const glm::vec<2,Tfloat,glm::defaultp> v) const {
			return glm::vec<3,Tfloat,glm::defaultp>(
				v.x >= 0.0f? 1.0f : -1.0f, 
				v.y >= 0.0f? 1.0f : -1.0f,
				std::abs(v.x) + std::abs(v.y) <= 1.0f? 1.0f : -1.0f
			);
		}

		const Projection<Tfloat> projection;
		
	public:

		~Tesselation()
		{
		}
		explicit Tesselation(Projection<Tfloat> projection) : projection(projection)
		{
		}

		glm::vec<2,Tfloat,glm::defaultp> standardize(const glm::vec<2,Tfloat,glm::defaultp> grid_position) const 
		{
			const glm::vec<2,Tfloat,glm::defaultp> tile_id = glm::round(grid_position/tile_length_world_count);
			const Tfloat negative_if_tesselation_is_inverted = std::cos(pi*(tile_id.x+tile_id.y));
			const glm::vec<2,Tfloat,glm::defaultp> standardized = 
				(grid_position - tile_id * tile_length_world_count) * negative_if_tesselation_is_inverted;
			return standardized;
		}

		glm::vec<2,Tfloat,glm::defaultp> sphere_to_tesselation(const glm::vec<3,Tfloat,glm::defaultp> sphere_position) const {
			// `octant_id` is the canonical octant on which all subsequent operations concerning octant are based.
			// it is used to prevent edge case errors in which different operations assume different octants due to float imprecision.
			const glm::vec<3,Tfloat,glm::defaultp> octant_id = get_octant_id(sphere_position);
			const Tfloat center_longitude = octant_id.z > 0.0f? 0.0f : pi;
			const glm::vec<2,Tfloat,glm::defaultp> projected = projection.hemisphere_to_collignon(sphere_position, center_longitude) / quadrant_projection_length;
			const glm::vec<2,Tfloat,glm::defaultp> rotated = glm::vec<2,Tfloat,glm::defaultp>(projected.y, -projected.x) * -octant_id.x * octant_id.y;
			const glm::vec<2,Tfloat,glm::defaultp> translated = rotated + glm::vec<2,Tfloat,glm::defaultp>(octant_id.x, octant_id.y); 
			return octant_id.z > 0.0f? projected : translated; 
		}

		glm::vec<3,Tfloat,glm::defaultp> tesselation_to_sphere(const glm::vec<2,Tfloat,glm::defaultp> grid_position) const {
			// `standardize` is the canonical grid position on which all subsequent calculations occur.
			// it is what allows Tesselation to preserve closeness, even between points that go over the edge of a tile in the tesselation.
			const glm::vec<2,Tfloat,glm::defaultp> standardized = standardize(grid_position);
			// `octant_id` is the canonical octant on which all subsequent operations concerning octant are based.
			// it is used to prevent edge case errors in which different operations assume different octants due to float imprecision.
			const glm::vec<3,Tfloat,glm::defaultp> octant_id = get_octant_id(standardized);
			const glm::vec<2,Tfloat,glm::defaultp> detranslated = standardized - glm::vec<2,Tfloat,glm::defaultp>(octant_id.x, octant_id.y);
			const glm::vec<2,Tfloat,glm::defaultp> derotated = glm::vec<2,Tfloat,glm::defaultp>(detranslated.y, -detranslated.x) * octant_id.x * octant_id.y;
			const glm::vec<3,Tfloat,glm::defaultp> sphere_position = octant_id.z > 0.0? 
				projection.collignon_to_hemisphere(quadrant_projection_length * standardized, 0.0f) 
			  : projection.collignon_to_hemisphere(quadrant_projection_length * derotated, pi);
			return sphere_position;
		}


	};
}
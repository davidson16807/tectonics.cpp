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

namespace collignon
{
	/*
	A `collignon::Projection` is a representation of the classic "collignon" projection for a unit sphere.

	`Projection` exclusively represents data structures and functions that define an isomorphism between points on a hemisphere and a collignon projection.
	*/

    template<typename scalar=double>
	class Projection
	{
        using vec2 = glm::vec<2,scalar,glm::defaultp>;
        using vec3 = glm::vec<3,scalar,glm::defaultp>;

		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr scalar quadrant_area = pi;
		static constexpr scalar hemisphere_area = 2.0f * pi;
		static constexpr scalar circumference = 2.0f * pi;
		static constexpr scalar quadrant_projection_length = std::sqrt(quadrant_area);

		// NOTE: we need a dedicated sign function to simplify code such that an input of 0 returns a nonzero number.
		inline constexpr scalar sign(const scalar x) const {
			return x >= 0.0f? 1.0 : -1.0;
		}

	public:

		/*
		`collignon` maps a 3d unit vector to an equal area 2d coordinate system for one of two hemispheres. 
		Each ordinate is scaled to the range [-1,1], and x=0 on the 2d grid represents a `center_longitude` along the sphere.
		`center_longitude` is described as an angle around the y axis in radians, where 0 indicates the vector [0,0,1].
		*/
		constexpr vec2 hemisphere_to_collignon(const vec3 hemisphere_position, const scalar center_longitude) const {
			const vec3 normalized = glm::normalize(hemisphere_position);
			const scalar scale_factor = std::sqrt(1.0f - std::abs(normalized.y));
			const scalar longitude = std::atan2(normalized.x * sign(normalized.z), std::abs(normalized.z));
			const scalar hemiwedge_area = hemisphere_area * longitude / circumference;
			const scalar hemiwedge_projection_width = 2.0f * hemiwedge_area / quadrant_projection_length;
			const vec2 collignon = vec2(
				hemiwedge_projection_width * scale_factor,
				quadrant_projection_length * (1.0f-scale_factor) * sign(normalized.y)
			);
			return collignon;
		}

		/*
		*/
		constexpr vec3 collignon_to_hemisphere(const vec2 collignon, const scalar center_longitude) const {
			const scalar scale_factor = 1.0f - std::abs(collignon.y) / quadrant_projection_length;
			const scalar hemiwedge_projection_width = scale_factor == 0.0f? 0.0f : (collignon.x / scale_factor);
			const scalar hemiwedge_area = hemiwedge_projection_width * quadrant_projection_length / 2.0f;
			const scalar longitude = (hemiwedge_area * circumference / hemisphere_area) + center_longitude;

			const scalar y = sign(collignon.y) * (1.0f - scale_factor * scale_factor);
			const scalar rxz = 1.0 - y*y <= 0.0f? 0.0f : std::sqrt(1.0 - y*y);
			const vec3 hemisphere_position = vec3(
				std::sin(longitude) * rxz,
				y,
				std::cos(longitude) * rxz
			);

			return hemisphere_position;
		}

	};
}
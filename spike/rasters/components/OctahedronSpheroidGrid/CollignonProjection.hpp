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
	An `CollignonProjection` is a representation of the classic "Collignon" projection for a unit sphere.

	`CollignonProjection` exclusively represents data structures and functions that define an isomorphism between points on a hemisphere and a collignon projection.
	*/

	class CollignonProjection
	{
		static constexpr float pi = 3.141592652653589793f;
		static constexpr float quadrant_area = pi;
		static constexpr float hemisphere_area = 2.0f * pi;
		static constexpr float circumference = 2.0f * pi;
		static constexpr float quadrant_projection_length = std::sqrt(quadrant_area);

		// NOTE: we need a dedicated sign function to simplify code such that an input of 0 returns a nonzero number.
		float sign(const float x) const {
			return x >= 0.0f? 1.0 : -1.0;
		}

	public:
		~CollignonProjection()
		{
		}
		explicit CollignonProjection()
		{
		}

		/*
		`collignon` maps a 3d unit vector to an equal area 2d coordinate system for one of two hemispheres. 
		Each ordinate is scaled to the range [-1,1], and x=0 on the 2d grid represents a `center_longitude` along the sphere.
		`center_longitude` is described as an angle around the y axis in radians, where 0 indicates the vector [0,0,1].
		*/
		glm::vec2 hemisphere_to_collignon(const glm::vec3 hemisphere_position, const float center_longitude) const {
			const glm::vec3 normalized = glm::normalize(hemisphere_position);
			const float scale_factor = std::sqrt(1.0f - std::abs(normalized.y));
			const float longitude = std::atan2(normalized.x * sign(normalized.z), std::abs(normalized.z));
			const float hemiwedge_area = hemisphere_area * longitude / circumference;
			const float hemiwedge_projection_width = 2.0f * hemiwedge_area / quadrant_projection_length;
			const glm::vec2 collignon = glm::vec2(
				hemiwedge_projection_width * scale_factor,
				quadrant_projection_length * (1.0f-scale_factor) * sign(normalized.y)
			);
			return collignon;
		}

		/*
		*/
		glm::vec3 collignon_to_hemisphere(const glm::vec2 collignon, const float center_longitude) const {
			const float scale_factor = 1.0f - std::abs(collignon.y) / quadrant_projection_length;
			const float hemiwedge_projection_width = scale_factor == 0.0f? 0.0f : (collignon.x / scale_factor);
			const float hemiwedge_area = hemiwedge_projection_width * quadrant_projection_length / 2.0f;
			const float longitude = (hemiwedge_area * circumference / hemisphere_area) + center_longitude;

			const float y = sign(collignon.y) * (1.0f - scale_factor * scale_factor);
			const float rxz = 1.0 - y*y <= 0.0f? 0.0f : std::sqrt(1.0 - y*y);
			const glm::vec3 hemisphere_position = glm::vec3(
				std::sin(longitude) * rxz,
				y,
				std::cos(longitude) * rxz
			);

			return hemisphere_position;
		}

	};
}
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
	An `Projection` is a representation of the classic "" projection for a unit sphere.

	`Projection` exclusively represents data structures and functions that define an isomorphism between points on a hemisphere and a collignon projection.
	*/

    template<typename Tfloat=float>
	class Projection
	{
		static constexpr Tfloat pi = 3.141592652653589793f;
		static constexpr Tfloat quadrant_area = pi;
		static constexpr Tfloat hemisphere_area = 2.0f * pi;
		static constexpr Tfloat circumference = 2.0f * pi;
		static constexpr Tfloat quadrant_projection_length = std::sqrt(quadrant_area);

		// NOTE: we need a dedicated sign function to simplify code such that an input of 0 returns a nonzero number.
		Tfloat sign(const Tfloat x) const {
			return x >= 0.0f? 1.0 : -1.0;
		}

	public:
		~Projection()
		{
		}
		explicit Projection()
		{
		}

		/*
		`collignon` maps a 3d unit vector to an equal area 2d coordinate system for one of two hemispheres. 
		Each ordinate is scaled to the range [-1,1], and x=0 on the 2d grid represents a `center_longitude` along the sphere.
		`center_longitude` is described as an angle around the y axis in radians, where 0 indicates the vector [0,0,1].
		*/
		glm::vec<2,Tfloat,glm::defaultp> hemisphere_to_collignon(const glm::vec<3,Tfloat,glm::defaultp> hemisphere_position, const Tfloat center_longitude) const {
			const glm::vec<3,Tfloat,glm::defaultp> normalized = glm::normalize(hemisphere_position);
			const Tfloat scale_factor = std::sqrt(1.0f - std::abs(normalized.y));
			const Tfloat longitude = std::atan2(normalized.x * sign(normalized.z), std::abs(normalized.z));
			const Tfloat hemiwedge_area = hemisphere_area * longitude / circumference;
			const Tfloat hemiwedge_projection_width = 2.0f * hemiwedge_area / quadrant_projection_length;
			const glm::vec<2,Tfloat,glm::defaultp> collignon = glm::vec<2,Tfloat,glm::defaultp>(
				hemiwedge_projection_width * scale_factor,
				quadrant_projection_length * (1.0f-scale_factor) * sign(normalized.y)
			);
			return collignon;
		}

		/*
		*/
		glm::vec<3,Tfloat,glm::defaultp> collignon_to_hemisphere(const glm::vec<2,Tfloat,glm::defaultp> collignon, const Tfloat center_longitude) const {
			const Tfloat scale_factor = 1.0f - std::abs(collignon.y) / quadrant_projection_length;
			const Tfloat hemiwedge_projection_width = scale_factor == 0.0f? 0.0f : (collignon.x / scale_factor);
			const Tfloat hemiwedge_area = hemiwedge_projection_width * quadrant_projection_length / 2.0f;
			const Tfloat longitude = (hemiwedge_area * circumference / hemisphere_area) + center_longitude;

			const Tfloat y = sign(collignon.y) * (1.0f - scale_factor * scale_factor);
			const Tfloat rxz = 1.0 - y*y <= 0.0f? 0.0f : std::sqrt(1.0 - y*y);
			const glm::vec<3,Tfloat,glm::defaultp> hemisphere_position = glm::vec<3,Tfloat,glm::defaultp>(
				std::sin(longitude) * rxz,
				y,
				std::cos(longitude) * rxz
			);

			return hemisphere_position;
		}

	};
}
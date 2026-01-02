#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/mat3x3.hpp>        // *mat3

// in-house libraries
#include <math/special.hpp>

#include "Point.hpp"

namespace dymaxion
{

	/*
	GUIDE TO VARIABLE NAMES:
	* `is_*` prefix: booleans
	* lowercase: scalars
	* UPPERCASE: vectors
	* `N/S/E/W`: cardinal direction
	* `V3`: 3d position
	* `V2`: 2d position
	* `N`: normal
	* `O`: origin
	* `i`: subgrid id
	*/

    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	class Triangles
	{
        using vec2 = glm::vec<2,scalar,Q>;
        using vec3 = glm::vec<3,scalar,Q>;
        using mat3 = glm::mat<3,3,scalar,Q>;

		static constexpr scalar pi = scalar(3.141592653589793238462643383279502884L);
		static constexpr id square_count = 10;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;
		static constexpr id i2 = 2;

	public:

		explicit Triangles()
		{
		}

		/*`triangle_id` retrieves a id for the triangle that can be used to cache triangle properties*/
		inline constexpr id triangle_id(
			const id i, 
			const bool is_polar
		) const {
			return (i*i2) + is_polar;
		}

		inline constexpr bool is_inverted_square_id(
			const id i,
			const bool is_polar
		) const {
			return is_polar == bool(i%2);
		}

		inline constexpr bool is_polar_square_id(
			const id i,
			const bool is_inverted
		) const {
			return is_inverted == i%2;
		}

		inline constexpr bool is_inverted_grid_position(
			const vec2 V2
		) const {
			return V2.y > V2.x;
		}

		inline constexpr vec3 origin(
			const id i, 
			const scalar square_polarity, 
			const bool is_polar
		) const {
			scalar z         (square_polarity*(is_polar? 1.0:-0.5));
			scalar longitude (i*half_subgrid_longitude_arc_length);
			return cartesian_from_zlon(z, longitude);
		}

		inline constexpr mat3 basis(
			const bool is_inverted,
			const vec3& W,
			const vec3& E,
			const vec3& O
		) const {
			return is_inverted? 
			mat3(E-O,W-O,O) 
			: 
			mat3(W-O,E-O,O)
			;
		}

		inline vec3 sphere_project(
			const vec3 V3
		) const {
			return glm::normalize(V3);
		}

		inline vec3 plane_project(
			const vec3& V3,
			const vec3& N,
			const vec3& O
		) const {
			return V3 * glm::dot(N,O)/glm::dot(N,V3);
		}

	};

}
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

namespace healpix
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
        using mat2 = glm::mat<2,2,scalar,Q>;

		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr id subgrid_count = 10;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/subgrid_count;

	public:

		explicit Triangles()
		{
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

		inline constexpr bool is_inverted_square_id(
			const id i,
			const bool is_polar
		) const {
			return is_polar == math::modulus(i,2);
		}

		inline constexpr bool is_polar_square_id(
			const id i,
			const bool is_inverted
		) const {
			return is_inverted == math::modulus(i,2);
		}

		inline constexpr bool is_inverted_grid_position(
			const vec2 V2
		) const {
			return V2.x + V2.y < scalar(1);
		}

		inline constexpr bool is_polar_point(
			const Point<id,scalar> point
		) const {
			return is_polar_square_id(
				point.square_id,
				is_inverted_grid_position(point.square_position)
			);
		}

		inline constexpr bool is_polar_sphere_position(
			const scalar square_polarity, 
			const vec3 V3, 
			const vec3 W, 
			const vec3 E
		) const {
			return std::abs(V3.z) > scalar(0.5);
		}

		inline constexpr bool is_eastern_sphere_position(
			const vec3 V3, 
			const vec3 N,
			const vec3 S
		) const {
			// V3⋅(N×S)>0 indicates that V3 occupies an eastern triangle
			return glm::dot(V3, glm::cross(N,S)) >= scalar(0);
		}

		inline constexpr mat2 basis(
			const bool is_inverted,
			const vec2 W,
			const vec2 E,
			const vec2 O
		) const {
			return is_inverted? mat2(E-O,W-O) : mat2(W-O,E-O);
		}

		inline constexpr vec3 sphere_project(
			const vec3 V3
		) const {
			return glm::normalize(V3);
		}

		inline constexpr vec3 plane_project(
			const vec3 V3,
			const vec3 N,
			const vec3 O
		) const {
			return V3 * glm::dot(N,O)/glm::dot(N,V3);
		}

	};

}
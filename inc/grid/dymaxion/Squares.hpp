#pragma once

// C libraries
#include <cmath>        // ceil, round 

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

    template<typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	class Squares
	{
        using vec3 = glm::vec<3,scalar,precision>;

		static constexpr scalar pi = scalar(3.141592653589793238462643383279502884L);
		static constexpr scalar half = scalar(0.5);
		static constexpr id i1 = 1;
		static constexpr id i2 = 2;
		static constexpr id square_count = 10;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/square_count;

	public:

		explicit Squares()
		{
		}

		inline constexpr vec3 westmost(const id i) const {
			scalar z         (half*std::pow(-i1,i));
			scalar longitude (i*half_subgrid_longitude_arc_length);
			return cartesian_from_zlon(z, longitude);
		}

		inline constexpr vec3 eastmost(const id i) const {
			return westmost(i+i2);
		}

		inline constexpr scalar polarity(const id i) const {
			return std::pow(-i1, i);
		}

	};

}

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
	class Squares
	{
        using vec3 = glm::vec<3,scalar,Q>;

		static constexpr scalar pi = 3.141592652653589793f;
		static constexpr id subgrid_count = 10;
		static constexpr scalar half_subgrid_longitude_arc_length = 2*pi/subgrid_count;

	public:

		explicit Squares()
		{
		}

		inline constexpr vec3 westmost(const id i) const {
			scalar z         (scalar(0.5) - scalar(math::modulus(i,id(2))));
			scalar longitude (i*half_subgrid_longitude_arc_length);
			return cartesian_from_zlon(z, longitude);
		}

		inline constexpr scalar polarity(const id i) const {
			return std::pow(-1, i);
		}

	};

}

#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp
#include <string>       // to_string

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/mat3x3.hpp>        // *mat3

// in-house libraries
#include <math/special.hpp>

namespace dymaxion
{

    template<typename scalar=float>
	glm::vec<3,scalar,glm::defaultp> cartesian_from_zlon(const scalar z, const scalar longitude)
	{
		scalar r(std::sqrt(scalar(1)-z*z));
		return glm::vec<3,scalar,glm::defaultp>(
			r*std::cos(longitude),
			r*std::sin(longitude),
			z);
	}

    template<typename id=int, typename scalar=double>
	struct Point
	{
        using vec2 = glm::vec<2,scalar,glm::defaultp>;
		id square_id;
		vec2 square_position;
		explicit Point(id i, vec2 V2):
			square_id(i),
			square_position(V2)
		{}
	};

    template<typename id, typename scalar>
    constexpr std::string to_string(const Point<id,scalar> point)
    {
        return "(" + std::to_string(point.square_id) + "," +std::to_string(point.square_position.x) + "," +std::to_string(point.square_position.y) + ")";
    }

    template<typename id, typename scalar>
    std::ostream& operator<<(std::ostream& os, const Point<id,scalar> point) { 
        os << to_string(point);
        return os;
    }

}
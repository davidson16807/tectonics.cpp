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

    template<typename id=int, typename scalar=double, glm::qualifier precision=glm::defaultp>
	struct Point
	{
        using vec2 = glm::vec<2,scalar,precision>;
		id square_id;
		vec2 square_position;

		explicit Point(id i, vec2 V2):
			square_id(i),
			square_position(V2)
		{}

		// cast constructor
    	template<typename id2, typename scalar2, glm::qualifier Q2>
		explicit Point(Point<id2,scalar2,Q2> p):
			square_id(p.square_id),
			square_position(p.square_position)
		{}

        constexpr Point<id,scalar,precision>& operator+=(const vec2 k)
        {
        	square_position += k;
            return *this;
        }

        constexpr Point<id,scalar,precision>& operator-=(const vec2 k)
        {
        	square_position -= k;
            return *this;
        }

        constexpr Point<id,scalar,precision>& operator*=(const scalar k)
        {
        	square_position *= k;
            return *this;
        }

        constexpr Point<id,scalar,precision>& operator/=(const scalar k)
        {
        	square_position /= k;
            return *this;
        }

	};

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr std::string to_string(const Point<id,scalar,precision> point)
    {
        return "Point(" + std::to_string(point.square_id) + "," +std::to_string(point.square_position.x) + "," +std::to_string(point.square_position.y) + ")";
    }

    template<typename id, typename scalar, glm::qualifier precision>
    std::ostream& operator<<(std::ostream& os, const Point<id,scalar,precision> point) { 
        os << to_string(point);
        return os;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator+(const Point<id,scalar,precision>& p, const glm::vec<2,scalar,precision> k)
    {
        Point<id,scalar,precision> y(p);
        y += k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator-(const Point<id,scalar,precision>& p, const glm::vec<2,scalar,precision> k)
    {
        Point<id,scalar,precision> y(p);
        y -= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator*(const Point<id,scalar,precision>& p, const scalar k)
    {
        Point<id,scalar,precision> y(p);
        y *= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator/(const Point<id,scalar,precision>& p, const scalar k)
    {
        Point<id,scalar,precision> y(p);
        y /= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator+(const scalar k, const Point<id, scalar>& p)
    {
        Point<id,scalar,precision> y(p);
        y += k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator-(const scalar k, const Point<id, scalar>& p)
    {
        Point<id,scalar,precision> y(p);
        y -= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator*(const scalar k, const Point<id, scalar>& p)
    {
        Point<id,scalar,precision> y(p);
        y *= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    constexpr Point<id,scalar,precision> operator/(const scalar k, const Point<id, scalar>& p)
    {
        Point<id,scalar,precision> y(p);
        y /= k;
        return y;
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> round(const Point<id,scalar,precision>& p)
    {
        return Point<id,scalar,precision>(p.square_id, glm::round(p.square_position));
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> floor(const Point<id,scalar,precision>& p)
    {
        return Point<id,scalar,precision>(p.square_id, glm::floor(p.square_position));
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> ceil(const Point<id,scalar,precision>& p)
    {
        return Point<id,scalar,precision>(p.square_id, glm::ceil(p.square_position));
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> min(const Point<id,scalar,precision>& p, const scalar k)
    {
        return Point<id,scalar,precision>(p.square_id, glm::min(p.square_position, k));
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> max(const Point<id,scalar,precision>& p, const scalar k)
    {
        return Point<id,scalar,precision>(p.square_id, glm::max(p.square_position, k));
    }

    template<typename id, typename scalar, glm::qualifier precision>
    inline constexpr Point<id,scalar,precision> clamp(const Point<id,scalar,precision>& p, const scalar lo, const scalar hi)
    {
        return Point<id,scalar,precision>(p.square_id, glm::clamp(p.square_position, lo, hi));
    }

}
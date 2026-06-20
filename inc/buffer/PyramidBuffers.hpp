#pragma once

// c libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include "PrimitiveBuffers.hpp"

namespace buffer {

	template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	struct PyramidBuffers
	{

        using vec3 = glm::vec<3,scalar,Q>;

		static constexpr scalar pi = scalar(3.141592653589793238462643383279502884L);
        static constexpr id vertices_per_triangle = 3;

        const buffer::PrimitiveBuffers<id,Q> primitives;

        PyramidBuffers():
        	primitives()
        {}

		constexpr inline id triangles_size(const id side_count) const
		{
			return vertices_per_triangle * 2 * side_count;
		}

		template<typename Buffer>
		id storeTriangles(
			const vec3 base, const vec3 tip, const vec3 prime_meridian, const scalar radius, const id side_count,
			Buffer& output, const id buffer_start_id=0) const 
		{
			vec3 direction(glm::normalize(tip-base));
			vec3 J(radius*glm::normalize(glm::cross(direction, prime_meridian)));
			vec3 I(radius*glm::normalize(glm::cross(J, direction)));
			const vec3& O = base;
			const vec3& V = tip;

			id buffer_id(buffer_start_id);
			scalar radians_per_side(2*pi/side_count);
			scalar radians0, radians1, x0, y0, x1, y1;
			for (id i = 0; i < side_count; ++i)
			{
				radians0 =  i   *radians_per_side;
				radians1 = (i+1)*radians_per_side;
				x0 = std::cos(radians0);
				y0 = std::sin(radians0);
				x1 = std::cos(radians1);
				y1 = std::sin(radians1);
				buffer_id = primitives.storeTriangle(V, O+x0*I+y0*J, O+x1*I+y1*J, output, buffer_id);
				buffer_id = primitives.storeTriangle(O, O+x1*I+y1*J, O+x0*I+y0*J, output, buffer_id);
			}
			return buffer_id;
		}

	};

}


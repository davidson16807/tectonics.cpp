#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

#include "PrimitiveBuffers.hpp"

namespace buffer {

	template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
	struct PyramidBuffers
	{

        using vec3 = glm::vec<3,scalar,Q>;

        const scalar pi = 3.141592653589793238462;
        const id vertices_per_triangle = 3;
        const buffer::PrimitiveBuffers<id,Q> primitives;

        PyramidBuffers():
        	primitives()
        {}

		template<int L=1>
		constexpr inline id triangles_size(const id side_count) const
		{
			return L * vertices_per_triangle * (2*side_count-1);
		}

		template<typename Buffer>
		constexpr id storeTriangles(
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
			scalar x0(1);
			scalar y0(0);
			scalar radians1, x1, y1;
			for (int i = 0; i < side_count; ++i)
			{
				radians1 = radians_per_side*(i+1);
				x1 = std::cos(radians1);
				y1 = std::sin(radians1);
				buffer_id = primitives.storeTriangle(V, O+x0*I+y0*J, O+x1*I+y1*J, output, buffer_id);
				if (0<i&&i<side_count-1)
				{
					buffer_id = primitives.storeTriangle(O+I, O+x1*I+y1*J, O+x0*I+y0*J, output, buffer_id);
				}
				x0 = x1;
				y0 = y1;
			}
			return buffer_id;
		}

	};

}
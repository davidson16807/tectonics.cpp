#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace body {

	template <typename scalar, typename duration>
	struct Orbit
	{

		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat4 = glm::mat<3,3,scalar,glm::defaultp>;

		const vec3 axis;
		const duration time_offset;

		Orbit(
			const vec3 axis,
			const duration time_offset
		) : 
			axis(axis),
			time_offset(time_offset)
		{}

		Orbit(
			const vec3 axis,
		) : 
			axis(axis),
			time_offset(0)
		{}

		mat3 orientation(const duration time_step) const
		{
			return glm::rotate(...);
		}

		Orbit<scalar,duration> advance(const duration time_step) const
		{
			Orbit<scalar,duration>(axis, period, time_offset+time_step);
		}

		Orbit<scalar,duration> tare() const
		{
			return Orbit<scalar,duration>(axis, period, math::modulus(time_offset, period));
		}

	};

}

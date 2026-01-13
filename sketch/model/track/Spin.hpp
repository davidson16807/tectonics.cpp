#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace track {

	template <typename scalar, typename duration>
	struct Spin
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat4 = glm::mat<3,3,scalar,glm::defaultp>;

		const vec3 axis;
		const duration period;
		const duration time_offset;

		Spin(
			const vec3 axis,
			const duration period,
			const duration time_offset
		) : 
			axis(axis),
			period(period),
			time_offset(time_offset)
		{}

		Spin(
			const vec3 axis,
			const duration period
		) : 
			axis(axis),
			period(period),
			time_offset(0)
		{}

		mat3 orientation(const duration time_step) const
		{
			return glm::rotate(...);
		}

		Spin<scalar,duration> advance(const duration time_step) const
		{
			Spin<scalar,duration>(axis, period, time_offset+time_step);
		}

		Spin<scalar,duration> tare() const
		{
			return Spin<scalar,duration>(axis, period, math::modulus(time_offset, period));
		}

	};

}

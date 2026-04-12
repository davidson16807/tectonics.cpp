#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace track {

	template <typename scalar, typename duration>
	struct Spin
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat3 = glm::mat<3,3,scalar,glm::defaultp>;

		const vec3 axis_radians;
		const scalar nutation_axis_radians_amplitude;
		const duration nutation_period;
		const duration precession_period;
		const duration spin_period;
		const duration time_offset;

		Spin(
			const vec3 axis_radians,
			const duration spin_period,
			const duration time_offset
		) : 
			axis_radians(axis_radians),
			spin_period(spin_period),
			time_offset(time_offset)
		{}

		Spin(
			const vec3 axis_radians,
			const duration spin_period
		) : 
			axis_radians(axis_radians),
			spin_period(spin_period),
			time_offset(0)
		{}

		mat3 orientation(const duration time_step) const
		{
			return glm::rotate(...);
		}

		Spin<scalar,duration> advance(const duration time_step) const
		{
			Spin<scalar,duration>(axis_radians, spin_period, time_offset+time_step);
		}

		Spin<scalar,duration> tare() const
		{
			return Spin<scalar,duration>(axis_radians, spin_period, math::modulus(time_offset, spin_period));
		}

	};

}

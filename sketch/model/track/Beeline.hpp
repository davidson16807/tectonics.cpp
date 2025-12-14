#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace body {

	template <typename scalar, typename duration>
	struct Beeline
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const vec3 initial_position;
		const vec3 initial_velocity;
		const duration time_offset;

		Beeline(
			const vec3 initial_position,
			const vec3 initial_velocity
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(0)
		{}

		Beeline(
			const vec3 initial_position,
			const vec3 initial_velocity,
			const duration time_offset
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

		State<scalar> state(const duration time_step) const
		{
			return State<scalar>(initial_position + time_step * initial_velocity, initial_velocity);
		}

		Beeline<scalar,duration> advance(const duration time_step) const
		{
			return Beeline<state,duration>(initial_position, initial_velocity, time_offset+time_step);
		}

		Beeline<scalar,duration> tare() const
		{
			return Beeline<state,duration>(state(time_offset));
		}

	};

}

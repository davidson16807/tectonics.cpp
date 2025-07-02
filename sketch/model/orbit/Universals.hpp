#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace orbit {

	template <typename scalar>
	struct Universal
	{
		const vec3 initial_position;
		vec3 initial_velocity;
		scalar time_offset;

		Universal(
			const vec3 initial_position,
			const vec3 initial_velocity,
			const scalar time_offset
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

	};

}

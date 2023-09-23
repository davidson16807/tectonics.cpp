#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // length2
#include <glm/vec3.hpp>     // *vec3

namespace orbit {

	template <typename Tfloat>
	struct Universal
	{
		const glm::vec<3,Tfloat,glm::defaultp> initial_position;
		glm::vec<3,Tfloat,glm::defaultp> initial_velocity;
		Tfloat time_offset;

		Universal(
			const glm::vec<3,Tfloat,glm::defaultp> initial_position,
			const glm::vec<3,Tfloat,glm::defaultp> initial_velocity,
			const Tfloat time_offset
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

	};

}

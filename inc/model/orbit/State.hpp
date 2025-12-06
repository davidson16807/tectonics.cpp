#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace orbit {

	template<typename scalar>
	struct State {

		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const vec3 position;
		const vec3 velocity;

		State(
			const vec3 position,
			const vec3 velocity
		) : 
			position(position),
			velocity(velocity)
		{}

	};

}


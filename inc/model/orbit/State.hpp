#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace orbit {

	template<typename scalar, glm::qualifier precision=glm::defaultp>
	struct State {

		using vec3 = glm::vec<3,scalar,precision>;

		const vec3 position;
		const vec3 velocity;

		constexpr State(
			const vec3& position,
			const vec3& velocity
		) noexcept : 
			position(position),
			velocity(velocity)
		{}

	};

}


#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // length2
#include <glm/vec3.hpp>     // *vec3

namespace orbit {

	template <typename Tfloat>
	struct State
	{
		const glm::vec<3,Tfloat,glm::defaultp> position;
		glm::vec<3,Tfloat,glm::defaultp> velocity;
		State(
			const glm::vec<3,Tfloat,glm::defaultp> position,
			const glm::vec<3,Tfloat,glm::defaultp> velocity
		) : 
			position(position),
			velocity(velocity)
		{}

	};

}

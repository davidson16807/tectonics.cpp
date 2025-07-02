#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace body {

	template <typename scalar, typename id>
	struct Illuminator
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const vec3 axis; // zero vector indicates omnidirectional Illuminator
		const id target;

		Illuminator(
			const vec3 axis,
			const id target,
		) : 
			axis(axis),
			target(target)
		{}

		Illuminator(
			const vec3 axis,
			const id target
		) : 
			axis(axis),
			target(target)
		{}

		mat3 orientation(const id time_step) const
		{
			return glm::rotate(...);
		}

	};

}

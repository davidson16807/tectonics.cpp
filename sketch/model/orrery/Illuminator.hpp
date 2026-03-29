#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace body {

	template <typename scalar, typename id>
	struct Illuminator
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const vec3 axis; // zero vector indicates omnidirectional Illuminator
		const id node;

		Illuminator(
			const vec3 axis,
			const id node,
		) : 
			axis(axis),
			node(node)
		{}

		Illuminator(
			const vec3 axis,
			const id node
		) : 
			axis(axis),
			node(node)
		{}

	};

}

#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace body {

	template <typename id, typename Component>
	struct Node
	{

		const id node_id,
		const Component component

		Node(
			const id node_id,
			const Component component
		) : 
			node_id(node_id),
			component(component)
		{}

		mat3 orientation(const duration time_step) const
		{
			return Node<Component>(node_id, component.orientation(time_step));
		}

		Node<Component> advance(const duration time_step) const
		{
			Node<Component>(node_id, component.advance(time_step));
		}

		Node<Component> tare() const
		{
			Node<Component>(node_id, component.tare());
		}

	};

}

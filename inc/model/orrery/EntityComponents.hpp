#pragma once

#include <cassert>       // assert

#include <utility>      // std::pair
#include <vector>       // std::vector

/*
`EntityComponents` is an alternate implementation of the `*Components` interface
that stores tuples of entity ids and their components using std::pair<id,Component>.
Some operations are O(N) so they are not included.
These operations that are common to other `*Component` classes, 
chiefly `has()`, `remove()`, `entity_destroyed()`, `component_for_entity()`, and `complete()`.
A consequence of this is that a single entity may possess multiple components of the same type,
since O(N) inclusion tests like `has()` are not performed.

`EntityComponents` is best suited to represent a table of components 
within an Entity-Component-System ("ECS") pattern where components are sparse and continously regenerated,
or tables where one entity could own many components of the same type.
*/

namespace orrery
{

template<typename id, typename Component>
class EntityComponents
{

	std::vector<std::pair<id,Component>> components;

public:

	/*
	Create an empty `EntityComponents<id,Component>` instance.
	*/
	EntityComponents():
		components()
	{
	}

	void clear()
	{
		components.clear();
	}

	void resize(std::size_t size)
	{
		components.resize(size);
	}

	void reserve(std::size_t size)
	{
		components.reserve(size);
	}

	void add(const id entity, const Component& component)
	{
		components.emplace_back(entity, component);
	}

	void add(const EntityComponents<id,Component>& other)
	{
		for (std::size_t i = 0; i < other.components.size(); ++i)
		{
			components.emplace_back(other.components[i].first, other.components[i].second);
		}
	}

	std::size_t component_count() const
	{
		return components.size();
	}

	// `size` mirrors std::vector-style sizing for callers that traverse entity slots directly.
	std::size_t size() const
	{
		return components.size();
	}

	const id entity_for_index(std::size_t component) const
	{
		return components[component].first;
	}

	const Component& component_for_index(std::size_t component) const
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component].second;
	}

	Component& component_for_index(std::size_t component)
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component].second;
	}

};

}


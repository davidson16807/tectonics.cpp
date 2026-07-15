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

	std::vector<std::pair<id,Component>> pairs;

public:

	/*
	Create an empty `EntityComponents<id,Component>` instance.
	*/
	EntityComponents():
		pairs()
	{
	}

	void clear()
	{
		pairs.clear();
	}

	void resize(std::size_t size)
	{
		pairs.resize(size);
	}

	void reserve(std::size_t size)
	{
		pairs.reserve(size);
	}

	void add(const id entity, const Component& component)
	{
		pairs.emplace_back(entity, component);
	}

	void add(const EntityComponents<id,Component>& other)
	{
		for (std::size_t i = 0; i < other.pairs.size(); ++i)
		{
			pairs.emplace_back(other.pairs[i].first, other.pairs[i].second);
		}
	}

	std::size_t component_count() const
	{
		return pairs.size();
	}

	// `size` mirrors std::vector-style sizing for callers that traverse entity slots directly.
	std::size_t size() const
	{
		return pairs.size();
	}

	const id entity_for_index(std::size_t component) const
	{
		return pairs[component].first;
	}

	const Component& component_for_index(std::size_t component) const
	{
		assert(component < pairs.size() && "Retrieving non-existent component.");
		return pairs[component].second;
	}

	Component& component_for_index(std::size_t component)
	{
		assert(component < pairs.size() && "Retrieving non-existent component.");
		return pairs[component].second;
	}

	void components_for_entity(const id entity, std::vector<Component>& components)
	{
		components.clear();
		for (std::size_t i = 0; i < pairs.size(); ++i)
		{
			if (pairs[i].first == entity)
			{
				components.push_back(pairs[i].second);
			}
		}
	}

	void components(std::vector<Component>& components)
	{
		components.clear();
		for (std::size_t i = 0; i < pairs.size(); ++i)
		{
			components.push_back(pairs[i].second);
		}
	}

};

}


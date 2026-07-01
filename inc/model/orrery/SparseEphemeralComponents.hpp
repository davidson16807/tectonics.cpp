#pragma once

#include <cassert>       // assert

#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map

/*
`SparseEphemeralComponents` represents a table of components within an Entity-Component-System ("ECS") pattern
where components or entities are frequently added or removed. `add` and `remove` are done in O(1) time.
It offers functionality beyond std::vector by managing lookups between component ids and entities,
thereby accelerating checks for systems that operate on entities with several kinds of components.

Since we want to avoid expensive memory reallocation,
The size of the std::vector is allocated according to the number of entities,
and components in the std::vector are rearranged to minimize changes in memory when entities are added or removed.
However this will mean that components are not sorted by their entity id,
so avoid `SparseEphemeralComponents` if sorting by entity id will better exploit cache memory in traversals involving multiple component types.

`SparseEphemeralComponents` is best suited if components are expected to be added or removed frequently,
and interaction between multiple component types is not expected so that contents here can be traversed in-order.
*/

namespace orrery 
{

template<typename id, typename Component>
class SparseEphemeralComponents
{

	// The packed array of components (of generic type Component)
	std::vector<Component> components;
	// Map from an array index to an entity ID.
	std::vector<id> entity_of_index;
	// Map from an entity ID to an array index.
	std::unordered_map<id, std::size_t> index_of_entity;
	// Total size of valid entries in the array.
	std::size_t size;

public:

	/*
	Create an empty `SparseEphemeralComponents<id,Component>` instance 
	*/
	SparseEphemeralComponents():
		components(),
		entity_of_index(),
		index_of_entity(),
		size(0)
	{
	}

	/*
	Create a `SparseEphemeralComponents<id,Component>` instance from the vector `components`
	where each components corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	SparseEphemeralComponents(const std::vector<Component>& components_):
		components(components_),
		entity_of_index(),
		index_of_entity(),
		size(components_.size())
	{
		for (std::size_t i = 0; i < components_.size(); ++i)
		{
			entity_of_index.push_back(id(i));
			index_of_entity[i] = std::size_t(i);
		}
	}

	/*
	Create a `SparseEphemeralComponents<id,Component>` 
	that has memory preallocated to serve a given number of entities.
	*/
	SparseEphemeralComponents(const id& entity_count):
		components(),
		entity_of_index(),
		index_of_entity(),
		size(0)
	{
	    components.reserve(std::size_t(entity_count));
	    entity_of_index.reserve(std::size_t(entity_count));
	}

	void add(const id entity, const Component& component)
	{
		assert(index_of_entity.find(entity) == index_of_entity.end() && "Component added to same entity more than once.");

		// Insert while preserving order by entity id.
		std::size_t insert_index = 0;
		while (insert_index < entity_of_index.size() && entity_of_index[insert_index] < entity)
		{
			++insert_index;
		}

		components.insert(components.begin() + insert_index, component);
		entity_of_index.insert(entity_of_index.begin() + insert_index, entity);

		// Update indices from insertion point onward.
		for (std::size_t i = insert_index; i < entity_of_index.size(); ++i)
		{
			index_of_entity[entity_of_index[i]] = i;
		}
	}

	void entity_destroyed(const id entity)
	{
		if (index_of_entity.find(entity) != index_of_entity.end())
		{
			// Remove the entity's component if it existed
			remove(entity);
		}
	}

	void remove(const id entity)
	{
		assert(index_of_entity.find(entity) != index_of_entity.end() && "Removing non-existent component.");

		const std::size_t removed_index = index_of_entity.at(entity);

		components.erase(components.begin() + removed_index);
		entity_of_index.erase(entity_of_index.begin() + removed_index);
		index_of_entity.erase(entity);

		// Update indices from removal point onward.
		for (std::size_t i = removed_index; i < entity_of_index.size(); ++i)
		{
			index_of_entity[entity_of_index[i]] = i;
		}
	}

	// `entity_count` returns the number of entities tracked by this component store
	[[nodiscard]] std::size_t entity_count() const
	{
		return components.size();
	}

	// `component_count` returns the number of components in this component store
	[[nodiscard]] std::size_t component_count() const
	{
		return size;
	}

	[[nodiscard]] bool has(id entity) const
	{
		return index_of_entity.find(entity) != index_of_entity.end();
	}

	[[nodiscard]] const id entity_for_index(std::size_t component) const
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return index_of_entity.at(component);
	}

	[[nodiscard]] const Component& component_for_entity(id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		// Return a reference to the entity's component
		return components.at(index_of_entity.at(entity));
	}

	[[nodiscard]] Component& component_for_entity(id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		// Return a reference to the entity's component
		return components.at(index_of_entity.at(entity));
	}

	[[nodiscard]] const Component& component_for_index(std::size_t component) const
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component];
	}

	[[nodiscard]] Component& component_for_index(std::size_t component)
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component];
	}
};

}

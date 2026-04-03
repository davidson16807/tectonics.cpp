#pragma once

#include <cassert>       // assert

#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map

/*
`UnsortedEphemeralComponents` represents a table of components within an Entity-Component-System ("ECS") pattern
where components or entities are frequently added or removed. `add` and `remove` are done in O(1) time.
It offers functionality beyond std::vector by managing lookups between component ids and entities,
thereby accelerating checks for systems that operate on entities with several kinds of components.

Since we want to avoid expensive memory reallocation,
The size of the std::vector is allocated according to the number of entities,
and components in the std::vector are rearranged to minimize changes in memory when entities are added or removed.
However this will mean that components are not sorted by their entity id,
so avoid `UnsortedEphemeralComponents` if sorting by entity id will better exploit cache memory in traversals involving multiple component types.

`UnsortedEphemeralComponents` is best suited if components are expected to be added or removed frequently,
and interaction between multiple component types is not expected so that contents here can be traversed in-order.
*/

namespace orrery 
{

template<typename id, typename Component>
class UnsortedEphemeralComponents
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
	Create an empty `UnsortedEphemeralComponents<id,Component>` instance 
	*/
	UnsortedEphemeralComponents():
		components(),
		entity_of_index(),
		index_of_entity(),
		size(0)
	{
	}

	/*
	Create a `UnsortedEphemeralComponents<id,Component>` instance from the vector `components`
	where each components corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	UnsortedEphemeralComponents(const std::vector<Component>& components_):
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
	Create a `UnsortedEphemeralComponents<id,Component>` 
	that has memory preallocated to serve a given number of entities.
	*/
	UnsortedEphemeralComponents(const id& entity_count):
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

		// Put new entry at end and update the maps
		std::size_t new_index = size;
		index_of_entity[entity] = new_index;
		++size;
		if (components.size() < size)
		{
			components.push_back(component);
			entity_of_index.push_back(entity);
		} 
		else 
		{
			entity_of_index[new_index] = entity;
			components[new_index] = component;
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

		// Copy element at end into deleted element's place to maintain density
		std::size_t index_of_removed_entity = index_of_entity[entity];
		std::size_t index_of_last_element = size - 1;

		if (index_of_removed_entity != index_of_last_element)
		{
			components[index_of_removed_entity] = components[index_of_last_element];

			// Update map to point to moved spot
			id entity_of_last_element = entity_of_index[index_of_last_element];
			index_of_entity[entity_of_last_element] = index_of_removed_entity;
			entity_of_index[index_of_removed_entity] = entity_of_last_element;
		}

		index_of_entity.erase(entity);

		--size;
	}

	// `entity_count` returns the number of entities tracked by this component store
	std::size_t entity_count() const
	{
		return size;
	}

	// `component_count` returns the number of components in this component store
	std::size_t component_count() const
	{
		return size;
	}

	bool has(id entity) const
	{
		return index_of_entity.find(entity) != index_of_entity.end();
	}

	const id entity_for_index(std::size_t component) const
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return index_of_entity.at(component);
	}

	const Component& component_for_entity(id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		// Return a reference to the entity's component
		return components.at(index_of_entity.at(entity));
	}

	Component& component_for_entity(id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		// Return a reference to the entity's component
		return components.at(index_of_entity.at(entity));
	}

	const Component& component_for_index(std::size_t component) const
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component];
	}

	Component& component_for_index(std::size_t component)
	{
		assert(component < components.size() && "Retrieving non-existent component.");
		return components[component];
	}
};

}


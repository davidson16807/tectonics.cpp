#pragma once

#include <cassert>       // assert

#include <vector>        // std::vector
#include <optional>      // std::optional

/*
`CommonComponents` represents a table of components within an Entity-Component-System ("ECS") pattern
where most entities are likely to have this component type. It stores components using std::vector.
this allows components to be stored in the same order as the entities, 
so in-order traversals involving multiple component types are much easier to obtain.

`CommonComponents` is best suited if components are likely to exist for most entities.
*/

namespace orrery
{

template<typename id, typename Component>
class CommonComponents
{
	// Components stored directly at the entity's index.
	std::vector<Component> component_store;
	// Whether a given entity currently owns a component.
	std::vector<bool> exists;
	// Number of entities that currently have a component.
	std::size_t component_count;

public:

	/*
	Create an empty `CommonComponents<id,Component>` instance.
	*/
	CommonComponents():
		component_store(),
		component_count(0)
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance from the vector `components_`
	where each component corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	CommonComponents(const std::vector<Component>& components_):
		component_store(components_),
		exists(components_.size()),
		component_count(components_.size())
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities.
	*/
	CommonComponents(const id& entity_count):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), false),
		component_count(0)
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities 
	and default components dependant on a the value of `existss`
	*/
	CommonComponents(const id& entity_count, const bool exists):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), exists),
		component_count(exists? std::size_t(entity_count) : 0)
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities with a single given component.
	*/
	CommonComponents(const id& entity_count, const Component component):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), true),
		component_count(std::size_t(entity_count))
	{
	}

	std::size_t size() const
	{
		return component_count;
	}

	void add(const id entity, const Component& component)
	{
		const std::size_t index = std::size_t(entity);
		if (index >= components.size())
		{
			components.resize(index + 1);
			exists.resize(index + 1, false);
		}

		assert(!exists[index] && "Component added to same entity more than once.");

		components[index] = component;
		exists[index] = true;
		++_size;
	}

	void entity_destroyed(const id entity)
	{
		if (has(entity))
		{
			remove(entity);
		}
	}

	void remove(const id entity)
	{
		assert(has(entity) && "Removing non-existent component.");

		exists[index] = false;
		--_size;
	}

	inline bool has(const id entity) const
	{
		const std::size_t index = std::size_t(entity);
		return index < component_store.size() && component_store[index].has_value();
	}

	const id entity_for_index(std::size_t component) const
	{
		return id(component);
	}

	const Component& component_for_entity(const id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return *component_store[std::size_t(entity)];
	}

	Component& component_for_entity(const id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return *component_store[std::size_t(entity)];
	}

	const Component& vector() const
	{
		return *component_store;
	}

};

}
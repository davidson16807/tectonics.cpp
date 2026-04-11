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

public:

	/*
	Create an empty `CommonComponents<id,Component>` instance.
	*/
	CommonComponents():
		component_store()
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance from the vector `components_`
	where each component corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	CommonComponents(const std::vector<Component>& components_):
		component_store(components_),
		exists(components_.size())
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities.
	*/
	CommonComponents(const id& entity_count):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), false)
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities 
	and default components dependant on a the value of `existss`
	*/
	CommonComponents(const id& entity_count, const bool exists):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), exists)
	{
	}

	/*
	Create a `CommonComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities with a single given component.
	*/
	CommonComponents(const id& entity_count, const Component component):
		component_store(std::size_t(entity_count)),
		exists(std::size_t(entity_count), true)
	{
	}

	void clear()
	{
		component_store.clear();
		exists.clear();
	}

	void resize(std::size_t size)
	{
		component_store.resize(size);
		exists.resize(size);
	}

	void add(const id entity, const Component& component)
	{
		const std::size_t index = std::size_t(entity);
		if (index >= component_store.size())
		{
			component_store.resize(index + 1);
			exists.resize(index + 1, false);
		}

		assert(!exists[index] && "Component added to same entity more than once.");

		component_store[index] = component;
		exists[index] = true;
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

		exists[std::size_t(entity)] = false;
	}

	// `entity_count` returns the number of entities tracked by this component store
	std::size_t entity_count() const
	{
		return component_store.size();
	}

	inline bool has(const id entity) const
	{
		const std::size_t index = std::size_t(entity);
		return index < exists.size() && exists[index];
	}

	void complete(const Component& fallback)
	{
		for (std::size_t entity = 0; entity < component_store.size(); ++entity)
		{
			if (!has(entity))
			{
				add(entity, fallback);
			}
		}
	}

	const id entity_for_index(std::size_t component) const
	{
		return id(component);
	}

	const Component& component_for_entity(const id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return component_store[std::size_t(entity)];
	}

	Component& component_for_entity(const id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return component_store[std::size_t(entity)];
	}

	const std::vector<Component>& vector() const
	{
		return component_store;
	}

};

}


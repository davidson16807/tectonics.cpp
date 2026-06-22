#pragma once

#include <cassert>       // assert

#include <optional>      // std::optional
#include <vector>        // std::vector

/*
`OptionalComponents` is an alternate implementation of `CommonComponents` that uses std::optionals
It represents a table of components within an Entity-Component-System ("ECS") pattern
where most entities are likely to have this component type. It stores components using std::optional
inside a std::vector to improve cache locality between components and their existance flags.

`OptionalComponents` is best suited if components are likely to exist for most entities 
and components never need to be cast to std::vector such as when using OpenGL draw calls.
*/

namespace orrery
{

template<typename id, typename Component>
class OptionalComponents
{
	std::vector<std::optional<Component>> components;

public:

	/*
	Create an empty `OptionalComponents<id,Component>` instance.
	*/
	OptionalComponents():
		components()
	{
	}

	/*
	Create an `OptionalComponents<id,Component>` instance from the vector `components_`
	where each component corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	OptionalComponents(const std::vector<Component>& components_):
		components()
	{
		components.reserve(components_.size());
		for (const Component& component : components_)
		{
			components.emplace_back(component);
		}
	}

	/*
	Create an `OptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities.
	*/
	OptionalComponents(const id& entity_count):
		components(std::size_t(entity_count))
	{
	}

	/*
	Create an `OptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities
	and default components dependant on the value of `exists`.
	*/
	OptionalComponents(const id& entity_count, const bool exists):
		components(std::size_t(entity_count))
	{
		if (exists)
		{
			for (std::optional<Component>& component : components)
			{
				component.emplace();
			}
		}
	}

	/*
	Create an `OptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities with a single given component.
	*/
	OptionalComponents(const id& entity_count, const Component component):
		components(std::size_t(entity_count), component)
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

	void add(const id entity, const Component& component)
	{
		const std::size_t index = std::size_t(entity);
		if (index >= components.size())
		{
			components.resize(index + 1);
		}

		assert(!components[index].has_value() && "Component added to same entity more than once.");

		components[index] = component;
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

		components[std::size_t(entity)].reset();
	}

	// `entity_count` returns the number of entities tracked by this component store
	std::size_t entity_count() const
	{
		return components.size();
	}

	// `size` mirrors std::vector-style sizing for callers that traverse entity slots directly.
	std::size_t size() const
	{
		return components.size();
	}

	inline bool has(const id entity) const
	{
		const std::size_t index = std::size_t(entity);
		return index < components.size() && components[index].has_value();
	}

	void complete(const Component& fallback)
	{
		for (std::optional<Component>& component : components)
		{
			if (!component.has_value())
			{
				component = fallback;
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
		return *components[std::size_t(entity)];
	}

	Component& component_for_entity(const id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return *components[std::size_t(entity)];
	}

};

}

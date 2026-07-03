#pragma once

#include <cassert>       // assert

#include <optional>      // std::optional
#include <vector>        // std::vector

/*
`DenseOptionalComponents` is an alternate implementation of `DenseContiguousComponents` that uses std::optionals
It represents a table of components within an Entity-Component-System ("ECS") pattern
where most entities are likely to have this component type. It stores components using std::optional
inside a std::vector to improve cache locality between components and their existance flags.

`DenseOptionalComponents` is best suited if components are likely to exist for most entities 
and components never need to be cast to std::vector such as when using OpenGL draw calls.
*/

namespace orrery
{

template<typename id, typename Component>
class DenseOptionalComponents
{
	std::vector<std::optional<Component>> components;

public:

	/*
	Create an empty `DenseOptionalComponents<id,Component>` instance.
	*/
	DenseOptionalComponents():
		components()
	{
	}

	/*
	Create an `DenseOptionalComponents<id,Component>` instance from the vector `components_`
	where each component corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	DenseOptionalComponents(const std::vector<Component>& components_):
		components()
	{
		components.reserve(components_.size());
		for (const Component& component : components_)
		{
			components.emplace_back(component);
		}
	}

	/*
	Create an `DenseOptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities.
	*/
	DenseOptionalComponents(const id& entity_count):
		components(std::size_t(entity_count))
	{
	}

	/*
	Create an `DenseOptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities
	and default components dependant on the value of `exists`.
	*/
	DenseOptionalComponents(const id& entity_count, const bool exists):
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
	Create an `DenseOptionalComponents<id,Component>` instance
	that has memory preallocated to serve a given number of entities with a single given component.
	*/
	DenseOptionalComponents(const id& entity_count, const Component component):
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
	[[nodiscard]] std::size_t entity_count() const
	{
		return components.size();
	}

	// `size` mirrors std::vector-style sizing for callers that traverse entity slots directly.
	[[nodiscard]] std::size_t size() const
	{
		return components.size();
	}

	[[nodiscard]] inline bool has(const id entity) const
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

	[[nodiscard]] const id entity_for_index(std::size_t index) const
	{
		return id(index);
	}

	[[nodiscard]] const Component& component_for_index(std::size_t index) const
	{
		assert(has(index) && "Retrieving non-existent index.");
		return *components[index];
	}

	[[nodiscard]] Component& component_for_index(std::size_t index)
	{
		assert(has(index) && "Retrieving non-existent index.");
		return *components[index];
	}

	[[nodiscard]] const Component& component_for_entity(const id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return *components[std::size_t(entity)];
	}

	[[nodiscard]] Component& component_for_entity(const id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return *components[std::size_t(entity)];
	}

};

}

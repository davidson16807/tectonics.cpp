#pragma once

#include <cassert>       // assert

#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map

/*
`MixedComponents` represents a table of components within an Entity-Component-System ("ECS") pattern
that alternates between approaches used by `EphemeralComponents` and `EnduringComponents`. 
Upon removal or addition, the vector will behave as `EphemeralComponents` if the entity id in is after a threshold, 
otherwise it will use logic from `EnduringComponents`.
Long lasting components will overtime find themselves toward the start of the container,
where they will be sorted in the same order as the entities they represent.
This will make it more likely for them to appear in the cache upon traversing through components
Components that last only a short while will not be sorted so traversal through components is uncached and slow,
but such components will be inserted and removed in O(1) time.
*/

namespace orrery
{
template<typename id, typename Component>
class MixedComponents
{
	// The packed array of components (of generic type Component)
	std::vector<Component> components;
	// Map from an array index to an entity ID.
	std::vector<id> entity_of_index;
	// Map from an entity ID to an array index.
	std::unordered_map<id, std::size_t> index_of_entity;
	// Total size of valid entries in the array.
	std::size_t size;
	// Boundary between enduring and ephemeral behavior.
	std::size_t threshold;

public:

	/*
	Create an empty `MixedComponents<id,Component>` instance
	with a given threshold.
	*/
	MixedComponents(std::size_t threshold_ = 0):
		components(),
		entity_of_index(),
		index_of_entity(),
		size(0),
		threshold(threshold_)
	{
	}

	/*
	Create a `MixedComponents<id,Component>` instance from the vector `components`
	where each component corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	MixedComponents(const std::vector<Component>& components_, std::size_t threshold_ = 0):
		components(components_),
		entity_of_index(),
		index_of_entity(),
		size(components_.size()),
		threshold(threshold_)
	{
		for (std::size_t i = 0; i < components_.size(); ++i)
		{
			entity_of_index.push_back(id(i));
			index_of_entity[id(i)] = i;
		}
	}

	/*
	Create a `MixedComponents<id,Component>`
	that has memory preallocated to serve a given number of entities.
	*/
	MixedComponents(const id& entity_count, std::size_t threshold_):
		components(),
		entity_of_index(),
		index_of_entity(),
		size(0),
		threshold(threshold_)
	{
		components.reserve(std::size_t(entity_count));
		entity_of_index.reserve(std::size_t(entity_count));
	}

	void set_threshold(std::size_t threshold_)
	{
		threshold = threshold_;
	}

	std::size_t get_threshold() const
	{
		return threshold;
	}

	void add(const id entity, const Component& component)
	{
		assert(index_of_entity.find(entity) == index_of_entity.end() && "Component added to same entity more than once.");

		// Determine where this entity would be inserted to preserve ascending entity order.
		std::size_t insert_index = 0;
		while (insert_index < size && entity_of_index[insert_index] < entity)
		{
			++insert_index;
		}

		// preserve order by shifting elements right.
		if (insert_index < threshold)
		{
			if (components.size() < size + 1)
			{
				components.push_back(component);
				entity_of_index.push_back(entity);
			}
			else
			{
				components[size] = component;
				entity_of_index[size] = entity;
			}

			for (std::size_t i = size; i > insert_index; --i)
			{
				components[i] = components[i - 1];
				entity_of_index[i] = entity_of_index[i - 1];
				index_of_entity[entity_of_index[i]] = i;
			}

			components[insert_index] = component;
			entity_of_index[insert_index] = entity;
			index_of_entity[entity] = insert_index;
			++size;
		}
		// append at end.
		else
		{
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
				components[new_index] = component;
				entity_of_index[new_index] = entity;
			}
		}
	}

	void entity_destroyed(const id entity)
	{
		if (index_of_entity.find(entity) != index_of_entity.end())
		{
			remove(entity);
		}
	}

	void remove(const id entity)
	{
		assert(index_of_entity.find(entity) != index_of_entity.end() && "Removing non-existent component.");

		const std::size_t removed_index = index_of_entity.at(entity);
		const std::size_t last_index = size - 1;

		// preserve order by shifting elements left.
		if (removed_index < threshold)
		{
			for (std::size_t i = removed_index; i < last_index; ++i)
			{
				components[i] = components[i + 1];
				entity_of_index[i] = entity_of_index[i + 1];
				index_of_entity[entity_of_index[i]] = i;
			}

			index_of_entity.erase(entity);
			--size;
		}
		// swap last into removed slot.
		else
		{
			if (removed_index != last_index)
			{
				components[removed_index] = components[last_index];

				id entity_of_last_element = entity_of_index[last_index];
				entity_of_index[removed_index] = entity_of_last_element;
				index_of_entity[entity_of_last_element] = removed_index;
			}

			index_of_entity.erase(entity);
			--size;
		}
	}

	// `entity_count` returns the number of entities tracked by this component store
	std::size_t entity_count() const
	{
		return components.size();
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
		assert(component < size && "Retrieving non-existent component.");
		return entity_of_index.at(component);
	}

	const Component& component_for_entity(id entity) const
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return components.at(index_of_entity.at(entity));
	}

	Component& component_for_entity(id entity)
	{
		assert(has(entity) && "Retrieving non-existent component.");
		return components.at(index_of_entity.at(entity));
	}

	const Component& component_for_index(std::size_t component) const
	{
		assert(component < size && "Retrieving non-existent component.");
		return components[component];
	}

	Component& component_for_index(std::size_t component)
	{
		assert(component < size && "Retrieving non-existent component.");
		return components[component];
	}
};

}
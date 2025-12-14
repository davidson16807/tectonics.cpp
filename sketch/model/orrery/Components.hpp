
#include <vector> // std::vector

/*
`Components` represents a table of components within an Entity-Component-System pattern.
It offers functionality beyond std::vector by managing lookups between component ids and entities,
thereby accelerating checks for systems that operate on entities with several kinds of components.
*/

template<typename id, typename Component>
class Components
{

	// The packed array of components (of generic type Component),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::vector<Component> components;
	// Map from an entity ID to an array index.
	std::unordered_map<id, size_t> index_of_entity;
	// Map from an array index to an entity ID.
	std::unordered_map<size_t, id> entity_of_index;
	// Total size of valid entries in the array.
	size_t size;

public:

	void add(id entity, Component component)
	{
		assert(index_of_entity.find(entity) == index_of_entity.end() && "Component added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t new_index = size;
		index_of_entity[entity] = new_index;
		entity_of_index[new_index] = entity;
		components[new_index] = component;
		++size;
	}

	void entity_destroyed(id entity)
	{
		if (index_of_entity.find(entity) != index_of_entity.end())
		{
			// Remove the entity's component if it existed
			remove(entity);
		}
	}

	void remove(id entity)
	{
		assert(index_of_entity.find(entity) != index_of_entity.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t index_of_removed_entity = index_of_entity[entity];
		size_t index_of_last_element = size - 1;
		components[index_of_removed_entity] = components[index_of_last_element];

		// Update map to point to moved spot
		id entity_of_last_element = entity_of_index[index_of_last_element];
		index_of_entity[entity_of_last_element] = index_of_removed_entity;
		entity_of_index[index_of_removed_entity] = entity_of_last_element;

		index_of_entity.erase(entity);
		entity_of_index.erase(index_of_last_element);

		--size;
	}

	bool has(id entity) const
	{
		return index_of_entity.find(entity) != index_of_entity.end();
	}

	Component& get(id entity) const
	{
		// assert(has(entity) && "Retrieving non-existent component.");
		// Return a reference to the entity's component
		return components[index_of_entity[entity]];
	}

};

// // examples:
// orrery::Components<std::uint32_t, orbit::Universals>;
// orrery::Components<std::uint32_t, track::Beeline>;
// orrery::Components<std::uint32_t, track::Spin>;
// orrery::Components<std::uint32_t, track::Lock>;

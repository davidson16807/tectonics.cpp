
#include <vector> // std::vector

/*
`Components` represents a table of components within an Entity-Component-System pattern.
It offers functionality beyond std::vector by managing lookups between component ids and entities,
thereby accelerating checks for systems that operate on entities with several kinds of components.
*/

template<typename T>
class Components
{

	using Entity = std::uint32_t;

	// The packed array of components (of generic type T),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::vector<T> components;
	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> index_of_entity;
	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> entity_of_index;
	// Total size of valid entries in the array.
	size_t size;

public:

	void add(Entity entity, T component)
	{
		assert(index_of_entity.find(entity) == index_of_entity.end() && "Component added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t new_index = size;
		index_of_entity[entity] = new_index;
		entity_of_index[new_index] = entity;
		components[new_index] = component;
		++size;
	}

	void entity_destroyed(Entity entity)
	{
		if (index_of_entity.find(entity) != index_of_entity.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

	void remove(Entity entity)
	{
		assert(index_of_entity.find(entity) != index_of_entity.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = index_of_entity[entity];
		size_t indexOfLastElement = size - 1;
		components[indexOfRemovedEntity] = components[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = entity_of_index[indexOfLastElement];
		index_of_entity[entityOfLastElement] = indexOfRemovedEntity;
		entity_of_index[indexOfRemovedEntity] = entityOfLastElement;

		index_of_entity.erase(entity);
		entity_of_index.erase(indexOfLastElement);

		--size;
	}

	T& get(Entity entity) const
	{
		assert(index_of_entity.find(entity) != index_of_entity.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return components[index_of_entity[entity]];
	}

};

body::Components<orbit::Universals>;
body::Components<body::Beeline>;
body::Components<body::Spin>;
body::Components<body::Lock>;

// implementation 1:
template<typename scalar>
class OrbitSystem
{
	using mat4 = glm::mat<4,4,scalar,glm::defaultp>;
	void orientation(const orbit::Universals& universals, mat4& result){}
};

// implementation 2:
template<typename scalar>
class OrbitSystem
{
	using mat4 = glm::mat<4,4,scalar,glm::defaultp>;
	void inertial(const orbit::Universals& universals, mat4& result){}
	void inertial(const orbit::Universals& universals, mat4& result){}
};

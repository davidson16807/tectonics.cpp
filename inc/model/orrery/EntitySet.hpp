#pragma once

#include <cassert>       // assert

#include <vector>        // std::vector
#include <unordered_set> // std::unordered_set

/*
`EntitySet` represents a set of entities within an Entity-Component-System ("ECS") pattern
The inclusion in this set implies no further information that might be expressed by a component,
and the set of entities is a sparse subset of the set of all entities.
*/

namespace orrery 
{

template<typename id>
class EntitySet
{

	// Map from an entity ID to an array index.
	std::unordered_set<id> entities;

public:

	/*
	Create an empty `EntitySet<id,Component>` instance 
	*/
	EntitySet():
		entities()
	{
	}

	/*
	Create a `EntitySet<id,Component>` instance from the vector `components`
	where each components corresponds to a unique and newly constructed entity
	whose id is equal to the component's index.
	*/
	EntitySet(const std::vector<id>& entities):
		entities()
	{
		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			entities.insert(entities[i]);
		}
	}

	void add(const id entity)
	{
		entities.insert(entities[i]);
	}

	void add(const EntitySet& entities)
	{
		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			entities.insert(entities[i]);
		}
	}

	void entity_destroyed(const id entity)
	{
		auto found = entities.find(entity);
		if (found != entities.end())
		{
			entities.erase(found);
		}
	}

	void remove(const id entity)
	{
		auto found = entities.find(entity);
		if (found != entities.end())
		{
			entities.erase(found);
		}
	}

	// `entity_count` returns the number of entities tracked by this component store
	[[nodiscard]] std::size_t entity_count() const
	{
		return entities.size();
	}

	[[nodiscard]] bool has(id entity) const
	{
		return entities.find(entity) != entities.end();
	}

};

}

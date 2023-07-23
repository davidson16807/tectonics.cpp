
#include "Mesh.hpp"

namespace collignon 
{

    template<typename id, typename scalar>
	class GridCache{
        using ivec2 = glm::vec<2,id,glm::defaultp>;\
        using vec2 = glm::vec<2,scalar,glm::defaultp>;\
        using vec3 = glm::vec<3,scalar,glm::defaultp>;\

		/*
		*/
		#define COLLIGNON_SERIES(TYPE, NAME, METHOD)
		class NAME \
		{ \
			const Mesh<id,scalar> mesh; \
		public:\
			constexpr inline explicit NAME(const Mesh& mesh) : mesh(mesh) {} \
			constexpr inline NAME(const NAME& a) : mesh(a.mesh) {} \
		    using size_type = std::size_t; \
			using value_type = TYPE; \
			using const_reference = const TYPE&; \
			using reference = TYPE&; \
			constexpr inline std::size_t size() const { return mesh.vertex_count; } \
			constexpr inline const_reference operator[](const size_type memory_id ) const { METHOD; } \
		};

		COLLIGNON_SERIES(ivec2, GridId,             return mesh.grid_id(memory_id))
		COLLIGNON_SERIES(vec2,  GridPosition,       return mesh.grid_position(memory_id))
		COLLIGNON_SERIES(vec3,  UnitSpherePosition, return mesh.unit_sphere_position(memory_id))
		COLLIGNON_SERIES(vec3,  SpherePosition,     return mesh.sphere_position(memory_id))

		#undef COLLIGNON_SERIES

	}


}
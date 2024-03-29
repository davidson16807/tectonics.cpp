#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace collignon
{

	/*
	series.hpp contains classes of indexible objects that wrap the methods of a collignon::Grid.
	with the intent of performantly participating in functions that require out-of-order memory access, 
	such as those under vector_calculus.hpp or morphology.hpp
	*/

	#define COLLIGNON_SERIES(TYPE, TITLE, LOWER, METHOD) \
	template<typename id, typename scalar> \
	struct TITLE \
	{ \
		Grid<id,scalar> grid; \
		constexpr inline explicit TITLE(const Grid<id,scalar> grid): grid(grid) {} \
	    using ivec3 = glm::vec<3,id,glm::defaultp>; \
	    using vec3 = glm::vec<3,scalar,glm::defaultp>; \
	    using mat3 = glm::mat<3,3,scalar,glm::defaultp>; \
	    using size_type = std::size_t; \
		using value_type = TYPE; \
		constexpr inline size_type size() const { return 1; } \
		constexpr inline value_type operator()(const size_type memory_id ) const { return METHOD(memory_id); } \
		constexpr inline value_type operator[](const size_type memory_id ) const { return METHOD(memory_id); } \
	};\
	template<typename id, typename scalar>\
	constexpr inline TITLE<id,scalar> LOWER(const Grid<id,scalar> grid)\
	{\
		return TITLE<id,scalar>(grid);\
	}

	COLLIGNON_SERIES(vec3,   VertexPositions, vertex_positions,  grid.vertex_position)
	COLLIGNON_SERIES(vec3,   VertexNormals,   vertex_normals,    grid.vertex_normal)
	COLLIGNON_SERIES(vec3,   VertexEast,      vertex_east,       grid.vertex_east)
	COLLIGNON_SERIES(vec3,   VertexNorth,     vertex_north,      grid.vertex_north)
	COLLIGNON_SERIES(mat3,   VertexFrame,     vertex_frame,      grid.vertex_frame)
	COLLIGNON_SERIES(scalar, VertexDualAreas, vertex_dual_areas, grid.vertex_dual_area)

	#undef COLLIGNON_SERIES

}


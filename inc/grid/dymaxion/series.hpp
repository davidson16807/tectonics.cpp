#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace dymaxion
{

	/*
	series.hpp contains classes of indexible objects that wrap the methods of a dymaxion::Grid.
	with the intent of performantly participating in functions that require out-of-order memory access, 
	such as those under vector_calculus.hpp or morphology.hpp
	*/

	#define DYMAXION_SERIES(TYPE, SIZE, TITLE, LOWER, METHOD) \
	template<typename id, typename scalar> \
	struct TITLE \
	{ \
		Grid<id,scalar> grid; \
		constexpr inline explicit TITLE(const Grid<id,scalar> grid): grid(grid) {} \
	    using ivec3 = glm::vec<3,id,glm::defaultp>; \
	    using vec3 = glm::vec<3,scalar,glm::defaultp>; \
	    using mat3 = glm::mat<3,3,scalar,glm::defaultp>; \
        using ipoint = Point<id,id>;\
        using point = Point<id,scalar>;\
	    using size_type = std::size_t; \
		using value_type = TYPE; \
		constexpr inline size_type size() const { return SIZE(); } \
		constexpr inline value_type operator()(const size_type memory_id ) const { return METHOD(memory_id); } \
		constexpr inline value_type operator[](const size_type memory_id ) const { return METHOD(memory_id); } \
	};\
	template<typename id, typename scalar>\
	constexpr inline TITLE<id,scalar> LOWER(const Grid<id,scalar> grid)\
	{\
		return TITLE<id,scalar>(grid);\
	}

	DYMAXION_SERIES(id,     grid.vertex_count,  SquareId,           square_ids,           grid.square_id)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexPositions,    vertex_positions,     grid.vertex_position)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexNormals,      vertex_normals,       grid.vertex_normal)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexEast,         vertex_east,          grid.vertex_east)
	DYMAXION_SERIES(vec3,   grid.vertex_count,  VertexNorth,        vertex_north,         grid.vertex_north)
	DYMAXION_SERIES(mat3,   grid.vertex_count,  VertexFrame,        vertex_frame,         grid.vertex_frame)
	DYMAXION_SERIES(scalar, grid.vertex_count,  VertexDualAreas,    vertex_dual_areas,    grid.vertex_dual_area)
	DYMAXION_SERIES(ipoint, grid.vertex_count,  VertexGridIds,      vertex_grid_ids,      grid.voronoi.grid_id)
	#undef DYMAXION_SERIES

}


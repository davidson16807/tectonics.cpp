#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

// 3rd-party libraries
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

// in-house libraries
#include "Grid.hpp"
#include "Point.hpp"

namespace dymaxion
{

	/*
	series.hpp contains classes of indexible objects that wrap the methods of a dymaxion::Grid.
	with the intent of performantly participating in functions that require out-of-order memory access, 
	such as those under vector_calculus.hpp or morphology.hpp
	*/

	#define DYMAXION_SERIES(TYPE, TITLE, LOWER, METHOD) \
	template<typename id, typename id2, typename scalar, glm::qualifier precision=glm::defaultp> \
	struct TITLE \
	{ \
		Grid<id,id2,scalar> grid; \
		constexpr inline explicit TITLE(const Grid<id,id2,scalar> grid): grid(grid) {} \
	    using ivec3 = glm::vec<3,id,precision>; \
	    using vec3 = glm::vec<3,scalar,precision>; \
	    using mat3 = glm::mat<3,3,scalar,precision>; \
        using ipoint = Point<id,id>;\
        using point = Point<id,scalar>;\
	    using size_type = std::size_t; \
		using value_type = TYPE; \
		constexpr inline value_type operator()(const vec3 position ) const { return METHOD(position); } \
	};\
	template<typename id, typename id2, typename scalar>\
	constexpr inline TITLE<id,id2,scalar> LOWER(const Grid<id,id2,scalar> grid)\
	{\
		return TITLE<id,id2,scalar>(grid);\
	}

	DYMAXION_SERIES(id2,     NearestVertexId,    nearest_vertex_id,    grid.nearest_vertex_id)
	#undef DYMAXION_SERIES

}

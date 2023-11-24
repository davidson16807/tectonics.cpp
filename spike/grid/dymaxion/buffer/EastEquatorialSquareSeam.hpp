#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace dymaxion {
namespace buffer {

	/*
	series.hpp contains classes of indexible objects that wrap the methods of a dymaxion::Grid.
	with the intent of performantly participating in functions that require out-of-order memory access, 
	such as those under vector_calculus.hpp or morphology.hpp
	*/

	// #define DYMAXION_SERIES(TYPE, TITLE, LOWER, METHOD)
	template<typename id, typename scalar>
	struct TITLE
	{
		Grid<id,scalar> grid;
		constexpr inline explicit TITLE(const Grid<id,scalar> grid): grid(grid) {}
	    using ivec3 = glm::vec<3,id,glm::defaultp>;
	    using vec3 = glm::vec<3,scalar,glm::defaultp>;
	    using mat3 = glm::mat<3,3,scalar,glm::defaultp>;
	    using size_type = std::size_t;
		using value_type = TYPE;
		constexpr inline size_type size() const { return 1; }
		constexpr inline value_type operator()(const size_type memory_id ) const { return METHOD(memory_id); }
		constexpr inline value_type operator[](const size_type memory_id ) const { return METHOD(memory_id); }
	};
	template<typename id, typename scalar>
	constexpr inline TITLE<id,scalar> LOWER(const Grid<id,scalar> grid)
	{
		return TITLE<id,scalar>(grid);
	}

}


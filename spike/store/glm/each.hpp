#pragma once

// 3rd party libraries
#include <glm/vector_relational.hpp>

// in-house libraries
#include "../each.hpp"

namespace each {

	#define EACH_BINARY_GLM_WRAPPER(NAME) \
	template <glm::length_t L, typename T1, typename T2, glm::qualifier Q> \
		inline auto NAME(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b){ return glm::NAME(a,b); }

	EACH_BINARY_GLM_WRAPPER(distance)
	EACH_BINARY_GLM_WRAPPER(equal)
	EACH_BINARY_GLM_WRAPPER(notEqual)
	EACH_BINARY_GLM_WRAPPER(greaterThan)
	EACH_BINARY_GLM_WRAPPER(lessThan)
	EACH_BINARY_GLM_WRAPPER(greaterThanEqual)
	EACH_BINARY_GLM_WRAPPER(lessThanEqual)

}
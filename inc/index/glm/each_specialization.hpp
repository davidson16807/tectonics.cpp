#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vector_relational.hpp>

// in-house libraries
#include "../each.hpp"

namespace each {

	#define EACH_UNARY_GLM_WRAPPER(NAME) \
	template <glm::length_t L, typename T1, glm::qualifier Q> \
		inline auto NAME(const glm::vec<L,T1,Q> a){ return glm::NAME(a); }

	#define EACH_BINARY_GLM_WRAPPER(NAME) \
	template <glm::length_t L, typename T1, typename T2, glm::qualifier Q> \
		inline auto NAME(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b){ return glm::NAME(a,b); }

	EACH_UNARY_GLM_WRAPPER(abs)
	EACH_UNARY_GLM_WRAPPER(floor)
	EACH_UNARY_GLM_WRAPPER(trunc)
	EACH_UNARY_GLM_WRAPPER(round)
	EACH_UNARY_GLM_WRAPPER(ceil)
	EACH_UNARY_GLM_WRAPPER(isnan)
	EACH_UNARY_GLM_WRAPPER(isinf)
	EACH_UNARY_GLM_WRAPPER(exp)
	EACH_UNARY_GLM_WRAPPER(exp2)
	EACH_UNARY_GLM_WRAPPER(log)
	EACH_UNARY_GLM_WRAPPER(log2)
	// EACH_UNARY_GLM_WRAPPER(log10)
	EACH_UNARY_GLM_WRAPPER(sqrt)
	// EACH_UNARY_GLM_WRAPPER(pow)
	EACH_UNARY_GLM_WRAPPER(sin)
	EACH_UNARY_GLM_WRAPPER(cos)
	EACH_UNARY_GLM_WRAPPER(tan)
	EACH_UNARY_GLM_WRAPPER(asin)
	EACH_UNARY_GLM_WRAPPER(acos)
	EACH_UNARY_GLM_WRAPPER(atan)
	EACH_UNARY_GLM_WRAPPER(asinh)
	EACH_UNARY_GLM_WRAPPER(acosh)
	EACH_UNARY_GLM_WRAPPER(atanh)

	EACH_UNARY_GLM_WRAPPER(sign)
	EACH_UNARY_GLM_WRAPPER(fract)
	EACH_UNARY_GLM_WRAPPER(inversesqrt)
	// EACH_UNARY_GLM_WRAPPER(negate)
	// EACH_UNARY_GLM_WRAPPER(exp10)
	// EACH_UNARY_GLM_WRAPPER(sec)
	// EACH_UNARY_GLM_WRAPPER(csc)
	// EACH_UNARY_GLM_WRAPPER(cot)
	// EACH_UNARY_GLM_WRAPPER(sinh)
	// EACH_UNARY_GLM_WRAPPER(cosh)
	// EACH_UNARY_GLM_WRAPPER(tanh)
	// EACH_UNARY_GLM_WRAPPER(asec)
	// EACH_UNARY_GLM_WRAPPER(acsc)
	// EACH_UNARY_GLM_WRAPPER(acot)
	// EACH_UNARY_GLM_WRAPPER(asech)
	// EACH_UNARY_GLM_WRAPPER(acsch)
	// EACH_UNARY_GLM_WRAPPER(acoth)
	EACH_UNARY_GLM_WRAPPER(radians)
	EACH_UNARY_GLM_WRAPPER(degrees)

	EACH_BINARY_GLM_WRAPPER(min)
	EACH_BINARY_GLM_WRAPPER(max)
	EACH_BINARY_GLM_WRAPPER(pow)
	// EACH_BINARY_GLM_WRAPPER(atan2)

	EACH_BINARY_GLM_WRAPPER(mod)  
	EACH_BINARY_GLM_WRAPPER(equal)
	EACH_BINARY_GLM_WRAPPER(notEqual)
	EACH_BINARY_GLM_WRAPPER(greaterThan)
	EACH_BINARY_GLM_WRAPPER(lessThan)
	EACH_BINARY_GLM_WRAPPER(greaterThanEqual)
	EACH_BINARY_GLM_WRAPPER(lessThanEqual)

	#undef EACH_UNARY_GLM_WRAPPER
	#undef EACH_BINARY_GLM_WRAPPER

}


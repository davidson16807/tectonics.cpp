#pragma once

// 3rd party libraries
#define GLM_ENABLE_EXPERIMENTAL // needed to enable <glm/gtx/component_wise.hpp>
#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtx/component_wise.hpp> 

// in-house libraries

namespace math {

	#define MATH_UNARY_GLM_WRAPPER(NAME) \
	template <glm::length_t L, typename T1, glm::qualifier Q> \
		inline auto NAME(const glm::vec<L,T1,Q> a){ return glm::NAME(a); }

	MATH_UNARY_GLM_WRAPPER(abs)
	MATH_UNARY_GLM_WRAPPER(floor)
	MATH_UNARY_GLM_WRAPPER(trunc)
	MATH_UNARY_GLM_WRAPPER(round)
	MATH_UNARY_GLM_WRAPPER(ceil)
	// MATH_UNARY_GLM_WRAPPER(isnan)
	// MATH_UNARY_GLM_WRAPPER(isinf)
	MATH_UNARY_GLM_WRAPPER(exp)
	MATH_UNARY_GLM_WRAPPER(exp2)
	MATH_UNARY_GLM_WRAPPER(log)
	MATH_UNARY_GLM_WRAPPER(log2)
	// MATH_UNARY_GLM_WRAPPER(log10)
	MATH_UNARY_GLM_WRAPPER(sqrt)
	// MATH_UNARY_GLM_WRAPPER(pow)
	MATH_UNARY_GLM_WRAPPER(sin)
	MATH_UNARY_GLM_WRAPPER(cos)
	MATH_UNARY_GLM_WRAPPER(tan)
	MATH_UNARY_GLM_WRAPPER(asin)
	MATH_UNARY_GLM_WRAPPER(acos)
	MATH_UNARY_GLM_WRAPPER(atan)
	MATH_UNARY_GLM_WRAPPER(asinh)
	MATH_UNARY_GLM_WRAPPER(acosh)
	MATH_UNARY_GLM_WRAPPER(atanh)

	MATH_UNARY_GLM_WRAPPER(sign)
	MATH_UNARY_GLM_WRAPPER(fract)
	MATH_UNARY_GLM_WRAPPER(inversesqrt)
	// MATH_UNARY_GLM_WRAPPER(negate)
	// MATH_UNARY_GLM_WRAPPER(exp10)
	// MATH_UNARY_GLM_WRAPPER(sec)
	// MATH_UNARY_GLM_WRAPPER(csc)
	// MATH_UNARY_GLM_WRAPPER(cot)
	// MATH_UNARY_GLM_WRAPPER(sinh)
	// MATH_UNARY_GLM_WRAPPER(cosh)
	// MATH_UNARY_GLM_WRAPPER(tanh)
	// MATH_UNARY_GLM_WRAPPER(asec)
	// MATH_UNARY_GLM_WRAPPER(acsc)
	// MATH_UNARY_GLM_WRAPPER(acot)
	// MATH_UNARY_GLM_WRAPPER(asech)
	// MATH_UNARY_GLM_WRAPPER(acsch)
	// MATH_UNARY_GLM_WRAPPER(acoth)
	MATH_UNARY_GLM_WRAPPER(radians)
	MATH_UNARY_GLM_WRAPPER(degrees)

	#undef MATH_UNARY_GLM_WRAPPER

	#define MATH_BINARY_GLM_WRAPPER(NAME) \
	template <glm::length_t L, typename T1, typename T2, glm::qualifier Q> \
		inline auto NAME(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b){ return glm::NAME(a,b); }

	MATH_BINARY_GLM_WRAPPER(min)
	MATH_BINARY_GLM_WRAPPER(max)
	MATH_BINARY_GLM_WRAPPER(pow)
	// MATH_BINARY_GLM_WRAPPER(atan2)

	MATH_BINARY_GLM_WRAPPER(distance)
	MATH_BINARY_GLM_WRAPPER(mod)  
	MATH_BINARY_GLM_WRAPPER(step)
	MATH_BINARY_GLM_WRAPPER(equal)
	MATH_BINARY_GLM_WRAPPER(notEqual)
	MATH_BINARY_GLM_WRAPPER(greaterThan)
	MATH_BINARY_GLM_WRAPPER(lessThan)
	MATH_BINARY_GLM_WRAPPER(greaterThanEqual)
	MATH_BINARY_GLM_WRAPPER(lessThanEqual)

	#undef MATH_BINARY_GLM_WRAPPER

	template<typename glm::length_t L, typename T1, typename T2, typename T3, glm::qualifier Q>
	inline auto smoothstep(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b, const T3 c){ return glm::smoothstep(a,b,c); }
	template<typename glm::length_t L, typename T1, typename T2, typename T3, glm::qualifier Q>
	inline auto smoothstep(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b, const T3 c){ return glm::smoothstep(a,b,c); }
	template<typename glm::length_t L, typename T1, typename T2, typename T3, glm::qualifier Q>
	inline auto fma(const glm::vec<L,T1,Q> a, const glm::vec<L,T2,Q> b, const glm::vec<L,T3,Q> c){ return glm::fma(a,b,c); }

	template<glm::length_t L, typename T, glm::qualifier Q>
	inline constexpr bool isinf(const glm::vec<L,T,Q> V){
		return glm::any(glm::isinf(V));
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	inline constexpr bool isnan(const glm::vec<L,T,Q> V){
		return glm::any(glm::isnan(V));
	}

}


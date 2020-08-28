#pragma once

#include <cmath>

#include <glm/common.hpp>	// all the GLSL common functions: abs, floor, etc.

#include "../types.hpp"

namespace many
{

	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> abs(const glm::vec<L,T,Q> a)
	{
		return glm::abs(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> sign(const glm::vec<L,T,Q> a)
	{
		return glm::sign(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> floor(const glm::vec<L,T,Q> a)
	{
		return glm::floor(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	glm::vec<L,T,Q> trunc(const glm::vec<L,T,Q> a)
	{
		return glm::trunc(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> round(const glm::vec<L,T,Q> a)
	{
		return glm::round(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> ceil(const glm::vec<L,T,Q> a)
	{
		return glm::ceil(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> fract(const glm::vec<L,T,Q> a)
	{
		return glm::fract(a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> mod(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::mod(a,b);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> max(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::max(a,b);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> mix(const glm::vec<L,T,Q> x, const glm::vec<L,T,Q> y, const glm::vec<L,T,Q> a)
	{
		return glm::mix(x,y,a);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> clamp(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi)
	{
		return glm::clamp(a,lo,hi);
	}
	// template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	// inline glm::vec<L,T,Q> step(const glm::vec<L,T,Q> edge, const glm::vec<L,T,Q> x, const glm::vec<L,T,Q> out)
	// {
	// 	return x < edge? T(0) : T(1);
	// }
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> smoothstep(const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi, const glm::vec<L,T,Q> x)
	{
		return glm::smoothstep(lo,hi,x); 
	}
	// template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	// inline glm::vec<L,T,Q> linearstep(const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi, const glm::vec<L,T,Q> x)
	// {
	// 	return x<=lo? T(0) : x >= hi? T(1) : ((x-lo)/(hi-lo));
	// }
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline void isnan(const glm::vec<L,T,Q>  x)
	{
		return glm::isnan(x);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline void isinf(const glm::vec<L,T,Q>  x)
	{
		return glm::isinf(x);
	}
}//namespace many




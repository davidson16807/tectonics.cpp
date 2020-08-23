#pragma once

#include <cmath>

#include <many/types.hpp>
#include "Layers.hpp"
#include "convenience.hpp"

namespace layers
{
	
	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template<std::size_t N, glm::length_t L, typename T, glm::qualifier Q>
	inline Layers<glm::vec<L,T,Q>,N>& operator+=(Layers<glm::vec<L,T,Q>,N>& a, const T b) 
	{
		layers::add(a, b, a);
		return a;
	}
	template<std::size_t N, glm::length_t L, typename T, glm::qualifier Q>
	inline Layers<glm::vec<L,T,Q>,N>& operator-=(Layers<glm::vec<L,T,Q>,N>& a, const T b) 
	{
		layers::sub(a, b, a);
		return a;
	}
	template<std::size_t N, glm::length_t L, typename T, glm::qualifier Q>
	inline Layers<glm::vec<L,T,Q>,N>& operator*=(Layers<glm::vec<L,T,Q>,N>& a, const T b) 
	{
		layers::mult(a, b, a);
		return a;
	}
	template<std::size_t N, glm::length_t L, typename T, glm::qualifier Q>
	inline Layers<glm::vec<L,T,Q>,N>& operator/=(Layers<glm::vec<L,T,Q>,N>& a, const T b) 
	{
		layers::div(a, b, a);
		return a;
	}
	
}
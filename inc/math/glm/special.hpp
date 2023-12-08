#pragma once

// std libraries
#include <cmath>

// 3rd party libraries
#define GLM_ENABLE_EXPERIMENTAL // needed to enable <glm/gtx/component_wise.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/component_wise.hpp> 

namespace math{

	template<typename T, glm::qualifier Q>
	inline constexpr T trace(const glm::mat<2,2,T,Q> A)
	{
	    return A[0][0] + A[1][1];
	}

	template<typename T, glm::qualifier Q>
	inline constexpr T trace(const glm::mat<3,3,T,Q> A)
	{
	    return A[0][0] + A[1][1] + A[2][2];
	}

	template<typename T, glm::qualifier Q>
	inline constexpr T trace(const glm::mat<4,4,T,Q> A)
	{
	    return A[0][0] + A[1][1] + A[2][2] + A[3][3];
	}

	// "permutation" (aka "levi-cevita") functions

	template<typename T, glm::qualifier Q>
	inline constexpr T permutation(const glm::mat<2,2,T,Q> A)
	{
	    return glm::mat<2,2,T,Q>(0,-1,1,0) * A;
	}

	template<typename T, glm::qualifier Q>
	inline constexpr T permutation(const glm::mat<3,3,T,Q> A)
	{
	    return glm::vec<3,T,Q>(
		    glm::dot(
		    	glm::vec<3,T,Q>(1,1,1), 
		    	glm::mat<3,3,T,Q>(0,0,0,0,0,-1,0,1,0) * A
		    ),
		    glm::dot(
		    	glm::vec<3,T,Q>(1,1,1), 
		    	glm::mat<3,3,T,Q>(0,0,1,0,0,0,-1,0,0) * A
		    ),
		    glm::dot(
		    	glm::vec<3,T,Q>(1,1,1), 
		    	glm::mat<3,3,T,Q>(0,-1,0,1,0,0,0,0,0) * A
		    )
    	);
	}

	template<int L, typename T, glm::qualifier Q>
	inline constexpr T compMaxAbs(glm::vec<L,T,Q> V)
	{
	    return std::max(-glm::compMin(V), glm::compMax(V));
	}

	template<int L, typename T, glm::qualifier Q>
	inline constexpr auto distance(const glm::vec<L,T,Q> U, const glm::vec<L,T,Q> V){
		return glm::distance(U,V);
	}

	template<int L, typename T, glm::qualifier Q>
	inline constexpr bool isinf(const glm::vec<L,T,Q> V){
		return glm::any(glm::isinf(V));
	}

	template<int L, typename T, glm::qualifier Q>
	inline constexpr bool isnan(const glm::vec<L,T,Q> V){
		return glm::any(glm::isnan(V));
	}

}


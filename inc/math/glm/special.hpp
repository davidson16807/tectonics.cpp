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

	// https://en.wikipedia.org/wiki/Cosine_similarity
	template<int L, typename T, glm::qualifier Q>
	inline constexpr T similarity(const glm::vec<L,T,Q> U, const glm::vec<L,T,Q> V)
	{
	    return glm::dot(U,V) / (glm::length(U)*glm::length(V));
	}

	template<int L, typename T, glm::qualifier Q>
	inline constexpr T compMaxAbs(const glm::vec<L,T,Q> V)
	{
		T result(V[0]);
		for (int i = 1; i < L; ++i)
		{
			result = std::abs(V[i]) > std::abs(result)? V[i] : result;
		}
	    return result;
	}


}


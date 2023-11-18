#pragma once

// std libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2.hpp>
#include <glm/mat3.hpp>
#include <glm/mat4.hpp>

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

}


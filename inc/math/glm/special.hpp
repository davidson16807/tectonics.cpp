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

	/*
	`trace` returns the trace of a matrix, i.e. the sum of its diagonal components.
	*/

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

	/*
	`permutation` returns the permutation of a matrix, also known as the "levi-cevita" operation.
	It can be considered the operation that generalizes cross products to arbitrary dimensions.
	Currently, `permutation` is only implemented for 2d and 3d.
	*/

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

	/*
	`similarity` returns the cosine similarity of two vectors.
	See https://en.wikipedia.org/wiki/Cosine_similarity.
	*/
	template<int L, typename T, glm::qualifier Q>
	inline constexpr T similarity(const glm::vec<L,T,Q> U, const glm::vec<L,T,Q> V)
	{
	    return glm::dot(U,V) / (glm::length(U)*glm::length(V));
	}

	template <int L, typename T, glm::qualifier Q>
	inline constexpr auto scalar_projection (const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::dot(a, glm::normalize(b));
	}

	template <int L, typename T, glm::qualifier Q>
	inline constexpr auto vector_projection (const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::dot(a, glm::normalize(b)) * b;
	}

	template <int L, typename T, glm::qualifier Q>
	inline constexpr auto scalar_rejection (const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return glm::length(a - glm::dot(a, glm::normalize(b)) * b);
	}

	template <int L, typename T, glm::qualifier Q>
	inline constexpr auto vector_rejection (const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b)
	{
		return a - glm::dot(a, glm::normalize(b)) * b;
	}

	/*
	`compMaxAbs` returns the component of a vector whose maximum absolute value is largest.
	It is meant to be similar in nature to other component-wise operations in glm, e.g. `compMax`, `compAdd`, etc.
	*/
	template<typename T, glm::qualifier Q>
	inline constexpr T compMaxAbs(const glm::vec<2,T,Q> V)
	{
		return   std::abs(V[1]) > std::abs(V[0])? V[1] : V[0];
	}

	template<typename T, glm::qualifier Q>
	inline constexpr T compMaxAbs(const glm::vec<3,T,Q> V)
	{
		T result(V[0]);
		result = std::abs(V[1]) > std::abs(result)? V[1] : result;
		return   std::abs(V[2]) > std::abs(result)? V[2] : result;
	}

	template<typename T, glm::qualifier Q>
	inline constexpr T compMaxAbs(const glm::vec<4,T,Q> V)
	{
		T result(V[0]);
		result = std::abs(V[1]) > std::abs(result)? V[1] : result;
		result = std::abs(V[2]) > std::abs(result)? V[2] : result;
		return   std::abs(V[3]) > std::abs(result)? V[3] : result;
	}

}


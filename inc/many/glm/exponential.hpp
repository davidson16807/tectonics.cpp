#pragma once

#include <glm/exponential.hpp>// all the GLSL geometry functions: dot, cross, reflect, etc.

#include "../types.hpp"
#include "../exponential.hpp"

namespace many
{
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> pow(const glm::vec<L,T,Q> basei, const glm::vec<L,T,Q> exponenti)
	{
		return glm::pow(basei, exponenti);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> pow(const glm::vec<L,T,Q> basei, T exponenti)
	{
		return glm::pow(basei, exponenti);
	}
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> pow(const T basei, const glm::vec<L,T,Q> exponenti)
	{
		return glm::pow(basei, exponenti);
	}
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> exp(const glm::vec<L,T,Q> ai)
	{
		return glm::exp(ai);
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> log(const glm::vec<L,T,Q> ai)
	{
		return glm::log(ai);
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> exp2(const glm::vec<L,T,Q> ai)
	{
		return glm::exp2(ai);
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> log2(const glm::vec<L,T,Q> ai)
	{
		return glm::log2(ai);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> sqrt(const glm::vec<L,T,Q> ai)
	{
		return glm::sqrt(ai);
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline glm::vec<L,T,Q> inversesqrt(const glm::vec<L,T,Q> ai)
	{
		return T(1)/glm::sqrt(ai);
	}
}//namespace many


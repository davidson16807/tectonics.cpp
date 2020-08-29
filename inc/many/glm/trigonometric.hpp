/// Function parameters specified as angle are assumed to be in units of radians.
/// In no case will any of these functions result in a divide by zero error. If
/// the divisor of a ratio is 0, then results will be undefined.
///
/// These all operate component-wise. The description is per component.

#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T sin(const T a)
	{
		return glm::sin(a);
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T cos(const T a)
	{
		return glm::cos(a);
	}

	/// The standard trigonometric tangent function.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T tan(const T a)
	{
		return glm::tan(a);
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T asin(const T a)
	{
		return glm::asin(a);
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T acos(const T a)
	{
		return glm::acos(a);
	}

	/// Arc tangent. Returns an angle whose tangent is y_over_x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T atan(const T a)
	{
		return glm::atan(a);
	}


	/// Arc tangent. Returns an angle whose tangent is y/x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T atan(const T a)
	{
		return glm::atan(a);
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T sinh(const T a)
	{
		return glm::sinh(a);
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T cosh(const T a)
	{
		return glm::cosh(a);
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T tanh(const T a)
	{
		return glm::tanh(a);
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T asinh(const T a)
	{
		return glm::asinh(a);
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T acosh(const T a)
	{
		return glm::acosh(a);
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	inline T atanh(const T a)
	{
		return glm::atanh(a);
	}

}//namespace many


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

	/// Converts degrees to radians and returns the result.
	template <typename T, typename Tout>
	void radians(const T& degrees, Tout& out)
	{
		typename T::value_type conversion_factor(M_PI/180.);
		out.store(degrees, [conversion_factor](T degreesi){ return conversion_factor * degreesi; });
	}

	/// Converts degrees to radians and returns the result.
	template <typename T, typename Tout>
	void degrees(const T& radians, Tout& out)
	{
		typename T::value_type conversion_factor(180./M_PI);
		out.store(radians, [conversion_factor](T radiansi){ return conversion_factor * radiansi; });
	}

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T, typename Tout>
	void sin(const T& radians, Tout& out)
	{
		out.store(std::sin, radians);
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T, typename Tout>
	void cos(const T& radians, Tout& out)
	{
		out.store(std::cos, radians);
	}

	/// The standard trigonometric tangent function.
	template <typename T, typename Tout>
	void tan(const T& radians, Tout& out)
	{
		out.store(std::tan, radians);
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <typename T, typename Tout>
	void asin(const T& x, Tout& out)
	{
		out.store(std::asin, x);
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <typename T, typename Tout>
	void acos(const T& x, Tout& out)
	{
		out.store(std::acos, x);
	}

	/// Arc tangent. Returns an angle whose tangent is y_over_x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T, typename Tout>
	void atan(const T& y_over_x, Tout& out)
	{
		out.store(std::atan, y_over_x);
	}


	/// Arc tangent. Returns an angle whose tangent is y/x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T, typename Tout>
	void atan(const T& x, const T& y, Tout& out)
	{
		out.store(std::atan2, x, y);
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <typename T, typename Tout>
	void sinh(const T& radians, Tout& out)
	{
		out.store(std::sinh, radians);
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <typename T, typename Tout>
	void cosh(const T& radians, Tout& out)
	{
		out.store(std::cosh, radians);
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <typename T, typename Tout>
	void tanh(const T& radians, Tout& out)
	{
		out.store(std::tanh, radians);
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <typename T, typename Tout>
	void asinh(const T& x, Tout& out)
	{
		out.store(std::asinh, x);
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <typename T, typename Tout>
	void acosh(const T& x, Tout& out)
	{
		out.store(std::acosh, x);
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <typename T, typename Tout>
	void atanh(const T& x, Tout& out)
	{
		out.store(std::atanh, x);
	}

}//namespace many


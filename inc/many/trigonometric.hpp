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
	template <class T>
	void radians(const std::vector<T>& degrees, std::vector<T>& out)
	{
		T conversion_factor(M_PI/180.);
		return apply(degrees, [conversion_factor](T degreesi){ return conversion_factor * degreesi; }, out);
	}

	/// Converts degrees to radians and returns the result.
	template <class T>
	void degrees(const std::vector<T>& radians, std::vector<T>& out)
	{
		T conversion_factor(180./M_PI);
		return apply(radians, [conversion_factor](T radiansi){ return conversion_factor * radiansi; }, out);
	}

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <class T>
	void sin(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::sin, out);
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <class T>
	void cos(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::cos, out);
	}

	/// The standard trigonometric tangent function.
	template <class T>
	void tan(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::tan, out);
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <class T>
	void asin(const std::vector<T>& x, std::vector<T>& out)
	{
		return apply(x, std::asin, out);
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <class T>
	void acos(const std::vector<T>& x, std::vector<T>& out)
	{
		return apply(x, std::acos, out);
	}

	/// Arc tangent. Returns an angle whose tangent is y_over_x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <class T>
	void atan(const std::vector<T>& y_over_x, std::vector<T>& out)
	{
		return apply(y_over_x, std::atan, out);
	}


	/// Arc tangent. Returns an angle whose tangent is y/x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <class T>
	void atan(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& out)
	{
		return apply(x, y, std::atan2, out);
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <class T>
	void sinh(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::sinh, out);
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <class T>
	void cosh(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::cosh, out);
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <class T>
	void tanh(const std::vector<T>& radians, std::vector<T>& out)
	{
		return apply(radians, std::tanh, out);
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <class T>
	void asinh(const std::vector<T>& x, std::vector<T>& out)
	{
		return apply(x, std::asinh, out);
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <class T>
	void acosh(const std::vector<T>& x, std::vector<T>& out)
	{
		return apply(x, std::acosh, out);
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <class T>
	void atanh(const std::vector<T>& x, std::vector<T>& out)
	{
		return apply(x, std::atanh, out);
	}

}//namespace many


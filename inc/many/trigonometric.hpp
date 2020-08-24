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
	template <typename T>
	void radians(const series<T>& degrees, series<T>& out)
	{
		T conversion_factor(M_PI/180.);
		out.store(degrees, [conversion_factor](T degreesi){ return conversion_factor * degreesi; });
	}

	/// Converts degrees to radians and returns the result.
	template <typename T>
	void degrees(const series<T>& radians, series<T>& out)
	{
		T conversion_factor(180./M_PI);
		out.store(radians, [conversion_factor](T radiansi){ return conversion_factor * radiansi; });
	}

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	void sin(const series<T>& radians, series<T>& out)
	{
		out.store(std::sin, radians);
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	void cos(const series<T>& radians, series<T>& out)
	{
		out.store(std::cos, radians);
	}

	/// The standard trigonometric tangent function.
	template <typename T>
	void tan(const series<T>& radians, series<T>& out)
	{
		out.store(std::tan, radians);
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <typename T>
	void asin(const series<T>& x, series<T>& out)
	{
		out.store(std::asin, x);
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <typename T>
	void acos(const series<T>& x, series<T>& out)
	{
		out.store(std::acos, x);
	}

	/// Arc tangent. Returns an angle whose tangent is y_over_x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T>
	void atan(const series<T>& y_over_x, series<T>& out)
	{
		out.store(std::atan, y_over_x);
	}


	/// Arc tangent. Returns an angle whose tangent is y/x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T>
	void atan(const series<T>& x, const series<T>& y, series<T>& out)
	{
		out.store(std::atan2, x, y);
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <typename T>
	void sinh(const series<T>& radians, series<T>& out)
	{
		out.store(std::sinh, radians);
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <typename T>
	void cosh(const series<T>& radians, series<T>& out)
	{
		out.store(std::cosh, radians);
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <typename T>
	void tanh(const series<T>& radians, series<T>& out)
	{
		out.store(std::tanh, radians);
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <typename T>
	void asinh(const series<T>& x, series<T>& out)
	{
		out.store(std::asinh, x);
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <typename T>
	void acosh(const series<T>& x, series<T>& out)
	{
		out.store(std::acosh, x);
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <typename T>
	void atanh(const series<T>& x, series<T>& out)
	{
		out.store(std::atanh, x);
	}

}//namespace many


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
		out.store(degrees, [conversion_factor](glm::vec<L,T,Q> degreesi){ return conversion_factor * degreesi; });
	}

	/// Converts degrees to radians and returns the result.
	template <typename T, typename Tout>
	void degrees(const T& radians, Tout& out)
	{
		typename T::value_type conversion_factor(180./M_PI);
		out.store(radians, [conversion_factor](glm::vec<L,T,Q> radiansi){ return conversion_factor * radiansi; });
	}

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline glm::vec<L,T,Q> sin(const glm::vec<L,T,Q> a)
	{
		return std::sin(a);
	}
	template <typename T, typename Tout>
	void sin(const T& radians, Tout& out)
	{
		out.store(sin, radians);
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline glm::vec<L,T,Q> cos(const glm::vec<L,T,Q> a)
	{
		return std::cos(a);
	}
	template <typename T, typename Tout>
	void cos(const T& radians, Tout& out)
	{
		out.store(std::cos, radians);
	}

	/// The standard trigonometric tangent function.
	template <typename T>
	inline glm::vec<L,T,Q> tan(const glm::vec<L,T,Q> a)
	{
		return std::tan(a);
	}
	template <typename T, typename Tout>
	void tan(const T& radians, Tout& out)
	{
		out.store(std::tan, radians);
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <typename T>
	inline glm::vec<L,T,Q> asin(const glm::vec<L,T,Q> a)
	{
		return std::asin(a);
	}
	template <typename T, typename Tout>
	void asin(const T& x, Tout& out)
	{
		out.store(std::asin, x);
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <typename T>
	inline glm::vec<L,T,Q> acos(const glm::vec<L,T,Q> a)
	{
		return std::acos(a);
	}
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
	template <typename T>
	inline glm::vec<L,T,Q> atan(const glm::vec<L,T,Q> a)
	{
		return std::atan(a);
	}
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
	template <typename T>
	inline glm::vec<L,T,Q> atan(const glm::vec<L,T,Q> a)
	{
		return std::atan(a);
	}
	template <typename T, typename Tout>
	void atan(const T& x, const T& y, Tout& out)
	{
		out.store(std::atan2, x, y);
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <typename T>
	inline glm::vec<L,T,Q> sinh(const glm::vec<L,T,Q> a)
	{
		return std::sinh(a);
	}
	template <typename T, typename Tout>
	void sinh(const T& radians, Tout& out)
	{
		out.store(std::sinh, radians);
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <typename T>
	inline glm::vec<L,T,Q> cosh(const glm::vec<L,T,Q> a)
	{
		return std::cosh(a);
	}
	template <typename T, typename Tout>
	void cosh(const T& radians, Tout& out)
	{
		out.store(std::cosh, radians);
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <typename T>
	inline glm::vec<L,T,Q> tanh(const glm::vec<L,T,Q> a)
	{
		return std::tanh(a);
	}
	template <typename T, typename Tout>
	void tanh(const T& radians, Tout& out)
	{
		out.store(std::tanh, radians);
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <typename T>
	inline glm::vec<L,T,Q> asinh(const glm::vec<L,T,Q> a)
	{
		return std::asinh(a);
	}
	template <typename T, typename Tout>
	void asinh(const T& x, Tout& out)
	{
		out.store(std::asinh, x);
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <typename T>
	inline glm::vec<L,T,Q> acosh(const glm::vec<L,T,Q> a)
	{
		return std::acosh(a);
	}
	template <typename T, typename Tout>
	void acosh(const T& x, Tout& out)
	{
		out.store(std::acosh, x);
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <typename T>
	inline glm::vec<L,T,Q> atanh(const glm::vec<L,T,Q> a)
	{
		return std::atanh(a);
	}
	template <typename T, typename Tout>
	void atanh(const T& x, Tout& out)
	{
		out.store(std::atanh, x);
	}

}//namespace many


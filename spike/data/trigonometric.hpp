/// Function parameters specified as angle are assumed to be in units of radians.
/// In no case will any of these functions result in a divide by zero error. If
/// the divisor of a ratio is 0, then results will be undefined.
///
/// These all operate component-wise. The description is per component.

#pragma once

#include <cmath>

#include "compatibility.hpp"

namespace data
{

	/// Converts degrees to radians and returns the result.
	template <typename T, typename Tout>
	void radians(const T& degrees, Tout& out)
	{
		typename T::value_type conversion_factor(M_PI/180.);
		assert(compatible(degrees,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = conversion_factor * degrees[i];
		}
	}

	/// Converts degrees to radians and returns the result.
	template <typename T, typename Tout>
	void degrees(const T& radians, Tout& out)
	{
		typename T::value_type conversion_factor(180./M_PI);
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = conversion_factor * radians[i];
		}
	}

	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline T sin(const T a)
	{
		return std::sin(a);
	}
	template <typename T, typename Tout>
	void sin(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = sin(radians[i]);
		}
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline T cos(const T a)
	{
		return std::cos(a);
	}
	template <typename T, typename Tout>
	void cos(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = cos(radians[i]);
		}
	}

	/// The standard trigonometric tangent function.
	template <typename T>
	inline T tan(const T a)
	{
		return std::tan(a);
	}
	template <typename T, typename Tout>
	void tan(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = tan(radians[i]);
		}
	}


	/// The standard trigonometric sine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline T sec(const T a)
	{
		return T(1)/std::cos(a);
	}
	template <typename T, typename Tout>
	void sec(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = sec(radians[i]);
		}
	}

	/// The standard trigonometric cosine function.
	/// The values returned by this function will range from [-1, 1].
	template <typename T>
	inline T csc(const T a)
	{
		return T(1)/std::sin(a);
	}
	template <typename T, typename Tout>
	void csc(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = csc(radians[i]);
		}
	}

	/// The standard trigonometric tangent function.
	template <typename T>
	inline T cot(const T a)
	{
		return T(1)/std::tan(a);
	}
	template <typename T, typename Tout>
	void cot(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = cot(radians[i]);
		}
	}

	/// Arc sine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [-PI/2, PI/2].
	/// Results are undefined if |x| > 1.
	template <typename T>
	inline T asin(const T a)
	{
		return std::asin(a);
	}
	template <typename T, typename Tout>
	void asin(const T& x, Tout& out)
	{
		assert(compatible(x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = asin(x[i]);
		}
	}

	/// Arc cosine. Returns an angle whose sine is x.
	/// The range of values returned by this function is [0, PI].
	/// Results are undefined if |x| > 1.
	template <typename T>
	inline T acos(const T a)
	{
		return std::acos(a);
	}
	template <typename T, typename Tout>
	void acos(const T& x, Tout& out)
	{
		assert(compatible(x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = acos(x[i]);
		}
	}

	/// Arc tangent. Returns an angle whose tangent is y_over_x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T>
	inline T atan(const T a)
	{
		return std::atan(a);
	}
	template <typename T, typename Tout>
	void atan(const T& y_over_x, Tout& out)
	{
		assert(compatible(y_over_x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = atan(y_over_x[i]);
		}
	}


	/// Arc tangent. Returns an angle whose tangent is y/x.
	/// The signs of x and y are used to determine what
	/// quadrant the angle is in. The range of values returned
	/// by this function is [-PI, PI]. Results are undefined
	/// if x and y are both 0.
	template <typename T>
	inline T atan2(const T y, const T x)
	{
		return std::atan2(y,x);
	}
	template <typename T, typename Tout>
	void atan2(const T& y, const T& x, Tout& out)
	{
		assert(compatible(y,x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = atan2(y[i],x[i]);
		}
	}


	/// Returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	template <typename T>
	inline T sinh(const T a)
	{
		return std::sinh(a);
	}
	template <typename T, typename Tout>
	void sinh(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = sinh(radians[i]);
		}
	}

	/// Returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	template <typename T>
	inline T cosh(const T a)
	{
		return std::cosh(a);
	}
	template <typename T, typename Tout>
	void cosh(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = cosh(radians[i]);
		}
	}

	/// Returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	template <typename T>
	inline T tanh(const T a)
	{
		return std::tanh(a);
	}
	template <typename T, typename Tout>
	void tanh(const T& radians, Tout& out)
	{
		assert(compatible(radians,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = tanh(radians[i]);
		}
	}

	/// Arc hyperbolic sine; returns the inverse of sinh.
	template <typename T>
	inline T asinh(const T a)
	{
		return std::asinh(a);
	}
	template <typename T, typename Tout>
	void asinh(const T& x, Tout& out)
	{
		assert(compatible(x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = asinh(x[i]);
		}
	}

	/// Arc hyperbolic cosine; returns the non-negative inverse
	/// of cosh. Results are undefined if x < 1.
	template <typename T>
	inline T acosh(const T a)
	{
		return std::acosh(a);
	}
	template <typename T, typename Tout>
	void acosh(const T& x, Tout& out)
	{
		assert(compatible(x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = acosh(x[i]);
		}
	}

	/// Arc hyperbolic tangent; returns the inverse of tanh.
	/// Results are undefined if abs(x) >= 1.
	template <typename T>
	inline T atanh(const T a)
	{
		return std::atanh(a);
	}
	template <typename T, typename Tout>
	void atanh(const T& x, Tout& out)
	{
		assert(compatible(x,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = atanh(x[i]);
		}
	}

}//namespace series


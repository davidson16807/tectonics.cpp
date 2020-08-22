#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{
	/// Returns x if x >= 0; otherwise, it returns -x.
	template <typename T>
	void abs(const series<T>& a, series<T>& out)
	{
		many::transform(a, [](T ai){ return ai >= 0? ai : -ai; }, out);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <typename T, typename Tout>
	void sign(const series<T>& a, series<Tout>& out)
	{
		many::transform(a, [](T ai){ return (T(0) < ai) - (ai < T(0)); }, out);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <typename T>
	void floor(const series<T>& a, series<T>& out)
	{
		many::transform(a, std::floor, out);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <typename T>
	void trunc(const series<T>& a, series<T>& out)
	{
		many::transform(a, std::trunc, out);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <typename T>
	void round(const series<T>& a, series<T>& out)
	{
		many::transform(a, std::round, out);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <typename T>
	void ceil(const series<T>& a, series<T>& out)
	{
		many::transform(a, std::ceil, out);
	}

	/// Return x - floor(x).
	template <typename T>
	void fract(const series<T>& a, series<T>& out)
	{
		many::transform(a, [](T ai){ return ai - std::floor(ai); }, out);
	}

	/// Modulus. Returns x - y * floor(x / y)
	/// for each component in x using the floating point value y.
	template <typename T>
	void mod(const series<T>& a, const series<T>& b, series<T>& out)
	{
		many::transform(a, b, [](T ai, T bi){ return ai - bi * std::floor(ai / bi); }, out);
	}

	/// Returns the fractional part of x and sets i to the integer
	/// part (as a whole number floating point value). Both the
	/// return value and the output parameter will have the same
	/// sign as x.
	template <typename T>
	void modf(const series<T>& a, series<int>& intout, series<T>& fractout)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			fractout[i] = a[i] - std::floor(a[i]);
			intout[i] = int(a[i]-fractout[i]);
		}
	}

	/// Returns y if y < x; otherwise, it returns x.
	template <typename T>
	void min(const series<T>& a, const series<T>& b, series<T>& out)
	{
		many::transform(a, b, [](T ai, T bi){ return ai < bi? ai : bi; }, out);
	}
	template <typename T>
	void min(const series<T>& a, const T b, series<T>& out)
	{
		many::transform(a, b, [](T ai, T bi){ return ai < bi? ai : bi; }, out);
	}

	// component-wise min
	template <typename T>
	T min(const series<T>& a, const bool no_nan = true, const bool no_inf = true)
	{
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
		}
		T out = a[0];
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			if (no_nan && std::isnan(a[i]))
			{
				continue;
			}
			if (no_inf && std::isinf(a[i]))
			{
				continue;
			}
			out = a[i] < out? a[i] : out;
		}
		return out;
	}

	/// Returns y if x < y; otherwise, it returns x.
	template <typename T>
	void max(const series<T>& a, const series<T>& b, series<T>& out)
	{
		many::transform(a, b, [](T ai, T bi){ return ai > bi? ai : bi; }, out);
	}
	template <typename T>
	void max(const series<T>& a, const T b, series<T>& out)
	{
		many::transform(a, b, [](T ai, T bi){ return ai > bi? ai : bi; }, out);
	}
	// component-wise max
	template <typename T>
	T max(const series<T>& a, const bool no_nan = true, const bool no_inf = true)
	{
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
		}
		T out = a[0];
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			if (no_nan && std::isnan(a[i]))
			{
				continue;
			}
			if (no_inf && std::isinf(a[i]))
			{
				continue;
			}
			out = a[i] > out? a[i] : out;
		}
		return out;
	}

	/// Returns min(max(x, minVal), maxVal) for each component in x
	/// using the floating-point values minVal and maxVal.
	template <typename T>
	T clamp(const T a, const T lo, const T hi, series<T>& out)
	{
		return a > hi? hi : a < lo? lo : a;
	}
	template <typename T>
	void clamp(const series<T>& a, const T lo, const T hi, series<T>& out)
	{
		many::transform(a, lo, hi, [](T ai, T loi, T hii){ return ai > hii? hii : ai < loi? loi : ai; }, out);
	}
	template <typename T>
	void clamp(const series<T>& a, const T lo, const series<T>& hi, series<T>& out)
	{
		many::transform(a, lo, hi, [](T ai, T loi, T hii){ return ai > hii? hii : ai < loi? loi : ai; }, out);
	}
	template <typename T>
	void clamp(const series<T>& a, const series<T>& lo, const T hi, series<T>& out)
	{
		many::transform(a, lo, hi, [](T ai, T loi, T hii){ return ai > hii? hii : ai < loi? loi : ai; }, out);
	}
	template <typename T>
	void clamp(const series<T>& a, const series<T>& lo, const series<T>& hi, series<T>& out)
	{
		many::transform(a,   hi, [](T ai, T hii){ return ai > hii? hii : ai; }, out);
		many::transform(out, lo, [](T ai, T loi){ return ai < loi? loi : ai; }, out);
	}


	/// If genTypeU is a floating scalar or vector:
	/// Returns x * (1.0 - a) + y * a, i.e., the linear blend of
	/// x and y using the floating-point value a.
	/// The value for a is not restricted to the range [0, 1].
	///
	/// NOTE: should probably implement this:
	/// If genTypeU is a boolean scalar or vector:
	/// Selects which vector each returned component comes
	/// from. For a component of 'a' that is false, the
	/// corresponding component of 'x' is returned. For a
	/// component of 'a' that is true, the corresponding
	/// component of 'y' is returned. Components of 'x' and 'y' that
	/// are not selected are allowed to be invalid floating point
	/// values and will have no effect on the results. Thus, this
	/// provides different functionality than
	/// genType mix(genType x, genType y, genType(a))
	/// where a is a Boolean vector.
	///
	/// @param[in]  x Value to interpolate.
	/// @param[in]  y Value to interpolate.
	/// @param[in]  a Interpolant.
	template <typename T>
	void mix(const series<T>& x, const series<T>& y, const series<T>& a, series<T>& out)
	{
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] = x[i] * (1.0 - a[i]);
		}
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] += y[i] * a[i];
		}
	}
	template <typename T>
	void mix(const series<T>& x, const series<T>& y, const T a, series<T>& out)
	{
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] = x[i] * (1.0 - a);
		}
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] += y[i] * a;
		}
	}
	template <typename T>
	void mix(const series<T>& x, const T y, const series<T>& a, series<T>& out)
	{
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] = x[i] * (1.0 - a[i]);
		}
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] += y * a[i];
		}
	}
	template <typename T>
	void mix(const series<T>& x, const T y, const T a, series<T>& out)
	{
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] = x[i] * (1.0 - a);
		}
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			out[i] += y * a;
		}
	}
	template <typename T>
	void mix(const T x, const series<T>& y, const series<T>& a, series<T>& out)
	{
		for (unsigned int i = 0; i < y.size(); ++i)
		{
			out[i] = x * (1.0 - a[i]);
		}
		for (unsigned int i = 0; i < y.size(); ++i)
		{
			out[i] += y[i] * a[i];
		}
	}
	template <typename T>
	void mix(const T x, const series<T>& y, const T a, series<T>& out)
	{
		for (unsigned int i = 0; i < y.size(); ++i)
		{
			out[i] = x * (1.0 - a);
		}
		for (unsigned int i = 0; i < y.size(); ++i)
		{
			out[i] += y[i] * a;
		}
	}
	template <typename T>
	void mix(const T x, const T y, const series<T>& a, series<T>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = x * (1.0 - a[i]);
		}
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] += y * a[i];
		}
	}
	template <typename T>
	T mix(const T x, const T y, const T a)
	{
		return x*(1.0-a) + y*a;
	}

	/// Returns 0.0 if x < edge, otherwise it returns 1.0 for each component of a genType.
	template <typename T>
	void step(const series<T>& edge, const series<T>&  x, series<T>& out)
	{
		many::transform(edge, x, [](T edgei, T xi){ return xi < edgei? T(0) : T(1); }, out); 
	}
	template <typename T>
	void step(const series<T>&  edge, const T x, series<T>& out)
	{
		many::transform(edge, x, [](T edgei, T xi){ return xi < edgei? T(0) : T(1); }, out); 
	}
	template <typename T>
	void step(const T edge, const series<T>&  x, series<T>& out)
	{
		many::transform(edge, x, [](T edgei, T xi){ return xi < edgei? T(0) : T(1); }, out); 
	}

	/// Returns 0.0 if x <= lo and 1.0 if x >= hi and
	/// performs smooth Hermite interpolation between 0 and 1
	/// when lo < x < hi. This is useful in cases where
	/// you would want a threshold function with a smooth
	/// transition. This is equivalent to:
	/// genType t;
	/// t = clamp ((x - lo) / (hi - lo), 0, 1);
	/// return t * t * (3 - 2 * t);
	/// Results are undefined if lo >= hi.
	template <typename T>
	void smoothstep(const series<T>& lo, const series<T>& hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const T lo, const series<T>& hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const series<T>& lo, T hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const T lo, const T hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const series<T>& lo, const series<T>& hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const T lo, const series<T>& hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	void smoothstep(const series<T>& lo, const T hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ T t = xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); return t*t*(T(3)-T(2)*t); }, out); 
	}
	template <typename T>
	T smoothstep(const T lo, const T hi, const T x)
	{
		T t = x<=lo? T(0) : x >= hi? T(1) : ((x-lo)/(hi-lo)); 
		return t*t*(T(3)-T(2)*t); 
	}


	/// Returns 0.0 if x <= lo and 1.0 if x >= hi and
	/// performs smooth Hermite interpolation between 0 and 1
	/// when lo < x < hi. This is useful in cases where
	/// you would want a threshold function with a smooth
	/// transition. This is equivalent to:
	/// genType t;
	/// t = clamp ((x - lo) / (hi - lo), 0, 1);
	/// return t * t * (3 - 2 * t);
	/// Results are undefined if lo >= hi.
	template <typename T>
	void linearstep(const series<T>& lo, const series<T>& hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const T lo, const series<T>& hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const series<T>& lo, T hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const T lo, const T hi, const series<T>& x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const series<T>& lo, const series<T>& hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const T lo, const series<T>& hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	void linearstep(const series<T>& lo, const T hi, const T x, series<T>& out)
	{
		many::transform(x, lo, hi, [](T xi, T loi, T hii){ return xi<=loi? T(0) : xi >= hii? T(1) : ((xi-loi)/(hii-loi)); }, out); 
	}
	template <typename T>
	T linearstep(const T lo, const T hi, const T x)
	{
		return x<=lo? T(0) : x >= hi? T(1) : ((x-lo)/(hi-lo)); 
	}

	/// Returns true if x holds a NaN (not a number)
	/// representation in the underlying implementation's set of
	/// floating point representations. Returns false otherwise,
	/// including for implementations with no NaN
	/// representations.
	template <typename T>
	void isnan(const series<T>&  x, series<bool>& out)
	{
		many::transform(x, std::isnan, out);
	}

	/// Returns true if x holds a positive infinity or negative
	/// infinity representation in the underlying implementation's
	/// set of floating point representations. Returns false
	/// otherwise, including for implementations with no infinity
	/// representations.
	template <typename T>
	void isinf(const series<T>&  x, series<bool>& out)
	{
		many::transform(x, std::isinf, out);
	}

	/// Computes and returns a * b + c.
	template <typename T>
	void fma(const series<T>& a, const series<T>& b, const series<T>& c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const T a, const series<T>& b, const series<T>& c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const series<T>& a, T b, const series<T>& c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const T a, const T b, const series<T>& c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const series<T>& a, const series<T>& b, const T c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const T a, const series<T>& b, const T c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}
	template <typename T>
	void fma(const series<T>& a, const T b, const T c, series<T>& out)
	{
		many::transform(a, b, c, [](T ai, T bi, T ci){ return ai*bi+ci; }, out); 
	}

//	/// Returns a signed integer value representing
//	/// the encoding of a floating-point value. The floating-point
//	/// value's bit-level representation is preserved.
//	int floatBitsToInt(float const& v);
//
//	/// Returns a signed integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	template <length_t L, qualifier Q>
//	vec<L, int, Q> floatBitsToInt(vec<L, float, Q> const& v);
//
//	/// Returns a unsigned integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	uint floatBitsToUint(float const& v);
//
//	/// Returns a unsigned integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	template <length_t L, qualifier Q>
//	vec<L, uint, Q> floatBitsToUint(vec<L, float, Q> const& v);
//
//	/// Returns a floating-point value corresponding to a signed
//	/// integer encoding of a floating-point value.
//	/// If an inf or NaN is passed in, it will not signal, and the
//	/// resulting floating point value is unspecified. Otherwise,
//	/// the bit-level representation is preserved.
//	float intBitsToFloat(int const& v);
//
//	/// Returns a floating-point value corresponding to a signed
//	/// integer encoding of a floating-point value.
//	/// If an inf or NaN is passed in, it will not signal, and the
//	/// resulting floating point value is unspecified. Otherwise,
//	/// the bit-level representation is preserved.
//	template <length_t L, qualifier Q>
//	vec<L, float, Q> intBitsToFloat(vec<L, int, Q> const& v);
//
//	/// Returns a floating-point value corresponding to a
//	/// unsigned integer encoding of a floating-point value.
//	/// If an inf or NaN is passed in, it will not signal, and the
//	/// resulting floating point value is unspecified. Otherwise,
//	/// the bit-level representation is preserved.
//	float uintBitsToFloat(uint const& v);
//
//	/// Returns a floating-point value corresponding to a
//	/// unsigned integer encoding of a floating-point value.
//	/// If an inf or NaN is passed in, it will not signal, and the
//	/// resulting floating point value is unspecified. Otherwise,
//	/// the bit-level representation is preserved.
//	template <length_t L, qualifier Q>
//	vec<L, float, Q> uintBitsToFloat(vec<L, uint, Q> const& v);
//
//	/// Splits x into a floating-point significand in the range
//	/// [0.5, 1.0) and an integral exponent of two, such that:
//	/// x = significand * exp(2, exponent)
//	///
//	/// The significand is returned by the function and the
//	/// exponent is returned in the parameter exp. For a
//	/// floating-point value of zero, the significant and exponent
//	/// are both zero. For a floating-point value that is an
//	/// infinity or is not a number, the results are undefined.
//	template <typename genType, typename genIType>
//	genType frexp(genType const& x, genIType& exp);
//
//	/// Builds a floating-point number from x and the
//	/// corresponding integral exponent of two in exp, returning:
//	/// significand * exp(2, exponent)
//	///
//	/// If this product is too large to be represented in the
//	/// floating-point type, the result is undefined.
//	template <typename genType, typename genIType>
//	genType ldexp(genType const& x, genIType const& exp);

}//namespace many




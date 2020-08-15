#pragma once

#include <cmath>

#include <glm/common.hpp>	// all the GLSL common functions: abs, floor, etc.

#include "../types.hpp"

namespace many
{
	/// Returns x if x >= 0; otherwise, it returns -x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void abs(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::abs, out);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <glm::length_t L, typename T, glm::qualifier Q, class Tout>
	void sign(const series<glm::vec<L,T,Q>>& a, series<Tout>& out)
	{
		transform(a, glm::sign, out);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void floor(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::floor, out);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void trunc(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::trunc, out);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void round(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::round, out);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void ceil(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::ceil, out);
	}

	/// Return x - floor(x).
	template <glm::length_t L, typename T, glm::qualifier Q>
	void fract(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, glm::floor, out);
	}

	/// Modulus. Returns x - y * floor(x / y)
	/// for each component in x using the floating point value y.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mod(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, glm::mod, out);
	}

	/// Returns the fractional part of x and sets i to the integer
	/// part (as a whole number floating point value). Both the
	/// return value and the output parameter will have the same
	/// sign as x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void modf(const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,int,Q>>& intout, series<glm::vec<L,T,Q>>& fractout)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			fractout[i] = a[i] % 1.;
			intout[i] = glm::vec<L,int,Q>(a[i]-fractout[i]);
		}
	}

	/// Returns y if y < x; otherwise, it returns x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void min(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, glm::min, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void min(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, glm::min, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L,T,Q> min(const series<glm::vec<L,T,Q>>& a)
	{
		glm::vec<L,T,Q> out = a[0];
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out = glm::min(a[i], out);
		}
		return out;
	}
	/// Returns y if y < x; otherwise, it returns x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void max(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, glm::max, out);
	}
	/// Returns y if y < x; otherwise, it returns x.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void max(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, glm::max, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L,T,Q> max(const series<glm::vec<L,T,Q>>& a)
	{
		glm::vec<L,T,Q> out = a[0];
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out = glm::max(a[i], out);
		}
		return out;
	}

	/// Returns min(max(x, minVal), maxVal) for each component in x
	/// using the floating-point values minVal and maxVal.
	template <glm::length_t L, typename T, glm::qualifier Q>
	void clamp(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, lo, hi, glm::clamp, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void clamp(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> lo, const series<glm::vec<L,T,Q>>& hi, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, lo, hi, glm::clamp, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void clamp(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& lo, const glm::vec<L,T,Q> hi, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, lo, hi, glm::clamp, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void clamp(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& lo, const series<glm::vec<L,T,Q>>& hi, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, lo, hi, glm::clamp, out);
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
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const series<glm::vec<L,T,Q>>& y, const series<T>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const series<glm::vec<L,T,Q>>& y, const T a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const glm::vec<L,T,Q> y, const series<T>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const glm::vec<L,T,Q> y, const T a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const series<glm::vec<L,T,Q>>& y, const series<T>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const series<glm::vec<L,T,Q>>& y, const T a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const glm::vec<L,T,Q> y, const series<T>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}




	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const series<glm::vec<L,T,Q>>& y, const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const series<glm::vec<L,T,Q>>& y, const glm::vec<L,T,Q> a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const glm::vec<L,T,Q> y, const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const series<glm::vec<L,T,Q>>& x, const glm::vec<L,T,Q> y, const glm::vec<L,T,Q> a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const series<glm::vec<L,T,Q>>& y, const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const series<glm::vec<L,T,Q>>& y, const glm::vec<L,T,Q> a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}
	template <glm::length_t L, typename T, glm::qualifier Q>
	void mix(const glm::vec<L,T,Q> x, const glm::vec<L,T,Q> y, const series<glm::vec<L,T,Q>>& a, series<glm::vec<L,T,Q>>& out)
	{
		transform(x, y, a, glm::mix, out);
	}




	/// Returns 0.0 if x < edge, otherwise it returns 1.0 for each component of a genType.
	template<glm::length_t L, typename T, glm::qualifier Q>
	void step(const series<glm::vec<L,T,Q>>& edge, const series<glm::vec<L,T,Q>>&  x, series<glm::vec<L,T,Q>>& out)
	{
		transform(edge, x, glm::step, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void step(const series<glm::vec<L,T,Q>>&  edge, const glm::vec<L,T,Q> x, series<glm::vec<L,T,Q>>& out)
	{
		transform(edge, x, glm::step, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void step(const glm::vec<L,T,Q> edge, const series<glm::vec<L,T,Q>>&  x, series<glm::vec<L,T,Q>>& out)
	{
		transform(edge, x, glm::step, out);
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
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const series<glm::vec<L,T,Q>>& lo, const series<glm::vec<L,T,Q>>& hi, const series<glm::vec<L,T,Q>>& x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const glm::vec<L,T,Q> lo, const series<glm::vec<L,T,Q>>& hi, const series<glm::vec<L,T,Q>>& x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const series<glm::vec<L,T,Q>>& lo, glm::vec<L,T,Q> hi, const series<glm::vec<L,T,Q>>& x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi, const series<glm::vec<L,T,Q>>& x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const series<glm::vec<L,T,Q>>& lo, const series<glm::vec<L,T,Q>>& hi, const glm::vec<L,T,Q> x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const glm::vec<L,T,Q> lo, const series<glm::vec<L,T,Q>>& hi, const glm::vec<L,T,Q> x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void smoothstep(const series<glm::vec<L,T,Q>>& lo, const glm::vec<L,T,Q> hi, const glm::vec<L,T,Q> x, series<glm::vec<L,T,Q>>& out)
	{
		transform(lo, hi, x, glm::smoothstep, out);
	}

	/// Returns true if x holds a NaN (not a number)
	/// representation in the underlying implementation's set of
	/// floating point representations. Returns false otherwise,
	/// including for implementations with no NaN
	/// representations.
	template<glm::length_t L, typename T, glm::qualifier Q>
	void isnan(const series<glm::vec<L,T,Q>>&  x, series<bool>& out)
	{
		transform(x, glm::isnan, out);
	}

	/// Returns true if x holds a positive infinity or negative
	/// infinity representation in the underlying implementation's
	/// set of floating point representations. Returns false
	/// otherwise, including for implementations with no infinity
	/// representations.
	template<glm::length_t L, typename T, glm::qualifier Q>
	void isinf(const series<glm::vec<L,T,Q>>&  x, series<bool>& out)
	{
		transform(x, glm::isinf, out);
	}

	/// Computes and returns a * b + c.
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, const series<glm::vec<L,T,Q>>& c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const glm::vec<L,T,Q> a, const series<glm::vec<L,T,Q>>& b, const series<glm::vec<L,T,Q>>& c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const series<glm::vec<L,T,Q>>& a, glm::vec<L,T,Q> b, const series<glm::vec<L,T,Q>>& c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const glm::vec<L,T,Q> a, const glm::vec<L,T,Q> b, const series<glm::vec<L,T,Q>>& c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, const glm::vec<L,T,Q> c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const glm::vec<L,T,Q> a, const series<glm::vec<L,T,Q>>& b, const glm::vec<L,T,Q> c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void fma(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, const glm::vec<L,T,Q> c, series<glm::vec<L,T,Q>>& out)
	{
		transform(a, b, c, glm::fma, out);
	}

//	/// Returns a signed integer value representing
//	/// the encoding of a floating-point value. The floating-point
//	/// value's bit-level representation is preserved.
//	int floatBitsToInt(float const& v);
//
//	/// Returns a signed integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	template<glm::length_t L, glm::qualifier Q>
//	glm::vec<L, int, Q> floatBitsToInt(glm::vec<L, float, Q> const& v);
//
//	/// Returns a unsigned integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	uint floatBitsToUint(float const& v);
//
//	/// Returns a unsigned integer value representing
//	/// the encoding of a floating-point value. The floatingpoint
//	/// value's bit-level representation is preserved.
//	template<glm::length_t L, glm::qualifier Q>
//	glm::vec<L, uint, Q> floatBitsToUint(glm::vec<L, float, Q> const& v);
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
//	template<glm::length_t L, glm::qualifier Q>
//	glm::vec<L, float, Q> intBitsToFloat(glm::vec<L, int, Q> const& v);
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
//	template<glm::length_t L, glm::qualifier Q>
//	glm::vec<L, float, Q> uintBitsToFloat(glm::vec<L, uint, Q> const& v);
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
//	template<typename genType, typename genIType>
//	genType frexp(genType const& x, genIType& exp);
//
//	/// Builds a floating-point number from x and the
//	/// corresponding integral exponent of two in exp, returning:
//	/// significand * exp(2, exponent)
//	///
//	/// If this product is too large to be represented in the
//	/// floating-point type, the result is undefined.
//	template<typename genType, typename genIType>
//	genType ldexp(genType const& x, genIType const& exp);

}//namespace many




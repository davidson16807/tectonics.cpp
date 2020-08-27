#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{
	/// Returns x if x >= 0; otherwise, it returns -x.
	template <typename T>
	inline T abs(const T a)
	{
		return std::abs(a);
	}
	template <typename T, typename Tout>
	void abs(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return abs(ai); }, a);
	}

	/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
	template <typename T>
	inline T sign(const T a)
	{
		return (T(0) < a) - (a < T(0));
	}
	template <typename T, typename Tout>
	void sign(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return sign(ai); }, a);
	}

	/// Returns a value equal to the nearest integer that is less then or equal to x.
	template <typename T>
	inline T floor(const T a)
	{
		return std::floor(a);
	}
	template <typename T, typename Tout>
	void floor(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return floor(ai); }, a);
	}

	/// Returns a value equal to the nearest integer to x
	/// whose absolute value is not larger than the absolute value of x.
	template <typename T>
	T trunc(const T a)
	{
		return std::trunc(a);
	}
	template <typename T, typename Tout>
	void trunc(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return trunc(ai); }, a);
	}

	/// Returns a value equal to the nearest integer to x.
	/// The fraction 0.5 will round in a direction chosen by the
	/// implementation, presumably the direction that is fastest.
	/// This includes the possibility that round(x) returns the
	/// same value as roundEven(x) for all values of x.
	template <typename T>
	T round(const T a)
	{
		return std::round(a);
	}
	template <typename T, typename Tout>
	void round(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return round(ai); }, a);
	}

	/// Returns a value equal to the nearest integer
	/// that is greater than or equal to x.
	template <typename T>
	inline T ceil(const T a)
	{
		return std::ceil(a);
	}
	template <typename T, typename Tout>
	void ceil(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return std::ceil(ai); }, a);
	}

	/// Return x - floor(x).
	template <typename T>
	inline T fract(const T a)
	{
		return a-floor(a);
	}
	template <typename T, typename Tout>
	void fract(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return ai - floor(ai); }, a);
	}

	/// Modulus. Returns x - y * floor(x / y)
	/// for each component in x using the floating point value y.
	template <typename T>
	inline T mod(const T a, const T b)
	{
		return a-b*floor(a/b);
	}
	template <typename T, typename Tout>
	void mod(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai - bi * floor(ai / bi); }, a, b);
	}
	template <typename T, typename Tout>
	void mod(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai - bi * floor(ai / bi); }, a, b);
	}
	template <typename T, typename Tout>
	void mod(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai - bi * floor(ai / bi); }, a, b);
	}

	/// Returns the fractional part of x and sets i to the integer
	/// part (as a whole number floating point value). Both the
	/// return value and the output parameter will have the same
	/// sign as x.
	template <typename T, typename Tint, typename Tout>
	void modf(const T& a, Tint& intout, Tout& fractout)
	{
		typedef typename T::value_type Ti;
		typedef typename Tint::value_type Tinti;
		typedef typename Tout::value_type Touti;
		fractout.store([](Ti ai){ return ai - floor(ai); }, a);
		intout.store([](Ti ai, Touti fractouti){ return Tinti(ai-fractouti); }, a, fractout);
	}

	/// Returns y if y < x; otherwise, it returns x.
	template <typename T>
	T min(const T a, const T b)
	{
		return std::min(a,b);
	}
	template <typename T, typename Tout>
	void min(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return min(ai,bi); }, a, b);
	}
	template <typename T, typename Tout>
	void min(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return min(ai,bi); }, a, b);
	}
	template <typename T, typename Tout>
	void min(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return min(ai,bi); }, a, b);
	}

	// component-wise min
	template <typename T>
	typename T::value_type min(const T& a, const bool no_nan = true, const bool no_inf = true)
	{
		typedef typename T::value_type Ti;
		typedef typename T::size_type Tsize;
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
		}
		Ti out = a[0];
		for (Tsize i(0); i < a.size(); ++i)
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
	T max(const T a, const T b)
	{
		return std::max(a,b);
	}
	template <typename T, typename Tout>
	void max(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return max(ai,bi); }, a, b);
	}
	template <typename T, typename Tout>
	void max(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return max(ai,bi); }, a, b);
	}
	template <typename T, typename Tout>
	void max(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return max(ai,bi); }, a, b);
	}
	// component-wise max
	template <typename T>
	typename T::value_type max(const T& a, const bool no_nan = true, const bool no_inf = true)
	{
		typedef typename T::value_type Ti;
		typedef typename T::size_type Tsize;
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
		}
		Ti out = a[0];
		for (Tsize i(0); i < a.size(); ++i)
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
	inline T clamp(const T a, const T lo, const T hi)
	{
		return a > hi? hi : a < lo? lo : a;
	}
	template <typename T, typename Tout>
	void clamp(const T& a, const typename T::value_type lo, const typename T::value_type hi, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti loi, Ti hii){ return clamp(ai,loi,hii); }, a, lo, hi);
	}
	template <typename T, typename Tout>
	void clamp(const T& a, const typename T::value_type lo, const T& hi, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti loi, Ti hii){ return clamp(ai,loi,hii); }, a, lo, hi);
	}
	template <typename T, typename Tout>
	void clamp(const T& a, const T& lo, const typename T::value_type hi, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti loi, Ti hii){ return clamp(ai,loi,hii); }, a, lo, hi);
	}
	template <typename T, typename Tout>
	void clamp(const T& a, const T& lo, const T& hi, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti hii){ return min(ai,hii); }, a,   hi);
		out.store([](Ti ai, Ti loi){ return max(ai,loi); }, out, lo);
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
	template <typename T, typename Tout>
	void mix(const series<T>& x, const series<T>& y, const series<T>& a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const series<T>& x, const series<T>& y, const T a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const series<T>& x, const T y, const series<T>& a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const series<T>& x, const T y, const T a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const T x, const series<T>& y, const series<T>& a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const T x, const series<T>& y, const T a, Tout& out)
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
	template <typename T, typename Tout>
	void mix(const T x, const T y, const series<T>& a, Tout& out)
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
	template <typename T, typename Tout>
	void step(const series<T>& edge, const series<T>&  x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](T edgei, T xi){ return xi < edgei? Ti(0) : Ti(1); }, edge, x); 
	}
	template <typename T, typename Tout>
	void step(const series<T>&  edge, const T x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](T edgei, T xi){ return xi < edgei? Ti(0) : Ti(1); }, edge, x); 
	}
	template <typename T, typename Tout>
	void step(const T edge, const series<T>&  x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](T edgei, T xi){ return xi < edgei? Ti(0) : Ti(1); }, edge, x); 
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
	template <typename T, typename Tout>
	void smoothstep(const T& lo, const T& hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const typename T::value_type lo, const T& hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const T& lo, T hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const typename T::value_type lo, const typename T::value_type hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const T& lo, const T& hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const typename T::value_type lo, const T& hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void smoothstep(const T& lo, const typename T::value_type hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ Ti t = xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); return t*t*(Ti(3)-Ti(2)*t); }, x, lo, hi); 
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
	template <typename T, typename Tout>
	void linearstep(const T& lo, const T& hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const typename T::value_type lo, const T& hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const T& lo, T hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const typename T::value_type lo, const typename T::value_type hi, const T& x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const T& lo, const T& hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const typename T::value_type lo, const T& hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
	}
	template <typename T, typename Tout>
	void linearstep(const T& lo, const typename T::value_type hi, const typename T::value_type x, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti xi, Ti loi, Ti hii){ return xi<=loi? Ti(0) : xi >= hii? Ti(1) : ((xi-loi)/(hii-loi)); }, x, lo, hi); 
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
	template <typename T, typename Tout>
	void isnan(const T&  x, Tout& out)
	{
		out.store(std::isnan, x);
	}

	/// Returns true if x holds a positive infinity or negative
	/// infinity representation in the underlying implementation's
	/// set of floating point representations. Returns false
	/// otherwise, including for implementations with no infinity
	/// representations.
	template <typename T, typename Tout>
	void isinf(const T&  x, Tout& out)
	{
		out.store(std::isinf, x);
	}

	/// Computes and returns a * b + c.
	template <typename T, typename Tout>
	void fma(const T& a, const T& b, const T& c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const typename T::value_type a, const T& b, const T& c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const T& a, T b, const T& c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const typename T::value_type a, const typename T::value_type b, const T& c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const T& a, const T& b, const typename T::value_type c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const typename T::value_type a, const T& b, const typename T::value_type c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
	}
	template <typename T, typename Tout>
	void fma(const T& a, const typename T::value_type b, const typename T::value_type c, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi, Ti ci){ return ai*bi+ci; }, a, b, c); 
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




#pragma once

#include <cmath>

namespace composites
{
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <class T>
	inline void pow(const many<T>& base, const many<T>& exponent, many<T>& out)
	{
		transform(base, exponent, std::pow, out);
	}
	// TODO: vector variant
	/// Returns the natural exponentiation of x, i.e., e^x.
	///
	/// @param a exp function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <class T>
	inline void exp(const many<T>& a, many<T>& out)
	{
		transform(a, std::exp, out);
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void log(const many<T>& a, many<T>& out)
	{
		transform(a, std::log, out);
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <class T>
	inline void exp2(const many<T>& a, many<T>& out)
	{
		transform(a, std::exp2, out);
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void log2(const many<T>& a, many<T>& out)
	{
		transform(a, std::log2, out);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void sqrt(const many<T>& a, many<T>& out)
	{
		transform(a, std::sqrt, out);
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void inversesqrt(const many<T>& a, many<T>& out)
	{
		transform(a, [](T ai){ return 1./std::sqrt(ai); }, out);
	}
}//namespace composites


#pragma once

#include <cmath>

#include "composite.hpp"

namespace many
{
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <class T>
	inline void pow(const composite<T>& base, const composite<T>& exponent, composite<T>& out)
	{
		transform(base, exponent, [](T basei, T exponenti){ return std::pow(basei, exponenti); }, out);
	}
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <class T>
	inline void pow(const composite<T>& base, const T exponent, composite<T>& out)
	{
		transform(base, exponent, [](T basei, T exponenti){ return std::pow(basei, exponenti); }, out);
	}
	// TODO: vector variant
	/// Returns the natural exponentiation of x, i.e., e^x.
	///
	/// @param a exp function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <class T>
	inline void exp(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return std::exp(ai); }, out);
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void log(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return std::log(ai); }, out);
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <class T>
	inline void exp2(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return std::exp2(ai); }, out);
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void log2(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return std::log2(ai); }, out);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void sqrt(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return std::sqrt(ai); }, out);
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <class T>
	inline void inversesqrt(const composite<T>& a, composite<T>& out)
	{
		transform(a, [](T ai){ return 1./std::sqrt(ai); }, out);
	}
}//namespace many

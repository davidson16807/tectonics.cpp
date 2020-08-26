#pragma once

#include <cmath>

#include "types.hpp"

namespace many
{
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <typename T>
	inline void pow(const T& base, const T& exponent, T& out)
	{
		out.store([](typename T::value_type basei, typename T::value_type exponenti){ return std::pow(basei, exponenti); }, base, exponent);
	}
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <typename T>
	inline void pow(const T& base, const typename T::value_type exponent, T& out)
	{
		out.store([](typename T::value_type basei, typename T::value_type exponenti){ return std::pow(basei, exponenti); }, base, exponent);
	}
	// TODO: vector variant
	/// Returns the natural exponentiation of x, i.e., e^x.
	///
	/// @param a exp function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void exp(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return std::exp(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void log(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return std::log(ai); }, a);
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void exp2(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return std::exp2(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void log2(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return std::log2(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void sqrt(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return std::sqrt(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline void inversesqrt(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return 1./std::sqrt(ai); }, a);
	}
}//namespace many


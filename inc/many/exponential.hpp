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
	inline T pow(const T basei, const T exponenti)
	{
		return std::pow(basei, exponenti);
	}
	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <typename T1, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T1>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline void pow(const T1& base, const T2& exponent, Tout& out)
	{
		out.store([](typename T1::value_type basei, typename T2::value_type exponenti){ return pow(basei, exponenti); }, base, exponent);
	}
	template <typename T1, typename T2, typename Tout, 
		std::enable_if_t<std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline void pow(const T1& base, const T2 exponent, Tout& out)
	{
		out.store([](typename T1::value_type basei, T2 exponenti){ return pow(basei, exponenti); }, base, exponent);
	}
	template <typename T1, typename T2, typename Tout, 
		std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && std::is_base_of<AbstractSeries, T2>::value, int> = 0>
	inline void pow(const T1 base, const T2& exponent, Tout& out)
	{
		out.store([](T1 basei, typename T2::value_type exponenti){ return pow(basei, exponenti); }, base, exponent);
	}
	// TODO: vector variant
	/// Returns the natural exponentiation of x, i.e., e^x.
	///
	/// @param a exp function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T exp(const T ai)
	{
		return std::exp(ai);
	}
	template <typename T>
	inline void exp(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return exp(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T log(const T ai)
	{
		return std::log(ai);
	}
	template <typename T>
	inline void log(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return log(ai); }, a);
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T exp2(const T ai)
	{
		return std::exp2(ai);
	}
	template <typename T>
	inline void exp2(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return exp2(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T log2(const T ai)
	{
		return std::log2(ai);
	}
	template <typename T>
	inline void log2(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return log2(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T sqrt(const T ai)
	{
		return std::sqrt(ai);
	}
	template <typename T>
	inline void sqrt(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return sqrt(ai); }, a);
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline T inversesqrt(const T ai)
	{
		return T(1)/std::sqrt(ai);
	}
	template <typename T>
	inline void inversesqrt(const T& a, T& out)
	{
		out.store([](typename T::value_type ai){ return inversesqrt(ai); }, a);
	}
}//namespace many


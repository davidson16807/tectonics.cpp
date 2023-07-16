#pragma once

#include <cmath>

namespace data
{

	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <typename T1, typename T2>
	inline auto pow(const T1 basei, const T2 exponenti)
	{
		return std::pow(basei, exponenti);
	}

	/// Returns 'base' raised to the power 'exponent'.
	///
	/// @param base Floating point value. pow function is defined for input values of 'base' defined in the range (inf-, inf+) in the limit of the type qualifier.
	/// @param exponent Floating point value representing the 'exponent'.
	template <typename T1, typename T2, typename Tout>
	inline void pow(const T1& base, const T2& exponent, Tout& out)
	{
		auto size = out.size(base, exponent);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = pow(base[i], exponent[i]);
		}
	}

	// TODO: vector variant
	/// Returns the natural exponentiation of x, i.e., e^x.
	///
	/// @param a exp function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto exp(const T ai)
	{
		return std::exp(ai);
	}

	template <typename T1, typename T2>
	inline void exp(const T1& a, T2& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = exp(a[i]);
		}
	}

	// TODO: vector variant
	/// Returns the natural logarithm of a, i.e.,
	/// returns the value y which satisfies the equation x = e^y.
	/// Results are undefined if a <= 0.
	///
	/// @param a log function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto log(const T ai)
	{
		return std::log(ai);
	}

	template <typename T>
	inline void log(const T& a, T& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = log(a[i]);
		}
	}

	// TODO: vector variant
	/// Returns 2 raised to the a power.
	///
	/// @param a exp2 function is defined for input values of a defined in the range (inf-, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto exp2(const T ai)
	{
		return std::exp2(ai);
	}

	template <typename T>
	inline void exp2(const T& a, T& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = exp2(a[i]);
		}
	}

	// TODO: vector variant
	/// Returns the base 2 log of x, i.e., returns the value y,
	/// which satisfies the equation x = 2 ^ y.
	///
	/// @param a log2 function is defined for input values of a defined in the range (0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto log2(const T ai)
	{
		return std::log2(ai);
	}

	template <typename T>
	inline auto sqrt(const T ai)
	{
		return std::sqrt(ai);
	}

	// TODO: vector variant
	/// Returns the positive square root of a.
	///
	/// @param a sqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto log2(const T& a, T& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = log2(a[i]);
		}
	}

	template <typename T>
	inline void sqrt(const T& a, T& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = sqrt(a[i]);
		}
	}

	// TODO: vector variant
	/// Returns the reciprocal of the positive square root of a.
	///
	/// @param a inversesqrt function is defined for input values of a defined in the range [0, inf+) in the limit of the type qualifier.
	template <typename T>
	inline auto inversesqrt(const T ai)
	{
		return T(1)/std::sqrt(ai);
	}

	template <typename T1, typename T2>
	inline void inversesqrt(const T1& a, T2& out)
	{
		auto size = out.size(a);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = 1.0/sqrt(a[i]);
		}
	}
}//namespace series


#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatibility.hpp"
#include "each.hpp"

namespace whole
{


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

	template <typename T>
	typename T::value_type max(const T& a, const bool no_nan = true, const bool no_inf = true)
	{
		auto size = a.size();
		typedef typename T::value_type Ti;
		typedef typename T::size_type Tsize;
		if (a.size() < 1)
		{
			throw std::out_of_range("cannot find the minimum value of an empty composite");
		}
		Ti out = a[0];
		for (Tsize i(0); i < size; ++i)
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



	template <typename T1, typename T2>
	double distance(const T1& a, const T2& b)
	{
		double out(0);
		// assert(compatible(a,b));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out += data::distance(a[i], b[i]);
		}
		return out;
	}



	template <typename T1, typename T2>
	bool all(const T1& a)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (!a[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool any(const T1& a)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b, const float threshold)
	{
		return whole::distance(a,b) < threshold;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b)
	{
		if (!compatible(a,b)){
			return false;
		}
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] != b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool notEqual(const T1& a, const T2& b)
	{
		if (!compatible(a,b)){
			return true;
		}
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] != b[i]){
				return true;
			}
		}
		return false;
	}

	template <typename T1, typename T2>
	bool greaterThan(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] <= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool lessThan(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] >= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool greaterThanEqual(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] <= b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool lessThanEqual(const T1& a, const T2& b)
	{
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] >= b[i]){
				return false;
			}
		}
		return true;
	}

}


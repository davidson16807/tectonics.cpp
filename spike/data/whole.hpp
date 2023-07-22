#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatibility.hpp"

namespace data
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

}


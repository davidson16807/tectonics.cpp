#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

namespace data
{

	double distance(const bool a, const bool b)
	{
		return std::abs(a-b);
	}

	double distance(const int a, const int b)
	{
		return std::abs(a-b);
	}

	double distance(const float a, const float b)
	{
		return std::abs(a-b);
	}

	double distance(const double a, const double b)
	{
		return std::abs(a-b);
	}

	template <typename T1, typename T2>
	double distance(const T1& a, const T2& b)
	{
		double out(0);
		// assert(compatible(a,b));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out += distance(a[i], b[i]);
		}
		return out;
	}

}


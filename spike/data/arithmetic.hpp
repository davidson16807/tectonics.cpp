#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

// in-house libraries
#include "compatibility.hpp"

namespace data
{

	template <typename T1, typename T2, typename Tout>
	void add(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] + b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void sub(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] - b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void mult(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] * b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void div(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] / b[i];
		}
	}

}


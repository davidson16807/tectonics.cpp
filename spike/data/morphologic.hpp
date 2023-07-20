#pragma once

#include "compatibility.hpp"

namespace data
{

	template <typename T1, typename T2, typename Tout>
	void unite(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] || b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void intersect(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] && b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void differ(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] && !b[i];
		}
	}

	template <typename T, typename Tout>
	void negate(const T& a, Tout& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = !a[i];
		}
	}

}


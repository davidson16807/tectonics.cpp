#pragma once

// C libraries
#include <cmath>     /* std math */
#include <limits>    /* numeric_limits */
#include <assert.h>  /* assert */

// in-house libraries
#include "each.hpp"

namespace group
{

	template <typename Ids, typename Out>
	void count (const Ids& group_ids, const Out& out)
	{
		std::fill(out.begin(), out.end(), typename Out::value_type(0));
		auto size = group_ids.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[group_ids[i]] += 1;
		}
	}

	template <typename Ids, typename In, typename Out>
	void sum (const Ids& group_ids, const In& a, const Out& out)
	{
		std::fill(out.begin(), out.end(), typename Out::value_type(0));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[group_ids[i]] += a[i];
		}
	}

	template <typename Ids, typename In, typename Out>
	void product (const Ids& group_ids, const In& a, const Out& out)
	{
		std::fill(out.begin(), out.end(), typename Out::value_type(1));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[group_ids[i]] *= a[i];
		}
	}

	template <typename Ids, typename In, typename Out>
	void min (const Ids& group_ids, const In& a, const Out& out)
	{
		std::fill(out.begin(), out.end(), std::numeric_limits<typename Out::value_type>::max());
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[group_ids[i]] = each::min(out[group_ids[i]], a[i]);
		}
	}

	template <typename Ids, typename In, typename Out>
	void max (const Ids& group_ids, const In& a, const Out& out)
	{
		std::fill(out.begin(), out.end(), std::numeric_limits<typename Out::value_type>::min());
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[group_ids[i]] = each::max(out[group_ids[i]], a[i]);
		}
	}

}

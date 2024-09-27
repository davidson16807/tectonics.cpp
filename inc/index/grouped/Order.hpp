#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace grouped
{

	template <typename ElementOrder>
	class Order
	{
		const ElementOrder elements;
	public:
		Order(const ElementOrder& elements):
			elements(elements)
		{}

		template <typename Ids, typename In, typename Out>
		void min (const Ids& group_ids, const In& a, const Out& out)
		{
			std::fill(out.begin(), out.end(), std::numeric_limits<typename Out::value_type>::max());
			auto size = a.size();
			for (std::size_t i = 0; i < size; ++i)
			{
				out[group_ids[i]] = elements.min(out[group_ids[i]], a[i]);
			}
		}

		template <typename Ids, typename In, typename Out>
		void max (const Ids& group_ids, const In& a, const Out& out)
		{
			std::fill(out.begin(), out.end(), std::numeric_limits<typename Out::value_type>::min());
			auto size = a.size();
			for (std::size_t i = 0; i < size; ++i)
			{
				out[group_ids[i]] = elements.max(out[group_ids[i]], a[i]);
			}
		}

	};

}


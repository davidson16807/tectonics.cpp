#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries
#include <index/iterated/Nary.hpp>

namespace branched
{

	template <typename ElementOrder>
	class OrderRootFirst
	{
		const ElementOrder elements;
		const iterated::Identity copy;
	public:
		OrderRootFirst(const ElementOrder& elements):
			elements(elements),
			copy()
		{}
		OrderRootFirst():
			elements(),
			copy()
		{}

		template <typename Ids, typename In, typename Out>
		void min (const Ids& parent_ids, const In& a, const Out& out) const
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[parent_ids[i]] = elements.min(out[parent_ids[i]], out[i]);
			}
		}

		template <typename Ids, typename In, typename Out>
		void max (const Ids& parent_ids, const In& a, const Out& out) const
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[parent_ids[i]] = elements.max(out[parent_ids[i]], out[i]);
			}
		}

	};

}


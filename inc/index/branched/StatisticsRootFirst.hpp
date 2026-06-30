#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace branched
{

	template <typename ElementArithmetic>
	class StatisticsRootFirst
	{
		const ElementArithmetic arithmetic;
		const iterated::Identity copy;
	public:
		StatisticsRootFirst(const ElementArithmetic& arithmetic):
			arithmetic(arithmetic),
			copy()
		{}
		StatisticsRootFirst():
			arithmetic(),
			copy()
		{}

		template <typename Ids, typename Out>
		void count (const Ids& group_ids, Out& out) const
		{
			auto size = group_ids.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[group_ids[i]] += 1;
			}
		}

		template <typename Ids, typename In, typename Out>
		void sum (const Ids& group_ids, const In& a, Out& out) const
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[group_ids[i]] = arithmetic.add(out[group_ids[i]], a[i]);
			}
		}

		template <typename Ids, typename In, typename Out>
		void product (const Ids& group_ids, const In& a, Out& out) const
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[group_ids[i]] = arithmetic.multiply(out[group_ids[i]], a[i]);
			}
		}

	};

}


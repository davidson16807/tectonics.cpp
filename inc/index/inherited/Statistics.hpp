#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace inherited
{

	template <typename ElementArithmetic>
	class Statistics
	{
		const ElementArithmetic arithmetic;
		const iterated::Identity copy;
	public:
		Statistics(const ElementArithmetic& arithmetic):
			arithmetic(arithmetic),
			copy()
		{}
		Statistics():
			arithmetic(),
			copy()
		{}

		template <typename Ids, typename Out>
		void count (const Ids& parent_ids, Out& out)
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[i] = out[parent_ids[i]] + 1;
			}
		}

		template <typename Ids, typename In, typename Out>
		void sum (const Ids& parent_ids, const In& a, Out& out)
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[i] = arithmetic.add(out[parent_ids[i]], a[i]);
			}
		}

		template <typename Ids, typename In, typename Out>
		void product (const Ids& parent_ids, const In& a, Out& out)
		{
			copy(a, out);
			auto size = a.size();
			for (std::size_t i = size-1; i >= 0; --i)
			{
				out[i] = arithmetic.multiply(out[parent_ids[i]], a[i]);
			}
		}

	};

}

#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace grouped
{

	template <typename ElementArithmetic>
	class Statistics
	{
		const ElementArithmetic arithmetic;
	public:
		Statistics(const ElementArithmetic& arithmetic):
			arithmetic(arithmetic)
		{}

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
				out[group_ids[i]] = arithmetic.add(out[group_ids[i]], a[i]);
			}
		}

		template <typename Ids, typename In, typename Out>
		void product (const Ids& group_ids, const In& a, const Out& out)
		{
			std::fill(out.begin(), out.end(), typename Out::value_type(1));
			auto size = a.size();
			for (std::size_t i = 0; i < size; ++i)
			{
				out[group_ids[i]] = arithmetic.multiply(out[group_ids[i]], a[i]);
			}
		}

		template <typename In1, typename In2, typename Out>
		auto linear_combination(const Ids& group_ids, const In1& a, const In2& weights, const Out& out) const
		{
			auto out = arithmetic.zero;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out[group_ids[i]] = arithmetic.add(out[group_ids[i]], arithmetic.multiply(a[i], weights[i]));
			}
			return out;
		}

		template <typename In1, typename In2, typename Out>
		auto mean(const Ids& group_ids, const In1& a, const In2& counts, const Out& out) const
		{
			auto out = arithmetic.zero;
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				out[group_ids[i]] = arithmetic.add(out[group_ids[i]], arithmetic.divide(a[i], counts[i]));
			}
			return out;
		}

		// // comp

	};

}


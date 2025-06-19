#pragma once

// C libraries
#include <assert.h>  /* assert */
#include <algorithm>  /* clamp */

// in-house libraries
#include <index/procedural/Uniform.hpp>
#include <index/iterated/Nary.hpp>

namespace binned
{

	template <typename Order, typename ElementArithmetic>
	class Statistics
	{

		const Order order;
		const ElementArithmetic arithmetic;
		const iterated::Identity copy;

	public:

		Statistics(
			const Order& order,
			const ElementArithmetic& arithmetic
		):
			order(order),
			arithmetic(arithmetic),
			copy()
		{}

		Statistics():
			order(),
			arithmetic(),
			copy()
		{}

		template <typename Binables, typename Outs>
		void count (const Binables& binables, Outs& out) const
		{
			using id = typename Outs::size_type;
			using Out = typename Outs::value_type;
			using Binable = typename Binables::value_type;
			id size = out.size();
			Binable lo = order.min(binables);
			Binable hi = order.max(binables);
			Binable step(Binable(hi-lo)/(out.size()+1)); 
			// ^^^ use out.size()+1 is used so that we can bin both lowest and highest value(s)
			copy(procedural::uniform(id(0)), out);
			for (std::size_t i = 0; i < size; ++i)
			{
				out[std::clamp(id((binables[i]-lo)/step), 0, out.size()-1)] += Out(1);
			}
		}

		template <typename Binables, typename Ins, typename Outs>
		void sum (const Binables& binables, const Ins& in, Outs& out) const
		{
			using id = typename Outs::size_type;
			using Binable = typename Binables::value_type;
			id size = out.size();
			Binable lo = order.min(binables);
			Binable hi = order.max(binables);
			Binable step(Binable(hi-lo)/(out.size()+1));
			// ^^^ use out.size()+1 is used so that we can bin both lowest and highest value(s)
			copy(procedural::uniform(id(0)), out);
			id bin;
			for (std::size_t i = 0; i < size; ++i)
			{
				bin = std::clamp(id((binables[i]-lo)/step), id(0), id(out.size()-1));
				out[bin] = arithmetic.add(out[bin], in[i]);
			}
		}

		template <typename Binables, typename Ins, typename Outs>
		void product (const Binables& binables, const Ins& in, Outs& out) const
		{
			using id = typename Outs::size_type;
			using Binable = typename Binables::value_type;
			id size = out.size();
			Binable lo = order.min(binables);
			Binable hi = order.max(binables);
			Binable step(Binable(hi-lo)/(out.size()+1)); 
			// ^^^ use out.size()+1 is used so that we can bin both lowest and highest value(s)
			copy(procedural::uniform(id(1)), out);
			id bin;
			for (std::size_t i = 0; i < size; ++i)
			{
				bin = std::clamp(id((binables[i]-lo)/step), id(0), id(out.size()-1));
				out[bin] = arithmetic.multiply(out[bin], in[i]);
			}
		}

	};

}


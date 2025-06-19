#pragma once

// C libraries
#include <assert.h>  /* assert */
#include <algorithm>  /* clamp */

// in-house libraries
#include <index/procedural/Uniform.hpp>

namespace preceded
{

	template <typename ElementArithmetic>
	class Statistics
	{

		const ElementArithmetic arithmetic;

	public:

		Statistics(
			const ElementArithmetic& arithmetic
		):
			arithmetic(arithmetic)
		{}

		Statistics(
		):
			arithmetic()
		{}

		template <typename Ins, typename Outs>
		void sum (const Ins& in, Outs& out) const
		{
			using id = typename Outs::size_type;
			using Out = typename Outs::value_type;
			id size = out.size();
			Out accumulator(0);
			for (std::size_t i = 0; i < size; ++i)
			{
				accumulator = arithmetic.add(accumulator, in[i]);
				out[i] = accumulator;
			}
		}

		template <typename Ins, typename Outs>
		void product (const Ins& in, Outs& out) const
		{
			using id = typename Outs::size_type;
			using Out = typename Outs::value_type;
			id size = out.size();
			Out accumulator(1);
			for (std::size_t i = 0; i < size; ++i)
			{
				accumulator = arithmetic.multiply(accumulator, in[i]);
				out[i] = accumulator;
			}
		}

	};

}


#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace aggregated
{

	/*
	An `aggregated::Equivalence` aggregates elements that are subject to an `adapter::Equivalence` relationship.
	It intends to provide all well-known operations and relations that could be provided simply by knowing that a `*Metric` exists.
	*/

	template <typename ElementBitset>
	class Equivalence
	{
		const ElementBitset elements;
	public:
		Equivalence(const ElementBitset& elements):
			elements(elements)
		{}
		Equivalence():
			elements()
		{}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return false;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.equal(a[i], b[i])) {
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool not_equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return true;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.equal(a[i], b[i])) {
					return true;
				}
			}
			return false;
		}

	};

}


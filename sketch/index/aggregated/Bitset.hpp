#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace aggregated
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	template <typename ElementBitset>
	class Bitset
	{
		const ElementBitset elements;
	public:
		Bitset(const ElementBitset& elements):
			elements(elements)
		{}

		template <typename In1>
		bool all(const In1& a) const
		{
			auto size = a.size();
			for (typename In1::size_type i = 0; i < size; ++i)
			{
				if (!elements.all(a[i])){
					return false;
				}
			}
			return true;
		}

		template <typename In1>
		bool any(const In1& a) const
		{
			auto size = a.size();
			for (typename In1::size_type i = 0; i < size; ++i)
			{
				if (elements.any(a[i])){
					return true;
				}
			}
			return false;
		}

	};

}


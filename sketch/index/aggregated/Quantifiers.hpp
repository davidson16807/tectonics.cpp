#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <math/special.hpp>

#include "each.hpp"

namespace aggregated
{

	template <typename ElementQuantifiers>
	class Quantifiers
	{
		const ElementQuantifiers elements;
	public:
		Quantifiers(const ElementQuantifiers& elements):
			elements(elements)
		{}

		template <typename In1>
		bool all(const In1& a) const
		{
			auto size = a.size();
			for (typename In1::size_type i = 0; i < size; ++i)
			{
				if (!a[i]){
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
				if (a[i]){
					return true;
				}
			}
			return false;
		}

	};

}


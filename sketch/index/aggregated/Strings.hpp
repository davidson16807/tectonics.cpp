#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <math/special.hpp>

namespace aggregated
{

	template <typename ElementStrings, typename Order>
	class Strings
	{
		const ElementStrings strings;
		const Order order;
	public:
		Strings(const ElementStrings& strings, const Order& order):
			strings(strings),
			order(order)
		{}

		template <typename Series, typename T>
		std::string to_string(const Series& a, const T lo, const T hi, const int line_char_width = 80) const
		{
			std::string out("");
			using id = typename Series::size_type;
			for (id i = 0; i < a.size(); ++i)
			{
			    if (i % line_char_width == 0)
			    {
			    	out += "\n";
			    }
			    out += strings.character(a[i],lo,hi);
			}
			out += "\n";
			out += legend(a[0], lo, hi);
			return out;
		}

		template <typename Series>
		inline std::string to_string(const Series& a, const int line_char_width = 80) const
		{
			return to_string(a, order.min(a), order.max(a), line_char_width);
		}

	};

}


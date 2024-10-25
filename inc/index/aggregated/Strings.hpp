#pragma once

// C libraries
#include <assert.h>  /* assert */

// in-house libraries

namespace aggregated
{

	template <typename ElementStrings, typename OrderAggregation>
	class Strings
	{
		const ElementStrings strings;
		const OrderAggregation aggregation;
		const int line_char_width;
	public:
		Strings(const ElementStrings& strings, const OrderAggregation& aggregation, const int line_char_width):
			strings(strings),
			aggregation(aggregation),
			line_char_width(line_char_width)
		{}
		Strings(const ElementStrings& strings, const OrderAggregation& aggregation):
			strings(strings),
			aggregation(aggregation),
			line_char_width(80)
		{}

		template <typename Series, typename T>
		std::string format(const Series& a, const T lo, const T hi) const
		{
			std::string out("");
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
			    if (i % line_char_width == 0)
			    {
			    	out += "\n";
			    }
			    out += strings.character(a[i],lo,hi);
			}
			out += "\n";
			out += strings.legend(a[0], lo, hi);
			return out;
		}

		template <typename Series>
		inline std::string format(const Series& a) const
		{
			return format(a, aggregation.min(a), aggregation.max(a));
		}

	};

}


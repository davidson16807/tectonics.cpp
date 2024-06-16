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
	public:
		Strings(const ElementStrings& strings, const OrderAggregation& aggregation):
			strings(strings),
			aggregation(aggregation)
		{}

		template <typename Series, typename T>
		std::string to_string(const Series& a, const T lo, const T hi, const int line_char_width = 80) const
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
		inline std::string to_string(const Series& a, const int line_char_width = 80) const
		{
			return to_string(a, aggregation.min(a), aggregation.max(a), line_char_width);
		}

	};

}


#pragma once

// C libraries
#include <cmath>     /* std math */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <unit/si.hpp>

namespace adapted
{

	class SiStrings
	{
		using string = std::string;

		#if defined(__clang__)
			const std::vector<const string> 
		#else
			const std::array<const string, 5>
		#endif
		shades {" ", "░", "▒", "▓", "█" };

	public:
		SiStrings(){}

		template<typename quantity>
		string legend(quantity sample, const quantity lo, const quantity hi) const 
		{
			using scalar = typename quantity::value_type;
			string out("");
			for (unsigned int i = 0; i < shades.size(); ++i)
			{
				auto bound = si::mix(lo, hi, scalar(i)/scalar(shades.size()));
				out += shades[i];
				out += " ≥ ";
				out += si::to_string(bound);
				out += "\n";
			}
			return out;
		}

		template<typename quantity>
		string character(const quantity a, const quantity lo, const quantity hi) const 
		{
			if (si::isnan(a))
			{
				return "N";
			}
			else if (si::isinf(a))
			{
				return "∞";
			}
			else 
			{
			    return shades[
			    	std::clamp(int((shades.size()) * si::linearstep(lo, hi, a)), 
			    				0, int(shades.size()-1))
		    	];
			}
		}

	};

}


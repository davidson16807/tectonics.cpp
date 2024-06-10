#pragma once

// C libraries
#include <cmath>     /* std math */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries

namespace adapter
{

	class ScalarStrings
	{
	public:
		ScalarStrings(){}

		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 5>
		#endif
		shades {" ", "░", "▒", "▓", "█" };

		template<typename T>
		std::string legend(T sample, const T lo, const T hi) const 
		{
			std::string out("");
			for (unsigned int i = 0; i < shades.size(); ++i)
			{
				auto bound = math::mix(float(lo), float(hi), float(i)/float(shades.size()));
				out += shades[i];
				out += " ≥ ";
				out += std::to_string(bound);
				out += "\n";
			}
			return out;
		}

		template<typename T>
		std::string character(const T a, const T lo, const T hi) const 
		{
			if (std::isnan(a))
			{
				return "N";
			}
			else if (std::isinf(a))
			{
				return "∞";
			}
			else 
			{
			    return shades[
			    	std::clamp(int((shades.size()) * math::linearstep(lo, hi, a)), 
			    				0, int(shades.size()-1))
		    	];
			}
		}

	};

}


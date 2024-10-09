#pragma once

// C libraries
#include <cmath>     /* std math */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries

namespace adapted
{

	std::vector<std::string> integers {"0","1","2","3","4","5","6","7","8","9"};

	template<typename id>
	class IdStrings
	{

		const std::vector<std::string> characters;

	public:
		IdStrings(std::vector<std::string> characters):
			characters(characters)
		{}
		IdStrings():
			characters(integers)
		{}

		std::string legend(const id sample, const id lo, const id hi) const 
		{
			std::string out("");
			for (unsigned int i = 0; i < characters.size(); ++i)
			{
				out += characters[i];
				out += " = ";
				out += std::to_string(i);
				out += "\n";
			}
			return out;
		}

		std::string character(const id a, const id lo, const id hi) const 
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
			    return characters[
			    	std::clamp(int(a), 0, int(characters.size()-1))
		    	];
			}
		}

	};

}


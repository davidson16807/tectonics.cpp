#pragma once

// C libraries

// std libraries
#include <string>

// in-house libraries

namespace adapted
{

	struct BooleanStrings
	{
		BooleanStrings(){}

		inline std::string legend(bool sample, const bool lo, const bool hi) const 
		{
			std::string out("");
			out += "true  : █";
			out += "\n";
			out += "false :  ";
			out += "\n";
			return out;
		}

		inline std::string character(const bool a, const bool lo, const bool hi) const 
		{
			return a? "█":" ";
		}

	};

}


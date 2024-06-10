#pragma once

// C libraries

// in-house libraries

namespace adapter
{

	class BooleanStrings
	{
		ElementStrings elements;
	public:
		BooleanStrings(){}

		template<>
		inline std::string legend(bool sample, const bool lo, const bool hi) const 
		{
			std::string out("");
			out += "true  : █";
			out += "\n";
			out += "false :  ";
			out += "\n";
			return out;
		}

		template<>
		inline std::string character(const bool a, const bool lo, const bool hi) const 
		{
			return a? "█":" ";
		}

	};

}


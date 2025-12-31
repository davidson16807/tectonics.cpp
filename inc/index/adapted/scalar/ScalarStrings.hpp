#pragma once

// C libraries
#include <cmath>     /* std math */
#include <algorithm> /* std::clamp */
#include <vector>    /* std::vector */
#include <string>    /* std::string */

// in-house libraries
#include <math/special.hpp>

namespace adapted
{

	inline std::vector<std::string> dots {" ", "⋅", "⁚", "⸫", "⸬", "⁙"};
	inline std::vector<std::string> shades {" ", "░", "▒", "▓", "█"  };
	inline std::vector<std::string> dotshades {" ", "⋅", "⁚", "⁖", "⸬", "⁙", "░", "▒", "▓", "█"  };
	inline std::vector<std::string> numeric {"0","1","2","3","4","5","6","7","8","9"};
	inline std::vector<std::string> alphabetic {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
	inline std::vector<std::string> alphanumeric {
		"0","1","2","3","4","5","6","7","8","9", 
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
	};
	// for when you absolutely, positively, need to distinguish a lot of values...
	inline std::vector<std::string> matrix { 
		"0","1","2","3","4","5","6","7","8","9", 
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"ア","イ","ウ","エ","オ",
		"カ","キ","ク","ケ","コ",
		"サ","シ","ス","セ","ソ",
		"タ","チ","ツ","テ","ト",
		"ナ","ニ","ヌ","ネ","ノ",
		"ハ","ヒ","フ","ヘ","ホ",
		"マ","ミ","ム","メ","モ",
		"ヤ","ユ","ヨ",
		"ラ","リ","ル","レ","ロ",
		"ワ","ヰ","ヱ","ヲ", "ン"
	};

	template<typename scalar>
	class ScalarStrings
	{

		const std::vector<std::string> characters;

	public:
		ScalarStrings(std::vector<std::string> characters):
			characters(characters)
		{}
		ScalarStrings():
			characters(dotshades)
		{}

		std::string legend(const scalar sample, const scalar lo, const scalar hi) const 
		{
			std::string out("");
			for (unsigned int i = 0; i < characters.size(); ++i)
			{
				auto bound = math::mix(lo, hi, scalar(i)/scalar(characters.size()));
				out += characters[i];
				out += " ≥ ";
				out += std::to_string(bound);
				out += "\n";
			}
			out += "  ≤ ";
			out += std::to_string(hi);
			out += "\n";
			return out;
		}

		std::string character(const scalar a, const scalar lo, const scalar hi) const 
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
			    	std::clamp(int((characters.size()) * math::linearstep(lo, hi, a)), 
			    				0, int(characters.size()-1))
		    	];
			}
		}

	};

}


#pragma once

// C libraries
#include <cmath>     /* std math */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries
#include <unit/si.hpp>

namespace adapted
{

	namespace {
		std::vector<std::string> si_dotshades {" ", "⋅", "⁚", "⁖", "⸬", "⁙", "░", "▒", "▓", "█"  };
	}

	class SiStrings
	{
		using string = std::string;

		const std::vector<std::string> characters;

	public:
		SiStrings(std::vector<std::string> characters):
			characters(characters)
		{}
		SiStrings():
			characters(si_dotshades)
		{}

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		string legend(
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> sample, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> lo, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> hi
		) const {
			string out("");
			for (unsigned int i = 0; i < characters.size(); ++i)
			{
				auto bound = si::mix(lo, hi, T1(i)/T1(characters.size()));
				out += characters[i];
				out += " ≥ ";
				out += si::to_string(bound);
				out += "\n";
			}
			return out;
		}

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		string character(
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> lo, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> hi
		) const {
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
			    return characters[
			    	std::clamp(int((characters.size()) * si::linearstep(lo, hi, a)), 
			    				0, int(characters.size()-1))
		    	];
			}
		}

	};

}


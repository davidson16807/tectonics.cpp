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

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		string legend(
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> sample, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> lo, 
			const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> hi
		) const {
			string out("");
			for (unsigned int i = 0; i < shades.size(); ++i)
			{
				auto bound = si::mix(lo, hi, T1(i)/T1(shades.size()));
				out += shades[i];
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
			    return shades[
			    	std::clamp(int((shades.size()) * si::linearstep(lo, hi, a)), 
			    				0, int(shades.size()-1))
		    	];
			}
		}

	};

}


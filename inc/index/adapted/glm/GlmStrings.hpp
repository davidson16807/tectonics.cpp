#pragma once

// C libraries
#include <cmath>     /* std math */
#include <array>     /* std::array */
#include <algorithm> /* std::clamp */

// in-house libraries

namespace adapted
{

	class GlmStrings
	{
		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 16> 
		#endif
		weak 
		{
			"→","↗","↗","↑","↑","↖","↖","←","←","↙","↙","↓","↓","↘","↘","→"
		};
		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 16> 
		#endif
		strong 
		{
			"➡","⬈","⬈","⬆","⬆","⬉","⬉","⬅","⬅","⬋","⬋","⬇","⬇","⬊","⬊","➡"
		};

	public:
		GlmStrings(){}

		template<int L, typename T, glm::qualifier Q>
		auto magnitude(const glm::vec<L,T,Q> V){
			return glm::length(V);
		}

		template<int L, typename T, glm::qualifier Q>
		std::string legend(const glm::vec<L,T,Q> sample, const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi) const {
			std::string out("");
			out += "|→| ≥ ";
			out += std::to_string(glm::length(hi) * T(1./3.));
			out += "\n";
			out += "|➡| ≥ ";
			out += std::to_string(glm::length(hi) * T(2./3.));
			out += "\n";
			return out;
		}

		template<int L, typename T, glm::qualifier Q>
		std::string character(const glm::vec<L,T,Q> V, const glm::vec<L,T,Q> lo, const glm::vec<L,T,Q> hi) const {
			if (std::isnan(V.x) || std::isnan(V.y))
			{
				return "N";
			}
			else if (std::isinf(V.x) || std::isinf(V.y))
			{
				return "∞";
			}
			else if (glm::length(V) < glm::length(hi) * T(1./3.))
			{
				return " ";
			}
			else if (glm::length(V) < glm::length(hi) * T(2./3.))
			{
				T turn = std::atan2(V.y, V.x)/(2.*math::pi);
				int i  = std::clamp(int(weak.size()*turn), 0, int(weak.size()-1));
				return weak[i];
			}
			else
			{
				T turn = std::atan2(V.y, V.x)/(2.*math::pi);
				int i  = std::clamp(int(strong.size()*turn), 0, int(strong.size()-1));
				return strong[i];
			}
		}


	};

}


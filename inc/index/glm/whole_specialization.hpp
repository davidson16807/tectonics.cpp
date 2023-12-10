#pragma once

#include <cmath>       // isnan, isinf
#include <string>      // string
#include <algorithm>   // clamp
#include <type_traits> // enable_if_t

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/common.hpp>	// all the GLSL common functions: abs, floor, etc.

#include <math/glm/special_specialization.hpp>
#include <math/special.hpp>

#include "each.hpp"

namespace whole {

	#if defined(__clang__)
		const std::vector<const std::string> 
	#else
		const std::array<const std::string, 16> 
	#endif
	weak 
	{
		"←","↙","↙","↓","↓","↘","↘","→","→","↗","↗","↑","↑","↖","↖","←"
	};
	#if defined(__clang__)
		const std::vector<const std::string> 
	#else
		const std::array<const std::string, 16> 
	#endif
	strong 
	{
		"⬅","⬋","⬋","⬇","⬇","⬊","⬊","➡","➡","⬈","⬈","⬆","⬆","⬉","⬉","⬅"
	};

	template<int L, typename T, glm::qualifier Q>
	auto magnitude(const glm::vec<L,T,Q> V){
		return glm::length(V);
	}

	template<int L, typename T, glm::qualifier Q>
	std::string character(glm::vec<L,T,Q> V, const T lo, const T hi){
		if (std::isnan(V.x) || std::isnan(V.y))
		{
			return "N";
		}
		else if (std::isinf(V.x) || std::isinf(V.y))
		{
			return "∞";
		}
		else if (glm::length(V) < hi * T(1./3.))
		{
			return " ";
		}
		else if (glm::length(V) < hi * T(2./3.))
		{
			T turn = (std::atan2(V.y, V.x)+math::pi)/(2.*math::pi);
			int i  = std::clamp(int(weak.size()*turn), 0, int(weak.size()-1));
			return weak[i];
		}
		else
		{
			T turn = (std::atan2(V.y, V.x)+math::pi)/(2.*math::pi);
			int i  = std::clamp(int(strong.size()*turn), 0, int(strong.size()-1));
			return strong[i];
		}
	}

	template<int L, typename T, glm::qualifier Q>
	std::string legend(glm::vec<L,T,Q> sample, const T lo, const T hi){
		std::string out("");
		out += "|→| ≥ ";
		out += std::to_string(hi * T(1./3.));
		out += "\n";
		out += "|➡| ≥ ";
		out += std::to_string(hi * T(2./3.));
		out += "\n";
		return out;
	}

}//namespace whole




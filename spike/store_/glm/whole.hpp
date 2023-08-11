#pragma once

#include <cmath>  // isnan, isinf
#include <string>

#include <glm/common.hpp>	// all the GLSL common functions: abs, floor, etc.

#include "../types.hpp"

namespace series
{

	// Converts from vec3s to flattened floats
	template <glm::length_t L, typename T, glm::qualifier Q>
	void flatten (Series<glm::vec<L,T,Q>>& a, Series<T>& output) {
		assert(a.size()*L == output.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			output[3*i+0] = a[i].x;
			output[3*i+1] = a[i].y;
			output[3*i+2] = a[i].z;
		}
	}
	// Converts from flattened floats to vec3s
	template <glm::length_t L, typename T, glm::qualifier Q>
	void unflatten (Series<T>& a, Series<glm::vec<L,T,Q>>& output) {
		assert(output.size()*L == a.size());
	    for (unsigned int i = 0; i < output.size(); ++i)
		{
			output[i].x = a[3*i+0];
			output[i].y = a[3*i+1];
			output[i].z = a[3*i+2];
		}
	}


	namespace 
	{
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
	} 

	template <typename T, glm::qualifier Q>
	std::string to_string(const Series<glm::vec<2,T,Q>>& a, const int line_char_width = 80)
	{
		Series<T> length_a(a.size());
		series::length(a, length_a);
		T max_length_a = series::max(length_a);

		std::string out("");
		for (unsigned int i = 0; i < a.size(); ++i)
		{
		    if (i % line_char_width == 0)
		    {
		    	out += "\n";
		    }

			if (std::isnan(a[i].x) || std::isnan(a[i].y))
			{
				out += "N";
			}
			else if (std::isinf(a[i].x) || std::isinf(a[i].y))
			{
				out += "∞";
			}
			else if (glm::length(a[i]) < max_length_a * T(1./3.))
			{
				out += " ";
			}
			else if (glm::length(a[i]) < max_length_a * T(2./3.))
			{
				T turn = (std::atan2(a[i].y, a[i].x)+M_PI)/(2.*M_PI);
				int i  = std::clamp(int(weak.size()*turn), 0, int(weak.size()-1));
				out   += weak[i];
			}
			else
			{
				T turn = (std::atan2(a[i].y, a[i].x)+M_PI)/(2.*M_PI);
				int i  = std::clamp(int(strong.size()*turn), 0, int(strong.size()-1));
				out   += strong[i];
			}
		}
		out += "\n";
		out += "|→| ≥ ";
		out += std::to_string(max_length_a * T(1./3.));
		out += "\n";
		out += "|➡| ≥ ";
		out += std::to_string(max_length_a * T(2./3.));
		out += "\n";
		return out;
	}

	template <typename T, glm::qualifier Q>
	std::string to_string(const Series<glm::vec<3,T,Q>>& a, const glm::mat<3,2,T,Q>& basis, const int line_char_width = 80)
	{
		Series<glm::vec<2,T,Q>> a2d(a.size());
		series::mult(basis, a, a2d);
		return series::to_string(a2d, line_char_width);
	}

}//namespace series




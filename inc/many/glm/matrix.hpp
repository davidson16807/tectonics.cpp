#pragma once

#include "../types.hpp"

namespace many{
	template<glm::length_t R, glm::length_t C, typename T, glm::qualifier Q>
	void mult(const glm::mat<R,C,T,Q>& A, const tmany<glm::vec<R,T,Q>>& b, tmany<glm::vec<C,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = A * b[i];
		}
	}
	template<glm::length_t R, glm::length_t C, typename T, glm::qualifier Q>
	void mult(const tmany<glm::vec<R,T,Q>>& a, const glm::mat<R,C,T,Q>& B, tmany<glm::vec<C,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = B * a[i];
		}
	}


	template<typename T, glm::qualifier Q>
	void mult(const tmany<glm::vec<3,T,Q>>& a, const glm::mat<4,3,T,Q>& B, tmany<glm::vec<3,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = B * vec4(a[i], one);
		}
	}
	template<typename T, glm::qualifier Q>
	void mult(const glm::mat<4,3,T,Q>& A, const tmany<glm::vec<3,T,Q>>& b, tmany<glm::vec<3,T,Q>>& out)
	{
		constexpr T one = T(1.);
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = A * vec4(b[i], one);
		}
	}
}
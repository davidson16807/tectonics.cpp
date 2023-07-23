#pragma once

#include "../types.hpp"

namespace series
{
	template<glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L,T,Q> weighted_average(const Series<glm::vec<L,T,Q>>& a, const Series<T>& weights)
	{
		glm::vec<L,T,Q> out = glm::vec<L,T,Q>(0);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out += a[i] * weights[i];
		}
		out /= sum(weights);
		return out;
	}
	// TODO: vector version
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline void rescale(const Series<glm::vec<L,T,Q>>& a, Series<glm::vec<L,T,Q>>& out, T max_new = 1.)
	{
		mult(a, max_new / max(length(a)), out);
	}
	
} //namespace series

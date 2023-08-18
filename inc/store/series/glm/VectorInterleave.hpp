#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace series
{

	/*
	Given a `Series`: ℕ→ℝ, a `VectorInterleave` maps: ℕ→ℝᴸ.
	*/
	template<int L, typename Series, glm::qualifier Q=glm::highp>
	struct VectorInterleave
	{
	    using R = typename Series::value_type;
	    using size_type = std::size_t;
		using value_type = glm::vec<L,R,Q>;

		Series values;

		constexpr inline explicit VectorInterleave(const Series& values) : 
			values(values)
		{}

		constexpr inline size_type size() const { return std::max(size_type(1),values.size()/L); }

		constexpr value_type operator()(const size_type memory_id ) const
		{
			glm::vec<L,R,Q> out;
			for (int i = 0; i < L; ++i)
			{
				out[i] = values[L*memory_id+i];
			}
			return out;
		}

		constexpr value_type operator[](const size_type memory_id ) const
		{
			glm::vec<L,R,Q> out;
			for (int i = 0; i < L; ++i)
			{
				out[i] = values[L*memory_id+i];
			}
			return out;
		}

	};

	template<int L, typename Series, glm::qualifier Q=glm::highp>
	inline VectorInterleave<L,Series,Q> vector_interleave(const Series& series)
	{
		return VectorInterleave<L,Series,Q>(series);
	}

}


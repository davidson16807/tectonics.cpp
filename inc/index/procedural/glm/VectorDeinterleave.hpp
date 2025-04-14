#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace procedural
{

	/*
	Given a `Series`: ℕ→ℝ, a `VectorDeinterleave` maps: ℕ→ℝᴸ.
	*/
	template<int L, typename Series, glm::qualifier Q=glm::highp>
	struct VectorDeinterleave
	{
		using vec4 = typename Series::value_type;
	    using size_type = std::size_t;
		using value_type = typename vec4::value_type;

		static constexpr unsigned int dimension_count = 4;

		Series v;

		constexpr inline explicit VectorDeinterleave(const Series& v) : v(v)
		{}

		constexpr inline size_type size() const { return L*v.size(); }

		constexpr value_type operator()(const size_type i ) const
		{
			return v[i/L][i%L];
		}
		constexpr value_type operator[](const size_type i ) const
		{
			return v[i/L][i%L];
		}

	};

	template<int L, typename Series, glm::qualifier Q=glm::highp>
	inline VectorDeinterleave<L,Series,Q> vector_deinterleave(const Series& series)
	{
		return VectorDeinterleave<L,Series,Q>(series);
	}

}


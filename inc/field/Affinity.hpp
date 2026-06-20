#pragma once

// C libraries
#include <cstddef>

// 3rd party libraries
#include <glm/matrix.hpp>

namespace field
{

	/*
	Given a `Series`: ℕ→ℝ, a `VectorZip` maps: ℕ→ℝᴸ.
	*/
	template<int R, int C, typename T, glm::qualifier Q=glm::defaultp>
	struct Affinity
	{
	    using size_type = std::size_t;
		using value_type = glm::vec<C,T,Q>;
		glm::mat<C,R,T,Q> A;
		constexpr inline explicit Affinity(const glm::mat<C,R,T,Q>& A) : 
			A(A)
		{}
		constexpr inline size_type size() const { return 1; }
		[[nodiscard]] constexpr inline value_type operator()(const glm::vec<R,T,Q>& v ) const
		{
			return A*v;
		}

	};

}


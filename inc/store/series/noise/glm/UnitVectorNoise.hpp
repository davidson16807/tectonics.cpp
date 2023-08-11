#pragma once

// C libraries
#include <cmath>     /* std::clamp */

// 3rd party libraries
#include <glm/geometric.hpp>

// in-house libraries
#include <math/special.hpp>
#include <store/series/noise/UnitIntervalNoise.hpp>

namespace series
{

	/*
	Given a `UnitInterval` map: ℕ→[0,1], `UnitVector3Interleave` 
	maps ℕ to L-dimensional unit vectors that are evenly distributed 
	over the set of all such vectors.
	*/
	template<int L, typename UnitInterval, glm::qualifier Q=glm::highp>
	struct UnitVectorNoise
	{
	    using R = typename UnitInterval::value_type;

	    using size_type = std::size_t;
		using value_type = glm::vec<L,R,Q>;

		UnitInterval quantile;
		R epsilon;

		constexpr inline explicit UnitVectorNoise(const UnitInterval& quantile) : 
			quantile(quantile),
			epsilon(1e-7)
		{}

		// copy constructor
		constexpr inline UnitVectorNoise(const UnitVectorNoise<L,UnitInterval,Q>& a):
			quantile(a.quantile),
			epsilon(1e-7)
		{}

		constexpr inline size_type size() const { return std::max(1,quantile.size()/L); }

		constexpr value_type operator[](const size_type memory_id ) const
		{
	        glm::vec<L,R,Q> out;
	        for (int i = 0; i < L; ++i)
	        {
	        	out[i] = math::probit(std::clamp(quantile[(L*memory_id)+i], epsilon, R(1)-epsilon));
	        }
	        return glm::normalize(out);
		}

	};

	template<int L, typename UnitInterval, glm::qualifier Q=glm::highp>
	constexpr inline auto unit_vector_interleave(const UnitInterval& quantile)
	{
		return UnitVectorNoise<L,UnitInterval,Q>(quantile);
	}

	template<int L, typename T, glm::qualifier Q=glm::highp>
	constexpr inline auto unit_vector_noise(const T a = T(10), const T b=T(10000))
	{
		return unit_vector_interleave<L>(noise(a,b));
	}

}


#pragma once

// C libraries
#include <cmath>     /* std::clamp */

// in-house libraries
#include <math/special.hpp>
#include <index/procedural/noise/UnitIntervalNoise.hpp>

namespace series
{

	/*
	`GaussianNoise` is a map: ℕ→ℝ.
	It represents an indexible object that can participate in functions under `known::` and `procedural::`.
	The value at each index is in the range [0,1], it is not correlated with the value at contiguous indices,
	and it is completely determined by the index together with a user defined `seed`.
	*/
	template<typename UnitInterval>
	struct GaussianNoise
	{
	    using T = typename UnitInterval::value_type;

	    using size_type = std::size_t;
		using value_type = T;

		UnitInterval quantile;
		T epsilon;

		constexpr inline explicit GaussianNoise(const UnitInterval& quantile) : 
			quantile(quantile),
			epsilon(1e-7)
		{}

		constexpr inline size_type size() const { return 1; }

		constexpr inline T operator()(const size_type memory_id ) const
		{
			return math::probit(std::clamp(quantile[memory_id], epsilon, T(1)-epsilon));
		}
		constexpr inline T operator[](const size_type memory_id ) const
		{
			return math::probit(std::clamp(quantile[memory_id], epsilon, T(1)-epsilon));
		}

	};

	/*
	NOTE: constructing `noise::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `GaussianNoise` here.
	*/
	template<typename UnitInterval>
	constexpr inline auto gaussian(const UnitInterval& quantile)
	{
		return GaussianNoise<UnitInterval>(quantile);
	}

	template<typename T>
	constexpr inline auto gaussian(const T a, const T b)
	{
		return GaussianNoise<UnitIntervalNoise<T>>(UnitIntervalNoise<T>(a,b));
	}


}


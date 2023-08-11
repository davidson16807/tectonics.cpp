#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace series
{

	/*
	`UnitIntervalNoise` is a map: ℕ→[0,1].
	It represents an indexible object that can participate in functions under `each::` and `store::`.
	The value at each index is in the range [0,1], it is not correlated with the value at contiguous indices,
	and it is completely determined by the index together with a user defined `seed`.
	*/
	template<typename T>
	struct UnitIntervalNoise
	{
	    using size_type = std::size_t;
		using value_type = T;

		T a;
		T b;

		constexpr inline explicit UnitIntervalNoise(const T a = T(10), const T b=T(10000)) : 
			a(a),
			b(b)
		{}

		// copy constructor
		constexpr inline UnitIntervalNoise(const UnitIntervalNoise& a) :
			a(a.a),
			b(a.b)
		{}

		constexpr inline size_type size() const { return 1; }

		constexpr inline T operator[](const size_type memory_id ) const
		{
			auto c = sin(T(memory_id)*a)*b;
		    return c - floor(c);
		}

	};

	/*
	NOTE: constructing `noise::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `UnitIntervalNoise` here.
	*/
	template<typename T>
	constexpr inline UnitIntervalNoise<T> noise(const T a = T(10), const T b=T(10000))
	{
		return UnitIntervalNoise<T>(a,b);
	}


}


#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries

namespace series
{

	/*
	`Noise` represents an indexible object that can particlate in functions under `each::` and `store::`.
	The value at each index is completely determined by the index together with a user defined `seed`.
	*/
	template<typename T>
	struct Noise
	{
		float a;
		float b;

		constexpr inline explicit Noise(const float a = 10.f, const float b=10000.f) : 
			a(a),
			b(b)
		{}

		// copy constructor
		constexpr inline Noise(const Noise& a) :
			a(a.a),
			b(a.b)
		{}

	    using size_type = std::size_t;
		using value_type = T;
		using const_reference = const T&;
		using reference = T&;
		constexpr inline size_type size() const { return 1; }

		constexpr inline T operator[](const size_type memory_id ) const
		{
			auto c = std::sin(memory_id*a)*b;
		    return c - std::floor(c);
		}

	};

	/*
	NOTE: constructing `series::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Noise` here.
	*/
	template<typename T>
	constexpr inline Noise<T> noise(const T& seed)
	{
		return Noise<T>(seed);
	}


}


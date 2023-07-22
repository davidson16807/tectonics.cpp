#pragma once

// C libraries

// std libraries

namespace series
{

	/*
	`Range` represents an indexible object that can particlate in functions under `each::` and `store::`.
	The value at each index is the index itself.
	*/
	template<typename T>
	struct Range
	{

	    using size_type = std::size_t;
		using value_type = T;
		using const_reference = const T&;
		using reference = T&;
		constexpr inline size_type size() const { return 1; }

		constexpr inline T operator[](const size_type memory_id ) const
		{
			return T(memory_id);
		}

	};

	/*
	NOTE: constructing `series::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Range` here.
	*/
	template<typename T>
	constexpr inline Range<T> range()
	{
		return Range<T>();
	}


}


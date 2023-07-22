#pragma once

// C libraries

// std libraries

namespace series
{


	/*
	`Uniform` packages a single value of arbitrary data type so that
	it may operate as an indexible data type similar to `Series` and `Raster`.
	For any index given, its indexing operation will always return a single value.
	`Uniform` is used to reduce the number of function overloads that must be implemented 
	for combinations of series, rasters, and scalar values.
	*/
	template<typename T>
	struct Uniform
	{
		T value;

		constexpr inline explicit Uniform(const T& value) : 
			value(value) 
		{}

		// copy constructor
		constexpr inline Uniform(const Uniform& a) :
			value(a.value)
		{}

		constexpr inline std::size_t size() const { return 1; }
	    using size_type = std::size_t;
		using value_type = T;
		using const_reference = const T&;
		using reference = T&;

		constexpr inline T operator[](const std::size_t memory_id ) const
		{
		   return value;
		}

		constexpr inline void fill(const T a )
		{
			value = a;
		}

	};

	/*
	NOTE: constructing `series::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for Uniform here.
	*/
	template<typename T>
	constexpr inline Uniform<T> uniform(const T& value)
	{
		return Uniform<T>(value);
	}


}


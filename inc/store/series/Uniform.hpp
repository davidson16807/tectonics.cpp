#pragma once

// C libraries

// std libraries

namespace series
{


	/*
	`Uniform` is a map: ℕ→𝕋.
	It packages a single value of arbitrary data type so that
	it may operate as an indexible data type similar to `Series` and `Raster`.
	For any index given, its indexing operation will always return a single value.
	`Uniform` is used to reduce the number of function overloads that must be implemented 
	for combinations of series, rasters, and scalar values.
	*/
	template<typename T>
	struct Uniform
	{
	    using size_type = std::size_t;
		using value_type = T;
		using const_reference = const T&;
		using reference = T&;

		T value;
		size_type reported_size;

		constexpr inline Uniform(const Uniform<T>& uniform): 
			value(uniform.value),
			reported_size(uniform.reported_size)
		{}
		constexpr inline explicit Uniform(const T value): 
			value(value),
			reported_size(1)
		{}
		constexpr inline explicit Uniform(const T value, const size_type reported_size): 
			value(value),
			reported_size(reported_size)
		{}

		constexpr inline std::size_t size() const { return 1; }

		constexpr inline const_reference operator()(const size_type memory_id ) const
		{
		   return value;
		}

		constexpr inline const_reference operator[](const size_type memory_id ) const
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


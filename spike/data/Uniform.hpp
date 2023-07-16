#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

namespace data
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
		explicit Uniform(const T& value) :
			value(value) 
		{
		}

		// copy constructor
		Uniform(const Uniform& a) :
			value(a.value)
		{}

		inline T operator[](const std::size_t id ) const
		{
		   return value;
		}

		template <typename T2>
		inline void fill(const T2 a )
		{
			value = a;
		}

		template<typename T2>
		inline std::size_t size(const T2& v) const {
			return v.size();
		}

		template<typename T2>
		inline T2 footprint(const T& v) const {
			return v;
		}

	};

	/*
	NOTE: constructing rasters can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`,
	however we forego this convention for brevity since 
	our library must implement template functions that support many combinations of type,
	but we also wish to avoid creating implicit casts.
	*/
	template<typename T>
	Uniform<T> uniform(const T& value)
	{
		return Uniform<T>(value);
	}


}


#pragma once

// C libraries

// std libraries

namespace series
{

	/*
	`Range` is a map: ℕ→ℕ.
	It represents an indexible object that can particlate in functions under `each::` and `store::`.
	The value at each index is the index itself.
	*/
	template<typename T=std::size_t>
	struct Range
	{
	    using size_type = std::size_t;
		using value_type = T;

		T start_value;
		T reported_size;
		constexpr explicit Range(): 
			start_value(0),
			reported_size(1)
		{}
		constexpr explicit Range(const T reported_size): 
			start_value(0),
			reported_size(reported_size)
		{}
		constexpr explicit Range(const T start_value, const T end_value): 
			start_value(start_value),
			reported_size(end_value-start_value)
		{}
		Range(const Range<T>& range): 
			start_value(0),
			reported_size(range.reported_size)
		{}

		constexpr inline bool includes(T i) const { return start_value < i && i < start_value+reported_size; }

		constexpr inline size_type size() const { return reported_size; }

		constexpr inline value_type operator[](const size_type memory_id ) const
		{
			return T(memory_id+start_value);
		}

	};

	/*
	NOTE: constructing `series::` objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`, but we forego this convention for brevity.
	For consistency, we create one such function for `Range` here.
	*/
	constexpr inline Range<std::size_t> range()
	{
		return Range();
	}
	template<typename T>
	constexpr inline Range<T> range(const T reported_size)
	{
		return Range<T>(reported_size);
	}
	template<typename T>
	constexpr inline Range<T> range(const T start_value, const T end_value)
	{
		return Range<T>(start_value, end_value);
	}


}


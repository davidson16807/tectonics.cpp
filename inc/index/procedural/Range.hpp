#pragma once

// C libraries

// std libraries

namespace procedural
{

	/*
	`Range` is a map: ℕ→ℕ.
	It represents an indexible object that can participate in functions under `known::` and `procedural::`.
	The value at each index is the index itself.
	*/
	template<typename T=std::size_t>
	struct Range
	{
	    using size_type = std::size_t;
		using value_type = T;

		T start_value;
		T step_size;
		T reported_size;
		constexpr explicit Range(): 
			start_value(0),
			step_size(1),
			reported_size(1)
		{}
		constexpr explicit Range(const T end_value): 
			start_value(0),
			step_size(end_value>0? 1:-1),
			reported_size((end_value) > T(0)? (end_value) : -(end_value))
		{}
		constexpr explicit Range(const T start_value, const T end_value): 
			start_value(start_value),
			step_size(end_value>start_value? 1:-1),
			reported_size((end_value-start_value) > T(0)? (end_value-start_value) : -(end_value-start_value))
		{}

		constexpr inline size_type size() const { return reported_size; }

		constexpr inline bool includes(T value) const 
		{ 
			auto i = (value - start_value) / step_size;
			return 0 <= i && i < reported_size;
		}

		constexpr inline value_type operator()(const size_type memory_id ) const
		{
			return step_size*T(memory_id)+start_value;
		}

		constexpr inline value_type operator[](const size_type memory_id ) const
		{
			return step_size*T(memory_id)+start_value;
		}

	};

	/*
	NOTE: constructing `procedural::` objects can be annoying due to the number of template parameters involved, 
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


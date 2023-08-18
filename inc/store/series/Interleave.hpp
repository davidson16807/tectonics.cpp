#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <vector>           // std::vector
#include <memory>

namespace series
{

	/*
	`Interleave` is a thin wrapper for a container of type Series that permits interleaving.
	Interleaving is accomplished using a map: "lookup_id ⟶ memory_id",
	that allows multiple sequential lookup_ids to map to the same memory_id,
	and allows value_ids to be repeatedly looped over as the modulo of vector size.
	The number of sequential lookup_ids that correspond to a single memory_id is given by `copies_per_value`.
	The number of times that value_ids may be looped over is indefinite.
	See the `operator[]()` overload for more details.
	*/
	template <typename Series>
	struct Interleave
	{

	    using size_type = std::size_t;
		using value_type = typename Series::value_type;

		Series values;
		std::size_t copies_per_value;

		explicit Interleave(const Series& series) : 
			values(series),
			copies_per_value(1)
		{
		}

		explicit Interleave(const std::size_t copies_per_value, Series& series) : 
			values(series),
			copies_per_value(copies_per_value)
		{
		}


		// NOTE: all wrapper functions should to be marked inline 
		inline size_type size() const     { return values.size() * copies_per_value;  }
		inline size_type max_size() const { return values.size() * copies_per_value;  }
		inline size_type capacity() const { return values.capacity() * copies_per_value; }
		inline size_type empty() const    { return values.empty(); }

		inline auto operator()(const size_type lookup_id ) const
		{
		   return values[(lookup_id / copies_per_value) % values.size()];
		}

		inline auto operator[](const size_type lookup_id ) const
		{
		   return values[(lookup_id / copies_per_value) % values.size()];
		}

		template<typename Tid>
		inline Interleave<Series> operator[](const Interleave<Tid>& ids )
		{
			Interleave<Series> out = Interleave<Series>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Interleave<Series>& operator=(const Interleave<Series>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline Interleave<Series>& operator=(const Series& other )
		{
			values.resize(other.size());
			fill(*this, other);
			return *this;
		}

        template<typename T2>
		constexpr bool compatible(const T2& a) const
		{
        	return
        		size() >= a.size()&&a.size() > 0 &&
        		size() % a.size() == 0;
		}

        template<typename T2>
        constexpr Interleave<Series>& operator+=(const T2& a)
        {
        	assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] += a[i];
			}
            return *this;
        }

        template<typename T2>
		constexpr Interleave<Series>& operator-=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] -= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator*=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] *= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator/=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] /= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator%=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] %= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator&=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] &= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator|=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] |= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<Series>& operator^=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] ^= a[i];
			}
            return *this;
		}


		template <typename T2>
		void fill(const T2 a)
		{
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] = a;
			}
		}


	};

	/*
	NOTE: constructing series objects can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`,
	however we forego this convention for brevity since 
	our library must implement template functions that support many combinations of type,
	but we also wish to avoid creating implicit casts.
	*/

	template<typename Series>
	inline Interleave<Series> interleave(const Series series)
	{
		return Interleave<Series>(series);
	}

	template<typename Series>
	inline Interleave<Series> interleave(const std::size_t copies_per_value, Series series)
	{
		return Interleave<Series>(copies_per_value, series);
	}

	template<typename Series>		
	inline Interleave<Series> reshape(const Interleave<Series>& a, const std::size_t copies_per_value, Interleave<Series>& out)
	{
		if (out.front() != a.front())
		{
			std::copy(a.begin(), a.end(), a.begin());
		}
		out.copies_per_value = copies_per_value;
	}

}


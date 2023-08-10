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

	protected:
		Series values;
		std::size_t copies_per_value;

	public:

		// copy constructor
		Interleave(const Interleave<Series>& a)  : 
			values(a.values),
			copies_per_value(1)
		{}

		// initializer list constructor
		template <typename T2>
		explicit Interleave(std::initializer_list<T2> list) : 
			values(list),
			copies_per_value(1)
		{
		}

		// std container style constructor
		template<typename TIterator>
		explicit Interleave(TIterator first, TIterator last) : 
			values(first, last),
			copies_per_value(1)
		{
		}

		explicit Interleave(const Series& vector) : 
			values(vector),
			copies_per_value(1)
		{
		}



		// initializer list constructor
		template <typename T2>
		explicit Interleave(const std::size_t copies_per_value, std::initializer_list<T2> list) : 
			values(list),
			copies_per_value(copies_per_value)
		{
		}
		// std container style constructor
		template<typename TIterator>
		explicit Interleave(const std::size_t copies_per_value, TIterator first, TIterator last) : 
			values(first, last),
			copies_per_value(copies_per_value)
		{
		}

		explicit Interleave(const std::size_t copies_per_value, Series& vector) : 
			values(vector),
			copies_per_value(copies_per_value)
		{
		}


		// NOTE: all wrapper functions should to be marked inline 
	    using size_type = std::size_t;
		using value_type = typename Series::value_type;
		using const_reference = const value_type&;
		using reference = value_type&;
		using const_iterator = typename Series::const_iterator;
		using iterator = typename Series::iterator;
		inline size_type size() const     { return values.size() * copies_per_value;  }
		inline size_type max_size() const { return values.size() * copies_per_value;  }
		inline size_type capacity() const { return values.capacity() * copies_per_value; }
		inline size_type empty() const    { return values.empty(); }
        inline typename Interleave<Series>::reference front()             { return values.front(); }
        inline typename Interleave<Series>::const_reference front() const { return values.front(); }
        inline typename Interleave<Series>::reference back()              { return values.back();  }
        inline typename Interleave<Series>::const_reference back() const  { return values.back();  }
	    inline typename Interleave<Series>::const_iterator begin() const  { return values.begin(); }
	    inline typename Interleave<Series>::const_iterator end()   const  { return values.end();   }
	    inline typename Interleave<Series>::iterator begin()              { return values.begin(); }
	    inline typename Interleave<Series>::iterator end()                { return values.end();   }

		inline const_reference operator[](const size_type lookup_id ) const
		{
		   return values.operator[]((lookup_id / copies_per_value) % values.size());
		}

		inline reference operator[](const size_type lookup_id )
		{
		   return values[(lookup_id / copies_per_value) % values.size()]; // reference return 
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
	inline Interleave<Series> interleave(const Series vector)
	{
		return Interleave<Series>(vector);
	}
	template<typename Series>
	inline Interleave<std::vector<Series>> interleave(const std::initializer_list<Series>& list)
	{
		const std::vector<Series> v(list);
		return Interleave<std::vector<Series>>(v);
	}
	template<typename Series>		// convenience constructor for vectors
	inline Interleave<std::vector<Series>> interleave(const std::size_t N, const Series a)
	{
		return Interleave<std::vector<Series>>(std::vector<Series>(N, a));
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


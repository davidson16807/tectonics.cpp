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
	`Interleave` is a thin wrapper for a container of type T that permits interleaving.
	Interleaving is accomplished using a map: "lookup_id ⟶ memory_id",
	that allows multiple sequential lookup_ids to map to the same memory_id,
	and allows value_ids to be repeatedly looped over as the modulo of vector size.
	The number of sequential lookup_ids that correspond to a single memory_id is given by `copies_per_value`.
	The number of times that value_ids may be looped over is indefinite.
	See the `operator[]()` overload for more details.
	*/
	template <typename T>
	struct Interleave
	{

	protected:
		T values;
		std::size_t copies_per_value;

	public:

		// copy constructor
		Interleave(const Interleave<T>& a)  : 
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

		explicit Interleave(const T& vector) : 
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

		explicit Interleave(const std::size_t copies_per_value, T& vector) : 
			values(vector),
			copies_per_value(copies_per_value)
		{
		}


		// NOTE: all wrapper functions should to be marked inline 
	    using size_type = std::size_t;
		using value_type = typename T::value_type;
		using const_reference = const value_type&;
		using reference = value_type&;
		using const_iterator = typename T::const_iterator;
		using iterator = typename T::iterator;
		inline size_type size() const     { return values.size() * copies_per_value;  }
		inline size_type max_size() const { return values.size() * copies_per_value;  }
		inline size_type capacity() const { return values.capacity() * copies_per_value; }
		inline size_type empty() const    { return values.empty(); }
        inline typename Interleave<T>::reference front()             { return values.front(); }
        inline typename Interleave<T>::const_reference front() const { return values.front(); }
        inline typename Interleave<T>::reference back()              { return values.back();  }
        inline typename Interleave<T>::const_reference back() const  { return values.back();  }
	    inline typename Interleave<T>::const_iterator begin() const  { return values.begin(); }
	    inline typename Interleave<T>::const_iterator end()   const  { return values.end();   }
	    inline typename Interleave<T>::iterator begin()              { return values.begin(); }
	    inline typename Interleave<T>::iterator end()                { return values.end();   }

		inline const_reference operator[](const size_type lookup_id ) const
		{
		   return values.operator[]((lookup_id / copies_per_value) % values.size());
		}

		inline reference operator[](const size_type lookup_id )
		{
		   return values[(lookup_id / copies_per_value) % values.size()]; // reference return 
		}

		template<typename Tid>
		inline Interleave<T> operator[](const Interleave<Tid>& ids )
		{
			Interleave<T> out = Interleave<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Interleave<T>& operator=(const Interleave<T>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline Interleave<T>& operator=(const T& other )
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
        constexpr Interleave<T>& operator+=(const T2& a)
        {
        	assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] += a[i];
			}
            return *this;
        }

        template<typename T2>
		constexpr Interleave<T>& operator-=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] -= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator*=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] *= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator/=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] /= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator%=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] %= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator&=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] &= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator|=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] |= a[i];
			}
            return *this;
		}

		template<typename T2>
		constexpr Interleave<T>& operator^=(const T2& a)
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

	template<typename T>
	inline Interleave<T> interleave(const T vector)
	{
		return Interleave<T>(vector);
	}
	template<typename T>
	inline Interleave<std::vector<T>> interleave(const std::initializer_list<T>& list)
	{
		const std::vector<T> v(list);
		return Interleave<std::vector<T>>(v);
	}
	template<typename T>		// convenience constructor for vectors
	inline Interleave<std::vector<T>> interleave(const std::size_t N, const T a)
	{
		return Interleave<std::vector<T>>(std::vector<T>(N, a));
	}


	template<typename T>		
	inline Interleave<T> reshape(const Interleave<T>& a, const std::size_t copies_per_value, Interleave<T>& out)
	{
		if (out.front() != a.front())
		{
			std::copy(a.begin(), a.end(), a.begin());
		}
		out.copies_per_value = copies_per_value;
	}

}


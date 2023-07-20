#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <vector>           // std::vector
#include <memory>

#include "Uniform.hpp"

namespace data
{

	/*
	`Series` is a thin wrapper for a std::vector that permits interleaving.
	Interleaving is accomplished using a map: "lookup_id ⟶ memory_id",
	that allows multiple sequential lookup_ids to map to the same memory_id,
	and allows value_ids to be repeatedly looped over as the modulo of vector size.
	The number of sequential lookup_ids that correspond to a single memory_id is given by `copies_per_value`.
	The number of times that value_ids may be looped over is indefinite.
	See the `operator[]()` overload for more details.
	*/
	template <typename T>
	struct Series
	{

	protected:
		std::vector<T> values;
		std::size_t copies_per_value;

	public:

		// copy constructor
		Series(const Series<T>& a)  : 
			values(a.values),
			copies_per_value(1)
		{}

		// initializer list constructor
		template <typename T2>
		explicit Series(std::initializer_list<T2> list) : 
			values(list),
			copies_per_value(1)
		{
		}

		// std container style constructor
		template<typename TIterator>
		explicit Series(TIterator first, TIterator last) : 
			values(first, last),
			copies_per_value(1)
		{
		}

		// convenience constructor for vectors
		explicit Series(std::vector<T>& vector) : 
			values(vector),
			copies_per_value(1)
		{
		}

		constexpr explicit Series(const std::size_t N) : 
			values(N),
			copies_per_value(1)
		{}

		constexpr explicit Series(const std::size_t N, const T a)  : 
			values(N, a),
			copies_per_value(1)
		{}

		template <typename T2>
		explicit Series(const Series<T2>& a) : 
			values(a.values),
			copies_per_value(1)
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = a[i];
			}
		}



		// initializer list constructor
		template <typename T2>
		explicit Series(const std::size_t copies_per_value, std::initializer_list<T2> list) : 
			values(list),
			copies_per_value(copies_per_value)
		{
		}
		// std container style constructor
		template<typename TIterator>
		explicit Series(const std::size_t copies_per_value, TIterator first, TIterator last) : 
			values(first, last),
			copies_per_value(copies_per_value)
		{
		}

		// convenience constructor for vectors
		explicit Series(const std::size_t copies_per_value, std::vector<T>& vector) : 
			values(vector),
			copies_per_value(copies_per_value)
		{
		}

		constexpr explicit Series(const std::size_t copies_per_value, const std::size_t N) : 
			values(N),
			copies_per_value(copies_per_value)
		{}

		constexpr explicit Series(const std::size_t copies_per_value, const std::size_t N, const T a)  : 
			values(N, a),
			copies_per_value(copies_per_value)
		{}

		template <typename T2>
		explicit Series(const std::size_t copies_per_value, const Series<T2>& a) : 
			values(a.values),
			copies_per_value(copies_per_value)
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = a[i];
			}
		}

		// NOTE: all wrapper functions should to be marked inline 
		inline std::size_t size() const                               { return values.size() * copies_per_value;  }
		inline std::size_t max_size() const                           { return values.size() * copies_per_value;  }
		inline std::size_t capacity() const                           { return values.capacity() * copies_per_value; }
		inline std::size_t empty() const                              { return values.empty(); }
        inline typename std::vector<T>::reference front()             { return values.front(); }
        inline typename std::vector<T>::const_reference front() const { return values.front(); }
        inline typename std::vector<T>::reference back()              { return values.back();  }
        inline typename std::vector<T>::const_reference back() const  { return values.back();  }
	    inline typename std::vector<T>::const_iterator begin() const  { return values.begin(); }
	    inline typename std::vector<T>::const_iterator end()   const  { return values.end();   }
	    inline typename std::vector<T>::iterator begin()              { return values.begin(); }
	    inline typename std::vector<T>::iterator end()                { return values.end();   }
	    using size_type = std::size_t;
		using value_type = T;

		inline typename std::vector<T>::const_reference operator[](const std::size_t lookup_id ) const
		{
		   return values.operator[]((lookup_id / copies_per_value) % values.size());
		}

		inline typename std::vector<T>::reference operator[](const std::size_t lookup_id )
		{
		   return values[(lookup_id / copies_per_value) % values.size()]; // reference return 
		}

		template<typename Tid>
		inline Series<T> operator[](const Series<Tid>& ids )
		{
			Series<T> out = Series<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Series<T>& operator=(const Series<T>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline Series<T>& operator=(const T& other )
		{
			values.resize(other.size());
			fill(*this, other);
			return *this;
		}

		inline std::vector<T>& vector()
		{
			return values;
		}

		template <typename T2>
		void fill(const T2 a)
		{
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] = a;
			}
		}

		template<typename T2>
		inline Series<T> footprint(const Uniform<T2>& u) const {
			return Series<T>(this->size());
		}

		template<typename T2>
		inline Series<T> footprint(const Series<T2>& r) const {
			return Series<T>(this->size());
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

	template<typename T>		// convenience constructor for vectors
	inline Series<T> series(const std::vector<T> vector)
	{
		return Series<T>(vector);
	}
	template<typename T>		// convenience constructor for vectors
	inline Series<T> series(const Series<T> vector)
	{
		return Series<T>(vector);
	}
	template<typename T>		// convenience constructor for vectors
	inline Series<T> series(const std::initializer_list<T>& list)
	{
		return Series<T>(list);
	}


	template<typename T>		
	inline Series<T> reshape(const Series<T>& a, const std::size_t copies_per_value, Series<T>& out)
	{
		if (out.front() != a.front())
		{
			std::copy(a.begin(), a.end(), a.begin());
		}
		out.copies_per_value = copies_per_value;
	}

}


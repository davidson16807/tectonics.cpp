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
	`Series` represents a statically-sized contiguous block of heap memory that is occupied by primitive data of the same arbitrary type.
	It is a thin wrapper for a std::vector and shares most of the same method signatures.
	It is intended as a base class for other data structures that may be subject to operations similar to the operations of their elements.
	*/
	template <typename T>
	struct Series
	{
		
	protected:
		std::vector<T> values;

	public:

		// initializer list constructor
		template <typename T2>
		Series(std::initializer_list<T2> list) : 
			values(list)
		{
		}
		// std container style constructor
		template<typename TIterator>
		explicit Series(TIterator first, TIterator last) : 
			values(first, last)
		{
		}

		// copy constructor
		Series(const Series<T>& a)  : 
			values(a)
		{}

		// convenience constructor for vectors
		explicit Series(std::vector<T> vector) : 
			values(vector)
		{
		}

		explicit Series(const std::size_t N) : 
			values(N)
		{}

		explicit Series(const std::size_t N, const T a)  : 
			values(N, a)
		{}

		template <typename T2>
		explicit Series(const Series<T2>& a) : 
			values(a.values)
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = a[i];
			}
		}

		// NOTE: all wrapper functions should to be marked inline 
		inline std::size_t size() const                               { return values.size();  }
		inline std::size_t max_size() const                           { return values.size();  }
		inline std::size_t capacity() const                           { return values.capacity(); }
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

		inline typename std::vector<T>::const_reference operator[](const std::size_t id ) const
		{
		   return values.operator[](id);
		}
		inline typename std::vector<T>::reference operator[](const std::size_t id )
		{
		   return values[id]; // reference return 
		}

		template<typename Tid>
		inline Series<T> operator[](const Series<Tid>& ids )
		{
			Series<T> out = Series<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Series<T> operator[](const Series<bool>& mask )
		{
			Series<T> out = Series<T>(mask.size());
			get(*this, mask, out);
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
		inline std::size_t size(const Uniform<T2>& u) const {
			return values.size();
		}

		template<typename T2>
		inline std::size_t size(const Series<T2>& r) const {
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Uniform<T2>& u, const Series<T3>& r) const {
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Series<T2>& r, const Uniform<T3>& u) const {
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Series<T2>& r, const Series<T3>& s) const {
			assert(values.size() == r.size());
			assert(values.size() == s.size());
			return values.size();
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
	inline Series<T> series(const std::initializer_list<T>& list)
	{
		return Series<T>(list);
	}


}


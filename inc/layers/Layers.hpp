#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>     /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>			// std::distance

namespace layers
{
	struct AbstractLayers{};
	/*
	This template represents a statically-sized contiguous block of stack memory occupied by primitive data of the same arbitrary type.
	It is a thin wrapper for a std::array and shares most of the same method signatures.
	However, it can also be used with a set of functions and operator overloads that allow it to be handled as if it were a primitive data type.
	It is intended for use 
	See README.md for more details
	*/
	template <typename T, std::size_t N>
	class Layers : AbstractLayers
	{
	protected:
		std::array<T,N> values;

	public:

		// initializer list constructor
		template <typename T2>
		Layers(std::initializer_list<T2> list)
		{
			std::copy(std::begin(list), std::end(list), std::begin(values));
		}
		// std container style constructor
		template <typename TIterator>
		Layers(TIterator first, TIterator last)
		{
			std::copy(first, last, std::begin(values));
		}

		// copy constructor
		Layers(const Layers<T,N>& a)  : values(a.values) {}

		// convenience constructor for arrays
		explicit Layers(std::array<T,N> vector) : values(vector)
		{
		}

		explicit Layers(const T a) {
			values.fill(a);
		}

		template <typename T2>
		explicit Layers(const Layers<T2,N>& a)  : values(a.size())
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
        inline typename std::array<T,N>::reference front()             { return values.front(); }
        inline typename std::array<T,N>::const_reference front() const { return values.front(); }
        inline typename std::array<T,N>::reference back()              { return values.back();  }
        inline typename std::array<T,N>::const_reference back() const  { return values.back();  }
	    inline typename std::array<T,N>::const_iterator begin() const  { return values.begin(); }
	    inline typename std::array<T,N>::const_iterator end()   const  { return values.end();   }
	    inline typename std::array<T,N>::iterator begin()              { return values.begin(); }
	    inline typename std::array<T,N>::iterator end()                { return values.end();   }

		inline typename std::array<T,N>::const_reference operator[](const unsigned int id ) const
		{
		   return values.operator[](id);
		}
		inline typename std::array<T,N>::reference operator[](const unsigned int id )
		{
		   return values[id]; // reference return 
		}
	
		inline Layers<T,N>& operator=(const Layers<T,N>& other )
		{
			copy(*this, other);
			return *this;
		}
		inline Layers<T,N>& operator=(const T& other )
		{
			values.resize(other.size());
			fill(*this, other);
			return *this;
		}

		inline std::array<T,N>& array()
		{
			return values;
		}
	};


	template <std::size_t N, typename T, typename Tid>
	inline T get(const Layers<T,N>& a, const Tid id )
	{
		return a[id];
	}

	template <std::size_t N, typename T>
	void fill(Layers<T,N>& out, const T a )
	{
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}

	template <std::size_t N, typename T, typename TIterator>
	void copy_iterators(Layers<T,N>& out, TIterator first, TIterator last)
	{
		unsigned int id = 0;
		while (first!=last) 
		{
			out[id] = *first;
			++first;
			++id;
		}
	}
	template <std::size_t N, typename T, typename T2>
	void copy(Layers<T,N>& out, const Layers<T2,N>& a )
	{
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <std::size_t N, typename T>
	inline void copy(Layers<T,N>& out, unsigned int id, const Layers<T,N>& a )
	{
		out[id] = a[id];
	}


	template <std::size_t N, typename T, typename Tid>
	inline void set(Layers<T,N>& out, unsigned int id, const T a )
	{
		out[id] = a;
	}






	
	// UNARY TRANSFORM
	template <std::size_t N, typename T1, typename Tout, typename F>
	inline void transform(const Layers<T1,N>& a, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i]);
		}
	}
	template <std::size_t N, typename T1, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value, int> = 0>
	inline void transform(const T1 a, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a);
		}
	}




	// BINARY TRANSFORM
	template <std::size_t N, typename T1, typename T2, typename Tout, typename F>
	inline void transform(const Layers<T1,N>& a, const Layers<T2,N>& b, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i]);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	inline void transform(const Layers<T1,N>& a, const T2 b, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value, int> = 0>
	inline void transform(const T1 a, const Layers<T2,N>& b, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i]);
		}
	}











	// TRINARY TRANSFORM
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const Layers<T1,N>& a, const Layers<T2,N>& b, const Layers<T3,N>& c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c[i]);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T3>::value, int> = 0>
	inline void transform(const Layers<T1,N>& a, const Layers<T2,N>& b, const T3 c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	inline void transform(const Layers<T1,N>& a, const T2 b, const Layers<T3,N>& c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c[i]);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T2>::value && !std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	inline void transform(const Layers<T1,N>& a, const T2 b, const T3 c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F, std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value, int> = 0>
	inline void transform(const T1 a, const Layers<T2,N>& b, const Layers<T3,N>& c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c[i]);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value && !std::is_base_of<AbstractLayers, T3>::value, int> = 0>
	inline void transform(const T1 a, const Layers<T2,N>& b, const T3 c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c);
		}
	}
	template <std::size_t N, typename T1, typename T2, typename T3, typename Tout, typename F,
		std::enable_if_t<!std::is_base_of<AbstractLayers, T1>::value && !std::is_base_of<AbstractLayers, T2>::value, int> = 0>
	inline void transform(const T1 a, const T2 b, const Layers<T3,N>& c, F f, Layers<Tout,N>& out)
	{
		for (std::size_t i = 0; i < c.size(); ++i)
		{
			out[i] = f(a, b, c[i]);
		}
	}


}

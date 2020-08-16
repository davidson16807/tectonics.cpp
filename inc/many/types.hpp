#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>     /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>			// std::distance

namespace many
{
	/*
	This template represents a statically-sized contiguous block of heap memory occupied by primitive data of the same arbitrary type.
	It is a thin wrapper for a std::vector and shares most of the same method signatures.
	However, it can also be used with a set of functions and operator overloads that allow it to be handled as if it were a primitive data type.
	See README.md for more details
	*/
	template <typename T>
	class series
	{
		std::vector<T> values;

	public:

		virtual ~series()
		{
		}

		// initializer list constructor
		series(std::initializer_list<T> list) : values(list.begin(), list.end())
		{
		}
		// std container style constructor
		template<typename TIterator>
		series(TIterator first, TIterator last) : values(std::distance(first, last))
		{
			unsigned int id = 0;
			while (first!=last) 
			{
				this->values[id] = *first;
				++first;
				++id;
			}
		}

		// copy constructor
		series(const series<T>& a)  : values(a.values) {}

		// convenience constructor for vectors
		explicit series(std::vector<T> vector) : values(vector)
		{
		}

		explicit series(const unsigned int N) : values(N) {}

		explicit series(const unsigned int N, const T a)  : values(N, a) {}

		template <typename T2>
		explicit series(const series<T2>& a)  : values(a.size())
		{
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				values[i] = a[i];
			}
		}

		// NOTE: all wrapper functions should to be marked inline 
		inline unsigned int size() const                              { return values.size();  }
		inline unsigned int max_size() const                          { return values.size();  }
		inline unsigned int capacity() const                          { return values.capacity(); }
		inline unsigned int empty() const                             { return values.empty(); }
        inline typename std::vector<T>::reference front()             { return values.front(); }
        inline typename std::vector<T>::const_reference front() const { return values.front(); }
        inline typename std::vector<T>::reference back()              { return values.back();  }
        inline typename std::vector<T>::const_reference back() const  { return values.back();  }
	    inline typename std::vector<T>::const_iterator begin() const  { return values.begin(); }
	    inline typename std::vector<T>::const_iterator end()   const  { return values.end();   }
	    inline typename std::vector<T>::iterator begin()              { return values.begin(); }
	    inline typename std::vector<T>::iterator end()                { return values.end();   }

		inline typename std::vector<T>::const_reference operator[](const unsigned int id ) const
		{
		   return values.operator[](id);
		}
		inline typename std::vector<T>::reference operator[](const unsigned int id )
		{
		   return values[id]; // reference return 
		}
	
		inline series<T> operator[](const series<bool>& mask )
		{
			series<T> out = series<T>(mask.size());
			get(*this, mask, out);
			return out;
		}
		template<typename Tid>
		inline series<T> operator[](const series<Tid>& ids )
		{
			series<T> out = series<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline series<T>& operator=(const series<T>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline series<T>& operator=(const T& other )
		{
			values.resize(other.size());
			fill(*this, other);
			return *this;
		}

		inline std::vector<T>& vector()
		{
			return values;
		}
	};


	template <typename T, typename Tid>
	inline T get(const series<T>& a, const Tid id )
	{
		return a[id];
	}
	template <typename T, typename Tid>
	inline series<T> get(const series<T>& a, const series<Tid>& ids )
	{
		series<T> out = series<T>(ids.size());
		get(a, ids, out);
		return out;
	}
	/*
	template <typename T, typename Tid>
	inline series<T> get(const series<T>& a, const series<bool>& mask )
	{
		series<T> out = series<T>(sum(mask));
		get(a, ids, out);
		return out;
	}
	*/
	template <typename T, typename Tid>
	void get(const series<T>& a, const series<Tid>& ids, series<T>& out )
	{
		assert(ids.size() == out.size());
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[i] = a[ids[i]];
		}
	}
	template <typename T>
	void get(const series<T>& a, const series<bool>& mask, series<T>& out )
	{
		assert(a.size()  == mask.size());
		int out_i = 0;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			if (mask[i])
			{
				out[out_i] = a[i];
				out_i++;
			}
		}
	}

	template <typename T>
	void fill(series<T>& out, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}
	template <typename T, typename Tid>
	void fill(series<T>& out, const series<Tid>& ids, const T a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a;
		}
	}
	template <typename T>
	void fill(series<T>& out, const series<bool>& mask, const T a )
	{
		assert(out.size() == mask.size());
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a : out[i];
		}
	}

	template<typename T, typename TIterator>
	void copy_iterators(series<T>& out, TIterator first, TIterator last)
	{
		unsigned int id = 0;
		while (first!=last) 
		{
			out[id] = *first;
			++first;
			++id;
		}
	}
	template <typename T, typename T2>
	void copy(series<T>& out, const series<T2>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <typename T>
	inline void copy(series<T>& out, unsigned int id, const series<T>& a )
	{
		out[id] = a[id];
	}
	template <typename T, typename Tid>
	void copy(series<T>& out, const series<Tid>& ids, const series<T>& a )
	{
		assert(ids.size() == a.size());
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a[ids[i]];
		}
	}
	template <typename T>
	void copy(series<T>& out, const series<bool>& mask, const series<T>& a )
	{
		assert(out.size() == mask.size());
		assert(out.size() == a.size());
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a[i] : out[i];
		}
	}


	template <typename T, typename Tid>
	inline void set(series<T>& out, unsigned int id, const T a )
	{
		out[id] = a;
	}
	template <typename T, typename Tid>
	void set(series<T>& out, const series<Tid>& ids, const series<T>& a )
	{
		assert(ids.size() == a.size());
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a[i];
		}
	}






	
	// UNARY TRANSFORM
	template <typename T1, typename Tout, typename F>
	inline void transform(const series<T1>& a, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i]);
		}
	}
	template <typename T1, typename Tout, typename F>
	inline void transform(const T1 a, F f, series<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a);
		}
	}




	// BINARY TRANSFORM
	template <typename T1, typename T2, typename Tout, typename F>
	inline void transform(const series<T1>& a, const series<T2>& b, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(a.size() >= b.size());
		assert(a.size() % b.size() == 0);
		uint N = a.size() / b.size();
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			for (unsigned int j = 0; j < N; ++j)
			{
				out[N*i+j] = f(a[N*i+j], b[i]);
			}
		}
	}
	template <typename T1, typename T2, typename Tout, typename F>
	inline void transform(const series<T1>& a, const T2 b, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b);
		}
	}
	template <typename T1, typename T2, typename Tout, typename F>
	inline void transform(const T1 a, const series<T2>& b, F f, series<Tout>& out)
	{
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i]);
		}
	}











	// TRINARY TRANSFORM
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const series<T1>& a, const series<T2>& b, const series<T3>& c, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(b.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c[i]);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const series<T1>& a, const series<T2>& b, const T3 c, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const series<T1>& a, const T2 b, const series<T3>& c, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c[i]);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const series<T1>& a, const T2 b, const T3 c, F f, series<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const T1 a, const series<T2>& b, const series<T3>& c, F f, series<Tout>& out)
	{
		assert(b.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c[i]);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const T1 a, const series<T2>& b, const T3 c, F f, series<Tout>& out)
	{
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c);
		}
	}
	template <typename T1, typename T2, typename T3, typename Tout, typename F>
	inline void transform(const T1 a, const T2 b, const series<T3>& c, F f, series<Tout>& out)
	{
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < c.size(); ++i)
		{
			out[i] = f(a, b, c[i]);
		}
	}



	template<typename T, typename Tid, typename Taggregator>
	void aggregate_into(const series<T>& a, const series<Tid>& group_ids, Taggregator aggregator, series<T>& group_out)
	{
		assert(a.size() == group_ids.size());
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			assert(0 <= group_ids[i]);
			assert(group_ids[i] < group_out.size());
			assert(!std::isinf(group_ids[i]));
			assert(!std::isnan(group_ids[i]));
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
	}

	template<typename T, typename Tid, typename Taggregator>
	void aggregate_into(const series<Tid>& group_ids, Taggregator aggregator, series<T>& group_out)
	{
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			assert(0 <= group_ids[i] && group_ids[i] < group_out.size() && !std::isinf(group_ids[i]) && !std::isnan(group_ids[i]));
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
	}


















	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions



	typedef series<bool>	     bools;
	typedef series<int>	         ints;
	typedef series<unsigned int> uints;
	typedef series<float>	     floats;
	typedef series<double>       doubles;
}

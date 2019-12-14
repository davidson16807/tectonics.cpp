#pragma once

#include <iostream>
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>			// std::distance

namespace many
{
	template <class T>
	inline T get(const std::vector<T>& a, const unsigned int id )
	{
		return a[id];
	}
	template <class T>
	void get(const std::vector<T>& a, const std::vector<unsigned int>& ids, std::vector<T>& out )
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
	template <class T>
	void get(const std::vector<T>& a, const std::vector<bool>& mask, std::vector<T>& out )
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

	template <class T>
	void fill(std::vector<T>& out, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}
	template <class T>
	void fill(std::vector<T>& out, const std::vector<unsigned int>& ids, const T a )
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
	template <class T>
	void fill(std::vector<T>& out, const std::vector<bool>& mask, const T a )
	{
		assert(out.size() == mask.size());
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a : out[i];
		}
	}

	template<class T, class TIterator>
	void copy_iterators(std::vector<T>& out, TIterator first, TIterator last)
	{
		unsigned int id = 0;
		while (first!=last) 
		{
			out[id] = *first;
			++first;
			++id;
		}
	}
	template <class T>
	void copy(std::vector<T>& out, const std::vector<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <class T>
	inline void copy(std::vector<T>& out, const unsigned int id, const std::vector<T>& a )
	{
		out[id] = a[id];
	}
	template <class T>
	void copy(std::vector<T>& out, const std::vector<unsigned int>& ids, const std::vector<T>& a )
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
	template <class T>
	void copy(std::vector<T>& out, const std::vector<bool>& mask, const std::vector<T>& a )
	{
		assert(out.size() == mask.size());
		assert(out.size() == a.size());
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a[i] : out[i];
		}
	}


	template <class T>
	inline void set(std::vector<T>& out, const unsigned int id, const T a )
	{
		out[id] = a;
	}
	template <class T>
	void set(std::vector<T>& out, const std::vector<unsigned int>& ids, const std::vector<T>& a )
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
	template <class T1, class Tout, typename F>
	inline void transform(const std::vector<T1>& a, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i]);
		}
	}
	template <class T1, class Tout, typename F>
	inline void transform(const T1 a, F f, std::vector<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a);
		}
	}




	// BINARY TRANSFORM
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const std::vector<T1>& a, const std::vector<T2>& b, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i]);
		}
	}
	template <class T1, class Tout, typename F>
	inline void transform(const std::vector<T1>& a, const T1 b, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b);
		}
	}
	template <class T2, class Tout, typename F>
	inline void transform(const T2 a, const std::vector<T2>& b, F f, std::vector<Tout>& out)
	{
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i]);
		}
	}











	// TRINARY TRANSFORM
	template <class T, class Tout, typename F>
	inline void transform(const std::vector<T>& a, const std::vector<T>& b, const std::vector<T>& c, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(b.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c[i]);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const std::vector<T>& a, const std::vector<T>& b, const T c, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const std::vector<T>& a, const T b, const std::vector<T>& c, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c[i]);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const std::vector<T>& a, const T b, const T c, F f, std::vector<Tout>& out)
	{
		assert(a.size() == out.size());
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const T a, const std::vector<T>& b, const std::vector<T>& c, F f, std::vector<Tout>& out)
	{
		assert(b.size() == out.size());
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c[i]);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const T a, const std::vector<T>& b, const T c, F f, std::vector<Tout>& out)
	{
		assert(b.size() == out.size());
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c);
		}
	}
	template <class T, class Tout, typename F>
	inline void transform(const T a, const T b, const std::vector<T>& c, F f, std::vector<Tout>& out)
	{
		assert(c.size() == out.size());
		for (unsigned int i = 0; i < c.size(); ++i)
		{
			out[i] = f(a, b, c[i]);
		}
	}



	template<class T, typename Taggregator>
	void aggregate_into(const std::vector<T>& a, const std::vector<unsigned int>& group_ids, Taggregator aggregator, std::vector<T>& group_out)
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

	template<class T, typename Taggregator>
	void aggregate_into(const std::vector<unsigned int>& group_ids, Taggregator aggregator, std::vector<T>& group_out)
	{
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			assert(0 <= group_ids[i] && group_ids[i] < group_out.size() && !std::isinf(group_ids[i]) && !std::isnan(group_ids[i]));
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
	}


















	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions



	typedef std::vector<bool>	        bools;
	typedef std::vector<int>	        ints;
	typedef std::vector<unsigned int> uints;
	typedef std::vector<float>	    floats;
	typedef std::vector<double>       doubles;
}

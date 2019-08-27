#pragma once

#include <iostream>
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>			// std::distance

namespace many
{
	// This template represents a statically-sized contiguous block of heap memory occupied by primitive data of the same arbitrary type
	// See README.md for more details
	template <class T>
	class composite
	{
	protected:
		std::vector<T> values;

	public:

		virtual ~composite()
		{
		};

		// initializer list constructor
		composite(std::initializer_list<T> list) : values(list.size())
		{
			unsigned int id = 0;
			for (auto i = list.begin(); i != list.end(); ++i)
			{
				this->values[id] = *i;
				id++;
			}
		};
		template<class TIterator>
		composite(TIterator first, TIterator last) : values(std::distance(first, last))
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
		composite(const composite<T>& a)  : values(a.values) {};

		explicit composite(const unsigned int N) : values(N) {};

		explicit composite(const unsigned int N, const T a)  : values(N, a) {};

		template <class T2>
		explicit composite(const composite<T2>& a)  : values(a.N)
		{
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				values[i] = a[i];
			}
		};

		inline unsigned int size() const
		{
			return values.size();
		}

		inline std::vector<T>& vector()
		{
			return values;
		}

	    inline typename std::vector<T>::const_iterator begin() const { return values.begin(); }
	    inline typename std::vector<T>::const_iterator end()   const { return values.end();   }

	    inline typename std::vector<T>::iterator begin() { return values.begin(); }
	    inline typename std::vector<T>::iterator end()   { return values.end();   }

		// NOTE: all operators should to be inline because they are thin wrappers of functions
		inline typename std::vector<T>::const_reference operator[](const unsigned int id ) const
		{
		   return values.operator[](id);
		}
		inline typename std::vector<T>::reference operator[](const unsigned int id )
		{
		   return values[id]; // reference return 
		}
	
		inline composite<T> operator[](const composite<bool>& mask )
		{
			composite<T> out = composite<T>(mask.size());
			get(*this, mask, out);
			return out;
		}
		inline composite<T> operator[](const composite<unsigned int>& ids )
		{
			composite<T> out = composite<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline composite<T>& operator=(const composite<T>& other )
		{
			copy(*this, other);
			return *this;
		}
		inline composite<T>& operator=(const T& other )
		{
			fill(*this, other);
			return *this;
		}
	};


	template <class T>
	inline T get(const composite<T>& a, const unsigned int id )
	{
		return a[id];
	}
	template <class T>
	void get(const composite<T>& a, const composite<unsigned int>& ids, composite<T>& out )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[i] = a[ids[i]];
		}
	}
	template <class T>
	void get(const composite<T>& a, const composite<bool>& mask, composite<T>& out )
	{
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
	void fill(composite<T>& out, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}
	template <class T>
	void fill(composite<T>& out, const composite<unsigned int>& ids, const T a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a;
		}
	}
	template <class T>
	void fill(composite<T>& out, const composite<bool>& mask, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a : out[i];
		}
	}

	template<class T, class TIterator>
	void copy_iterators(composite<T>& out, TIterator first, TIterator last)
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
	void copy(composite<T>& out, const composite<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <class T>
	inline void copy(composite<T>& out, const unsigned int id, const composite<T>& a )
	{
		out[id] = a[id];
	}
	template <class T>
	void copy(composite<T>& out, const composite<unsigned int>& ids, const composite<T>& a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a[ids[i]];
		}
	}
	template <class T>
	void copy(composite<T>& out, const composite<bool>& mask, const composite<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a[i] : out[i];
		}
	}


	template <class T>
	inline void set(composite<T>& out, const unsigned int id, const T a )
	{
		out[id] = a;
	}
	template <class T>
	void set(composite<T>& out, const composite<unsigned int>& ids, const composite<T>& a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a[i];
		}
	}






	
	// UNARY TRANSFORM
	template <class T1, class Tout, typename F>
	inline void transform(const composite<T1>& a, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i]);
		}
	}
	template <class T1, class Tout, typename F>
	inline void transform(const T1 a, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a);
		}
	}




	// BINARY TRANSFORM
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const composite<T1>& a, const composite<T2>& b, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i]);
		}
	}
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const composite<T1>& a, const T2 b, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b);
		}
	}
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const T1 a, const composite<T2>& b, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i]);
		}
	}











	// TRINARY TRANSFORM
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const composite<T1>& a, const composite<T2>& b, const composite<T3>& c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const composite<T1>& a, const composite<T2>& b, const T3 c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const composite<T1>& a, const T2 b, const composite<T3>& c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const composite<T1>& a, const T2 b, const T3 c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const composite<T2>& b, const composite<T3>& c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const composite<T2>& b, const T3 c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const T2 b, const composite<T3>& c, F f, composite<Tout>& out)
	{
		for (unsigned int i = 0; i < c.size(); ++i)
		{
			out[i] = f(a, b, c[i]);
		}
	}


	template<class T, typename Taggregator>
	void aggregate_into(const composite<T>& a, const composite<unsigned int>& group_ids, Taggregator aggregator, composite<T>& group_out)
	{
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
	}
	template<class T, typename Taggregator>
	void aggregate(const composite<T>& a, const composite<unsigned int>& group_ids, Taggregator aggregator, composite<T>& group_out)
	{
		fill(group_out, T(0));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
	}

	template<class T, typename Taggregator>
	void aggregate_into(const composite<unsigned int>& group_ids, Taggregator aggregator, composite<T>& group_out)
	{
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
	}
	template<class T, typename Taggregator>
	void aggregate(const composite<unsigned int>& group_ids, Taggregator aggregator, composite<T>& group_out)
	{
		fill(group_out, T(0));
		for (unsigned int i = 0; i < group_ids.size(); ++i)
		{
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
	}


















	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions



	typedef composite<bool>	       bools;
	typedef composite<int>	       ints;
	typedef composite<unsigned int> uints;
	typedef composite<float>	       floats;
	typedef composite<double>       doubles;
}

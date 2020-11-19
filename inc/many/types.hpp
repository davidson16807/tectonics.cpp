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
	class AbstractSeries {};

	/*
	This template represents a statically-sized contiguous block of heap memory occupied by primitive data of the same arbitrary type.
	It is a thin wrapper for a std::vector and shares most of the same method signatures.
	However, it can also be used with a set of functions and operator overloads that allow it to be handled as if it were a primitive data type.
	See README.md for more details
	*/
	template <typename T>
	class Series : public AbstractSeries
	{
	protected:
		std::vector<T> values;

	public:

		// initializer list constructor
		template <typename T2>
		Series(std::initializer_list<T2> list) : values(list.begin(), list.end())
		{
		}
		// std container style constructor
		template<typename TIterator>
		Series(TIterator first, TIterator last) : values(std::distance(first, last))
		{
			std::size_t id = 0;
			while (first!=last) 
			{
				this->values[id] = *first;
				++first;
				++id;
			}
		}

		// copy constructor
		Series(const Series<T>& a)  : values(a.values) {}

		// convenience constructor for vectors
		explicit Series(std::vector<T> vector) : values(vector)
		{
		}

		explicit Series(const std::size_t N) : values(N) {}

		explicit Series(const std::size_t N, const T a)  : values(N, a) {}

		template <typename T2>
		explicit Series(const Series<T2>& a)  : values(a.size())
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

		inline typename std::vector<T>::const_reference operator[](const unsigned int id ) const
		{
		   return values.operator[](id);
		}
		inline typename std::vector<T>::reference operator[](const unsigned int id )
		{
		   return values[id]; // reference return 
		}
	
		inline Series<T> operator[](const Series<bool>& mask )
		{
			Series<T> out = Series<T>(mask.size());
			get(*this, mask, out);
			return out;
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



		/*
		`store(...)` methods modify the state of the object to store the output of a computation.
		The computation is defined by applying a function `f` over any combination of `Series` objects and singletons.
		We then curry the `store()` function to create derived functions outside the class, e.g. add() or dot()

		Q:  Why aren't derived functions (like add() or dot()) implemented as methods?
		A:  There are too many of them. Consider all the functions we'd need for trigonometry, statistics, morphology, vector calculus, etc.
		    We want the API user to selectively include this functionality using `#include` statements,
		    and since we don't want to change the behavior of the class depending on which include statements are present, 
		    we decide this functionality is best defined using regular functions outside the class. 

		Q:  Why is store() a method and not just a function outside the class?
		A:  The `store()` method by default only allows element-wise comparison between Series and singletons,
		    however we want to extend this behavior when a Series has a particular interpretation
		    (for instance when multiplying a rank 2 tensor by a rank 1 tensor).
		    We would like to allow this extension to be done outside the `many` namespace,
		    since we want to prevent the `many` namespace from growing past the limits of our comprehension.
		    To implement this extended behavior outside the namespace would mean that derived functions could 
			not use this extended functionality (unless the API user added a `using` statement, which is unwise),
			so we would have to reimplement the same large library of derived functions for each behavioral extension.
		    We instead use derived classes to represent instances of `Series` that carry extended meaning.

		Q:  Why do we implement methods that modify state? Why don't we implement methods that use the object as input?
		A:  We want it to be trivial to create derived functions like add() or dot(). 
		    Derived functions are always declared outside the class with a standardized method signature, 
		    like `add(const T1& input1, const T2& input2, T3& output)`
		    We cannot guarantee whether `T1` and `T2` are singletons, 
		    so relying on a method that treats a `Series` as input would require special consideration for each overload,
		    but we can be certain that `T3` will be a `Series` in all cases where we want to use output references.
		    This excludes functions that "condense" down into singletons, like `sum(Series)`,
		    however we want to pass output by value in those cases anyways, 
		    and they're few enough to handle on a case-by-case basis.
		    Creating a `store()` function that modifies state also enables encapsulation of `Series` objects,
		    so that only the object itself can modify its own state, though we admit this concern is secondary.
		    Despite modifying state, the `store()` methods can be considered regular functions (as defined by Stepanov),
		    in the sense that the state of the `Series` object afterwards can be determined strictly from method input.
		*/


		// UNARY TRANSFORM
		template <typename T1, typename F>
		inline void store(const F f, const Series<T1>& a)
		{
			assert(a.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i]);
			}
		}
		template <typename T1, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value, int> = 0>
		inline void store(const F f, const T1 a)
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a);
			}
		}




		// BINARY TRANSFORM
		template <typename T1, typename T2, typename F>
		inline void store(const F f, const Series<T1>& a, const Series<T2>& b)
		{
			assert(a.size() == values.size());
			assert(a.size() == b.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b[i]);
			}
		}
		template <typename T1, typename T2, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const Series<T1>& a, const T2 b)
		{
			assert(a.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b);
			}
		}
		template <typename T1, typename T2, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value, int> = 0>
		inline void store(const F f, const T1 a, const Series<T2>& b)
		{
			assert(b.size() == values.size());
			for (std::size_t i = 0; i < b.size(); ++i)
			{
				values[i] = f(a, b[i]);
			}
		}


		// TRINARY TRANSFORM
		template <typename T1, typename T2, typename T3, typename F>
		inline void store(const F f, const Series<T1>& a, const Series<T2>& b, const Series<T3>& c)
		{
			assert(a.size() == values.size());
			assert(b.size() == values.size());
			assert(c.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b[i], c[i]);
			}
		}
		template <typename T1, typename T2, typename T3, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T3>::value, int> = 0>
		inline void store(const F f, const Series<T1>& a, const Series<T2>& b, const T3 c)
		{
			assert(a.size() == values.size());
			assert(b.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b[i], c);
			}
		}
		template <typename T1, typename T2, typename T3, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const Series<T1>& a, const T2 b, const Series<T3>& c)
		{
			assert(a.size() == values.size());
			assert(c.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b, c[i]);
			}
		}
		template <typename T1, typename T2, typename T3, typename F,
			std::enable_if_t<!std::is_base_of<AbstractSeries, T2>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const Series<T1>& a, const T2 b, const T3 c)
		{
			assert(a.size() == values.size());
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				values[i] = f(a[i], b, c);
			}
		}
		template <typename T1, typename T2, typename T3, typename F, std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value, int> = 0>
		inline void store(const F f, const T1 a, const Series<T2>& b, const Series<T3>& c)
		{
			assert(b.size() == values.size());
			assert(c.size() == values.size());
			for (std::size_t i = 0; i < b.size(); ++i)
			{
				values[i] = f(a, b[i], c[i]);
			}
		}
		template <typename T1, typename T2, typename T3, typename F,
			std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, T3>::value, int> = 0>
		inline void store(const F f, const T1 a, const Series<T2>& b, const T3 c)
		{
			assert(b.size() == values.size());
			for (std::size_t i = 0; i < b.size(); ++i)
			{
				values[i] = f(a, b[i], c);
			}
		}
		template <typename T1, typename T2, typename T3, typename F,
			std::enable_if_t<!std::is_base_of<AbstractSeries, T1>::value && !std::is_base_of<AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const T1 a, const T2 b, const Series<T3>& c)
		{
			assert(c.size() == values.size());
			for (std::size_t i = 0; i < c.size(); ++i)
			{
				values[i] = f(a, b, c[i]);
			}
		}








	};


	template <typename T, typename Tid>
	inline T get(const Series<T>& a, const Tid id )
	{
		return a[id];
	}
	template <typename T, typename Tid>
	inline Series<T> get(const Series<T>& a, const Series<Tid>& ids )
	{
		Series<T> out = Series<T>(ids.size());
		get(a, ids, out);
		return out;
	}
	/*
	template <typename T, typename Tid>
	inline Series<T> get(const Series<T>& a, const Series<bool>& mask )
	{
		Series<T> out = Series<T>(sum(mask));
		get(a, ids, out);
		return out;
	}
	*/
	template <typename T, typename Tid>
	void get(const Series<T>& a, const Series<Tid>& ids, Series<T>& out )
	{
		assert(ids.size() == out.size());
		for (std::size_t i = 0; i < ids.size(); ++i)
		{
			assert(0u <= std::size_t(ids[i]));
			assert(std::size_t(ids[i]) < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[i] = a[ids[i]];
		}
	}
	template <typename T>
	void get(const Series<T>& a, const Series<bool>& mask, Series<T>& out )
	{
		assert(a.size()  == mask.size());
		int out_i = 0;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			if (mask[i])
			{
				out[out_i] = a[i];
				out_i++;
			}
		}
	}

	template <typename T>
	void fill(Series<T>& out, const T a )
	{
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}
	template <typename T, typename Tid>
	void fill(Series<T>& out, const Series<Tid>& ids, const T a )
	{
		for (std::size_t i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a;
		}
	}
	template <typename T>
	void fill(Series<T>& out, const Series<bool>& mask, const T a )
	{
		assert(out.size() == mask.size());
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a : out[i];
		}
	}

	template<typename T, typename TIterator>
	void copy_iterators(Series<T>& out, TIterator first, TIterator last)
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
	void copy(Series<T>& out, const Series<T2>& a )
	{
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <typename T>
	inline void copy(Series<T>& out, unsigned int id, const Series<T>& a )
	{
		out[id] = a[id];
	}
	template <typename T, typename Tid>
	void copy(Series<T>& out, const Series<Tid>& ids, const Series<T>& a )
	{
		assert(ids.size() == a.size());
		for (std::size_t i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a[ids[i]];
		}
	}
	template <typename T>
	void copy(Series<T>& out, const Series<bool>& mask, const Series<T>& a )
	{
		assert(out.size() == mask.size());
		assert(out.size() == a.size());
		for (std::size_t i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a[i] : out[i];
		}
	}


	template <typename T, typename Tid>
	inline void set(Series<T>& out, unsigned int id, const T a )
	{
		out[id] = a;
	}
	template <typename T, typename Tid>
	void set(Series<T>& out, const Series<Tid>& ids, const Series<T>& a )
	{
		assert(ids.size() == a.size());
		for (std::size_t i = 0; i < ids.size(); ++i)
		{
			
			assert(0 <= ids[i]);
			assert(ids[i] < a.size());
			assert(!std::isinf(ids[i]));
			assert(!std::isnan(ids[i]));
			out[ids[i]] = a[i];
		}
	}






	template<typename T, typename Tid, typename Taggregator>
	void aggregate_into(const Series<T>& a, const Series<Tid>& group_ids, Taggregator aggregator, Series<T>& group_out)
	{
		assert(a.size() == group_ids.size());
		for (std::size_t i = 0; i < group_ids.size(); ++i)
		{
			assert(0 <= group_ids[i]);
			assert(std::size_t(group_ids[i]) < group_out.size());
			assert(!std::isinf(group_ids[i]));
			assert(!std::isnan(group_ids[i]));
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]], a[i]);
		}
	}

	template<typename T, typename Tid, typename Taggregator>
	void aggregate_into(const Series<Tid>& group_ids, Taggregator aggregator, Series<T>& group_out)
	{
		for (std::size_t i = 0; i < group_ids.size(); ++i)
		{
			assert(0u <= std::size_t(group_ids[i]) && std::size_t(group_ids[i]) < group_out.size() && !std::isinf(group_ids[i]) && !std::isnan(group_ids[i]));
			group_out[group_ids[i]] = aggregator(group_out[group_ids[i]]);
		}
	}


















	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions



	typedef Series<bool>	     bools;
	typedef Series<int>	         ints;
	typedef Series<unsigned int> uints;
	typedef Series<float>	     floats;
	typedef Series<double>       doubles;
}

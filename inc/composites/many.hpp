#pragma once

#include <iostream>
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>			// std::distance

namespace composites
{
	// This template represents a statically-sized contiguous block of heap memory occupied by primitive data of the same arbitrary type
	// See README.md for more details
	template <class T>
	class many
	{
	protected:
		std::vector<T> values;

	public:

		virtual ~many()
		{
		};

		// initializer list constructor
		many(std::initializer_list<T> list) : values(list.size())
		{
			unsigned int id = 0;
			for (auto i = list.begin(); i != list.end(); ++i)
			{
				this->values[id] = *i;
				id++;
			}
		};
		template<class TIterator>
		many(TIterator first, TIterator last) : values(std::distance(first, last))
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
		many(const many<T>& a)  : values(a.values) {};

		explicit many(const unsigned int N) : values(N) {};

		explicit many(const unsigned int N, const T a)  : values(N, a) {};

		template <class T2>
		explicit many(const many<T2>& a)  : values(a.N)
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
	
		inline many<T> operator[](const many<bool>& mask )
		{
			many<T> out = many<T>(mask.size());
			get(*this, mask, out);
			return out;
		}
		inline many<T> operator[](const many<unsigned int>& ids )
		{
			many<T> out = many<T>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline many<T>& operator=(const many<T>& other )
		{
			copy(*this, other);
			return *this;
		}
		inline many<T>& operator=(const T& other )
		{
			fill(*this, other);
			return *this;
		}
	};


	template <class T>
	inline T get(const many<T>& a, const unsigned int id )
	{
		return a[id];
	}
	template <class T>
	void get(const many<T>& a, const many<unsigned int>& ids, many<T>& out )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[i] = a[ids[i]];
		}
	}
	template <class T>
	void get(const many<T>& a, const many<bool>& mask, many<T>& out )
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
	void fill(many<T>& out, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a;
		}
	}
	template <class T>
	void fill(many<T>& out, const many<unsigned int>& ids, const T a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a;
		}
	}
	template <class T>
	void fill(many<T>& out, const many<bool>& mask, const T a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a : out[i];
		}
	}

	template<class T, class TIterator>
	void copy_iterators(many<T>& out, TIterator first, TIterator last)
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
	void copy(many<T>& out, const many<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = a[i];
		}
	}
	template <class T>
	inline void copy(many<T>& out, const unsigned int id, const many<T>& a )
	{
		out[id] = a[id];
	}
	template <class T>
	void copy(many<T>& out, const many<unsigned int>& ids, const many<T>& a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a[ids[i]];
		}
	}
	template <class T>
	void copy(many<T>& out, const many<bool>& mask, const many<T>& a )
	{
		for (unsigned int i = 0; i < out.size(); ++i)
		{
			out[i] = mask[i]? a[i] : out[i];
		}
	}


	template <class T>
	inline void set(many<T>& out, const unsigned int id, const T a )
	{
		out[id] = a;
	}
	template <class T>
	void set(many<T>& out, const many<unsigned int>& ids, const many<T>& a )
	{
		for (unsigned int i = 0; i < ids.size(); ++i)
		{
			out[ids[i]] = a[i];
		}
	}


	float COMPOSITES_EPSILON = 1e-4;

	template <class T>
	bool equal(const many<T>& a, const T b)
	{
		bool out(true);
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <class T>
	bool notEqual(const many<T>& a, const T b)
	{
		bool out(false);
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out |= diff*diff > threshold;
		}
		return out;
	}
	template <class T>
	bool equal(const many<T>& a, const many<T>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		bool out(true);
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out &= diff*diff <= threshold;
		}
		return out;
	}
	template <class T>
	bool notEqual(const many<T>& a, const many<T>& b)
	{
		if (a.size() != b.size())
		{
			return true;
		}
		bool out(false);
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out |= diff*diff > threshold;
		}
		return out;
	}



	template <class T>
	void equal(const many<T>& a, const T b, many<bool>& out)
	{
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff <= threshold;
		}
	}
	template <class T>
	void notEqual(const many<T>& a, const T b, many<bool>& out)
	{
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b;
			out[i] = diff*diff > threshold;
		}
	}
	template <class T>
	void equal(const many<T>& a, const many<T>& b, many<bool>& out)
	{
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff <= threshold;
		}
	}
	template <class T>
	void notEqual(const many<T>& a, const many<T>& b, many<bool>& out)
	{
		T diff(0);
		T threshold(COMPOSITES_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			diff = a[i] - b[i];
			out[i] = diff*diff > threshold;
		}
	}



	// NOTE: all operators are suggested to be inline because they are thin wrappers of functions
	template <class T>
	inline bool operator==(const many<T>& a, const T b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const many<T>& a, const T b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const T a, const many<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const T a, const many<T>& b)
	{
		return notEqual(a, b);
	}
	template <class T>
	inline bool operator==(const many<T>& a, const many<T>& b)
	{
		return equal(a, b);
	}
	template <class T>
	inline bool operator!=(const many<T>& a, const many<T>& b)
	{
		return notEqual(a, b);
	}





	
	// UNARY TRANSFORM
	template <class T1, class Tout, typename F>
	inline void transform(const many<T1>& a, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i]);
		}
	}
	template <class T1, class Tout, typename F>
	inline void transform(const T1 a, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a);
		}
	}




	// BINARY TRANSFORM
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const many<T1>& a, const many<T2>& b, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i]);
		}
	}
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const many<T1>& a, const T2 b, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b);
		}
	}
	template <class T1, class T2, class Tout, typename F>
	inline void transform(const T1 a, const many<T2>& b, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i]);
		}
	}











	// TRINARY TRANSFORM
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const many<T1>& a, const many<T2>& b, const many<T3>& c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const many<T1>& a, const many<T2>& b, const T3 c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b[i], c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const many<T1>& a, const T2 b, const many<T3>& c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const many<T1>& a, const T2 b, const T3 c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = f(a[i], b, c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const many<T2>& b, const many<T3>& c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c[i]);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const many<T2>& b, const T3 c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < b.size(); ++i)
		{
			out[i] = f(a, b[i], c);
		}
	}
	template <class T1, class T2, class T3, class Tout, typename F>
	inline void transform(const T1 a, const T2 b, const many<T3>& c, F f, many<Tout>& out)
	{
		for (unsigned int i = 0; i < c.size(); ++i)
		{
			out[i] = f(a, b, c[i]);
		}
	}







	template <class T, class T2>
	void greaterThan(const many<T>& a, const T2 b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <class T, class T2>
	void greaterThanEqual(const many<T>& a, const T2 b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <class T, class T2>
	void lessThan(const many<T>& a, const T2 b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}
	template <class T, class T2>
	void lessThanEqual(const many<T>& a, const T2 b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}



	template <class T, class T2>
	void greaterThan(const many<T>& a, const many<T2>& b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai > bi; }, out); 
	}
	template <class T, class T2>
	void greaterThanEqual(const many<T>& a, const many<T2>& b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai >= bi; }, out); 
	}
	template <class T, class T2>
	void lessThan(const many<T>& a, const many<T2>& b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai <= bi; }, out); 
	}
	template <class T, class T2>
	void lessThanEqual(const many<T>& a, const many<T2>& b, many<bool>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai < bi; }, out); 
	}



	template <class T, class T2, class T3>
	void add(const many<T>& a, const T2 b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <class T, class T2, class T3>
	void sub(const many<T>& a, const T2 b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <class T, class T2, class T3>
	void mult(const many<T>& a, const T2 b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const many<T>& a, const T2 b, many<T3>& out)
	{
		const T2 binv = T2(1.)/b;
		transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}



	// NOTE: we define operators for multiple classes T and T2 in order to support 
	//  vector/scalar multiplication, matrix/vect multiplication, etc.
	template <class T, class T2, class T3>
	void add(const many<T>& a, const many<T2>& b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai + bi; }, out); 
	}
	template <class T, class T2, class T3>
	void sub(const many<T>& a, const many<T2>& b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai - bi; }, out); 
	}
	template <class T, class T2, class T3>
	void mult(const many<T>& a, const many<T2>& b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai * bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const many<T>& a, const many<T2>& b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}
	template <class T, class T2, class T3>
	void div(const T a, const many<T2>& b, many<T3>& out)
	{
		transform(a, b, [](T ai, T2 bi){ return ai / bi; }, out); 
	}
	











	// NOTE: all wrappers are suggested to be inline because they are thin wrappers of functions



	typedef many<bool>	       bools;
	typedef many<int>	       ints;
	typedef many<unsigned int> uints;
	typedef many<float>	       floats;
	typedef many<double>       doubles;
}

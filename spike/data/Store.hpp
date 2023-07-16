#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

namespace data
{


	/*
	`Uniform` packages a single value of arbitrary data type so that
	it may operate as an indexible data type similar to `Series` and `Raster`.
	For any index given, its indexing operation will always return a single value.
	`Uniform` is used to reduce the number of function overloads that must be implemented 
	for combinations of series, rasters, and scalar values.
	*/
	template<typename T>
	struct Uniform
	{
		T value;
		explicit Uniform(const T& value) :
			value(value) 
		{
		}

		// copy constructor
		Uniform(const Uniform& a) :
			value(a.value)
		{}

		inline T operator[](const std::size_t id ) const
		{
		   return value;
		}

		template <typename T2>
		inline void fill(const T2 a )
		{
			value = a;
		}

		template<typename T2>
		inline std::size_t size(const T2& v) const {
			return v.size();
		}

		template<typename T2>
		inline T2 footprint(const T& v) const {
			return v;
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
	template<typename T>
	Uniform<T> uniform(const T& value)
	{
		return Uniform<T>(value);
	}

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

	/*
	`mapping` is used as a template parameter for rasters to indicate whether 
	the Raster maps each element of its container to a cell or an arrow within its grid.
	*/
	enum mapping
	{
		cell,
		arrow
	};

	/*
	`Raster` operates much like `Series` with the additional constraint
	that values correspond to points on a user specified grid.
	`Raster` guarantees that it will only operate on other data structures
	that are compatible with its grid. 
	*/
	template<typename T, typename Tgrid, mapping Tmap = mapping::cell>
	struct Raster
	{
		
	protected:
		std::vector<T> values;

	public:

		Tgrid grid;
		explicit Raster(const Tgrid& grid):
			values(grid.cell_count(Tmap)),
			grid(grid) 
		{
		}

		// std container style constructor
		template<typename TIterator>
		explicit Raster(const Tgrid& grid, TIterator first, TIterator last) : 
			values(grid.cell_count(Tmap)),
			grid(grid)
		{
			assert(std::distance(first, last) == this->size());
			std::size_t id = 0;
			while (first!=last) 
			{
				this->values[id] = *first;
				++first;
				++id;
			}
		}

		// copy constructor
		Raster(const Raster<T,Tgrid,Tmap>& a)  : 
			values(a.values),
			grid(a.grid)
		{}

		// convenience constructor for vectors
		explicit Raster(const Tgrid& grid, const std::initializer_list<T>& vector) : 
			values(grid.cell_count(Tmap)),
			grid(grid)
		{
			assert(vector.size() == this->size());
			std::copy(vector.begin(), vector.end(), this->begin());
		}

		template <typename T2>
		explicit Raster(const Raster<T2,Tgrid>& a)  : 
			values(a.values),
			grid(a.grid) 
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				this->values[i] = a[i];
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
		inline Raster<T,Tgrid,Tmap> operator[](const Series<Tid>& ids )
		{
			Raster<T,Tgrid,Tmap> out = Raster<T,Tgrid,Tmap>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Raster<T,Tgrid,Tmap> operator[](const Series<bool>& mask )
		{
			Raster<T,Tgrid,Tmap> out = Raster<T,Tgrid,Tmap>(mask.size());
			get(*this, mask, out);
			return out;
		}

		template<typename Tid>
		inline Raster<T,Tgrid,Tmap> operator[](const Raster<Tid,Tgrid,Tmap>& ids )
		{
			Raster<T,Tgrid,Tmap> out = Raster<T,Tgrid,Tmap>(ids.size());
			get(*this, ids, out);
			return out;
		}

		inline Raster<T,Tgrid,Tmap> operator[](const Raster<bool,Tgrid,Tmap>& mask )
		{
			Raster<T,Tgrid,Tmap> out = Raster<T,Tgrid,Tmap>(mask.size());
			get(*this, mask, out);
			return out;
		}

		inline Raster<T,Tgrid,Tmap>& operator=(const Raster<T,Tgrid,Tmap>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline Raster<T,Tgrid,Tmap>& operator=(const T& other )
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
		inline void fill(const T2 a)
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
		inline std::size_t size(const Raster<T2,Tgrid,Tmap>& r) const {
			assert(grid == r.grid);
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Uniform<T2>& u, const Raster<T3,Tgrid,Tmap>& r) const {
			assert(grid == r.grid);
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Raster<T2,Tgrid,Tmap>& r, const Uniform<T3>& u) const {
			assert(grid == r.grid);
			assert(values.size() == r.size());
			return values.size();
		}

		template<typename T2, typename T3>
		inline std::size_t size(const Raster<T2,Tgrid,Tmap>& r, const Raster<T3,Tgrid,Tmap>& s) const {
			assert(grid == r.grid);
			assert(grid == s.grid);
			assert(values.size() == r.size());
			assert(values.size() == s.size());
			return values.size();
		}

		template<typename T2>
		inline Raster<T,Tgrid,Tmap> footprint(const Uniform<T2>& u) const {
			return Raster<T,Tgrid,Tmap>(this->grid);
		}

		template<typename T2>
		inline Raster<T,Tgrid,Tmap> footprint(const Raster<T,Tgrid,Tmap>& r) const {
			return Raster<T,Tgrid,Tmap>(this->grid);
		}

	};

	/*
	NOTE: constructing rasters can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid.
	Typical C++ conventions might append these with `make_*`,
	however we forego this convention for brevity since 
	functions like `uniform` are called frequently to avoid implicit casts,
	we have found the typical convention is far too verbose.
	*/

	template<typename T, mapping Tmap = mapping::cell, typename Tgrid>
	Raster<T,Tgrid,Tmap> raster(const Tgrid& grid)
	{
		return Raster<T,Tgrid,Tmap>(grid);
	}
	template<typename T, mapping Tmap=mapping::cell, typename Tgrid>
	Raster<T,Tgrid,Tmap> raster(const Tgrid& grid, const std::initializer_list<T>& vector)
	{
		return Raster<T,Tgrid,Tmap>(grid, vector);
	}
	template<typename T, typename TIterator, mapping Tmap=mapping::cell, typename Tgrid>
	Raster<T,Tgrid,Tmap> raster(const Tgrid& grid, TIterator first, TIterator last)
	{
		return Raster<T,Tgrid,Tmap>(grid, first, last);
	}


	template <typename T1, typename T2, typename Tout>
	void add(const T1& a, const T2& b, Tout& out)
	{
		auto size = out.size(a, b);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] + b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void sub(const T1& a, const T2& b, Tout& out)
	{
		auto size = out.size(a, b);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] - b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void mult(const T1& a, const T2& b, Tout& out)
	{
		auto size = out.size(a, b);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] * b[i];
		}
	}

	template <typename T1, typename T2, typename Tout>
	void div(const T1& a, const T2& b, Tout& out)
	{
		auto size = out.size(a, b);
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = a[i] / b[i];
		}
	}

	float distance(const bool a, const bool b)
	{
		return std::abs(a-b);
	}

	float distance(const int a, const int b)
	{
		return std::abs(a-b);
	}

	float distance(const float a, const float b)
	{
		return std::abs(a-b);
	}

	template <typename T1, typename T2>
	float distance(const T1& a, const T2& b)
	{
		float out(0);
		auto size = a.size(b);
		for (std::size_t i = 0; i < size; ++i)
		{
			out += distance(a[i] - b[i]);
		}
		return out;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b)
	{
		auto size = a.size(b);
		for (std::size_t i = 0; i < size; ++i)
		{
			if (a[i] != b[i]){
				return false;
			}
		}
		return true;
	}

	template <typename T1, typename T2>
	bool equal(const T1& a, const T2& b, const float threshold)
	{
		return distance(a,b) < threshold;
	}


}


#pragma once

// C libraries
#include <cmath>     /* assert */
#include <assert.h>  /* assert */

// std libraries
#include <initializer_list>	// initializer_list
#include <iterator>			// std::distance
#include <vector>           // std::vector
#include <memory>

namespace series
{

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
	A `Raster` behaves as a `Series` where values have additional structure that is imposed by a "grid".
	The grid carries additional information that permits operations to be done on the `Raster` 
	that could not otherwise be done on the `Series`, such as anything that requires spatial awareness.
	The additional information that is provided by the grid could be passed 
	to such operations as separate parameters, but doing so increases the complexity of the call 
	and reduces type safety by permitting rasters to be erroneously reinterpreted as being part of other grids.

	For any nonspatial operation, a `Raster` behaves exactly as a `Series`, so the type system places no constraints 
	on what kinds of grids are required for two rasters to participate in the same operation.
	This approach permits some invalid behavior such as performing operations on rasters of incompatible grid types,
	however it also permits more complex behavior that we anticipate will be necessary, 
	such as operating on Rasters whose grids are of different resolution or layer count.
	In practice, we find that the type safety issues that arise from incompatible grid types do not occur often, 
	and ignoring such issues vastly reduces the complexity of the implementation.
	*/
	template<typename T, typename Grid, mapping Map = mapping::cell>
	struct Raster
	{

	protected:
		std::vector<T> values;
		Grid grid;

	public:
		// copy constructor
		Raster(const Raster<T,Grid,Map>& a)  : 
			values(a.values),
			grid(a.grid)
		{}

		explicit Raster(const Grid& grid):
			values(Map==mapping::cell? grid.vertex_count(Map) : grid.arrow_count()),
			grid(grid)
		{
		}

		// std container style constructor
		template<typename TIterator>
		explicit Raster(const Grid& grid, TIterator first, TIterator last) : 
			values(Map==mapping::cell? grid.vertex_count(Map) : grid.arrow_count()),
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


		// convenience constructor for vectors
		explicit Raster(const Grid& grid, const std::initializer_list<T>& vector) : 
			values(Map==mapping::cell? grid.vertex_count(Map) : grid.arrow_count()),
			grid(grid)
		{
			assert(vector.size() == this->size());
			std::copy(vector.begin(), vector.end(), this->begin());
		}

		template <typename T2>
		explicit Raster(const Raster<T2,Grid>& a)  : 
			values(a.values),
			grid(a.grid,)
		{
			for (std::size_t i = 0; i < a.size(); ++i)
			{
				this->values[i] = a[i];
			}
		}



		// NOTE: all wrapper functions should to be marked inline 
	    using size_type = std::size_t;
		using value_type = T;
		using const_reference = const value_type&;
		using reference = value_type&;
		using const_iterator = typename std::vector<T>::const_iterator;
		using iterator = typename std::vector<T>::iterator;
		inline std::size_t size() const                               { return values.size() * copies_per_value;  }
		inline std::size_t max_size() const                           { return values.max_size() * copies_per_value;  }
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

		inline typename std::vector<T>::const_reference operator[](const std::size_t memory_id ) const
		{
		   return values.operator[]((memory_id / copies_per_value) % values.size());
		}

		inline typename std::vector<T>::reference operator[](const std::size_t memory_id )
		{
		   return values[(memory_id / copies_per_value) % values.size()]; // reference return 
		}


		inline Raster<T,Grid,Map>& operator=(const Raster<T,Grid,Map>& other )
		{
			values.resize(other.size());
			copy(*this, other);
			return *this;
		}
		inline Raster<T,Grid,Map>& operator=(const T& other )
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
        constexpr Series<T>& operator+=(const T2& a)
        {
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] += a[i];
			}
            return *this;
        }

        template<typename T2>
		constexpr Series<T>& operator-=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] -= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator*=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] *= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator/=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] /= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator%=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] %= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator&=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] &= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator|=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] |= a[i];
			}
			return *this;
		}

		template<typename T2>
		constexpr Series<T>& operator^=(const T2& a)
		{
			assert(compatible(a));
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				values[i] ^= a[i];
			}
			return *this;
		}

		inline std::vector<T>& vector()
		{
			return values;
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

	template<typename T, mapping Map = mapping::cell, typename Grid>
	Raster<T,Grid,Map> raster(const Grid& grid)
	{
		return Raster<T,Grid,Map>(grid);
	}
	template<typename T, mapping Map=mapping::cell, typename Grid>
	Raster<T,Grid,Map> raster(const Grid& grid, const std::initializer_list<T>& vector)
	{
		return Raster<T,Grid,Map>(grid, vector);
	}
	template<typename T, typename TIterator, mapping Map=mapping::cell, typename Grid>
	Raster<T,Grid,Map> raster(const Grid& grid, TIterator first, TIterator last)
	{
		return Raster<T,Grid,Map>(grid, first, last);
	}

	template<typename T, typename Grid, mapping Map>
	inline Raster<T,Grid,Map> reshape(const Raster<T,Grid,Map>& a, const std::size_t copies_per_value, Raster<T,Grid,Map>& out)
	{
		if (out.front() != a.front())
		{
			std::copy(a.begin(), a.end(), a.begin());
		}
		out.copies_per_value = copies_per_value;
	}

}


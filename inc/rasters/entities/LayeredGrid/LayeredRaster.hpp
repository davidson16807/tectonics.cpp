#pragma once


// std libraries
#include <memory>

// 3rd party libraries
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/types.hpp>
#include <series/glm/types.hpp>

#include "../Grid/Raster.hpp"

namespace rasters
{

	/*
	`Raster` guarantees that its constructors will produce a valid object 
	that represents a map between cells in a grid and values of type T.
	e.g. the number of elements will match the number of grid cells.
	This will apply for any input that's passed to the constructors.
	Violation of this guarantee is a bug.
	*/
	template<typename T, typename Tgrid, rasters::mapping Tmap = rasters::mapping::cell>
	struct LayeredRaster: public Raster<T,Tgrid,Tmap>
	{
		/*
		NOTE: Grids are composed exclusively from shared pointers, 
		so there is little performance penalty in copying them.

		We compose rasters from grid but inherit from series::Series<T>.
		Inheriting from series::Series<T> allows us to inherit all the functionality 
		that makes it easy to work with series::Series<T> (e.g. stl container behavior and operator overloads)

		However, composing from grid allows us to use templates to implement a single Raster class for all grid classes.
		Template functions can make use of grid class attributes without having to introduce a complex class hierarchy.
		Composing from grid also allows us to implement Raster without knowing how shared pointers are managed within Grids,
		and it allows us to construct Raster without requiring Raster to be a friend class of Grids.
		*/
		LayeredRaster(const Tgrid& grid):
			Raster<T,Tgrid,Tmap>(grid)
		{
			assert(this->grid.layering->layer_count < this->grid.cache->vertex_count);
			assert(this->values.size() == this->grid.cell_count(Tmap));
		}
		// std container style constructor
		template<typename TIterator>
		LayeredRaster(const Tgrid& grid, TIterator first, TIterator last) : 
			LayeredRaster(grid)
		{
			assert(this->grid.layering->layer_count < this->grid.cache->vertex_count);
			assert(this->values.size() == this->grid.cell_count(Tmap));
			assert(std::distance(first, last) == this->size());
			unsigned int id = 0;
			while (first!=last) 
			{
				this->values[id] = *first;
				++first;
				++id;
			}
		}

		// copy constructor
		LayeredRaster(const LayeredRaster<T,Tgrid,Tmap>& a)  : 
			LayeredRaster(a.grid)
		{
			assert(this->grid.layering->layer_count < this->grid.cache->vertex_count);
			assert(this->values.size() == this->grid.cell_count(Tmap));
		}

		// convenience constructor for vectors
		explicit LayeredRaster(const Tgrid& grid, const std::initializer_list<T>& vector) : 
			LayeredRaster(grid)
		{
			assert(this->grid.layering->layer_count < this->grid.cache->vertex_count);
			assert(this->values.size() == this->grid.cell_count(Tmap));
			assert(vector.size() == this->values.size());
			std::copy(vector.begin(), vector.end(), this->begin());
		}
		template <typename T2>
		explicit LayeredRaster(const LayeredRaster<T2,Tgrid>& a)  : 
			LayeredRaster(a.grid)
		{
			assert(this->grid.layering->layer_count < this->grid.cache->vertex_count);
			assert(this->values.size() == this->grid.cell_count(Tmap));
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				this->values[i] = a[i];
			}
		}



		/*
		Design principles: 
		* The footprint of a tensor should ideally be constructed so that the dimension with the smallest footprint is looped through most frequently.
		* Our simplified use case (layered columns over a mesh) uses at most a rank-2 tensor where we can assume one dimension is always smaller.
		* If the dimensions were to match, the size of each dimension would be the square root of the number of elements in the tensor.
		* Following from the statements above, we want to cycle over any dimension that's smaller than the square root of the number of elements.
		*/

		// UNARY TRANSFORM
		template <typename F, typename T1>
		inline void store(const F f, const T1& a) 
		{
			std::size_t L = this->grid.layering->layer_count;
			std::size_t V = this->grid.column_count(Tmap);
			assert(this->values.size() == this->grid.cell_count(Tmap));

			std::size_t La = this->values.size() / a.size();
			assert(this->values.size() % a.size() == 0 && (La == V || La == L || La == 1));

			if (La==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i%La]);
				} 
			} 
			else 
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i/La]);
				}
			}
		}

		// BINARY TRANSFORM
		template <typename F, typename T1, typename T2, 
			std::enable_if_t<std::is_base_of<series::AbstractSeries, T1>::value && std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const T1& a, const T2& b) 
		{
			std::size_t L = this->grid.layering->layer_count;
			std::size_t V = this->grid.column_count(Tmap);
			assert(this->values.size() == this->grid.cell_count(Tmap));

			std::size_t La = this->values.size() / a.size();
			assert(this->values.size() % a.size() == 0 && (La == V || La == L || La == 1));

			std::size_t Lb = this->values.size() / b.size();
			assert(this->values.size() % b.size() == 0 && (Lb == V || Lb == L || Lb == 1));

			if (La==V && Lb==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i%La], b[i%Lb]);
				}
			}
			else if(La==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i%La], b[i/Lb]);
				}
			}
			else if(Lb==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i/La], b[i%Lb]);
				}
			}
			else
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i/La], b[i/Lb]);
				}
			}
		}
		template <typename F, typename T1, typename T2, 
			std::enable_if_t<std::is_base_of<series::AbstractSeries, T1>::value && !std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const T1& a, const T2 b)
		{
			std::size_t L = this->grid.layering->layer_count;
			std::size_t V = this->grid.column_count(Tmap);
			assert(this->values.size() == this->grid.cell_count(Tmap));

			std::size_t La = this->values.size() / a.size();
			assert(this->values.size() % a.size() == 0 && (La == V || La == L || La == 1));

			if (La==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i%La], b);
				} 
			} 
			else 
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a[i/La], b);
				}
			}
		}
		template <typename F, typename T1, typename T2, 
			std::enable_if_t<!std::is_base_of<series::AbstractSeries, T1>::value && std::is_base_of<series::AbstractSeries, T2>::value, int> = 0>
		inline void store(const F f, const T1 a, const T2& b)
		{
			std::size_t L = this->grid.layering->layer_count;
			std::size_t V = this->grid.column_count(Tmap);
			assert(this->values.size() == this->grid.cell_count(Tmap));

			std::size_t Lb = this->values.size() / b.size();
			assert(this->values.size() % b.size() == 0 && (Lb == V || Lb == L || Lb == 1));

			if (Lb==V)
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a, b[i%Lb]);
				} 
			} 
			else 
			{
				for (std::size_t i = 0; i < this->values.size(); ++i)
				{
					this->values[i] = f(a, b[i/Lb]);
				}
			}
		}

		// TRINARY TRANSFORM




	};
	
	/*
	NOTE: constructing rasters can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid
	*/
	template<typename T, rasters::mapping Tmap = rasters::mapping::cell, typename Tgrid>
	LayeredRaster<T,Tgrid,Tmap> make_LayeredRaster(const Tgrid& grid)
	{
		return LayeredRaster<T,Tgrid,Tmap>(grid);
	}
	template<typename T, rasters::mapping Tmap=rasters::mapping::cell, typename Tgrid>
	LayeredRaster<T,Tgrid,Tmap> make_LayeredRaster(const Tgrid& grid, const std::initializer_list<T>& vector)
	{
		return LayeredRaster<T,Tgrid,Tmap>(grid, vector);
	}
	template<typename T, typename TIterator, rasters::mapping Tmap=rasters::mapping::cell, typename Tgrid>
	LayeredRaster<T,Tgrid,Tmap> make_LayeredRaster(const Tgrid& grid, TIterator first, TIterator last)
	{
		return LayeredRaster<T,Tgrid,Tmap>(grid, first, last);
	}

}
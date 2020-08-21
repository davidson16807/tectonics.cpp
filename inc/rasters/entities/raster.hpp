#pragma once


// std libraries
#include <memory>

// 3rd party libraries
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/types.hpp>
#include <many/glm/types.hpp>

namespace rasters
{
	/*
	`mapping` is used as a template parameter for rasters to indicate whether 
	the raster maps each element of its container to a cell or an arrow within its grid.
	*/
	enum mapping
	{
		cell,
		arrow
	};

	/*
	`raster` guarantees that its constructors will produce a valid object 
	that represents a map between cells in a grid and values of type T.
	e.g. the number of elements will match the number of grid cells.
	This will apply for any input that's passed to the constructors.
	Violation of this guarantee is a bug.
	*/
	template<typename T, typename Tgrid, rasters::mapping Tmap = rasters::mapping::cell>
	struct raster final: public many::series<T>
	{
		/*
		NOTE: Grids are composed exclusively from shared pointers, 
		so there is little performance penalty in copying them.

		We compose rasters from grid but inherit from many::series<T>.
		Inheriting from many::series<T> allows us to inherit all the functionality 
		that makes it easy to work with many::series<T> (e.g. stl container behavior and operator overloads)

		However, composing from grid allows us to use templates to implement a single raster class for all grid classes.
		Template functions can make use of grid class attributes without having to introduce a complex class hierarchy.
		Composing from grid also allows us to implement raster without knowing how shared pointers are managed within Grids,
		and it allows us to construct raster without requiring raster to be a friend class of Grids.
		*/
		Tgrid grid;
		raster(const Tgrid& grid):
			many::series<T>(Tmap == rasters::mapping::cell? grid.cell_count() : grid.arrow_count()),
			grid(grid) 
		{}
		// std container style constructor
		template<typename TIterator>
		raster(const Tgrid& grid, TIterator first, TIterator last) : 
			many::series<T>(Tmap == rasters::mapping::cell? grid.cell_count() : grid.arrow_count()),
			grid(grid)
		{
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
		raster(const raster<T,Tgrid,Tmap>& a)  : 
			many::series<T>(a),
			grid(a.grid)
		{}

		// convenience constructor for vectors
		explicit raster(const Tgrid& grid, const std::initializer_list<T>& vector) : 
			many::series<T>(Tmap == rasters::mapping::cell? grid.cell_count() : grid.arrow_count()),
			grid(grid)
		{
			assert(vector.size() == this->size());
			std::copy(vector.begin(), vector.end(), this->begin());
		}
		template <typename T2>
		explicit raster(const raster<T2,Tgrid>& a)  : 
			many::series<T>(a),
			grid(a.grid) 
		{
			for (unsigned int i = 0; i < a.size(); ++i)
			{
				this->values[i] = a[i];
			}
		}
	};
	
	/*
	NOTE: constructing rasters can be annoying due to the number of template parameters involved, 
	so we use convenience methods for generating rasters that are compatible for a given grid
	*/
	template<typename T, typename Tgrid, rasters::mapping Tmap=rasters::mapping::cell>
	raster<T,Tgrid,Tmap> make_raster(const Tgrid& grid)
	{
		return raster<T,Tgrid,Tmap>(grid);
	}
	template<typename T, typename Tgrid, rasters::mapping Tmap=rasters::mapping::cell>
	raster<T,Tgrid,Tmap> make_raster(const Tgrid& grid, const std::initializer_list<T>& vector)
	{
		return raster<T,Tgrid,Tmap>(grid, vector);
	}
	template<typename T, typename Tgrid, typename TIterator, rasters::mapping Tmap=rasters::mapping::cell>
	raster<T,Tgrid,Tmap> make_raster(const Tgrid& grid, TIterator first, TIterator last)
	{
		return raster<T,Tgrid,Tmap>(grid, first, last);
	}
}
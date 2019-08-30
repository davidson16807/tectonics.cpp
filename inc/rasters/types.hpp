#pragma once

#include <initializer_list>	// initializer_list
#include <memory>	        // std::shared_ptr

#include <many/types.hpp> // floats, etc.

#include "Grid.hpp"            // Grid

namespace rasters
{

	// This template represents a statically-sized contiguous block of heap memory occupied by the primitive data of the same arbitrary type
	// Each data entry within the block maps to some vertex within an arbitrary 3d mesh
	// the data type held in each vertex should be small enough to fit in a computer's register (e.g. ints, floats, and even vec3s)
	// the data type must have basic operators common to all primitives: == != 
	// 
	// Q: Why not favor composition over inheritance? Why inherit from composite<T>?
	// A: We've invested a lot of code to streamline mathematical expressions using composite<T>,
	//      we can guarantee this functionality will be needed for rasters,
	//      and duplicating this code for use with rasters would be pretty insane.
	template <class T>
	class traster : public composite<T>
	{
	public:
		std::shared_ptr<Grid> grid;

		// destructor: delete pointer 
		~traster()
		{
		};

		// copy constructor
		traster(const traster<T>& a)
			: composite<T>(a),
			  grid(a.grid)
		{

		}

		// initializer list constructor
		explicit traster(const std::shared_ptr<Grid>& grid, std::initializer_list<T> list) 
			: composite<T>(list),
			  grid(grid)
		{

		};
		template<class TIterator>
		explicit traster(const std::shared_ptr<Grid>& grid, TIterator first, TIterator last)
			: composite<T>(grid->vertex_positions.size()),
			  grid(grid)
		{

		}

		template <class T2>
		explicit traster(const traster<T2>& a)
			: composite<T>(a),
			  grid(a.grid)
		{

		}

		explicit traster(const std::shared_ptr<Grid>& grid)
			: composite<T>(grid->vertex_positions.size()),
			  grid(grid)
		{

		}

		explicit traster(const std::shared_ptr<Grid>& grid, const T a)
			: composite<T>(grid->vertex_positions.size(), a),
			  grid(grid)
		{

		}

		template <class T2>
		explicit traster(const std::shared_ptr<Grid>& grid, const composite<T2>& a)
			: composite<T2>(a),
			  grid(grid)
		{
			
		}
	};

	typedef traster<bool>	     braster;
	typedef traster<int>	     iraster;
	typedef traster<unsigned int>uraster;
	typedef traster<float>	      raster;
	typedef traster<double>      draster;
}

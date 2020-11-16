#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <rasters/entities/Grid/Raster.hpp> // Strata
#include <models/strata/StrataStore.hpp> // Strata

namespace crust
{
	template <int L, int M, typename Tgrid>
    using Crust = rasters::Raster<strata::StrataStore<L,M>, Tgrid>;

	template<int L, int M, typename Tgrid>
	Crust<L,M,Tgrid> make_Crust(const Tgrid& grid)
	{
		return Crust<L,M,Tgrid>(grid);
	}
	// template<typename T, rasters::mapping Tmap=rasters::mapping::cell, typename Tgrid>
	// Crust<T,Tgrid,Tmap> make_Crust(const Tgrid& grid, const std::initializer_list<T>& vector)
	// {
	// 	return Crust<T,Tgrid,Tmap>(grid, vector);
	// }
	// template<typename T, typename TIterator, rasters::mapping Tmap=rasters::mapping::cell, typename Tgrid>
	// Crust<T,Tgrid,Tmap> make_Crust(const Tgrid& grid, TIterator first, TIterator last)
	// {
	// 	return Crust<T,Tgrid,Tmap>(grid, first, last);
	// }
} 

#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <rasters/entities/Grid/Raster.hpp> // Strata
#include <models/strata/StrataStore.hpp> // Strata

namespace crust
{
	template <std::size_t L, std::size_t M, typename Tgrid>
    using Crust = rasters::Raster<strata::StrataStore<L,M>, Tgrid>;

	template<std::size_t L, std::size_t M, typename Tgrid>
	Crust<L,M,Tgrid> make_Crust(const Tgrid& grid)
	{
		return Crust<L,M,Tgrid>(grid);
	}
	template<std::size_t L, std::size_t M, typename Tgrid>
	Crust<L,M,Tgrid> make_Crust(const Tgrid& grid, const std::initializer_list<strata::Strata<L,M>>& vector)
	{
		assert(vector.size() == grid.cache->vertex_count);
		Crust<L,M,Tgrid> crust = Crust<L,M,Tgrid>(grid);
		strata::StrataStore<L,M> store;
        int j(0);
        for (auto i = vector.begin(); i != vector.end(); ++i)
        {
			store.pack(*i);
            crust[j] = store;
            ++j;
        }
	}
	// template<typename T, typename TIterator, rasters::mapping Tmap=rasters::mapping::cell, typename Tgrid>
	// Crust<T,Tgrid,Tmap> make_Crust(const Tgrid& grid, TIterator first, TIterator last)
	// {
	// 	return Crust<T,Tgrid,Tmap>(grid, first, last);
	// }
} 

#pragma once

// in-house libraries
#include <rasters/entities/Grid/Raster.hpp> // Strata
#include <model/strata/StrataValues.hpp> // Strata

namespace crust
{
	template <typename T, std::size_t L, typename Tgrid>
    using CrustValues = rasters::Raster<strata::StrataValues<T, L>, Tgrid>;

	template<typename T, std::size_t L, typename Tgrid>
	CrustValues<T,L,Tgrid> make_CrustValues(const Tgrid& grid)
	{
		return CrustValues<T,L,Tgrid>(grid);
	}

	template<typename T, std::size_t L, typename Tgrid>
	CrustValues<T,L,Tgrid> make_CrustValues(const Tgrid& grid, const std::initializer_list<strata::StrataValues<T,L>>& list)
	{
		assert(list.size() == grid.structure->vertex_count);
		return CrustValues<T,L,Tgrid>(grid, list);
	}
}


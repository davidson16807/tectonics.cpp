#pragma once

// in-house libraries
#include <rasters/entities/Grid/Raster.hpp> // Strata
#include <models/strata/StrataValues.hpp> // Strata

namespace crust
{
	template <typename T, std::size_t L, typename Tgrid>
    using CrustValues = rasters::Raster<strata::StrataValues<T, L>, Tgrid>;

	template<typename T, std::size_t L, typename Tgrid>
	CrustValues<T,L,Tgrid> make_CrustValues(const Tgrid& grid)
	{
		return CrustValues<T,L,Tgrid>(grid);
	}
}


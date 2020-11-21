#pragma once

// in-house libraries
#include "CrustValues.hpp" // CrustValues

namespace crust
{
	struct StratumBorders
    {
    	float lower;
    	float upper;
        StratumBorders() :
            lower(0),
            upper(0)
        {

        }
        StratumBorders(
            float lower, 
            float upper
        ) :
            lower(lower),
            upper(upper)
        {

        }
    };
	template <std::size_t L, typename Tgrid>
    using StrataBorders = CrustValues<StratumBorders, L, Tgrid>;
    
    template<std::size_t L, typename Tgrid>
    StrataBorders<L,Tgrid> make_StrataBorders(const Tgrid& grid)
    {
        return StrataBorders<L,Tgrid>(grid);
    }
    
    template<std::size_t L, typename Tgrid>
    StrataBorders<L,Tgrid> make_StrataBorders(const Tgrid& grid, const std::initializer_list<strata::StrataValues<StratumBorders,L>> list)
    {
        return make_CrustValues(grid, list);
    }
}


#pragma once

// in-house libraries
#include "CrustValues.hpp" // CrustValues

namespace crust
{
	struct StratumBoundaries
    {
    	float lower;
    	float upper;
        StratumBoundaries() :
            lower(0),
            upper(0)
        {

        }
        StratumBoundaries(
            float lower, 
            float upper
        ) :
            lower(lower),
            upper(upper)
        {

        }
    };
	template <std::size_t L, typename Tgrid>
    using StrataBoundaries = CrustValues<StratumBoundaries, L, Tgrid>;
    
	template<std::size_t L, typename Tgrid>
	StrataBoundaries<L,Tgrid> make_StrataBoundaries(const Tgrid& grid)
	{
		return StrataBoundaries<L,Tgrid>(grid);
	}
}


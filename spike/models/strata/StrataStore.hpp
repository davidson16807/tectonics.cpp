#pragma once

// C libraries
#include <cstdint>

// std libraries
#include <array>

// in-house libraries
#include <models/stratum/StratumStore.hpp>
#include "Strata.hpp"

namespace strata
{
    /*
    `StrataStore` is a memory efficient variant of the 
    `Strata` data structure. Isn't it adorable!?
    It would take ridiculous amounts of memory to store a `Strata` 
    for every strata within a raster, so we store each strata in a raster
    as a `StrataStore`, then convert back to `Strata` when
    we want to perform some operation on it. 

    The interpretation of attributes within `StrataStore` is error prone,
    so to prevent users from doing so we encapsulate the class.

    The interpretation of attributes also comes with some performance penalty,
    so to encourage users not to spam calls to getters, 
    we only expose methods to convert to and from `StratumStore`.
    
    This also grants a certain mathematical purity to the object,
    since in the spirit of category theory the object at high level can be 
    treated strictly by its mappings to other states, 
    which in this case are isomorphic and invertible.
    */
    template <int L, int M>
    class StrataStore
    {
        std::array<stratum::StratumStore<M>, L> strata;
        std::uint16_t count;
    public:
        void decompress(Strata<L,M>& output) const
        {
            for (std::size_t i=0; i<L; i++)
            {
                strata[i].decompress(output.content[i]);
            }
            output.count = count;
        }
        void compress(const Strata<L,M>& input)
        {
            for (std::size_t i=0; i<L; i++)
            {
                strata[i].compress(input.content[i]);
            }
            count = input.count;
        }
    };
}

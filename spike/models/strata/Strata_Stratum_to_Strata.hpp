#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <models/stratum/Stratum_to_StratumTypes.hpp>
#include <models/stratum/Stratum_operators.hpp>
#include "Strata.hpp"

namespace strata
{
    /*
    Deposits a new layer to the top of the strata.
    Merges the deposited layer with the second layer down if there is a resembance between them.
    If the are more layers than what can be stored in memory, we combine the bottom excess layers into one. 
    We only allow combining the bottom two since doing so simplifies the problem when working with stack memory.
    This is fine, since we care the least about properly representing the bottom layers,
    just as long as the mass in each pool is conserved.
    Functionally equivalent to `overlap()` if `bottom` were a Strata object with 0 or 1 layers,
    */
    template <int L, int M>
    static void deposit(const Strata<L,M>& bottom, const stratum::Stratum<M>& top, Strata<L,M>& output)
    {
        int i;
        int offset = 0;
        if (stratum::get_stratum_types(bottom.content[0]).hash() != stratum::get_stratum_types(top).hash() &&
            top.mass() > 1e-4f)
        {
            output.content[0] = top;
            offset = 1;
            i = 0;
        }
        else 
        {
            stratum::combine(top, bottom.content[0], output.content[0]);
            offset = 0;
            i = 1;
        }
        for (; offset+i < L; ++i)
        {
            output.content[offset+i] = bottom.content[i];
        }
        for (; i < bottom.count && offset+i >= L; ++i)
        {
            stratum::combine(output.content[L-1], bottom.content[i], output.content[L-1]);
        }
        output.count = std::min(offset + bottom.count, L);
    }
}

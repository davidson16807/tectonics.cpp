#pragma once

#include <catch/catch.hpp>

#include "Stratum.hpp"
#include "StratumMassPool_test_utils.hpp"

namespace strata
{
    /*
    NOTE: 
    This file does not exist within the concept of the mathematical category defined in README.md.
    This is because the file is only relevant within the context of several potential unit test files.
    We differ from the usual style of method signature here since performance does not matter in test code. 
    We also use macros since we require running unit tests on individual attributes for granularity, 
    and our testing framework itself uses macros to run the unit tests.
    Our chief concern here is to simplify writing unit tests and interpreting their output.
    */

    template<int M, typename Tgenerator>
    Stratum<M> get_random_stratum(Tgenerator generator)
    {
        Stratum<M> output(generator(), generator(), generator());
        for (int i = 0; i < M; ++i)
        {
            output.mass_pools[i] = get_random_stratum_mass_pool(generator);
        }
        return output;
    }
    
    #define STRATUM_EQUAL(stratum1, stratum2)                                                  \
        for (int pool_i = 0; pool_i < M; ++pool_i)         \
        {                                                                        \
            STRATUM_MASS_POOL_EQUAL(stratum1.mass_pools[pool_i], stratum2.mass_pools[pool_i])  \
        } 
    
    #define STRATUM_VALID(stratum1)                                              \
        for (int pool_i = 0; pool_i < M; ++pool_i)  \
        {                                                                 \
            STRATUM_MASS_POOL_VALID(stratum1.mass_pools[pool_i])                 \
        }                                                                     
}


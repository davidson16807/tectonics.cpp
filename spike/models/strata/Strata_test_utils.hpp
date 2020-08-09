#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "Strata.hpp"
#include <models/stratum/Stratum_test_utils.hpp>

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

    template<int L, int M, typename Tgenerator>
    Strata<L,M> get_random(Tgenerator generator)
    {
        Strata<L,M> output;
        output.count = 16;
        for (int i = 0; i < L; ++i)
        {
            output.content[i] = stratum::get_random<M>(generator);
        }
        return output;
    }
    
    #define STRATA_EQUAL(strata1, strata2)                                      \
        CHECK(strata1.count == strata2.count);                                    \
        for (int stratum_i = 0; stratum_i < strata1.count; ++stratum_i)   \
        {                                                           \
            Stratum<M> stratum1 = strata1.content[stratum_i];\
            Stratum<M> stratum2 = strata2.content[stratum_i];\
            STRATUM_EQUAL(stratum1, stratum2) \
        } 
    
    #define STRATA_VALID(strata1)                                       \
        CHECK(0 < strata1.count);\
        CHECK(strata1.count <= L);                       \
        for (int stratum_i = 0; stratum_i < strata1.count; ++stratum_i) \
        {                                                         \
            STRATUM_VALID(strata1.content[stratum_i]) \
        }                                                                     
}

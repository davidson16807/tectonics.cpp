#pragma once

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "Crust.hpp"
#include <models/strata/Strata_test_utils.hpp>

namespace crust
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

    /*
    Just get random values for every float, don't care about spatial distribution
    */
    template<int L, int M, typename Tgenerator>
    Crust<L,M> get_random_crust(const int size, Tgenerator generator)
    {
        Crust<L,M> output(size);
        strata::Strata<L,M> output_strata;
        for (std::size_t i = 0; i < output.size(); ++i)
        {
            output_strata = strata::get_random<L,M>(generator);
            output[i].pack(output_strata);
        }
        return output;
    }
    
    #define CRUST_EQUAL(crust1, crust2)                                      \
        CHECK(crust1.size() == crust2.size());                                    \
        for (std::size_t strata_i = 0; strata_i < crust1.size(); ++strata_i)   \
        {                                                                     \
            strata::Strata<L,M> strata1; crust1[strata_i].unpack(strata1);\
            strata::Strata<L,M> strata2; crust2[strata_i].unpack(strata2);\
            STRATA_EQUAL(strata1, strata2) \
        }
    
    #define CRUST_VALID(crust1)                                       \
        CHECK(crust1.size() != 0);\
        for (std::size_t strata_i = 0; strata_i < crust1.size(); ++strata_i) \
        {                                                         \
            strata::Strata<L,M> strata1; crust1[strata_i].unpack(strata1);\
            STRATA_VALID(strata1) \
        }
}

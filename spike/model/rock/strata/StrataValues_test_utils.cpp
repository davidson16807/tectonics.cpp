#pragma once

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "StrataValues.hpp"

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
    
    #define STRATA_VALUES_EQUAL(strata1, strata2)                \
        CHECK(strata1.count == strata2.count);                                    \
        for (std::size_t stratum_i = 0; stratum_i < strata1.count && stratum_i < strata2.count; ++stratum_i)  \
        {                                                                     \
            CHECK( strata1.values[stratum_i] == Approx(strata2.values[stratum_i]).margin(0.001) );    \
        }

    #define STRATA_VALUES_LAYER_INTERPOLATION_EQUAL(strata1, strata2)                \
        CHECK(strata1.count == strata2.count);                                    \
        for (std::size_t stratum_i = 0; stratum_i < strata1.count && stratum_i < strata2.count; ++stratum_i)  \
        {                                                                     \
            bool is_equal = (                               strata1.values[stratum_i]   - strata2.values[stratum_i] < 0.1) || \
                            (stratum_i > 0               && strata1.values[stratum_i-1] - strata2.values[stratum_i] < 0.1) || \
                            (stratum_i < strata1.count-1 && strata1.values[stratum_i+1] - strata2.values[stratum_i] < 0.1); \
            if (!is_equal) \
            {\
                CHECK( strata1.values[stratum_i] == Approx(strata2.values[stratum_i]).margin(0.001) );    \
                if(strata_i > 0){CHECK( strata1.values[strata_i-1] == Approx(strata2.values[stratum_i]).margin(0.001) );}    \
                if(strata_i < strata1.count-1){CHECK( strata1.values[strata_i+1] == Approx(strata2.values[stratum_i]).margin(0.001) );}    \
            } \
        }
}
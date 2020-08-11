#pragma once

#include <catch/catch.hpp>

#include <models/mineral/Mineral_test_utils.hpp>
#include "Stratum.hpp"

namespace stratum
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
    Stratum<M> get_random(Tgenerator generator)
    {
        Stratum<M> output(generator(), generator(), generator());
        for (int i = 0; i < M; ++i)
        {
            output.minerals[i] = mineral::get_random(generator);
        }
        return output;
    }
    
    #define STRATUM_EQUAL(stratum1, stratum2)                                                  \
        CHECK(stratum1.max_pressure_received == Approx(stratum2.max_pressure_received).epsilon(0.02) ); \
        CHECK(stratum1.max_temperature_received == Approx(stratum2.max_temperature_received).epsilon(0.02) ); \
        CHECK(stratum1.age_of_world_when_deposited == Approx(stratum2.age_of_world_when_deposited).epsilon(0.02) ); \
        for (int pool_i = 0; pool_i < M; ++pool_i)         \
        {                                                                        \
            MINERAL_EQUAL(stratum1.minerals[pool_i], stratum2.minerals[pool_i])  \
        } 
    
    #define STRATUM_VALID(stratum1)                                              \
        CHECK(stratum1.max_pressure_received > -1e-4); \
        CHECK(stratum1.max_temperature_received > -1e-4); \
        CHECK(stratum1.age_of_world_when_deposited > -1e-4); \
        for (int pool_i = 0; pool_i < M; ++pool_i)  \
        {                                                                 \
            MINERAL_VALID(stratum1.minerals[pool_i])                 \
        }                                                                     
}


#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <units/si.hpp>
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

    template<std::size_t M, typename Tgenerator>
    Stratum<M> get_random(Tgenerator generator)
    {
        std::uniform_real_distribution<float> uniform;
        Stratum<M> output(32768.0f * uniform(generator) * si::kelvin, 6e12f * uniform(generator) * si::pascal, uniform(generator) * 65000.0f * si::megayear);
        for (std::size_t i = 0; i < M; ++i)
        {
            output.minerals[i] = mineral::get_random(generator);
        }
        return output;
    }
    
    #define STRATUM_EQUAL(stratum1, stratum2)                                                  \
        CHECK(stratum1.max_pressure_received/si::pascal == Approx(stratum2.max_pressure_received/si::pascal).epsilon(0.02) ); \
        CHECK(stratum1.max_temperature_received/si::kelvin == Approx(stratum2.max_temperature_received/si::kelvin).epsilon(0.02) ); \
        CHECK(stratum1.age_of_world_when_deposited/si::second == Approx(stratum2.age_of_world_when_deposited/si::second).epsilon(0.02) ); \
        for (std::size_t pool_i = 0; pool_i < M; ++pool_i)         \
        {                                                                        \
            MINERAL_EQUAL(stratum1.minerals[pool_i], stratum2.minerals[pool_i])  \
        } 
    
    #define STRATUM_VALID(stratum1)                                              \
        CHECK(stratum1.max_pressure_received/si::pascal > -1e-4); \
        CHECK(stratum1.max_temperature_received/si::kelvin > -1e-4); \
        CHECK(stratum1.age_of_world_when_deposited/si::second > -1e-4); \
        for (std::size_t pool_i = 0; pool_i < M; ++pool_i)  \
        {                                                                 \
            MINERAL_VALID(stratum1.minerals[pool_i])                 \
        }                                                                     
}


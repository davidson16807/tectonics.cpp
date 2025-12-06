#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "State.hpp"

namespace orbit {

    /*
    NOTE: 
    This file does not exist within the concept of the mathematical category defined in README.md.
    This is because the file is only relevant within the context of several potential unit test files.
    We differ from the usual style of method signature here since performance does not matter in test code. 
    We also use macros since we require running unit tests on individual attributes for granularity, 
    and our testing framework itself uses macros to run the unit tests.
    Our chief concern here is to simplify writing unit tests and interpreting their output.
    */

    #define STATE_EQUAL(state1, state2)                                      \
        CHECK(state1.position.x == Approx(state2.position.x).epsilon(1e-4)); \
        CHECK(state1.position.y == Approx(state2.position.y).epsilon(1e-4)); \
        CHECK(state1.position.z == Approx(state2.position.z).epsilon(1e-4)); \
        CHECK(state1.velocity.x == Approx(state2.velocity.x).epsilon(1e-4)); \
        CHECK(state1.velocity.y == Approx(state2.velocity.y).epsilon(1e-4)); \
        CHECK(state1.velocity.z == Approx(state2.velocity.z).epsilon(1e-4));
        
}

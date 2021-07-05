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

    #define STATE_EQUAL(state1, state2)                             \
        CHECK(state1.position.x == Approx(state2.position.x).margin(0.0001)); \
        CHECK(state1.position.y == Approx(state2.position.y).margin(0.0001)); \
        CHECK(state1.position.z == Approx(state2.position.z).margin(0.0001)); \
        CHECK(state1.velocity.x == Approx(state2.velocity.x).margin(0.0001)); \
        CHECK(state1.velocity.y == Approx(state2.velocity.y).margin(0.0001)); \
        CHECK(state1.velocity.z == Approx(state2.velocity.z).margin(0.0001));
        
}

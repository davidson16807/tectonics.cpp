#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "Elements.hpp"

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

    #define ORBIT_EQUAL(orbit1, orbit2)                             \
        CHECK(orbit1.semi_major_axis == Approx(orbit2.semi_major_axis).margin(0.0001)); \
        CHECK(orbit1.eccentricity == Approx(orbit2.eccentricity).margin(0.0001)); \
        CHECK(orbit1.inclination == Approx(orbit2.inclination).margin(0.0001)); \
        CHECK(orbit1.longitude_of_ascending_node == Approx(orbit2.longitude_of_ascending_node).margin(0.0001)); \
        CHECK(orbit1.argument_of_periapsis == Approx(orbit2.argument_of_periapsis).margin(0.0001)); \
        CHECK(orbit1.mean_anomaly == Approx(orbit2.mean_anomaly).margin(0.0001));
        
}

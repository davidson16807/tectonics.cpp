#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/Shifting.hpp>
#include <math/analytic/Identity.hpp>

#include <test/macros.hpp>
#include <test/grouplike.hpp>

#include "test_tools.hpp"

TEST_CASE( "Shiftings are a commutative monoid under composition", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);

    std::vector<math::Shifting<double>> shiftings {
        math::Shifting<double>(2.0),
        math::Shifting<double>(-2.0),
        math::Shifting<double>(0.0)
    };

    std::vector<double> scalars { -2.0, 0.0, 2.0 };

    test::CommutativeMonoid commutative_monoid(
        "the identity function", math::Identity<double>(),
        "composition",           TEST_BINARY(math::compose)
    );

}


#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/Identity.hpp>
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Rational.hpp>

#include <test/macros.hpp>
#include <test/structures/grouplike.hpp>

#include "test_tools.hpp"

TEST_CASE( "Scalings are a commutative monoid under composition", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);

    std::vector<analytic::Scaling<double>> scalings {
        analytic::Scaling<double>(2.0),
        analytic::Scaling<double>(-2.0),
        analytic::Scaling<double>(0.0)
    };

    std::vector<double> scalars { -2.0, 0.0, 2.0 };

    test::CommutativeMonoid commutative_monoid(
        "the identity function", analytic::Identity<double>(),
        "composition",           TEST_BINARY(analytic::compose)
    );

    REQUIRE(commutative_monoid.valid(broad, scalings));
}


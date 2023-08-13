#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/Polynomial.hpp>

#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/structures/ringlike.hpp>

TEST_CASE( "Scalar addition is a commutative group", "[math]" ) {
    test::PrimitiveAdapter<double> adapter(1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::CommutativeGroup structure(
        "0", 0.0, 
        "scalar addition",       TEST_SYMBOL(+),
        "scalar subtraction",    TEST_SYMBOL(-)
    );
    REQUIRE(structure.valid(adapter, scalars));
}

TEST_CASE( "Scalar addition is a group", "[math]" ) {
    test::PrimitiveAdapter<double> adapter(1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::Group structure(
        "0", 0.0, 
        "scalar addition",       TEST_SYMBOL(+),
        "scalar subtraction",    TEST_SYMBOL(-)
    );
    REQUIRE(structure.valid(adapter, scalars));
}

TEST_CASE( "Scalar addition is a commutative monoid", "[math]" ) {
    test::PrimitiveAdapter<double> adapter(1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::CommutativeMonoid structure(
        "0", 0.0, 
        "scalar addition",       TEST_SYMBOL(+)
    );
    REQUIRE(structure.valid(adapter, scalars));
}

TEST_CASE( "Scalar addition is a monoid", "[math]" ) {
    test::PrimitiveAdapter<double> adapter(1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::Monoid structure(
        "0", 0.0, 
        "scalar addition",       TEST_SYMBOL(+)
    );
    REQUIRE(structure.valid(adapter, scalars));
}


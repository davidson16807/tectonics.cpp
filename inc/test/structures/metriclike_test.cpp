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
#include <test/structures/metriclike.hpp>

TEST_CASE( "The distance between scalars is a metric", "[math]" ) {
    test::PrimitiveAdapter<double> adapter (1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::Metric metric("scalar distance", [=](auto x, auto y) { return std::abs(x-y); } );
    REQUIRE(metric.valid(adapter, scalars));
}

TEST_CASE( "The distance between scalars is a semimetric", "[math]" ) {
    test::PrimitiveAdapter<double> adapter (1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::SemiMetric metric("scalar distance", [=](auto x, auto y) { return std::abs(x-y); } );
    REQUIRE(metric.valid(adapter, scalars));
}

TEST_CASE( "The distance between scalars is a metametric", "[math]" ) {
    test::PrimitiveAdapter<double> adapter (1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::MetaMetric metric("scalar distance", [=](auto x, auto y) { return std::abs(x-y); } );
    REQUIRE(metric.valid(adapter, scalars));
}

TEST_CASE( "The distance between scalars is a premetric", "[math]" ) {
    test::PrimitiveAdapter<double> adapter (1e-6);
    std::vector<double> scalars {-2.0, 0.0, 1.0, 2.0};
    test::PreMetric metric("scalar distance", [=](auto x, auto y) { return std::abs(x-y); } );
    REQUIRE(metric.valid(adapter, scalars));
}


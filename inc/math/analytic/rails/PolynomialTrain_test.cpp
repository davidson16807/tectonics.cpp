#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd-party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <test/macros.hpp>
#include <test/structures/ringlike.hpp>
#include <test/structures/metriclike.hpp>

#include "PolynomialTrain.hpp"

#include "../test_tools.hpp"

TEST_CASE( "PolynomialTrain arithmetic is a commutative ring", "[math]" ) {
    const double threshold = 3e-3;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3e2;
    const double midhi =  3e2;

    ExpressionAdapter<double> broad(threshold, midlo, midhi);
    ExpressionAdapter<double> narrow(threshold, lo, hi);

    const double oo = std::numeric_limits<double>::max();

    using P0 = analytic::Polynomial<double,0,2>;
    using P1 = analytic::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});

    // standard polynomials, no overlap
    analytic::Train<double,P0> t0 = analytic::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    analytic::Train<double,P0> t1 = analytic::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    analytic::Train<double,P1> t2 = analytic::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    analytic::Train<double,P1> t3 = analytic::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    analytic::Polynomial<double,0,0> m0{2.0};
    analytic::Polynomial<double,2,2> m1{2.0};
    analytic::Polynomial<double,-2,-2> m2{2.0};

    analytic::Shifting<double> f0(2.0);
    analytic::Shifting<double> f1(-2.0);
    analytic::Shifting<double> f2(0.0);

    analytic::Scaling<double> g0(2.0);
    analytic::Scaling<double> g1(-2.0);
    analytic::Scaling<double> g2(0.0);

    auto P0s = std::vector{t0,t1};
    auto P1s = std::vector{t2,t3};
    auto ks = std::vector{0.0, 2.0, -2.0};
    auto m0s = std::vector{m0};
    auto m1s = std::vector{m1};
    auto m2s = std::vector{m2};
    auto fs = std::vector{f0,f1,f2};
    auto gs = std::vector{g0,g1,g2};

    analytic::Polynomial<double,0,0> zero = analytic::Polynomial<double,0,0>({0.0});
    analytic::Polynomial<double,0,0> one  = analytic::Polynomial<double,0,0>({1.0});

    test::CommutativeRing commutative_ring(
        "0", zero, 
        "1", one, 
        "polynomial addition",       TEST_SYMBOL(+),
        "polynomial subtraction",    TEST_SYMBOL(-),
        "polynomial multiplication", TEST_SYMBOL(*)
    );

    // UNARY TESTS
    REQUIRE(commutative_ring.valid(broad, P0s));
    REQUIRE(commutative_ring.valid(broad, P1s));

    // BINARY TESTS
    REQUIRE(commutative_ring.valid(broad, P0s, P1s));

    REQUIRE(commutative_ring.valid(broad, P0s, ks));
    REQUIRE(commutative_ring.valid(broad, P1s, ks));

    REQUIRE(commutative_ring.valid(broad, P0s, m0s));
    REQUIRE(commutative_ring.valid(broad, P0s, m1s));
    REQUIRE(commutative_ring.valid(broad, P0s, m2s));
    REQUIRE(commutative_ring.valid(broad, P1s, m0s));
    REQUIRE(commutative_ring.valid(broad, P1s, m1s));
    REQUIRE(commutative_ring.valid(broad, P1s, m2s));

    REQUIRE(commutative_ring.valid(broad, P0s, fs));
    REQUIRE(commutative_ring.valid(broad, P1s, fs));

    REQUIRE(commutative_ring.valid(broad, P0s, gs));
    REQUIRE(commutative_ring.valid(broad, P1s, gs));

    // TRINARY TESTS
    // REQUIRE(commutative_ring.valid(broad, P0s, P1s, ks));
    // REQUIRE(commutative_ring.valid(broad, P0s, P1s, m0s));
    // REQUIRE(commutative_ring.valid(broad, P0s, P1s, m1s));
    // REQUIRE(commutative_ring.valid(broad, P0s, P1s, m2s));
    // REQUIRE(commutative_ring.valid(broad, P0s, ks, m0s));
    // REQUIRE(commutative_ring.valid(broad, P0s, ks, m1s));
    // REQUIRE(commutative_ring.valid(broad, P0s, ks, m2s));
    // REQUIRE(commutative_ring.valid(broad, P0s, ks, m2s));
    // REQUIRE(commutative_ring.valid(broad, P1s, ks, m0s));
    // REQUIRE(commutative_ring.valid(broad, P1s, ks, m1s));
    // REQUIRE(commutative_ring.valid(broad, P1s, ks, m2s));
    // REQUIRE(commutative_ring.valid(broad, P1s, ks, m2s));

}


TEST_CASE( "PolynomialTrain integral/derivative invertibility", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -1e2;
    // const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = analytic::Polynomial<double,0,2>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});

    // standard polynomials, no overlap
    analytic::Train<double,P0> t0 = analytic::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    analytic::Train<double,P0> t1 = analytic::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(analytic::distance(t0, derivative(integral(t0)), lo, hi) < threshold);
        CHECK(analytic::distance(t1, derivative(integral(t1)), lo, hi) < threshold);
        // CHECK(analytic::distance(t2, derivative(integral(t2)), lo, hi) < threshold);
        // CHECK(analytic::distance(t3, derivative(integral(t3)), lo, hi) < threshold);
    }
}


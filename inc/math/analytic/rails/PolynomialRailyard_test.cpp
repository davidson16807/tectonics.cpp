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

#include "PolynomialRailyard.hpp"

#include "../test_tools.hpp"

TEST_CASE( "PolynomialRailyard arithmetic is a commutative ring", "[math]" ) {

    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e1;
    const double midhi =  1e1;

    ExpressionAdapter<double> broad(threshold, midlo, midhi);
    ExpressionAdapter<double> narrow(threshold, lo, hi);

    // const double oo = std::numeric_limits<double>::max();

    using P0 = analytic::Polynomial<double,0,2>;
    using P1 = analytic::Polynomial<double,-2,1>;
    using P2 = analytic::Polynomial<double,-2,2>;

    P0 p0 = P0({3.0f,2.0f,1.0f});
    P0 p1 = P0({-1.0f,0.0f,1.0f});
    P1 p2 = P1({4.0f,3.0f,2.0f,1.0f});
    P1 p3 = P1({-1.0f,1.0f,-2.0f,2.0f});

    using C0 = analytic::Railcar<double,P0>;
    using C1 = analytic::Railcar<double,P1>;
    using C2 = analytic::Railcar<double,P2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        C0(lo, -1.0f, p0), 
        C0(1.0f, hi,  p1) 
    }; 

    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        C0(lo, midhi, p0), 
        C0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        C1(midlo, -1.0f, p2), 
        C1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        C2(lo, midlo, P2(p2)), 
        C2(midhi, hi, P2(p3)), 
        C2(midlo, -1.0f, P2(p2)), 
        C2(1.0f, midhi, P2(p3)) 
    };

    analytic::Polynomial<double,0,0> m0{2.0};
    analytic::Polynomial<double,2,2> m1{2.0};
    analytic::Polynomial<double,-2,-2> m2{2.0};

    analytic::Shifting<double> f0(2.0);
    analytic::Shifting<double> f1(-2.0);
    analytic::Shifting<double> f2(0.0);

    analytic::Scaling<double> g0(2.0);
    analytic::Scaling<double> g1(-2.0);
    analytic::Scaling<double> g2(0.0);

    auto S0s = std::vector{s0,s1};
    auto S1s = std::vector{s2};
    auto S2s = std::vector{s3};

    auto P0s = std::vector{p0,p1};
    auto P1s = std::vector{p2,p3};

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
    REQUIRE(commutative_ring.valid(broad, S0s));
    REQUIRE(commutative_ring.valid(broad, S1s));
    REQUIRE(commutative_ring.valid(broad, S2s));

    // BINARY TESTS
    REQUIRE(commutative_ring.valid(broad, S0s,S1s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s));

    REQUIRE(commutative_ring.valid(broad, S0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s));

    REQUIRE(commutative_ring.valid(broad, S0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s));

    REQUIRE(commutative_ring.valid(broad, S0s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,ks));

    // TRINARY TESTS, 3 railyards
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,S2s));

    // TRINARY TESTS, 2 railyards
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,S1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,S1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,P1s));

    REQUIRE(commutative_ring.valid(broad, S0s,S1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,m2s));

    REQUIRE(commutative_ring.valid(broad, S0s,S1s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,S1s,ks));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,S2s,ks));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,S2s,ks));

    // TRINARY TESTS
    // TRINARY TESTS, 1 railyard
    // REQUIRE(commutative_ring.valid(broad, S0s,P0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,P0s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S0s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S0s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,m0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,P1s));
    // REQUIRE(commutative_ring.valid(broad, S0s,m0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,m0s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,m1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,m0s));
    // REQUIRE(commutative_ring.valid(broad, S0s,m1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,m1s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,m2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,m1s));
    // REQUIRE(commutative_ring.valid(broad, S0s,m2s,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,m2s,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,fs,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,m2s));
    // REQUIRE(commutative_ring.valid(broad, S0s,fs,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,fs,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,gs,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,fs));
    // REQUIRE(commutative_ring.valid(broad, S0s,gs,gs));
    REQUIRE(commutative_ring.valid(broad, S0s,gs,ks));

    REQUIRE(commutative_ring.valid(broad, S0s,ks,P0s));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,P1s));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,m0s));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,m1s));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,m2s));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,fs));
    REQUIRE(commutative_ring.valid(broad, S0s,ks,gs));
    // REQUIRE(commutative_ring.valid(broad, S0s,ks,ks));


    // REQUIRE(commutative_ring.valid(broad, S1s,P0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,P0s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S1s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S1s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,m0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,P1s));
    // REQUIRE(commutative_ring.valid(broad, S1s,m0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,m0s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,m1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,m0s));
    // REQUIRE(commutative_ring.valid(broad, S1s,m1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,m1s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,m2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,m1s));
    // REQUIRE(commutative_ring.valid(broad, S1s,m2s,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,m2s,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,fs,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,m2s));
    // REQUIRE(commutative_ring.valid(broad, S1s,fs,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,fs,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,gs,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,fs));
    // REQUIRE(commutative_ring.valid(broad, S1s,gs,gs));
    REQUIRE(commutative_ring.valid(broad, S1s,gs,ks));

    REQUIRE(commutative_ring.valid(broad, S1s,ks,P0s));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,P1s));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,m0s));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,m1s));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,m2s));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,fs));
    REQUIRE(commutative_ring.valid(broad, S1s,ks,gs));
    // REQUIRE(commutative_ring.valid(broad, S1s,ks,ks));


    // REQUIRE(commutative_ring.valid(broad, S2s,P0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,P0s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S2s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,P1s,P0s));
    // REQUIRE(commutative_ring.valid(broad, S2s,P1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,P1s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,m0s,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,P1s));
    // REQUIRE(commutative_ring.valid(broad, S2s,m0s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,m0s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,m1s,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,m0s));
    // REQUIRE(commutative_ring.valid(broad, S2s,m1s,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,m1s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,m2s,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,m1s));
    // REQUIRE(commutative_ring.valid(broad, S2s,m2s,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,m2s,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,fs,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,m2s));
    // REQUIRE(commutative_ring.valid(broad, S2s,fs,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,fs,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,gs,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,fs));
    // REQUIRE(commutative_ring.valid(broad, S2s,gs,gs));
    REQUIRE(commutative_ring.valid(broad, S2s,gs,ks));

    REQUIRE(commutative_ring.valid(broad, S2s,ks,P0s));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,P1s));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,m0s));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,m1s));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,m2s));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,fs));
    REQUIRE(commutative_ring.valid(broad, S2s,ks,gs));
    // REQUIRE(commutative_ring.valid(broad, S2s,ks,ks));

}





TEST_CASE( "PolynomialRailyard integral/derivative invertibility", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using P0 = analytic::Polynomial<double,0,2>;

    P0 p0 = P0({3.0f,2.0f,1.0f});
    P0 p1 = P0({-1.0f,0.0f,1.0f});

    using C0 = analytic::Railcar<double,P0>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        C0(lo, -1.0f, p0), 
        C0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        C0(lo, midhi, p0), 
        C0(midlo, hi, p1) 
    }; 
    SECTION("the derivative of a function's3 integral must equal the original function"){
        CHECK(analytic::distance(s0, derivative(integral(s0)), lo, hi) < threshold);
        CHECK(analytic::distance(s1, derivative(integral(s1)), lo, hi) < threshold);
        // CHECK(analytic::distance(s2, derivative(integral(s2)), lo, hi) < threshold);
        // CHECK(analytic::distance(s3, derivative(integral(s3)), lo, hi) < threshold);
    }
}


/*
TEST_CASE( "PolynomialRailyard composition commutativity", "[math]" ) {
    // `threshold` is the tolerance towards error in the result
    const double threshold = 1e-4;
    // `max` and `min` are the largest absolute value to be tested 
    const double min = 1e-1;
    const double max =  1e4;
    // `hi` and `lo` variables indicate thresholds that are reused when constructing railyards 
    const double lo = -1e3;
    const double hi =  1e3;
    const double midlo = -1e1;
    const double midhi =  1e1;
    // `multiple` is the multiple by which tests are incremented
    const double multiple = -std::pow(10.0, 0.5);

    using P0 = analytic::Polynomial<double,0,2>;
    using P1 = analytic::Polynomial<double,0,3>;
    // using P2 = analytic::Polynomial<double,0,3>;

    P0 p0 = P0({3.0f,2.0f,1.0f});
    P0 p1 = P0({-1.0f,0.0f,1.0f});
    P1 p2 = P1({4.0f,3.0f,2.0f,1.0f});
    P1 p3 = P1({-1.0f,1.0f,-2.0f,2.0f});

    using C0 = analytic::Railcar<double,P0>;
    using C1 = analytic::Railcar<double,P1>;
    // using C2 = analytic::Railcar<double,P2>;

    analytic::Railyard<double,P0> s0 = analytic::Railyard<double,P0>{
        C0(lo, -1.0f, p0), 
        C0(1.0f, hi,  p1) 
    }; 

    analytic::Railyard<double,P0> s1 = analytic::Railyard<double,P0>{
        C0(lo, midhi, p0), 
        C0(midlo, hi, p1) 
    }; 
    analytic::Railyard<double,P1> s2 = analytic::Railyard<double,P1>{ 
        C1(midlo, -1.0f, p2), 
        C1(1.0f, midhi, p3) 
    }; 
    analytic::Railyard<double,P1> s3 = analytic::Railyard<double,P1>{ 
        C1(lo, midlo, P1(p2)), 
        C1(midhi, hi, P1(p3)), 
        C1(midlo, -1.0f, P1(p2)), 
        C1(1.0f, midhi, P1(p3)) 
    };

    SECTION("si(x)+sj(x) must be equivalent to (si+sj)(x)"){
        auto s0s1 = compose(s0, s1);
        std::cout << s0s1 << std::endl;
        for (int x = min; std::abs(x) < max; x*=multiple)
        {
            std::cout << x << std::endl;
            // CHECK(Approx(compose(s0, s0)(x)).epsilon(threshold) == s0(s0(x)));
            CHECK(Approx(s0s1(x)).epsilon(threshold) == s0(s1(x)));
            // CHECK(Approx(compose(s0, s2)(x)).epsilon(threshold) == s0(s2(x)));
            // CHECK(Approx(compose(s0, s3)(x)).epsilon(threshold) == s0(s3(x)));

            // CHECK(Approx(compose(s1, s0)(x)).epsilon(threshold) == s1(s0(x)));
            // CHECK(Approx(compose(s1, s1)(x)).epsilon(threshold) == s1(s1(x)));
            // CHECK(Approx(compose(s1, s2)(x)).epsilon(threshold) == s1(s2(x)));
            // CHECK(Approx(compose(s1, s3)(x)).epsilon(threshold) == s1(s3(x)));

            // CHECK(Approx(compose(s2, s0)(x)).epsilon(threshold) == s2(s0(x)));
            // CHECK(Approx(compose(s2, s1)(x)).epsilon(threshold) == s2(s1(x)));
            // CHECK(Approx(compose(s2, s2)(x)).epsilon(threshold) == s2(s2(x)));
            // CHECK(Approx(compose(s2, s3)(x)).epsilon(threshold) == s2(s3(x)));

            // CHECK(Approx(compose(s3, s0)(x)).epsilon(threshold) == s3(s0(x)));
            // CHECK(Approx(compose(s3, s1)(x)).epsilon(threshold) == s3(s1(x)));
            // CHECK(Approx(compose(s3, s2)(x)).epsilon(threshold) == s3(s2(x)));
            // CHECK(Approx(compose(s3, s3)(x)).epsilon(threshold) == s3(s3(x)));
        }
    }
}
*/

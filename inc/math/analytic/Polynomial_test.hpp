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
#include <test/structures.hpp>

#include "test_tools.hpp"

TEST_CASE( "Polynomials are a commutative ring", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);
    ExpressionAdapter<double> narrow(1e-6, -1e2, 1e2);

    std::vector<math::Polynomial<double,0,4>> polynomials1 {
        math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0}),
        math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0})
    };

    std::vector<math::Polynomial<double,-2,2>> polynomials2 {
        math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0}),
        math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0})
    };

    std::vector<math::Polynomial<double,0,0>> monomials1 {
        math::Polynomial<double,0,0> (std::array<double,1>{2.0}),
    };

    std::vector<math::Polynomial<double,2,2>> monomials2 {
        math::Polynomial<double,2,2> (std::array<double,1>{2.0}),
    };

    std::vector<math::Polynomial<double,-2,2>> monomials3 {
        math::Polynomial<double,-2,-2> (std::array<double,1>{2.0})
    };

    std::vector<math::Shifting<double>> shiftings {
        math::Shifting<double>(2.0),
        math::Shifting<double>(-2.0),
        math::Shifting<double>(0.0)
    };

    std::vector<math::Scaling<double>> scalings {
        math::Scaling<double>(2.0),
        math::Scaling<double>(-2.0),
        math::Scaling<double>(0.0)
    };

    std::vector<double> scalars { -2.0, 0.0, 2.0 };

    math::Polynomial<double,0,0> zero = math::Polynomial<double,0,0>({0.0f});
    math::Polynomial<double,0,0> one  = math::Polynomial<double,0,0>({1.0f});

    test::CommutativeRing commutative_ring(
        "0", zero, 
        "1", one, 
        "polynomial addition",       TEST_SYMBOL(+),
        "polynomial subtraction",    TEST_SYMBOL(-),
        "polynomial multiplication", TEST_SYMBOL(*)
    );

    // UNARY TESTS
    REQUIRE(commutative_ring.valid(broad, polynomials1));
    REQUIRE(commutative_ring.valid(broad, polynomials2));
    REQUIRE(commutative_ring.valid(broad, monomials1));
    REQUIRE(commutative_ring.valid(broad, monomials2));
    REQUIRE(commutative_ring.valid(broad, monomials3));
    REQUIRE(commutative_ring.valid(broad, shiftings));
    REQUIRE(commutative_ring.valid(broad, scalings));

    // BINARY TESTS
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials3  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, shiftings   ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, scalings    ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, scalars     ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, monomials1  ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, monomials2  ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, monomials3  ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, shiftings   ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, scalings    ));
    REQUIRE(commutative_ring.valid(broad, polynomials2, scalars     ));

    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials1));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials2));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, scalars));

}



TEST_CASE( "Polynomial integral/derivative invertibility", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    
    math::Polynomial<double,0,4> p = math::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    math::Polynomial<double,0,4> q = math::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    // math::Polynomial<double,-2,2> r = math::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    // math::Polynomial<double,-2,2> s = math::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(math::distance(p, derivative(integral(p)), lo, hi) < threshold);
        CHECK(math::distance(q, derivative(integral(q)), lo, hi) < threshold);
        // CHECK(math::distance(r, derivative(integral(r)), lo, hi) < threshold);
        // CHECK(math::distance(s, derivative(integral(s)), lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial composition happy path", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to s square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;

    const double p =-1.0;
    const double q = 2.0;
    const double r = 3.0;
    const double s = 1.78;
    const double t = 3.16;

    using P0 = math::Polynomial<double,0,2>;
    // using P1 = math::Polynomial<double,-2,1>;
    // using P2 = math::Polynomial<double,-2,2>;
    using P3 = math::Polynomial<double,0,1>;

    math::Identity<double> E;
    math::Scaling<double>  F(s);
    math::Shifting<double> G(s);
    P0 P  = P0(std::array<double,3>{p,q,r});
    P3 Q  = P3(std::array<double,2>{s,t});
    /*
    example:
    P(x)   = p+qx+rx²
    f(x)   = sx
    P∘f(x) = p+q(sx)+r(sx)²
    P∘f(x) = p+qsx+rs²x²
    */
    P0 PF = P0(std::array<double,3>{p, q*s, r*s*s});
    /*
    example:
    P(x)   = p+qx+rx²
    g(x)   = s+x
    P∘g(x) = p+q(s+x)+r(s+x)²
    P∘g(x) = p+q(s+x)+r(s²+2sx+x²)
    P∘g(x) = p+qs+qx+rs²+r2sx+rx²
    P∘g(x) = p+rs²+qs + qx+r2sx + rx²
    */
    P0 PG = P0(std::array<double,3>{p+r*s*s+q*s, q+r*2.0*s, r});
    /*
    P(x)   = p+qx+rx²
    Q(x)   = s+tx
    P∘Q(x) = p+q(s+tx)+r(s+tx)²
    P∘Q(x) = p+qs+qtx+r(s²+2stx+t²x²)
    P∘Q(x) = p+qs+qtx+rs²+r2stx+rt²x²
    P∘Q(x) = p+qs+rs² + qtx+r2stx + rt²x²
    */
    math::Polynomial<double,0,2> PQ = math::Polynomial<double,0,2>(std::array<double,3>{p+q*s+r*s*s, q*t+r*2.0*s*t, r*t*t});

    SECTION("the derivative of s function's integral must equal the original function"){
        CHECK(math::distance(math::compose(P,E), P, lo, hi) < threshold);
        CHECK(math::distance(math::compose(P,F), PF, lo, hi) < threshold);
        CHECK(math::distance(math::compose(P,G), PG, lo, hi) < threshold);
        CHECK(math::distance(math::compose(P,Q), PQ, lo, hi) < threshold);
    }
}

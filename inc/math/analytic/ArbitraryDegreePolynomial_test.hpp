#pragma once

// std libraries
#include <cmath>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/ArbitraryDegreePolynomial.hpp>  

#include <test/macros.hpp>
#include <test/grouplike.hpp>

#include "test_tools.hpp"

TEST_CASE( "ArbitraryDegreePolynomials are a commutative ring", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);
    ExpressionAdapter<double> narrow(1e-6, -1e2, 1e2);

    std::vector<math::ArbitraryDegreePolynomial<double,std::int8_t>> polynomials1 {
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{0,1.0},{1,2.0},{2,3.0},{3,4.0},{4,5.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{0,-1.0},{1,0.0},{2,1.0},{3,2.0},{4,3.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{-2,1.0},{-1,2.0},{0,3.0},{1,4.0},{2,5.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{-2,-1.0},{-1,1.0},{0,-2.0},{1,2.0},{2,3.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{0,2.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{2,2.0}}),
        math::ArbitraryDegreePolynomial<double,std::int8_t>({{-2,2.0}})
    };

    std::vector<math::Polynomial<double,0,4>> polynomials2 {
        math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0}),
        math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0})
    };

    std::vector<math::Polynomial<double,-2,2>> polynomials3 {
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

    test::CommutativeRing commutative_ring(
        "0", 0.0, 
        "1", 1.0, 
        "polynomial addition",       TEST_SYMBOL(+),
        "polynomial subtraction",    TEST_SYMBOL(-),
        "polynomial multiplication", TEST_SYMBOL(*)
    );

    REQUIRE(commutative_ring.valid(broad, polynomials1));

    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials3  ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, shiftings   ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, scalings    ));
    REQUIRE(commutative_ring.valid(broad, polynomials1, scalars     ));

    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, polynomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials1));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials2));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, monomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials2, scalars));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, monomials1));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, monomials2));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, monomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, polynomials3, scalars));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1,   monomials2));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1,   monomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1,   shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1,   scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials1,   scalars));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2,   monomials3));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2,   shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2,   scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials2,   scalars));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials3,   shiftings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials3,   scalings));
    REQUIRE(commutative_ring.valid(broad, polynomials1, monomials3,   scalars));
    REQUIRE(commutative_ring.valid(broad, polynomials1, scalings,     scalars));

}





TEST_CASE( "ArbitraryDegreePolynomial integral/derivative invertibility", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    
    math::ArbitraryDegreePolynomial<double,std::int8_t> p({{0,1.0},{1,2.0},{2,3.0},{3,4.0},{4,5.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> q({{0,-1.0},{1,0.0},{2,1.0},{3,2.0},{4,3.0}});
    // math::ArbitraryDegreePolynomial<double,std::int8_t> r({{-2,1.0},{-1,2.0},{0,3.0},{1,4.0},{2,5.0}});
    // math::ArbitraryDegreePolynomial<double,std::int8_t> s({{-2,-1.0},{-1,1.0},{0,-2.0},{1,2.0},{2,3.0}});

    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(math::distance(p, derivative(integral(p)), lo, hi) < threshold);
        CHECK(math::distance(q, derivative(integral(q)), lo, hi) < threshold);
        // CHECK(math::distance(r, derivative(integral(r)), lo, hi) < threshold);
        // CHECK(math::distance(s, derivative(integral(s)), lo, hi) < threshold);
    }
}


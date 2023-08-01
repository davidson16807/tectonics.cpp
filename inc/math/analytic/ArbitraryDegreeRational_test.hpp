#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide r main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/ArbitraryDegreeRational.hpp>  

#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/structures.hpp>

#include "test_tools.hpp"

TEST_CASE( "ArbitraryDegreeRationals are a field", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);
    ExpressionAdapter<double> narrow(1e-6, -1e2, 1e2);

    math::ArbitraryDegreePolynomial<double,std::int8_t> p0({{0,1.0},{1,2.0},{2,3.0},{3,4.0},{4,5.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> p1({{-2,1.0},{-1,2.0},{0,3.0},{1,4.0},{2,5.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> q0({{0,-1.0},{1,0.0},{2,1.0},{3,2.0},{4,3.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> q1({{-2,-1.0},{-1,1.0},{0,-2.0},{1,2.0},{2,3.0}});

    math::ArbitraryDegreePolynomial<double,std::int8_t> m0({{0,2.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> m1({{2,2.0}});
    math::ArbitraryDegreePolynomial<double,std::int8_t> m2({{-2,2.0}});

    std::vector<math::ArbitraryDegreeRational<double,std::int8_t>> rationals1 { p0/q0, p1/q0, p0/q1, p1/q1 };

    math::Polynomial<double,0,4> r0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> r1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> s0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> s1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    std::vector<math::Rational<double,0,4,0,4>>   rationals2 { r0/s0 };
    std::vector<math::Rational<double,-2,2,0,4>>  rationals3 { r1/s0 };
    std::vector<math::Rational<double,0,4,-2,2>>  rationals4 { r0/s1 };
    std::vector<math::Rational<double,-2,2,-2,2>> rationals5 { r1/s1 };

    std::vector<math::ArbitraryDegreePolynomial<double,std::int8_t>> polynomials1 { p0, q0, q1, q1, m0, m1, m2 };

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
        // math::Scaling<double>(0.0)  // NOTE: we exclude zero since division by zero is not defined
    };

    std::vector<double> scalars { -2.0, 2.0 }; // NOTE: we exclude zero since division by zero is not defined

    // math::Polynomial<double,0,0> zero = math::Polynomial<double,0,0>({0.0f});
    // math::Polynomial<double,0,0> one  = math::Polynomial<double,0,0>({1.0f});

    test::Field field(
        "0", 0.0, 
        "1", 1.0, 
        "rational addition",       TEST_SYMBOL(+),
        "rational subtraction",    TEST_SYMBOL(-),
        "rational multiplication", TEST_SYMBOL(*),
        "rational division",       TEST_SYMBOL(/)
    );

    // UNARY TESTS
    REQUIRE(field.valid(broad, rationals1));

    // // BINARY TESTS
    REQUIRE(field.valid(broad, rationals1, rationals2  ));
    REQUIRE(field.valid(broad, rationals1, rationals3  ));
    REQUIRE(field.valid(broad, rationals1, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, rationals5  ));
    REQUIRE(field.valid(broad, rationals1, polynomials1));
    REQUIRE(field.valid(broad, rationals1, polynomials2));
    REQUIRE(field.valid(broad, rationals1, polynomials3));
    REQUIRE(field.valid(broad, rationals1, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, scalars     ));
    REQUIRE(field.valid(broad, rationals1, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, scalings    ));

    // // TRINARY TESTS
    REQUIRE(field.valid(broad, rationals1, rationals2, rationals3  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, rationals5  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals2, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals2, polynomials3));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals2, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals2, scalings    ));

    REQUIRE(field.valid(broad, rationals1, rationals3, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, rationals5  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals3, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals3, polynomials3));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals3, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals3, scalings    ));

    REQUIRE(field.valid(broad, rationals1, rationals4, rationals5  ));
    REQUIRE(field.valid(broad, rationals1, rationals4, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals4, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals4, polynomials3));
    REQUIRE(field.valid(broad, rationals1, rationals4, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals4, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals4, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals4, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals4, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals4, scalings    ));

    REQUIRE(field.valid(broad, rationals1, rationals5, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals5, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals5, polynomials3));
    REQUIRE(field.valid(broad, rationals1, rationals5, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals5, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals5, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals5, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals5, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals5, scalings    ));

    REQUIRE(field.valid(broad, rationals1, polynomials1, polynomials2));
    REQUIRE(field.valid(broad, rationals1, polynomials1, polynomials3));
    REQUIRE(field.valid(broad, rationals1, polynomials1, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, polynomials1, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, polynomials1, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, polynomials1, scalars     ));
    REQUIRE(field.valid(broad, rationals1, polynomials1, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, polynomials1, scalings    ));

    REQUIRE(field.valid(broad, rationals1, polynomials2, polynomials3));
    REQUIRE(field.valid(broad, rationals1, polynomials2, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, polynomials2, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, polynomials2, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, polynomials2, scalars     ));
    REQUIRE(field.valid(broad, rationals1, polynomials2, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, polynomials2, scalings    ));

    REQUIRE(field.valid(broad, rationals1, polynomials3, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, polynomials3, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, polynomials3, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, polynomials3, scalars     ));
    REQUIRE(field.valid(broad, rationals1, polynomials3, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, polynomials3, scalings    ));

    REQUIRE(field.valid(broad, rationals1, monomials1,   monomials2  ));
    REQUIRE(field.valid(broad, rationals1, monomials1,   monomials3  ));
    REQUIRE(field.valid(broad, rationals1, monomials1,   scalars     ));
    REQUIRE(field.valid(broad, rationals1, monomials1,   shiftings   ));
    REQUIRE(field.valid(broad, rationals1, monomials1,   scalings    ));

    REQUIRE(field.valid(broad, rationals1, monomials2,   monomials3  ));
    REQUIRE(field.valid(broad, rationals1, monomials2,   scalars     ));
    REQUIRE(field.valid(broad, rationals1, monomials2,   shiftings   ));
    REQUIRE(field.valid(broad, rationals1, monomials2,   scalings    ));

    REQUIRE(field.valid(broad, rationals1, monomials3,   scalars     ));
    REQUIRE(field.valid(broad, rationals1, monomials3,   shiftings   ));
    REQUIRE(field.valid(broad, rationals1, monomials3,   scalings    ));

    REQUIRE(field.valid(broad, rationals1, scalars,      shiftings   ));
    REQUIRE(field.valid(broad, rationals1, scalars,      scalings    ));

    REQUIRE(field.valid(broad, rationals1, scalings,     shiftings   ));

}







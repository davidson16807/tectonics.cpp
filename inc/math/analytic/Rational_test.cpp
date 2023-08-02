#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide r main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/analytic/Rational.hpp>  

#include <test/macros.hpp>
#include <test/structures.hpp>

#include "test_tools.hpp"

TEST_CASE( "The distance between rationals is a semimetric", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);
    ExpressionAdapter<double> narrow(1e-5, -1e2, 1e2);

    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    std::vector<math::Polynomial<double,0,4>> polynomials1  { p0,q0 };
    std::vector<math::Polynomial<double,-2,2>> polynomials2 { p1,q1 };

    std::vector<math::Rational<double,0,4,0,4>>   rationals1 { p0/q0 };
    std::vector<math::Rational<double,-2,2,0,4>>  rationals2 { p1/q0 };
    std::vector<math::Rational<double,0,4,-2,2>>  rationals3 { p0/q1 };
    std::vector<math::Rational<double,-2,2,-2,2>> rationals4 { p1/q1 };

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

    std::vector<double> scalars { -2.0, 0.0, 2.0 }; // NOTE: we exclude zero since division by zero is not defined

    test::SemiMetric metric("polynomial distance", [=](auto x, auto y) { return math::distance(x,y,-1e3, 1e3); } );

    // UNARY TESTS
    REQUIRE(metric.valid(narrow, rationals1));
    REQUIRE(metric.valid(narrow, rationals2));
    REQUIRE(metric.valid(narrow, rationals3));
    REQUIRE(metric.valid(narrow, rationals4));

    // BINARY TESTS
    REQUIRE(metric.valid(broad, rationals1, rationals2  ));
    REQUIRE(metric.valid(broad, rationals1, rationals3  ));
    REQUIRE(metric.valid(broad, rationals1, rationals4  ));
    REQUIRE(metric.valid(broad, rationals1, polynomials1));
    REQUIRE(metric.valid(broad, rationals1, polynomials2));
    REQUIRE(metric.valid(broad, rationals1, monomials1  ));
    REQUIRE(metric.valid(broad, rationals1, monomials2  ));
    REQUIRE(metric.valid(broad, rationals1, monomials3  ));
    REQUIRE(metric.valid(broad, rationals1, scalars     ));

    REQUIRE(metric.valid(broad, rationals2, rationals3  ));
    REQUIRE(metric.valid(broad, rationals2, rationals4  ));
    REQUIRE(metric.valid(broad, rationals2, polynomials1));
    REQUIRE(metric.valid(broad, rationals2, polynomials2));
    REQUIRE(metric.valid(broad, rationals2, monomials1  ));
    REQUIRE(metric.valid(broad, rationals2, monomials2  ));
    REQUIRE(metric.valid(broad, rationals2, monomials3  ));
    REQUIRE(metric.valid(broad, rationals2, scalars     ));
    // REQUIRE(metric.valid(broad, rationals2, shiftings   ));
    // REQUIRE(metric.valid(broad, rationals2, scalings    ));

    REQUIRE(metric.valid(broad, rationals3, rationals4  ));
    REQUIRE(metric.valid(broad, rationals3, polynomials1));
    REQUIRE(metric.valid(broad, rationals3, polynomials2));
    REQUIRE(metric.valid(broad, rationals3, monomials1  ));
    REQUIRE(metric.valid(broad, rationals3, monomials2  ));
    REQUIRE(metric.valid(broad, rationals3, monomials3  ));
    REQUIRE(metric.valid(broad, rationals3, scalars     ));
    // REQUIRE(metric.valid(broad, rationals3, shiftings   ));
    // REQUIRE(metric.valid(broad, rationals3, scalings    ));

    REQUIRE(metric.valid(broad, rationals4, rationals4  ));
    REQUIRE(metric.valid(broad, rationals4, polynomials1));
    REQUIRE(metric.valid(broad, rationals4, polynomials2));
    REQUIRE(metric.valid(broad, rationals4, monomials1  ));
    REQUIRE(metric.valid(broad, rationals4, monomials2  ));
    REQUIRE(metric.valid(broad, rationals4, monomials3  ));
    REQUIRE(metric.valid(broad, rationals4, scalars     ));
    // REQUIRE(metric.valid(broad, rationals4, shiftings   ));
    // REQUIRE(metric.valid(broad, rationals4, scalings    ));

}

TEST_CASE( "Rationals are a field", "[math]" ) {

    ExpressionAdapter<double> broad (1e-6, -1e3, 1e3);
    ExpressionAdapter<double> narrow(1e-6, -1e2, 1e2);

    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    std::vector<math::Polynomial<double,0,4>> polynomials1  { p0,q0 };
    std::vector<math::Polynomial<double,-2,2>> polynomials2 { p1,q1 };

    std::vector<math::Rational<double,0,4,0,4>>   rationals1 { p0/q0 };
    std::vector<math::Rational<double,-2,2,0,4>>  rationals2 { p1/q0 };
    std::vector<math::Rational<double,0,4,-2,2>>  rationals3 { p0/q1 };
    std::vector<math::Rational<double,-2,2,-2,2>> rationals4 { p1/q1 };

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

    math::Polynomial<double,0,0> zero = math::Polynomial<double,0,0>({0.0f});
    math::Polynomial<double,0,0> one  = math::Polynomial<double,0,0>({1.0f});


    test::Field field(
        "0", zero, 
        "1", one, 
        "rational addition",       TEST_SYMBOL(+),
        "rational subtraction",    TEST_SYMBOL(-),
        "rational multiplication", TEST_SYMBOL(*),
        "rational division",       TEST_SYMBOL(/)
    );

    // UNARY TESTS
    REQUIRE(field.valid(broad, rationals1));
    REQUIRE(field.valid(broad, rationals2));
    REQUIRE(field.valid(broad, rationals3));
    REQUIRE(field.valid(broad, rationals4));

    // BINARY TESTS
    REQUIRE(field.valid(broad, rationals1, rationals2  ));
    REQUIRE(field.valid(broad, rationals1, rationals3  ));
    REQUIRE(field.valid(broad, rationals1, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, polynomials1));
    REQUIRE(field.valid(broad, rationals1, polynomials2));
    REQUIRE(field.valid(broad, rationals1, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, scalars     ));
    REQUIRE(field.valid(broad, rationals1, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, scalings    ));
    REQUIRE(field.valid(broad, rationals2, rationals3  ));
    REQUIRE(field.valid(broad, rationals2, rationals4  ));
    REQUIRE(field.valid(broad, rationals2, polynomials1));
    REQUIRE(field.valid(broad, rationals2, polynomials2));
    REQUIRE(field.valid(broad, rationals2, monomials1  ));
    REQUIRE(field.valid(broad, rationals2, monomials2  ));
    REQUIRE(field.valid(broad, rationals2, monomials3  ));
    REQUIRE(field.valid(broad, rationals2, scalars     ));
    REQUIRE(field.valid(broad, rationals2, shiftings   ));
    REQUIRE(field.valid(broad, rationals2, scalings    ));
    REQUIRE(field.valid(broad, rationals3, rationals4  ));
    REQUIRE(field.valid(broad, rationals3, polynomials1));
    REQUIRE(field.valid(broad, rationals3, polynomials2));
    REQUIRE(field.valid(broad, rationals3, monomials1  ));
    REQUIRE(field.valid(broad, rationals3, monomials2  ));
    REQUIRE(field.valid(broad, rationals3, monomials3  ));
    REQUIRE(field.valid(broad, rationals3, scalars     ));
    REQUIRE(field.valid(broad, rationals3, shiftings   ));
    REQUIRE(field.valid(broad, rationals3, scalings    ));
    REQUIRE(field.valid(broad, rationals4, polynomials1));
    REQUIRE(field.valid(broad, rationals4, polynomials2));
    REQUIRE(field.valid(broad, rationals4, monomials1  ));
    REQUIRE(field.valid(broad, rationals4, monomials2  ));
    REQUIRE(field.valid(broad, rationals4, monomials3  ));
    REQUIRE(field.valid(broad, rationals4, scalars     ));
    REQUIRE(field.valid(broad, rationals4, shiftings   ));
    REQUIRE(field.valid(broad, rationals4, scalings    ));

    // TRINARY TESTS
    REQUIRE(field.valid(broad, rationals1, rationals2, rationals3  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals2, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals2, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals2, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals2, scalings    ));
    REQUIRE(field.valid(broad, rationals1, rationals3, rationals4  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, polynomials1));
    REQUIRE(field.valid(broad, rationals1, rationals3, polynomials2));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials1  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials2  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, monomials3  ));
    REQUIRE(field.valid(broad, rationals1, rationals3, scalars     ));
    REQUIRE(field.valid(broad, rationals1, rationals3, shiftings   ));
    REQUIRE(field.valid(broad, rationals1, rationals3, scalings    ));
    REQUIRE(field.valid(broad, rationals2, rationals3, rationals4  ));
    REQUIRE(field.valid(broad, rationals2, rationals3, polynomials1));
    REQUIRE(field.valid(broad, rationals2, rationals3, polynomials2));
    REQUIRE(field.valid(broad, rationals2, rationals3, monomials1  ));
    REQUIRE(field.valid(broad, rationals2, rationals3, monomials2  ));
    REQUIRE(field.valid(broad, rationals2, rationals3, monomials3  ));
    REQUIRE(field.valid(broad, rationals2, rationals3, scalars     ));
    REQUIRE(field.valid(broad, rationals2, rationals3, shiftings   ));
    REQUIRE(field.valid(broad, rationals2, rationals3, scalings    ));
    REQUIRE(field.valid(broad, rationals3, rationals4, polynomials1));
    REQUIRE(field.valid(broad, rationals3, rationals4, polynomials2));
    REQUIRE(field.valid(broad, rationals3, rationals4, monomials1  ));
    REQUIRE(field.valid(broad, rationals3, rationals4, monomials2  ));
    REQUIRE(field.valid(broad, rationals3, rationals4, monomials3  ));
    REQUIRE(field.valid(broad, rationals3, rationals4, scalars     ));
    REQUIRE(field.valid(broad, rationals3, rationals4, shiftings   ));
    REQUIRE(field.valid(broad, rationals3, rationals4, scalings    ));

}







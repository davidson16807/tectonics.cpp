#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include "../scalar.hpp"

#include "PolynomialRailyard.hpp"
#include "PolynomialTrain.hpp"

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

// TEST_CASE( "PolynomialRailyard composition commutativity", "[math]" ) {
//     // `threshold` is the tolerance towards error in the result
//     const double threshold = 1e-4;
//     // `hi` is the largest absolute value to be tested
//     const double hi =  1e3;
//     // `lo` is the smallest absolute value to be tested
//     const double lo =  1e3;
//     // `multiple` is the multiple by which tests are incremented
//     const double multiple = -std::pow(10.0, 0.1);

//     using P0 = analytic::Polynomial<double,0,2>;
//     using P1 = analytic::Polynomial<double,-2,1>;
//     using P2 = analytic::Polynomial<double,-2,2>;

//     P0 p0 = P0({3.0f,2.0f,1.0f});
//     P0 p1 = P0({-1.0f,0.0f,1.0f});
//     P1 p2 = P1({4.0f,3.0f,2.0f,1.0f});
//     P1 p3 = P1({-1.0f,1.0f,-2.0f,2.0f});

//     using C0 = analytic::Railcar<double,P0>;
//     using C1 = analytic::Railcar<double,P1>;
//     using C2 = analytic::Railcar<double,P2>;

//     // standard polynomials, no overlap
//     analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
//         C0(lo, -1.0f, p0), 
//         C0(1.0f, hi,  p1) 
//     }; 

//     // standard polynomials, with overlap
//     analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
//         C0(lo, midhi, p0), 
//         C0(midlo, hi, p1) 
//     }; 
//     // laurent polynomials
//     analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
//         C1(midlo, -1.0f, p2), 
//         C1(1.0f, midhi, p3) 
//     }; 
//     // all polynomials
//     analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
//         C2(lo, midlo, P2(p2)), 
//         C2(midhi, hi, P2(p3)), 
//         C2(midlo, -1.0f, P2(p2)), 
//         C2(1.0f, midhi, P2(p3)) 
//     };

//     SECTION("si(x)+sj(x) must be equivalent to (si+sj)(x)"){
//         for (int x = lo; std::abs(x) < hi; x*=multiple)
//         {
//             CHECK(analytic::distance(compose(s0, s0)(x), s0(s0(x))) < threshold );
//             CHECK(analytic::distance(compose(s0, s1)(x), s0(s1(x))) < threshold );
//             CHECK(analytic::distance(compose(s0, s2)(x), s0(s2(x))) < threshold );
//             CHECK(analytic::distance(compose(s0, s3)(x), s0(s3(x))) < threshold );

//             CHECK(analytic::distance(compose(s1, s0)(x), s1(s0(x))) < threshold );
//             CHECK(analytic::distance(compose(s1, s1)(x), s1(s1(x))) < threshold );
//             CHECK(analytic::distance(compose(s1, s2)(x), s1(s2(x))) < threshold );
//             CHECK(analytic::distance(compose(s1, s3)(x), s1(s3(x))) < threshold );

//             CHECK(analytic::distance(compose(s2, s0)(x), s2(s0(x))) < threshold );
//             CHECK(analytic::distance(compose(s2, s1)(x), s2(s1(x))) < threshold );
//             CHECK(analytic::distance(compose(s2, s2)(x), s2(s2(x))) < threshold );
//             CHECK(analytic::distance(compose(s2, s3)(x), s2(s3(x))) < threshold );

//             CHECK(analytic::distance(compose(s3, s0)(x), s3(s0(x))) < threshold );
//             CHECK(analytic::distance(compose(s3, s1)(x), s3(s1(x))) < threshold );
//             CHECK(analytic::distance(compose(s3, s2)(x), s3(s2(x))) < threshold );
//             CHECK(analytic::distance(compose(s3, s3)(x), s3(s3(x))) < threshold );
//         }
//     }

// }

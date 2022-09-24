#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide r main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/analytic/Rational.hpp>  


TEST_CASE( "Rational arithmetic purity", "[math]" ) {
    const double threshold = 1e-7;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    SECTION("r+s must be called repeatedly without changing the output"){

        CHECK(analytic::distance(r+r, r+r, midlo, midhi) < threshold );
        CHECK(analytic::distance(r+s, r+s, midlo, midhi) < threshold );
        CHECK(analytic::distance(r+t, r+t, midlo, midhi) < threshold );
        CHECK(analytic::distance(r+u, r+u, midlo, midhi) < threshold );

        CHECK(analytic::distance(s+r, s+r, midlo, midhi) < threshold );
        CHECK(analytic::distance(s+s, s+s, midlo, midhi) < threshold );
        CHECK(analytic::distance(s+t, s+t, midlo, midhi) < threshold );
        CHECK(analytic::distance(s+u, s+u, midlo, midhi) < threshold );

        CHECK(analytic::distance(t+r, t+r, midlo, midhi) < threshold );
        CHECK(analytic::distance(t+s, t+s, midlo, midhi) < threshold );
        CHECK(analytic::distance(t+t, t+t, midlo, midhi) < threshold );
        CHECK(analytic::distance(t+u, t+u, midlo, midhi) < threshold );

        CHECK(analytic::distance(u+r, u+r, midlo, midhi) < threshold );
        CHECK(analytic::distance(u+s, u+s, midlo, midhi) < threshold );
        CHECK(analytic::distance(u+t, u+t, midlo, midhi) < threshold );
        CHECK(analytic::distance(u+u, u+u, midlo, midhi) < threshold );
    }

    SECTION("r*s must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r*r, r*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*s, r*s, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*t, r*t, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*u, r*u, midlo, midhi) < threshold );

        CHECK(analytic::distance(s*r, s*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*s, s*s, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*t, s*t, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*u, s*u, midlo, midhi) < threshold );

        CHECK(analytic::distance(t*r, t*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(t*s, t*s, midlo, midhi) < threshold );
        CHECK(analytic::distance(t*t, t*t, midlo, midhi) < threshold );
        CHECK(analytic::distance(t*u, t*u, midlo, midhi) < threshold );

        CHECK(analytic::distance(u*r, u*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(u*s, u*s, midlo, midhi) < threshold );
        CHECK(analytic::distance(u*t, u*t, midlo, midhi) < threshold );
        CHECK(analytic::distance(u*u, u*u, midlo, midhi) < threshold );
    }

    SECTION("r-s must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r-r, r-r, midlo, midhi) < threshold );
        CHECK(analytic::distance(r-s, r-s, midlo, midhi) < threshold );
        CHECK(analytic::distance(r-t, r-t, midlo, midhi) < threshold );
        CHECK(analytic::distance(r-u, r-u, midlo, midhi) < threshold );

        CHECK(analytic::distance(s-r, s-r, midlo, midhi) < threshold );
        CHECK(analytic::distance(s-s, s-s, midlo, midhi) < threshold );
        CHECK(analytic::distance(s-t, s-t, midlo, midhi) < threshold );
        CHECK(analytic::distance(s-u, s-u, midlo, midhi) < threshold );

        CHECK(analytic::distance(t-r, t-r, midlo, midhi) < threshold );
        CHECK(analytic::distance(t-s, t-s, midlo, midhi) < threshold );
        CHECK(analytic::distance(t-t, t-t, midlo, midhi) < threshold );
        CHECK(analytic::distance(t-u, t-u, midlo, midhi) < threshold );

        CHECK(analytic::distance(u-r, u-r, midlo, midhi) < threshold );
        CHECK(analytic::distance(u-s, u-s, midlo, midhi) < threshold );
        CHECK(analytic::distance(u-t, u-t, midlo, midhi) < threshold );
        CHECK(analytic::distance(u-u, u-u, midlo, midhi) < threshold );
    }
}

TEST_CASE( "Rational arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double lo = -1000.0f;
    const double hi =  1000.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;


    auto zero = analytic::Rational<double,0,0,0,0>(analytic::Polynomial<double,0,0>{0.0f}, analytic::Polynomial<double,0,0>{1.0});
    auto one  = analytic::Rational<double,0,0,0,0>(analytic::Polynomial<double,0,0>{1.0}, analytic::Polynomial<double,0,0>{1.0});

    SECTION("r+I must equal r"){
        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r*one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s*one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);

        CHECK(analytic::distance(t+zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t-zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t*one , t, lo, hi) < threshold);
        CHECK(analytic::distance(t-t, zero, lo, hi) < threshold);

        CHECK(analytic::distance(u+zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u-zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u*one , u, lo, hi) < threshold);
        CHECK(analytic::distance(u-u, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;


    SECTION("r+s must equal s+r"){
        CHECK(analytic::distance(r+s, s+r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r+t, t+r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r+u, u+r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s+r, r+s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s+t, t+s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s+u, u+s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t+r, r+t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t+s, s+t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t+u, u+t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u+r, r+u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u+s, s+u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u+t, t+u, midlo, midhi) < threshold);
    }
    SECTION("r*s must equal s*r"){
        CHECK(analytic::distance(r*s, s*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*t, t*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*u, u*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*r, r*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*t, t*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*u, u*s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*r, r*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*s, s*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*u, u*t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*r, r*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*s, s*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*t, t*u, midlo, midhi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3.0f;
    const double midhi =  3.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;


    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+t, r+(s+t), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+u, r+(s+u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+u, s+(t+u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+r, s+(t+r), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+r, t+(u+r), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+s, t+(u+s), midlo, midhi) < threshold);
    }
    SECTION("(r*s)*t must equal r*(s*t)"){
        CHECK(analytic::distance((r*s)*t, r*(s*t), midlo, midhi) < threshold);
        CHECK(analytic::distance((r*s)*u, r*(s*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s*t)*u, s*(t*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s*t)*r, s*(t*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((t*u)*r, t*(u*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((t*u)*s, t*(u*s), midlo, midhi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3.0f;
    const double midhi =  3.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;


    SECTION("r+s must equal s+r"){
        CHECK(analytic::distance((r+s)*t, r*t+s*t, midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)*u, r*u+s*u, midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*s, r*s+t*s, midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)*u, r*u+t*u, midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*s, r*s+u*s, midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)*t, r*t+u*t, midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*r, s*r+t*r, midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)*u, s*u+t*u, midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*r, s*r+u*r, midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)*t, s*t+u*t, midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*r, t*r+u*r, midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)*s, t*s+u*s, midlo, midhi) < threshold);
    }
}







TEST_CASE( "Rational/scalar arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -30.0f;
    const double hi =  30.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("r+k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r+k0, r+k0, lo, hi) < threshold);
        CHECK(analytic::distance(r+k1, r+k1, lo, hi) < threshold);
        CHECK(analytic::distance(r+k2, r+k2, lo, hi) < threshold);

        CHECK(analytic::distance(s+k0, s+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s+k1, s+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s+k2, s+k2, lo, hi) < threshold);

        CHECK(analytic::distance(t+k0, t+k0, lo, hi) < threshold);
        CHECK(analytic::distance(t+k1, t+k1, lo, hi) < threshold);
        CHECK(analytic::distance(t+k2, t+k2, lo, hi) < threshold);

        CHECK(analytic::distance(u+k0, u+k0, lo, hi) < threshold);
        CHECK(analytic::distance(u+k1, u+k1, lo, hi) < threshold);
        CHECK(analytic::distance(u+k2, u+k2, lo, hi) < threshold);
    }

    SECTION("r*k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r*k0, r*k0, lo, hi) < threshold);
        CHECK(analytic::distance(r*k1, r*k1, lo, hi) < threshold);
        CHECK(analytic::distance(r*k2, r*k2, lo, hi) < threshold);

        CHECK(analytic::distance(s*k0, s*k0, lo, hi) < threshold);
        CHECK(analytic::distance(s*k1, s*k1, lo, hi) < threshold);
        CHECK(analytic::distance(s*k2, s*k2, lo, hi) < threshold);

        CHECK(analytic::distance(t*k0, t*k0, lo, hi) < threshold);
        CHECK(analytic::distance(t*k1, t*k1, lo, hi) < threshold);
        CHECK(analytic::distance(t*k2, t*k2, lo, hi) < threshold);

        CHECK(analytic::distance(u*k0, u*k0, lo, hi) < threshold);
        CHECK(analytic::distance(u*k1, u*k1, lo, hi) < threshold);
        CHECK(analytic::distance(u*k2, u*k2, lo, hi) < threshold);
    }

    SECTION("r-k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r-k0, r-k0, lo, hi) < threshold);
        CHECK(analytic::distance(r-k1, r-k1, lo, hi) < threshold);
        CHECK(analytic::distance(r-k2, r-k2, lo, hi) < threshold);

        CHECK(analytic::distance(s-k0, s-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s-k1, s-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s-k2, s-k2, lo, hi) < threshold);

        CHECK(analytic::distance(t-k0, t-k0, lo, hi) < threshold);
        CHECK(analytic::distance(t-k1, t-k1, lo, hi) < threshold);
        CHECK(analytic::distance(t-k2, t-k2, lo, hi) < threshold);

        CHECK(analytic::distance(u-k0, u-k0, lo, hi) < threshold);
        CHECK(analytic::distance(u-k1, u-k1, lo, hi) < threshold);
        CHECK(analytic::distance(u-k2, u-k2, lo, hi) < threshold);
    }

    SECTION("r/k1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(r/k0, r/k0, lo, hi) < threshold);
        CHECK(analytic::distance(r/k1, r/k1, lo, hi) < threshold);
        CHECK(analytic::distance(r/k2, r/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(s/k0, s/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s/k1, s/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s/k2, s/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(t/k0, t/k0, lo, hi) < threshold);
        CHECK(analytic::distance(t/k1, t/k1, lo, hi) < threshold);
        CHECK(analytic::distance(t/k2, t/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(u/k0, u/k0, lo, hi) < threshold);
        CHECK(analytic::distance(u/k1, u/k1, lo, hi) < threshold);
        CHECK(analytic::distance(u/k2, u/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/scalar arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    double zero(0.0f);
    double one (1.0);

    SECTION("r+I must equal r"){
        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r*one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r/one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s*one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s/one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);

        CHECK(analytic::distance(t+zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t-zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t*one , t, lo, hi) < threshold);
        CHECK(analytic::distance(t/one , t, lo, hi) < threshold);
        CHECK(analytic::distance(t-t, zero, lo, hi) < threshold);

        CHECK(analytic::distance(u+zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u-zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u*one , u, lo, hi) < threshold);
        CHECK(analytic::distance(u/one , u, lo, hi) < threshold);
        CHECK(analytic::distance(u-u, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/scalar arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("r+k must equal k+r"){
        CHECK(analytic::distance(r+k0, k0+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+k1, k1+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+k2, k2+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+k0, k0+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+k1, k1+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+k2, k2+s, lo, hi) < threshold);

        CHECK(analytic::distance(t+k0, k0+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+k1, k1+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+k2, k2+t, lo, hi) < threshold);

        CHECK(analytic::distance(u+k0, k0+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+k1, k1+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+k2, k2+u, lo, hi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(analytic::distance(r*k0, k0*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k1, k1*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k2, k2*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*k0, k0*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k1, k1*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k2, k2*s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*k0, k0*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*k1, k1*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*k2, k2*t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*k0, k0*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*k1, k1*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*k2, k2*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/scalar arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+k0, r+(s+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+k1, r+(s+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+k2, r+(s+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+k0, r+(t+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+k1, r+(t+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+k2, r+(t+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+k0, r+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+k1, r+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+k2, r+(u+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+k0, s+(t+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+k1, s+(t+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+k2, s+(t+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+k0, s+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+k1, s+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+k2, s+(u+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+k0, t+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+k1, t+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+k2, t+(u+k2), midlo, midhi) < threshold);
    }

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+k0, r+(s+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+k1, r+(s+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+k2, r+(s+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+k0, r+(t+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+k1, r+(t+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+k2, r+(t+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+k0, r+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+k1, r+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+k2, r+(u+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+k0, s+(t+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+k1, s+(t+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+k2, s+(t+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+k0, s+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+k1, s+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+k2, s+(u+k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+k0, t+(u+k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+k1, t+(u+k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+k2, t+(u+k2), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/scalar arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(analytic::distance((r+s)*k0, (r*k0+s*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)*k1, (r*k1+s*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)*k2, (r*k2+s*k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*k0, (r*k0+t*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)*k1, (r*k1+t*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)*k2, (r*k2+t*k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*k0, (r*k0+u*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)*k1, (r*k1+u*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)*k2, (r*k2+u*k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*k0, (s*k0+t*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)*k1, (s*k1+t*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)*k2, (s*k2+t*k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*k0, (s*k0+u*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)*k1, (s*k1+u*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)*k2, (s*k2+u*k2), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*k0, (t*k0+u*k0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)*k1, (t*k1+u*k1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)*k2, (t*k2+u*k2), midlo, midhi) < threshold);



        CHECK(analytic::distance((r+k0)*s, (r*s+k0*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k1)*s, (r*s+k1*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k2)*s, (r*s+k2*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+k0)*t, (r*t+k0*t), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k1)*t, (r*t+k1*t), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k2)*t, (r*t+k2*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+k0)*u, (r*u+k0*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k1)*u, (r*u+k1*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k2)*u, (r*u+k2*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+k0)*t, (s*t+k0*t), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+k1)*t, (s*t+k1*t), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+k2)*t, (s*t+k2*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+k0)*u, (s*u+k0*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+k1)*u, (s*u+k1*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+k2)*u, (s*u+k2*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+k0)*u, (t*u+k0*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+k1)*u, (t*u+k1*u), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+k2)*u, (t*u+k2*u), midlo, midhi) < threshold);

    }
}








TEST_CASE( "Rational/monomial arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double lo = -30.0f;
    const double hi =  30.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0f});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0f});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{2.0f});

    SECTION("r+m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r+m0, r+m0, lo, hi) < threshold);
        CHECK(analytic::distance(r+m1, r+m1, lo, hi) < threshold);
        CHECK(analytic::distance(r+m2, r+m2, lo, hi) < threshold);

        CHECK(analytic::distance(s+m0, s+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s+m1, s+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s+m2, s+m2, lo, hi) < threshold);

        CHECK(analytic::distance(t+m0, t+m0, lo, hi) < threshold);
        CHECK(analytic::distance(t+m1, t+m1, lo, hi) < threshold);
        CHECK(analytic::distance(t+m2, t+m2, lo, hi) < threshold);

        CHECK(analytic::distance(u+m0, u+m0, lo, hi) < threshold);
        CHECK(analytic::distance(u+m1, u+m1, lo, hi) < threshold);
        CHECK(analytic::distance(u+m2, u+m2, lo, hi) < threshold);
    }

    SECTION("r*m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r*m0, r*m0, lo, hi) < threshold);
        CHECK(analytic::distance(r*m1, r*m1, lo, hi) < threshold);
        CHECK(analytic::distance(r*m2, r*m2, lo, hi) < threshold);

        CHECK(analytic::distance(s*m0, s*m0, lo, hi) < threshold);
        CHECK(analytic::distance(s*m1, s*m1, lo, hi) < threshold);
        CHECK(analytic::distance(s*m2, s*m2, lo, hi) < threshold);

        CHECK(analytic::distance(t*m0, t*m0, lo, hi) < threshold);
        CHECK(analytic::distance(t*m1, t*m1, lo, hi) < threshold);
        CHECK(analytic::distance(t*m2, t*m2, lo, hi) < threshold);

        CHECK(analytic::distance(u*m0, u*m0, lo, hi) < threshold);
        CHECK(analytic::distance(u*m1, u*m1, lo, hi) < threshold);
        CHECK(analytic::distance(u*m2, u*m2, lo, hi) < threshold);
    }

    SECTION("r-m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r-m0, r-m0, lo, hi) < threshold);
        CHECK(analytic::distance(r-m1, r-m1, lo, hi) < threshold);
        CHECK(analytic::distance(r-m2, r-m2, lo, hi) < threshold);

        CHECK(analytic::distance(s-m0, s-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s-m1, s-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s-m2, s-m2, lo, hi) < threshold);

        CHECK(analytic::distance(t-m0, t-m0, lo, hi) < threshold);
        CHECK(analytic::distance(t-m1, t-m1, lo, hi) < threshold);
        CHECK(analytic::distance(t-m2, t-m2, lo, hi) < threshold);

        CHECK(analytic::distance(u-m0, u-m0, lo, hi) < threshold);
        CHECK(analytic::distance(u-m1, u-m1, lo, hi) < threshold);
        CHECK(analytic::distance(u-m2, u-m2, lo, hi) < threshold);
    }

    SECTION("r/m1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(r/m0, r/m0, lo, hi) < threshold);
        CHECK(analytic::distance(r/m1, r/m1, lo, hi) < threshold);
        CHECK(analytic::distance(r/m2, r/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(s/m0, s/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s/m1, s/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s/m2, s/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(t/m0, t/m0, lo, hi) < threshold);
        CHECK(analytic::distance(t/m1, t/m1, lo, hi) < threshold);
        CHECK(analytic::distance(t/m2, t/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(u/m0, u/m0, lo, hi) < threshold);
        CHECK(analytic::distance(u/m1, u/m1, lo, hi) < threshold);
        CHECK(analytic::distance(u/m2, u/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/monomial arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Polynomial<double,0,0> zero{0.0f};
    analytic::Polynomial<double,0,0> one {1.0};

    SECTION("r+I must equal r"){
        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r*one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r/one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s*one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s/one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);

        CHECK(analytic::distance(t+zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t-zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t*one , t, lo, hi) < threshold);
        CHECK(analytic::distance(t/one , t, lo, hi) < threshold);
        CHECK(analytic::distance(t-t, zero, lo, hi) < threshold);

        CHECK(analytic::distance(u+zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u-zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u*one , u, lo, hi) < threshold);
        CHECK(analytic::distance(u/one , u, lo, hi) < threshold);
        CHECK(analytic::distance(u-u, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/monomial arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0f});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0f});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.0f});

    SECTION("r+k must equal k+r"){
        CHECK(analytic::distance(r+m0, m0+r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r+m1, m1+r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r+m2, m2+r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s+m0, m0+s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s+m1, m1+s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s+m2, m2+s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t+m0, m0+t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t+m1, m1+t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t+m2, m2+t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u+m0, m0+u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u+m1, m1+u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u+m2, m2+u, midlo, midhi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(analytic::distance(r*m0, m0*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m1, m1*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m2, m2*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*m0, m0*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m1, m1*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m2, m2*s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*m0, m0*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*m1, m1*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*m2, m2*t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*m0, m0*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*m1, m1*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*m2, m2*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/monomial arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0f});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0f});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.0f});

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+m0, r+(s+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+m1, r+(s+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+m2, r+(s+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+m0, r+(t+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+m1, r+(t+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+m2, r+(t+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+m0, r+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+m1, r+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+m2, r+(u+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+m0, s+(t+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+m1, s+(t+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+m2, s+(t+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+m0, s+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+m1, s+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+m2, s+(u+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+m0, t+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+m1, t+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+m2, t+(u+m2), midlo, midhi) < threshold);
    }

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+m0, r+(s+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+m1, r+(s+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+m2, r+(s+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+m0, r+(t+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+m1, r+(t+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+m2, r+(t+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+m0, r+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+m1, r+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+m2, r+(u+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+m0, s+(t+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+m1, s+(t+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+m2, s+(t+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+m0, s+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+m1, s+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+m2, s+(u+m2), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+m0, t+(u+m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+m1, t+(u+m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+m2, t+(u+m2), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/monomial arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3.0f;
    const double midhi =  3.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0f});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0f});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.0f});

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(analytic::distance((r+s)*m0, (r*m0+s*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m0)*s, (r*s+m0*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*m1, (r*m1+s*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m1)*s, (r*s+m1*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*m2, (r*m2+s*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m2)*s, (r*s+m2*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+t)*m0, (r*m0+t*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m0)*t, (r*t+m0*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*m1, (r*m1+t*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m1)*t, (r*t+m1*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*m2, (r*m2+t*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m2)*t, (r*t+m2*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+u)*m0, (r*m0+u*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m0)*u, (r*u+m0*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*m1, (r*m1+u*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m1)*u, (r*u+m1*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*m2, (r*m2+u*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m2)*u, (r*u+m2*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+t)*m0, (s*m0+t*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m0)*t, (s*t+m0*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*m1, (s*m1+t*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m1)*t, (s*t+m1*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*m2, (s*m2+t*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m2)*t, (s*t+m2*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+u)*m0, (s*m0+u*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m0)*u, (s*u+m0*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*m1, (s*m1+u*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m1)*u, (s*u+m1*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*m2, (s*m2+u*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+m2)*u, (s*u+m2*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((t+u)*m0, (t*m0+u*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+m0)*u, (t*u+m0*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*m1, (t*m1+u*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+m1)*u, (t*u+m1*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*m2, (t*m2+u*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+m2)*u, (t*u+m2*u), midlo, midhi) < threshold);
    }
}










TEST_CASE( "Rational/Shifting arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("r+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, s+h, lo, hi) < threshold);

        CHECK(analytic::distance(t+f, t+f, lo, hi) < threshold);
        CHECK(analytic::distance(t+g, t+g, lo, hi) < threshold);
        CHECK(analytic::distance(t+h, t+h, lo, hi) < threshold);

        CHECK(analytic::distance(u+f, u+f, lo, hi) < threshold);
        CHECK(analytic::distance(u+g, u+g, lo, hi) < threshold);
        CHECK(analytic::distance(u+h, u+h, lo, hi) < threshold);
    }

    SECTION("r*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, s*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*f, t*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*g, t*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*h, t*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*f, u*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*g, u*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*h, u*h, midlo, midhi) < threshold);
    }

    SECTION("r-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(analytic::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(analytic::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(analytic::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(analytic::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(analytic::distance(s-h, s-h, lo, hi) < threshold);

        CHECK(analytic::distance(t-f, t-f, lo, hi) < threshold);
        CHECK(analytic::distance(t-g, t-g, lo, hi) < threshold);
        CHECK(analytic::distance(t-h, t-h, lo, hi) < threshold);

        CHECK(analytic::distance(u-f, u-f, lo, hi) < threshold);
        CHECK(analytic::distance(u-g, u-g, lo, hi) < threshold);
        CHECK(analytic::distance(u-h, u-h, lo, hi) < threshold);
    }

    // SECTION("r/g must be called repeatedly without changing the output"){
    //     // CHECK(analytic::distance(r/f, r/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(r/g, r/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(r/h, r/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(s/f, s/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s/g, s/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s/h, s/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(t/f, t/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(t/g, t/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(t/h, t/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(u/f, u/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(u/g, u/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(u/h, u/h, lo, hi) < threshold);
    // }
}

TEST_CASE( "Rational/Shifting arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("r+k must equal k+r"){
        CHECK(analytic::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, h+s, lo, hi) < threshold);

        CHECK(analytic::distance(t+f, f+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+g, g+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+h, h+t, lo, hi) < threshold);

        CHECK(analytic::distance(u+f, f+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+g, g+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+h, h+u, lo, hi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(analytic::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, h*s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*f, f*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*g, g*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*h, h*t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*f, f*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*g, g*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*h, h*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(analytic::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+t)*f, (r*f+t*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*t, (r*t+f*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*g, (r*g+t*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*t, (r*t+g*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*h, (r*h+t*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*t, (r*t+h*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+u)*f, (r*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*u, (r*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*g, (r*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*u, (r*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*h, (r*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*u, (r*u+h*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+t)*f, (s*f+t*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+f)*t, (s*t+f*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*g, (s*g+t*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+g)*t, (s*t+g*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*h, (s*h+t*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+h)*t, (s*t+h*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+u)*f, (s*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+f)*u, (s*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*g, (s*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+g)*u, (s*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*h, (s*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+h)*u, (s*u+h*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((t+u)*f, (t*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+f)*u, (t*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*g, (t*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+g)*u, (t*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*h, (t*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+h)*u, (t*u+h*u), midlo, midhi) < threshold);
    }
}











TEST_CASE( "Rational/Scaling arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("r+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, s+h, lo, hi) < threshold);

        CHECK(analytic::distance(t+f, t+f, lo, hi) < threshold);
        CHECK(analytic::distance(t+g, t+g, lo, hi) < threshold);
        CHECK(analytic::distance(t+h, t+h, lo, hi) < threshold);

        CHECK(analytic::distance(u+f, u+f, lo, hi) < threshold);
        CHECK(analytic::distance(u+g, u+g, lo, hi) < threshold);
        CHECK(analytic::distance(u+h, u+h, lo, hi) < threshold);
    }

    SECTION("r*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, s*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*f, t*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*g, t*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*h, t*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*f, u*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*g, u*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*h, u*h, midlo, midhi) < threshold);
    }

    SECTION("r-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(analytic::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(analytic::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(analytic::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(analytic::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(analytic::distance(s-h, s-h, lo, hi) < threshold);

        CHECK(analytic::distance(t-f, t-f, lo, hi) < threshold);
        CHECK(analytic::distance(t-g, t-g, lo, hi) < threshold);
        CHECK(analytic::distance(t-h, t-h, lo, hi) < threshold);

        CHECK(analytic::distance(u-f, u-f, lo, hi) < threshold);
        CHECK(analytic::distance(u-g, u-g, lo, hi) < threshold);
        CHECK(analytic::distance(u-h, u-h, lo, hi) < threshold);
    }

    SECTION("r/g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(r/f, r/f, lo, hi) < threshold);
        CHECK(analytic::distance(r/g, r/g, lo, hi) < threshold);
        // CHECK(analytic::distance(r/h, r/h, lo, hi) < threshold);

        CHECK(analytic::distance(s/f, s/f, lo, hi) < threshold);
        CHECK(analytic::distance(s/g, s/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s/h, s/h, lo, hi) < threshold);

        CHECK(analytic::distance(t/f, t/f, lo, hi) < threshold);
        CHECK(analytic::distance(t/g, t/g, lo, hi) < threshold);
        // CHECK(analytic::distance(t/h, t/h, lo, hi) < threshold);

        CHECK(analytic::distance(u/f, u/f, lo, hi) < threshold);
        CHECK(analytic::distance(u/g, u/g, lo, hi) < threshold);
        // CHECK(analytic::distance(u/h, u/h, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/Scaling arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Scaling<double> zero(0.0f);

    SECTION("r+I must equal r"){
        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        // CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        // CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);

        CHECK(analytic::distance(t+zero, t, lo, hi) < threshold);
        CHECK(analytic::distance(t-zero, t, lo, hi) < threshold);
        // CHECK(analytic::distance(t-t, zero, lo, hi) < threshold);

        CHECK(analytic::distance(u+zero, u, lo, hi) < threshold);
        CHECK(analytic::distance(u-zero, u, lo, hi) < threshold);
        // CHECK(analytic::distance(u-u, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/Scaling arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("r+k must equal k+r"){
        CHECK(analytic::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, h+s, lo, hi) < threshold);

        CHECK(analytic::distance(t+f, f+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+g, g+t, lo, hi) < threshold);
        CHECK(analytic::distance(t+h, h+t, lo, hi) < threshold);

        CHECK(analytic::distance(u+f, f+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+g, g+u, lo, hi) < threshold);
        CHECK(analytic::distance(u+h, h+u, lo, hi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(analytic::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, h*s, midlo, midhi) < threshold);

        CHECK(analytic::distance(t*f, f*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*g, g*t, midlo, midhi) < threshold);
        CHECK(analytic::distance(t*h, h*t, midlo, midhi) < threshold);

        CHECK(analytic::distance(u*f, f*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*g, g*u, midlo, midhi) < threshold);
        CHECK(analytic::distance(u*h, h*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(analytic::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    analytic::Polynomial<double,0,4> p0 = analytic::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,0,4> q0 = analytic::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    analytic::Polynomial<double,-2,2> p1 = analytic::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    analytic::Polynomial<double,-2,2> q1 = analytic::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(analytic::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+t)*f, (r*f+t*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*t, (r*t+f*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*g, (r*g+t*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*t, (r*t+g*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+t)*h, (r*h+t*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*t, (r*t+h*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+u)*f, (r*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*u, (r*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*g, (r*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*u, (r*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+u)*h, (r*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*u, (r*u+h*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+t)*f, (s*f+t*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+f)*t, (s*t+f*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*g, (s*g+t*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+g)*t, (s*t+g*t), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+t)*h, (s*h+t*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+h)*t, (s*t+h*t), midlo, midhi) < threshold);


        CHECK(analytic::distance((s+u)*f, (s*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+f)*u, (s*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*g, (s*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+g)*u, (s*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((s+u)*h, (s*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s+h)*u, (s*u+h*u), midlo, midhi) < threshold);


        CHECK(analytic::distance((t+u)*f, (t*f+u*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+f)*u, (t*u+f*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*g, (t*g+u*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+g)*u, (t*u+g*u), midlo, midhi) < threshold);

        CHECK(analytic::distance((t+u)*h, (t*h+u*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((t+h)*u, (t*u+h*u), midlo, midhi) < threshold);
    }
}







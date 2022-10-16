#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/analytic/Polynomial.hpp>  

TEST_CASE( "Polynomial arithmetic purity", "[math]" ) {
    const double threshold = 1e-7;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});


    SECTION("p+q must be called repeatedly without changing the output"){

        CHECK(analytic::distance(p+p, p+p, lo, hi) < threshold );
        CHECK(analytic::distance(p+q, p+q, lo, hi) < threshold );
        CHECK(analytic::distance(p+r, p+r, lo, hi) < threshold );
        CHECK(analytic::distance(p+s, p+s, lo, hi) < threshold );

        CHECK(analytic::distance(q+p, q+p, lo, hi) < threshold );
        CHECK(analytic::distance(q+q, q+q, lo, hi) < threshold );
        CHECK(analytic::distance(q+r, q+r, lo, hi) < threshold );
        CHECK(analytic::distance(q+s, q+s, lo, hi) < threshold );

        CHECK(analytic::distance(r+p, r+p, lo, hi) < threshold );
        CHECK(analytic::distance(r+q, r+q, lo, hi) < threshold );
        CHECK(analytic::distance(r+r, r+r, lo, hi) < threshold );
        CHECK(analytic::distance(r+s, r+s, lo, hi) < threshold );

        CHECK(analytic::distance(s+p, s+p, lo, hi) < threshold );
        CHECK(analytic::distance(s+q, s+q, lo, hi) < threshold );
        CHECK(analytic::distance(s+r, s+r, lo, hi) < threshold );
        CHECK(analytic::distance(s+s, s+s, lo, hi) < threshold );
    }

    SECTION("p*q must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p*p, p*p, midlo, midhi) < threshold );
        CHECK(analytic::distance(p*q, p*q, midlo, midhi) < threshold );
        CHECK(analytic::distance(p*r, p*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(p*s, p*s, midlo, midhi) < threshold );

        CHECK(analytic::distance(q*p, q*p, midlo, midhi) < threshold );
        CHECK(analytic::distance(q*q, q*q, midlo, midhi) < threshold );
        CHECK(analytic::distance(q*r, q*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(q*s, q*s, midlo, midhi) < threshold );

        CHECK(analytic::distance(r*p, r*p, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*q, r*q, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*r, r*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(r*s, r*s, midlo, midhi) < threshold );

        CHECK(analytic::distance(s*p, s*p, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*q, s*q, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*r, s*r, midlo, midhi) < threshold );
        CHECK(analytic::distance(s*s, s*s, midlo, midhi) < threshold );
    }

    SECTION("p-q must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p-p, p-p, lo, hi) < threshold );
        CHECK(analytic::distance(p-q, p-q, lo, hi) < threshold );
        CHECK(analytic::distance(p-r, p-r, lo, hi) < threshold );
        CHECK(analytic::distance(p-s, p-s, lo, hi) < threshold );

        CHECK(analytic::distance(q-p, q-p, lo, hi) < threshold );
        CHECK(analytic::distance(q-q, q-q, lo, hi) < threshold );
        CHECK(analytic::distance(q-r, q-r, lo, hi) < threshold );
        CHECK(analytic::distance(q-s, q-s, lo, hi) < threshold );

        CHECK(analytic::distance(r-p, r-p, lo, hi) < threshold );
        CHECK(analytic::distance(r-q, r-q, lo, hi) < threshold );
        CHECK(analytic::distance(r-r, r-r, lo, hi) < threshold );
        CHECK(analytic::distance(r-s, r-s, lo, hi) < threshold );

        CHECK(analytic::distance(s-p, s-p, lo, hi) < threshold );
        CHECK(analytic::distance(s-q, s-q, lo, hi) < threshold );
        CHECK(analytic::distance(s-r, s-r, lo, hi) < threshold );
        CHECK(analytic::distance(s-s, s-s, lo, hi) < threshold );
    }
}

TEST_CASE( "Polynomial arithmetic identity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    analytic::Polynomial<double,0,0> zero = analytic::Polynomial<double,0,0>({0.0f});
    analytic::Polynomial<double,0,0> one  = analytic::Polynomial<double,0,0>({1.0f});

    SECTION("p+I must equal p"){
        CHECK(analytic::distance(p+zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p-zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p*one , p, lo, hi) < threshold);
        CHECK(analytic::distance(p-p, zero, lo, hi) < threshold);

        CHECK(analytic::distance(q+zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q-zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q*one , q, lo, hi) < threshold);
        CHECK(analytic::distance(q-q, zero, lo, hi) < threshold);

        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r*one , r, lo, hi) < threshold);
        CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s*one , s, lo, hi) < threshold);
        CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    SECTION("p+q must equal q+p"){
        CHECK(analytic::distance(p+q, q+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+r, r+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+s, s+p, lo, hi) < threshold);

        CHECK(analytic::distance(q+p, p+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+r, r+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+s, s+q, lo, hi) < threshold);

        CHECK(analytic::distance(r+p, p+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+q, q+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+s, s+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+p, p+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+q, q+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+r, r+s, lo, hi) < threshold);
    }
    SECTION("p*q must equal q*p"){
        CHECK(analytic::distance(p*q, q*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*r, r*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*s, s*p, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*p, p*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*r, r*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*s, s*q, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*p, p*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*q, q*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*s, s*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*p, p*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*q, q*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*r, r*s, midlo, midhi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic associativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3e1;
    const double midhi =  3e1;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(analytic::distance((p+q)+r, p+(q+r), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+s, p+(q+s), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+s, q+(r+s), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+p, q+(r+p), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+p, r+(s+p), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+q, r+(s+q), lo, hi) < threshold);
    }
    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(analytic::distance((p*q)*r, p*(q*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((p*q)*s, p*(q*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((q*r)*s, q*(r*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((q*r)*p, q*(r*p), midlo, midhi) < threshold);
        CHECK(analytic::distance((r*s)*p, r*(s*p), midlo, midhi) < threshold);
        CHECK(analytic::distance((r*s)*q, r*(s*q), midlo, midhi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-6;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    SECTION("p+q must equal q+p"){
        CHECK(analytic::distance((p+q)*r, p*r+q*r, midlo, midhi) < threshold);
        CHECK(analytic::distance((p+q)*s, p*s+q*s, midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*q, p*q+r*q, midlo, midhi) < threshold);
        CHECK(analytic::distance((p+r)*s, p*s+r*s, midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*q, p*q+s*q, midlo, midhi) < threshold);
        CHECK(analytic::distance((p+s)*r, p*r+s*r, midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*p, q*p+r*p, midlo, midhi) < threshold);
        CHECK(analytic::distance((q+r)*s, q*s+r*s, midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*p, q*p+s*p, midlo, midhi) < threshold);
        CHECK(analytic::distance((q+s)*r, q*r+s*r, midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*p, r*p+s*p, midlo, midhi) < threshold);
        CHECK(analytic::distance((r+s)*q, r*q+s*q, midlo, midhi) < threshold);
    }
}







TEST_CASE( "Polynomial/scalar arithmetic purity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("p+k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p+k0, p+k0, lo, hi) < threshold);
        CHECK(analytic::distance(p+k1, p+k1, lo, hi) < threshold);
        CHECK(analytic::distance(p+k2, p+k2, lo, hi) < threshold);

        CHECK(analytic::distance(q+k0, q+k0, lo, hi) < threshold);
        CHECK(analytic::distance(q+k1, q+k1, lo, hi) < threshold);
        CHECK(analytic::distance(q+k2, q+k2, lo, hi) < threshold);

        CHECK(analytic::distance(r+k0, r+k0, lo, hi) < threshold);
        CHECK(analytic::distance(r+k1, r+k1, lo, hi) < threshold);
        CHECK(analytic::distance(r+k2, r+k2, lo, hi) < threshold);

        CHECK(analytic::distance(s+k0, s+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s+k1, s+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s+k2, s+k2, lo, hi) < threshold);
    }

    SECTION("p*k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p*k0, p*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*k1, p*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*k2, p*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*k0, q*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*k1, q*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*k2, q*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*k0, r*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k1, r*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k2, r*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*k0, s*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k1, s*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k2, s*k2, midlo, midhi) < threshold);
    }

    SECTION("p-k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p-k0, p-k0, lo, hi) < threshold);
        CHECK(analytic::distance(p-k1, p-k1, lo, hi) < threshold);
        CHECK(analytic::distance(p-k2, p-k2, lo, hi) < threshold);

        CHECK(analytic::distance(q-k0, q-k0, lo, hi) < threshold);
        CHECK(analytic::distance(q-k1, q-k1, lo, hi) < threshold);
        CHECK(analytic::distance(q-k2, q-k2, lo, hi) < threshold);

        CHECK(analytic::distance(r-k0, r-k0, lo, hi) < threshold);
        CHECK(analytic::distance(r-k1, r-k1, lo, hi) < threshold);
        CHECK(analytic::distance(r-k2, r-k2, lo, hi) < threshold);

        CHECK(analytic::distance(s-k0, s-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s-k1, s-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s-k2, s-k2, lo, hi) < threshold);
    }

    SECTION("p/k1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(p/k0, p/k0, lo, hi) < threshold);
        CHECK(analytic::distance(p/k1, p/k1, lo, hi) < threshold);
        CHECK(analytic::distance(p/k2, p/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(q/k0, q/k0, lo, hi) < threshold);
        CHECK(analytic::distance(q/k1, q/k1, lo, hi) < threshold);
        CHECK(analytic::distance(q/k2, q/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(r/k0, r/k0, lo, hi) < threshold);
        CHECK(analytic::distance(r/k1, r/k1, lo, hi) < threshold);
        CHECK(analytic::distance(r/k2, r/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(s/k0, s/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s/k1, s/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s/k2, s/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/scalar arithmetic identity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    double zero(0.0f);
    double one (1.0f);

    SECTION("p+I must equal p"){
        CHECK(analytic::distance(p+zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p-zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p*one , p, lo, hi) < threshold);
        CHECK(analytic::distance(p/one , p, lo, hi) < threshold);
        CHECK(analytic::distance(p-p, zero, lo, hi) < threshold);

        CHECK(analytic::distance(q+zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q-zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q*one , q, lo, hi) < threshold);
        CHECK(analytic::distance(q/one , q, lo, hi) < threshold);
        CHECK(analytic::distance(q-q, zero, lo, hi) < threshold);

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
    }
}

TEST_CASE( "Polynomial/scalar arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("p+k must equal k+p"){
        CHECK(analytic::distance(p+k0, k0+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+k1, k1+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+k2, k2+p, lo, hi) < threshold);

        CHECK(analytic::distance(q+k0, k0+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+k1, k1+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+k2, k2+q, lo, hi) < threshold);

        CHECK(analytic::distance(r+k0, k0+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+k1, k1+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+k2, k2+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+k0, k0+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+k1, k1+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+k2, k2+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(analytic::distance(p*k0, k0*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*k1, k1*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*k2, k2*p, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*k0, k0*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*k1, k1*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*k2, k2*q, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*k0, k0*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k1, k1*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*k2, k2*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*k0, k0*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k1, k1*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*k2, k2*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/scalar arithmetic associativity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(analytic::distance((p+q)+k0, p+(q+k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+k1, p+(q+k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+k2, p+(q+k2), lo, hi) < threshold);

        CHECK(analytic::distance((p+r)+k0, p+(r+k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+k1, p+(r+k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+k2, p+(r+k2), lo, hi) < threshold);

        CHECK(analytic::distance((p+s)+k0, p+(s+k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+k1, p+(s+k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+k2, p+(s+k2), lo, hi) < threshold);

        CHECK(analytic::distance((q+r)+k0, q+(r+k0), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+k1, q+(r+k1), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+k2, q+(r+k2), lo, hi) < threshold);

        CHECK(analytic::distance((q+s)+k0, q+(s+k0), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+k1, q+(s+k1), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+k2, q+(s+k2), lo, hi) < threshold);

        CHECK(analytic::distance((r+s)+k0, r+(s+k0), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+k1, r+(s+k1), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+k2, r+(s+k2), lo, hi) < threshold);
    }

    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(analytic::distance((p*q)*k0, p*(q*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*k1, p*(q*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*k2, p*(q*k2), lo, hi) < threshold);

        CHECK(analytic::distance((p*r)*k0, p*(r*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*k1, p*(r*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*k2, p*(r*k2), lo, hi) < threshold);

        CHECK(analytic::distance((p*s)*k0, p*(s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*k1, p*(s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*k2, p*(s*k2), lo, hi) < threshold);

        CHECK(analytic::distance((q*r)*k0, q*(r*k0), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*k1, q*(r*k1), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*k2, q*(r*k2), lo, hi) < threshold);

        CHECK(analytic::distance((q*s)*k0, q*(s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*k1, q*(s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*k2, q*(s*k2), lo, hi) < threshold);

        CHECK(analytic::distance((r*s)*k0, r*(s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*k1, r*(s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*k2, r*(s*k2), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/scalar arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(analytic::distance((p+q)*k0, (p*k0+q*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)*k1, (p*k1+q*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)*k2, (p*k2+q*k2), lo, hi) < threshold);

        CHECK(analytic::distance((p+r)*k0, (p*k0+r*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)*k1, (p*k1+r*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)*k2, (p*k2+r*k2), lo, hi) < threshold);

        CHECK(analytic::distance((p+s)*k0, (p*k0+s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)*k1, (p*k1+s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)*k2, (p*k2+s*k2), lo, hi) < threshold);

        CHECK(analytic::distance((q+r)*k0, (q*k0+r*k0), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)*k1, (q*k1+r*k1), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)*k2, (q*k2+r*k2), lo, hi) < threshold);

        CHECK(analytic::distance((q+s)*k0, (q*k0+s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)*k1, (q*k1+s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)*k2, (q*k2+s*k2), lo, hi) < threshold);

        CHECK(analytic::distance((r+s)*k0, (r*k0+s*k0), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)*k1, (r*k1+s*k1), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)*k2, (r*k2+s*k2), lo, hi) < threshold);



        CHECK(analytic::distance((p+k0)*q, (p*q+k0*q), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k1)*q, (p*q+k1*q), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k2)*q, (p*q+k2*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+k0)*r, (p*r+k0*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k1)*r, (p*r+k1*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k2)*r, (p*r+k2*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+k0)*s, (p*s+k0*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k1)*s, (p*s+k1*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+k2)*s, (p*s+k2*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+k0)*r, (q*r+k0*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+k1)*r, (q*r+k1*r), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+k2)*r, (q*r+k2*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+k0)*s, (q*s+k0*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+k1)*s, (q*s+k1*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+k2)*s, (q*s+k2*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+k0)*s, (r*s+k0*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k1)*s, (r*s+k1*s), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+k2)*s, (r*s+k2*s), midlo, midhi) < threshold);

    }
}








TEST_CASE( "Polynomial/monomial arithmetic purity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{2.0});

    SECTION("p+m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p+m0, p+m0, lo, hi) < threshold);
        CHECK(analytic::distance(p+m1, p+m1, lo, hi) < threshold);
        CHECK(analytic::distance(p+m2, p+m2, lo, hi) < threshold);

        CHECK(analytic::distance(q+m0, q+m0, lo, hi) < threshold);
        CHECK(analytic::distance(q+m1, q+m1, lo, hi) < threshold);
        CHECK(analytic::distance(q+m2, q+m2, lo, hi) < threshold);

        CHECK(analytic::distance(r+m0, r+m0, lo, hi) < threshold);
        CHECK(analytic::distance(r+m1, r+m1, lo, hi) < threshold);
        CHECK(analytic::distance(r+m2, r+m2, lo, hi) < threshold);

        CHECK(analytic::distance(s+m0, s+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s+m1, s+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s+m2, s+m2, lo, hi) < threshold);
    }

    SECTION("p*m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p*m0, p*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*m1, p*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*m2, p*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*m0, q*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*m1, q*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*m2, q*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*m0, r*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m1, r*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m2, r*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*m0, s*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m1, s*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m2, s*m2, midlo, midhi) < threshold);
    }

    SECTION("p-m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p-m0, p-m0, lo, hi) < threshold);
        CHECK(analytic::distance(p-m1, p-m1, lo, hi) < threshold);
        CHECK(analytic::distance(p-m2, p-m2, lo, hi) < threshold);

        CHECK(analytic::distance(q-m0, q-m0, lo, hi) < threshold);
        CHECK(analytic::distance(q-m1, q-m1, lo, hi) < threshold);
        CHECK(analytic::distance(q-m2, q-m2, lo, hi) < threshold);

        CHECK(analytic::distance(r-m0, r-m0, lo, hi) < threshold);
        CHECK(analytic::distance(r-m1, r-m1, lo, hi) < threshold);
        CHECK(analytic::distance(r-m2, r-m2, lo, hi) < threshold);

        CHECK(analytic::distance(s-m0, s-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s-m1, s-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s-m2, s-m2, lo, hi) < threshold);
    }

    SECTION("p/m1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(p/m0, p/m0, lo, hi) < threshold);
        CHECK(analytic::distance(p/m1, p/m1, lo, hi) < threshold);
        CHECK(analytic::distance(p/m2, p/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(q/m0, q/m0, lo, hi) < threshold);
        CHECK(analytic::distance(q/m1, q/m1, lo, hi) < threshold);
        CHECK(analytic::distance(q/m2, q/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(r/m0, r/m0, lo, hi) < threshold);
        CHECK(analytic::distance(r/m1, r/m1, lo, hi) < threshold);
        CHECK(analytic::distance(r/m2, r/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(s/m0, s/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s/m1, s/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s/m2, s/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/monomial arithmetic identity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Polynomial<double,0,0> zero{0.0f};
    analytic::Polynomial<double,0,0> one {1.0f};

    SECTION("p+I must equal p"){
        CHECK(analytic::distance(p+zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p-zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p*one , p, lo, hi) < threshold);
        CHECK(analytic::distance(p/one , p, lo, hi) < threshold);
        CHECK(analytic::distance(p-p, zero, lo, hi) < threshold);

        CHECK(analytic::distance(q+zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q-zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q*one , q, lo, hi) < threshold);
        CHECK(analytic::distance(q/one , q, lo, hi) < threshold);
        CHECK(analytic::distance(q-q, zero, lo, hi) < threshold);

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
    }
}

TEST_CASE( "Polynomial/monomial arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.f});

    SECTION("p+k must equal k+p"){
        CHECK(analytic::distance(p+m0, m0+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+m1, m1+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+m2, m2+p, lo, hi) < threshold);

        CHECK(analytic::distance(q+m0, m0+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+m1, m1+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+m2, m2+q, lo, hi) < threshold);

        CHECK(analytic::distance(r+m0, m0+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+m1, m1+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+m2, m2+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+m0, m0+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+m1, m1+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+m2, m2+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(analytic::distance(p*m0, m0*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*m1, m1*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*m2, m2*p, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*m0, m0*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*m1, m1*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*m2, m2*q, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*m0, m0*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m1, m1*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*m2, m2*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*m0, m0*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m1, m1*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*m2, m2*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/monomial arithmetic associativity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.f});

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(analytic::distance((p+q)+m0, p+(q+m0), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+m1, p+(q+m1), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+m2, p+(q+m2), lo, hi) < threshold);

        CHECK(analytic::distance((p+r)+m0, p+(r+m0), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+m1, p+(r+m1), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+m2, p+(r+m2), lo, hi) < threshold);

        CHECK(analytic::distance((p+s)+m0, p+(s+m0), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+m1, p+(s+m1), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+m2, p+(s+m2), lo, hi) < threshold);

        CHECK(analytic::distance((q+r)+m0, q+(r+m0), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+m1, q+(r+m1), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+m2, q+(r+m2), lo, hi) < threshold);

        CHECK(analytic::distance((q+s)+m0, q+(s+m0), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+m1, q+(s+m1), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+m2, q+(s+m2), lo, hi) < threshold);

        CHECK(analytic::distance((r+s)+m0, r+(s+m0), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+m1, r+(s+m1), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+m2, r+(s+m2), lo, hi) < threshold);
    }

    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(analytic::distance((p*q)*m0, p*(q*m0), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*m1, p*(q*m1), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*m2, p*(q*m2), lo, hi) < threshold);

        CHECK(analytic::distance((p*r)*m0, p*(r*m0), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*m1, p*(r*m1), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*m2, p*(r*m2), lo, hi) < threshold);

        CHECK(analytic::distance((p*s)*m0, p*(s*m0), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*m1, p*(s*m1), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*m2, p*(s*m2), lo, hi) < threshold);

        CHECK(analytic::distance((q*r)*m0, q*(r*m0), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*m1, q*(r*m1), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*m2, q*(r*m2), lo, hi) < threshold);

        CHECK(analytic::distance((q*s)*m0, q*(s*m0), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*m1, q*(s*m1), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*m2, q*(s*m2), lo, hi) < threshold);

        CHECK(analytic::distance((r*s)*m0, r*(s*m0), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*m1, r*(s*m1), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*m2, r*(s*m2), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/monomial arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-6;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Polynomial<double,0,0> m0(std::array<double,1>{2.0});
    analytic::Polynomial<double,2,2> m1(std::array<double,1>{2.0});
    analytic::Polynomial<double,-2,-2> m2(std::array<double,1>{-2.f});

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(analytic::distance((p+q)*m0, (p*m0+q*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m0)*q, (p*q+m0*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*m1, (p*m1+q*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m1)*q, (p*q+m1*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*m2, (p*m2+q*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m2)*q, (p*q+m2*q), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+r)*m0, (p*m0+r*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m0)*r, (p*r+m0*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*m1, (p*m1+r*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m1)*r, (p*r+m1*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*m2, (p*m2+r*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m2)*r, (p*r+m2*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+s)*m0, (p*m0+s*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m0)*s, (p*s+m0*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*m1, (p*m1+s*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m1)*s, (p*s+m1*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*m2, (p*m2+s*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+m2)*s, (p*s+m2*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+r)*m0, (q*m0+r*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m0)*r, (q*r+m0*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*m1, (q*m1+r*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m1)*r, (q*r+m1*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*m2, (q*m2+r*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m2)*r, (q*r+m2*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+s)*m0, (q*m0+s*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m0)*s, (q*s+m0*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*m1, (q*m1+s*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m1)*s, (q*s+m1*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*m2, (q*m2+s*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+m2)*s, (q*s+m2*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+s)*m0, (r*m0+s*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m0)*s, (r*s+m0*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*m1, (r*m1+s*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m1)*s, (r*s+m1*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*m2, (r*m2+s*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+m2)*s, (r*s+m2*s), midlo, midhi) < threshold);
    }
}










TEST_CASE( "Polynomial/Shifting arithmetic purity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("p+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p+f, p+f, lo, hi) < threshold);
        CHECK(analytic::distance(p+g, p+g, lo, hi) < threshold);
        CHECK(analytic::distance(p+h, p+h, lo, hi) < threshold);

        CHECK(analytic::distance(q+f, q+f, lo, hi) < threshold);
        CHECK(analytic::distance(q+g, q+g, lo, hi) < threshold);
        CHECK(analytic::distance(q+h, q+h, lo, hi) < threshold);

        CHECK(analytic::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, s+h, lo, hi) < threshold);
    }

    SECTION("p*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p*f, p*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*g, p*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*h, p*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*f, q*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*g, q*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*h, q*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, s*h, midlo, midhi) < threshold);
    }

    SECTION("p-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p-f, p-f, lo, hi) < threshold);
        CHECK(analytic::distance(p-g, p-g, lo, hi) < threshold);
        CHECK(analytic::distance(p-h, p-h, lo, hi) < threshold);

        CHECK(analytic::distance(q-f, q-f, lo, hi) < threshold);
        CHECK(analytic::distance(q-g, q-g, lo, hi) < threshold);
        CHECK(analytic::distance(q-h, q-h, lo, hi) < threshold);

        CHECK(analytic::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(analytic::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(analytic::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(analytic::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(analytic::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(analytic::distance(s-h, s-h, lo, hi) < threshold);
    }

    // SECTION("p/g must be called repeatedly without changing the output"){
    //     // CHECK(analytic::distance(p/f, p/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(p/g, p/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(p/h, p/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(q/f, q/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(q/g, q/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(q/h, q/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(r/f, r/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(r/g, r/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(r/h, r/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(s/f, s/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s/g, s/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s/h, s/h, lo, hi) < threshold);
    // }
}

TEST_CASE( "Polynomial/Shifting arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("p+k must equal k+p"){
        CHECK(analytic::distance(p+f, f+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+g, g+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+h, h+p, lo, hi) < threshold);

        CHECK(analytic::distance(q+f, f+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+g, g+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+h, h+q, lo, hi) < threshold);

        CHECK(analytic::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, h+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(analytic::distance(p*f, f*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*g, g*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*h, h*p, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*f, f*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*g, g*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*h, h*q, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, h*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(analytic::distance((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(analytic::distance((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(analytic::distance((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(analytic::distance((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(analytic::distance((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(analytic::distance((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), lo, hi) < threshold);
    }

    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(analytic::distance((p*q)*f, p*(q*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*g, p*(q*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*h, p*(q*h), lo, hi) < threshold);

        CHECK(analytic::distance((p*r)*f, p*(r*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*g, p*(r*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*h, p*(r*h), lo, hi) < threshold);

        CHECK(analytic::distance((p*s)*f, p*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*g, p*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*h, p*(s*h), lo, hi) < threshold);

        CHECK(analytic::distance((q*r)*f, q*(r*f), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*g, q*(r*g), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*h, q*(r*h), lo, hi) < threshold);

        CHECK(analytic::distance((q*s)*f, q*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*g, q*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*h, q*(s*h), lo, hi) < threshold);

        CHECK(analytic::distance((r*s)*f, r*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*g, r*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*h, r*(s*h), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-6;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(analytic::distance((p+q)*f, (p*f+q*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*q, (p*q+f*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*g, (p*g+q*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*q, (p*q+g*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*h, (p*h+q*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*q, (p*q+h*q), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+r)*f, (p*f+r*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*r, (p*r+f*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*g, (p*g+r*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*r, (p*r+g*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*h, (p*h+r*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*r, (p*r+h*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+s)*f, (p*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*s, (p*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*g, (p*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*s, (p*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*h, (p*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*s, (p*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+r)*f, (q*f+r*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+f)*r, (q*r+f*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*g, (q*g+r*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+g)*r, (q*r+g*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*h, (q*h+r*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+h)*r, (q*r+h*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+s)*f, (q*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+f)*s, (q*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*g, (q*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+g)*s, (q*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*h, (q*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+h)*s, (q*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);
    }
}











TEST_CASE( "Polynomial/Scaling arithmetic purity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("p+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p+f, p+f, lo, hi) < threshold);
        CHECK(analytic::distance(p+g, p+g, lo, hi) < threshold);
        CHECK(analytic::distance(p+h, p+h, lo, hi) < threshold);

        CHECK(analytic::distance(q+f, q+f, lo, hi) < threshold);
        CHECK(analytic::distance(q+g, q+g, lo, hi) < threshold);
        CHECK(analytic::distance(q+h, q+h, lo, hi) < threshold);

        CHECK(analytic::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, s+h, lo, hi) < threshold);
    }

    SECTION("p*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p*f, p*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*g, p*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*h, p*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*f, q*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*g, q*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*h, q*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, s*h, midlo, midhi) < threshold);
    }

    SECTION("p-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p-f, p-f, lo, hi) < threshold);
        CHECK(analytic::distance(p-g, p-g, lo, hi) < threshold);
        CHECK(analytic::distance(p-h, p-h, lo, hi) < threshold);

        CHECK(analytic::distance(q-f, q-f, lo, hi) < threshold);
        CHECK(analytic::distance(q-g, q-g, lo, hi) < threshold);
        CHECK(analytic::distance(q-h, q-h, lo, hi) < threshold);

        CHECK(analytic::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(analytic::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(analytic::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(analytic::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(analytic::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(analytic::distance(s-h, s-h, lo, hi) < threshold);
    }

    SECTION("p/g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(p/f, p/f, lo, hi) < threshold);
        CHECK(analytic::distance(p/g, p/g, lo, hi) < threshold);
        // CHECK(analytic::distance(p/h, p/h, lo, hi) < threshold);

        CHECK(analytic::distance(q/f, q/f, lo, hi) < threshold);
        CHECK(analytic::distance(q/g, q/g, lo, hi) < threshold);
        // CHECK(analytic::distance(q/h, q/h, lo, hi) < threshold);

        CHECK(analytic::distance(r/f, r/f, lo, hi) < threshold);
        CHECK(analytic::distance(r/g, r/g, lo, hi) < threshold);
        // CHECK(analytic::distance(r/h, r/h, lo, hi) < threshold);

        CHECK(analytic::distance(s/f, s/f, lo, hi) < threshold);
        CHECK(analytic::distance(s/g, s/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s/h, s/h, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/Scaling arithmetic identity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Scaling<double> zero(0.0f);

    SECTION("p+I must equal p"){
        CHECK(analytic::distance(p+zero, p, lo, hi) < threshold);
        CHECK(analytic::distance(p-zero, p, lo, hi) < threshold);
        // CHECK(analytic::distance(p-p, zero, lo, hi) < threshold);

        CHECK(analytic::distance(q+zero, q, lo, hi) < threshold);
        CHECK(analytic::distance(q-zero, q, lo, hi) < threshold);
        // CHECK(analytic::distance(q-q, zero, lo, hi) < threshold);

        CHECK(analytic::distance(r+zero, r, lo, hi) < threshold);
        CHECK(analytic::distance(r-zero, r, lo, hi) < threshold);
        // CHECK(analytic::distance(r-r, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s+zero, s, lo, hi) < threshold);
        CHECK(analytic::distance(s-zero, s, lo, hi) < threshold);
        // CHECK(analytic::distance(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/Scaling arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("p+k must equal k+p"){
        CHECK(analytic::distance(p+f, f+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+g, g+p, lo, hi) < threshold);
        CHECK(analytic::distance(p+h, h+p, lo, hi) < threshold);

        CHECK(analytic::distance(q+f, f+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+g, g+q, lo, hi) < threshold);
        CHECK(analytic::distance(q+h, h+q, lo, hi) < threshold);

        CHECK(analytic::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(analytic::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(analytic::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(analytic::distance(s+h, h+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(analytic::distance(p*f, f*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*g, g*p, midlo, midhi) < threshold);
        CHECK(analytic::distance(p*h, h*p, midlo, midhi) < threshold);

        CHECK(analytic::distance(q*f, f*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*g, g*q, midlo, midhi) < threshold);
        CHECK(analytic::distance(q*h, h*q, midlo, midhi) < threshold);

        CHECK(analytic::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(analytic::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(analytic::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(analytic::distance(s*h, h*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = 1e-6;
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(analytic::distance((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(analytic::distance((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(analytic::distance((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(analytic::distance((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(analytic::distance((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(analytic::distance((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(analytic::distance((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(analytic::distance((r+s)+h, r+(s+h), lo, hi) < threshold);
    }

    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(analytic::distance((p*q)*f, p*(q*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*g, p*(q*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*q)*h, p*(q*h), lo, hi) < threshold);

        CHECK(analytic::distance((p*r)*f, p*(r*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*g, p*(r*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*r)*h, p*(r*h), lo, hi) < threshold);

        CHECK(analytic::distance((p*s)*f, p*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*g, p*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((p*s)*h, p*(s*h), lo, hi) < threshold);

        CHECK(analytic::distance((q*r)*f, q*(r*f), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*g, q*(r*g), lo, hi) < threshold);
        CHECK(analytic::distance((q*r)*h, q*(r*h), lo, hi) < threshold);

        CHECK(analytic::distance((q*s)*f, q*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*g, q*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((q*s)*h, q*(s*h), lo, hi) < threshold);

        CHECK(analytic::distance((r*s)*f, r*(s*f), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*g, r*(s*g), lo, hi) < threshold);
        CHECK(analytic::distance((r*s)*h, r*(s*h), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-6;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(analytic::distance((p+q)*f, (p*f+q*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*q, (p*q+f*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*g, (p*g+q*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*q, (p*q+g*q), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+q)*h, (p*h+q*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*q, (p*q+h*q), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+r)*f, (p*f+r*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*r, (p*r+f*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*g, (p*g+r*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*r, (p*r+g*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+r)*h, (p*h+r*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*r, (p*r+h*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((p+s)*f, (p*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+f)*s, (p*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*g, (p*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+g)*s, (p*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((p+s)*h, (p*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((p+h)*s, (p*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+r)*f, (q*f+r*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+f)*r, (q*r+f*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*g, (q*g+r*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+g)*r, (q*r+g*r), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+r)*h, (q*h+r*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+h)*r, (q*r+h*r), midlo, midhi) < threshold);


        CHECK(analytic::distance((q+s)*f, (q*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+f)*s, (q*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*g, (q*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+g)*s, (q*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((q+s)*h, (q*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((q+h)*s, (q*s+h*s), midlo, midhi) < threshold);


        CHECK(analytic::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(analytic::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);
    }
}

TEST_CASE( "Polynomial integral/derivative invertibility", "[math]" ) {
    const double threshold = 1e-6;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    
    analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({1.0f,2.0f,3.0f,4.0f,5.0f});
    analytic::Polynomial<double,0,4> q = analytic::Polynomial<double,0,4>({-1.0f,0.0f,1.0f,2.0f,3.0f});
    // analytic::Polynomial<double,-2,2> r = analytic::Polynomial<double,-2,2>({1.0f,2.0f,3.0f,4.0f,5.0f});
    // analytic::Polynomial<double,-2,2> s = analytic::Polynomial<double,-2,2>({-1.0f,1.0f,-2.0f,2.0f,3.0f});

    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(analytic::distance(p, derivative(integral(p)), lo, hi) < threshold);
        CHECK(analytic::distance(q, derivative(integral(q)), lo, hi) < threshold);
        // CHECK(analytic::distance(r, derivative(integral(r)), lo, hi) < threshold);
        // CHECK(analytic::distance(s, derivative(integral(s)), lo, hi) < threshold);
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

    using P0 = analytic::Polynomial<double,0,2>;
    // using P1 = analytic::Polynomial<double,-2,1>;
    // using P2 = analytic::Polynomial<double,-2,2>;
    using P3 = analytic::Polynomial<double,0,1>;

    analytic::Identity<double> E;
    analytic::Scaling<double>  F(s);
    analytic::Shifting<double> G(s);
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
    analytic::Polynomial<double,0,2> PQ = analytic::Polynomial<double,0,2>(std::array<double,3>{p+q*s+r*s*s, q*t+r*2.0*s*t, r*t*t});

    SECTION("the derivative of s function's integral must equal the original function"){
        CHECK(analytic::distance(analytic::compose(P,E), P, lo, hi) < threshold);
        CHECK(analytic::distance(analytic::compose(P,F), PF, lo, hi) < threshold);
        CHECK(analytic::distance(analytic::compose(P,G), PG, lo, hi) < threshold);
        CHECK(analytic::distance(analytic::compose(P,Q), PQ, lo, hi) < threshold);
    }
}

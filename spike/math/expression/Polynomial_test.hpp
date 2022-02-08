#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/expression/Polynomial.hpp>  


TEST_CASE( "Polynomial arithmetic purity", "[math]" ) {
    const float threshold = 1e-7;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};


    SECTION("p+q must be called repeatedly without changing the output"){
        CHECK(!std::isnan(p(hi)));
        CHECK(!std::isnan(p(lo)));
        CHECK(!std::isnan(integral(p, lo, hi)));
        CHECK(!std::isnan(math::mean_square_difference(p, p, lo, hi)) );
        CHECK(!std::isnan(math::mean_square_difference((r+2.0f)*s, (r*s+2.0f*s), lo, hi)) );

        CHECK(math::mean_square_difference(p+p, p+p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p+q, p+q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p+r, p+r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p+s, p+s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(q+p, q+p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q+q, q+q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q+r, q+r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q+s, q+s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(r+p, r+p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r+q, r+q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r+r, r+r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r+s, r+s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(s+p, s+p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s+q, s+q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s+r, s+r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s+s, s+s, lo, hi) < threshold );
    }

    SECTION("p*q must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p*p, p*p, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(p*q, p*q, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(p*r, p*r, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(p*s, p*s, midlo, midhi) < threshold );

        CHECK(math::mean_square_difference(q*p, q*p, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(q*q, q*q, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(q*r, q*r, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(q*s, q*s, midlo, midhi) < threshold );

        CHECK(math::mean_square_difference(r*p, r*p, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(r*q, r*q, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(r*r, r*r, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(r*s, r*s, midlo, midhi) < threshold );

        CHECK(math::mean_square_difference(s*p, s*p, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(s*q, s*q, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(s*r, s*r, midlo, midhi) < threshold );
        CHECK(math::mean_square_difference(s*s, s*s, midlo, midhi) < threshold );
    }

    SECTION("p-q must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p-p, p-p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p-q, p-q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p-r, p-r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(p-s, p-s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(q-p, q-p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q-q, q-q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q-r, q-r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(q-s, q-s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(r-p, r-p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r-q, r-q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r-r, r-r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(r-s, r-s, lo, hi) < threshold );

        CHECK(math::mean_square_difference(s-p, s-p, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s-q, s-q, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s-r, s-r, lo, hi) < threshold );
        CHECK(math::mean_square_difference(s-s, s-s, lo, hi) < threshold );
    }
}

TEST_CASE( "Polynomial arithmetic identity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    math::Polynomial<0,0> zero = math::Polynomial<0,0>{0.0f};
    math::Polynomial<0,0> one  = math::Polynomial<0,0>{1.0f};

    SECTION("p+I must equal p"){
        CHECK(math::mean_square_difference(p+zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p*one , p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-p, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q*one , q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-q, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r*one , r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-r, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s*one , s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic commutativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("p+q must equal q+p"){
        CHECK(math::mean_square_difference(p+q, q+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+r, r+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+s, s+p, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+p, p+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+r, r+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+s, s+q, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+p, p+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+q, q+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+s, s+r, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+p, p+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+q, q+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+r, r+s, lo, hi) < threshold);
    }
    SECTION("p*q must equal q*p"){
        CHECK(math::mean_square_difference(p*q, q*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*r, r*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*s, s*p, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*p, p*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*r, r*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*s, s*q, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*p, p*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*q, q*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*s, s*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*p, p*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*q, q*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*r, r*s, midlo, midhi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic associativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -3e1;
    const float midhi =  3e1;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+r, p+(q+r), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+s, p+(q+s), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+s, q+(r+s), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+p, q+(r+p), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+p, r+(s+p), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+q, r+(s+q), lo, hi) < threshold);
    }
    SECTION("(p*q)*r must equal p*(q*r)"){
        CHECK(math::mean_square_difference((p*q)*r, p*(q*r), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p*q)*s, p*(q*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q*r)*s, q*(r*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q*r)*p, q*(r*p), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r*s)*p, r*(s*p), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r*s)*q, r*(s*q), midlo, midhi) < threshold);
    }
}

TEST_CASE( "Polynomial arithmetic distributivity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("p+q must equal q+p"){
        CHECK(math::mean_square_difference((p+q)*r, p*r+q*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+q)*s, p*s+q*s, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*q, p*q+r*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+r)*s, p*s+r*s, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*q, p*q+s*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+s)*r, p*r+s*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*p, q*p+r*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+r)*s, q*s+r*s, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*p, q*p+s*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+s)*r, q*r+s*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*p, r*p+s*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+s)*q, r*q+s*q, midlo, midhi) < threshold);
    }
}







TEST_CASE( "Polynomial/scalar arithmetic purity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("p+k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p+k0, p+k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+k1, p+k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+k2, p+k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+k0, q+k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+k1, q+k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+k2, q+k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+k0, r+k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+k1, r+k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+k2, r+k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+k0, s+k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+k1, s+k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+k2, s+k2, lo, hi) < threshold);
    }

    SECTION("p*k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p*k0, p*k0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*k1, p*k1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*k2, p*k2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*k0, q*k0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*k1, q*k1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*k2, q*k2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*k0, r*k0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*k1, r*k1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*k2, r*k2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*k0, s*k0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*k1, s*k1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*k2, s*k2, midlo, midhi) < threshold);
    }

    SECTION("p-k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p-k0, p-k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-k1, p-k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-k2, p-k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q-k0, q-k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-k1, q-k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-k2, q-k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r-k0, r-k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-k1, r-k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-k2, r-k2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s-k0, s-k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-k1, s-k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-k2, s-k2, lo, hi) < threshold);
    }

    SECTION("p/k1 must be called repeatedly without changing the output"){
        // CHECK(math::mean_square_difference(p/k0, p/k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/k1, p/k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/k2, p/k2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(q/k0, q/k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/k1, q/k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/k2, q/k2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(r/k0, r/k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/k1, r/k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/k2, r/k2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(s/k0, s/k0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/k1, s/k1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/k2, s/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/scalar arithmetic identity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float zero(0.0f);
    float one (1.0f);

    SECTION("p+I must equal p"){
        CHECK(math::mean_square_difference(p+zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p*one , p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/one , p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-p, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q*one , q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/one , q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-q, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r*one , r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/one , r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-r, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s*one , s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/one , s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/scalar arithmetic commutativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("p+k must equal k+p"){
        CHECK(math::mean_square_difference(p+k0, k0+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+k1, k1+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+k2, k2+p, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+k0, k0+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+k1, k1+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+k2, k2+q, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+k0, k0+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+k1, k1+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+k2, k2+r, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+k0, k0+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+k1, k1+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+k2, k2+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(math::mean_square_difference(p*k0, k0*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*k1, k1*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*k2, k2*p, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*k0, k0*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*k1, k1*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*k2, k2*q, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*k0, k0*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*k1, k1*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*k2, k2*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*k0, k0*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*k1, k1*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*k2, k2*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/scalar arithmetic associativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+k0, p+(q+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+k1, p+(q+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+k2, p+(q+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+k0, p+(r+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+k1, p+(r+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+k2, p+(r+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+k0, p+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+k1, p+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+k2, p+(s+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+k0, q+(r+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+k1, q+(r+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+k2, q+(r+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+k0, q+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+k1, q+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+k2, q+(s+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+k0, r+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+k1, r+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+k2, r+(s+k2), lo, hi) < threshold);
    }

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+k0, p+(q+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+k1, p+(q+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+k2, p+(q+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+k0, p+(r+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+k1, p+(r+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+k2, p+(r+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+k0, p+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+k1, p+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+k2, p+(s+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+k0, q+(r+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+k1, q+(r+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+k2, q+(r+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+k0, q+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+k1, q+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+k2, q+(s+k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+k0, r+(s+k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+k1, r+(s+k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+k2, r+(s+k2), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/scalar arithmetic distributivity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;

    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(math::mean_square_difference((p+q)*k0, (p*k0+q*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)*k1, (p*k1+q*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)*k2, (p*k2+q*k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)*k0, (p*k0+r*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)*k1, (p*k1+r*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)*k2, (p*k2+r*k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)*k0, (p*k0+s*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)*k1, (p*k1+s*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)*k2, (p*k2+s*k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)*k0, (q*k0+r*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)*k1, (q*k1+r*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)*k2, (q*k2+r*k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)*k0, (q*k0+s*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)*k1, (q*k1+s*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)*k2, (q*k2+s*k2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)*k0, (r*k0+s*k0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)*k1, (r*k1+s*k1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)*k2, (r*k2+s*k2), lo, hi) < threshold);



        CHECK(math::mean_square_difference((p+k0)*q, (p*q+k0*q), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k1)*q, (p*q+k1*q), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k2)*q, (p*q+k2*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+k0)*r, (p*r+k0*r), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k1)*r, (p*r+k1*r), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k2)*r, (p*r+k2*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+k0)*s, (p*s+k0*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k1)*s, (p*s+k1*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+k2)*s, (p*s+k2*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+k0)*r, (q*r+k0*r), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+k1)*r, (q*r+k1*r), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+k2)*r, (q*r+k2*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+k0)*s, (q*s+k0*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+k1)*s, (q*s+k1*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+k2)*s, (q*s+k2*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+k0)*s, (r*s+k0*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+k1)*s, (r*s+k1*s), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+k2)*s, (r*s+k2*s), midlo, midhi) < threshold);

    }
}








TEST_CASE( "Polynomial/monomial arithmetic purity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Polynomial<0,0> m0{2.0f};
    math::Polynomial<2,2> m1{2.0f};
    math::Polynomial<-2,-2> m2{2.0f};

    SECTION("p+m1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p+m0, p+m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+m1, p+m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+m2, p+m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+m0, q+m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+m1, q+m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+m2, q+m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+m0, r+m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+m1, r+m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+m2, r+m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+m0, s+m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+m1, s+m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+m2, s+m2, lo, hi) < threshold);
    }

    SECTION("p*m1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p*m0, p*m0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*m1, p*m1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*m2, p*m2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*m0, q*m0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*m1, q*m1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*m2, q*m2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*m0, r*m0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*m1, r*m1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*m2, r*m2, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*m0, s*m0, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*m1, s*m1, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*m2, s*m2, midlo, midhi) < threshold);
    }

    SECTION("p-m1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p-m0, p-m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-m1, p-m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-m2, p-m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q-m0, q-m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-m1, q-m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-m2, q-m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r-m0, r-m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-m1, r-m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-m2, r-m2, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s-m0, s-m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-m1, s-m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-m2, s-m2, lo, hi) < threshold);
    }

    SECTION("p/m1 must be called repeatedly without changing the output"){
        // CHECK(math::mean_square_difference(p/m0, p/m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/m1, p/m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/m2, p/m2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(q/m0, q/m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/m1, q/m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/m2, q/m2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(r/m0, r/m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/m1, r/m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/m2, r/m2, lo, hi) < threshold);

        // CHECK(math::mean_square_difference(s/m0, s/m0, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/m1, s/m1, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/m2, s/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/monomial arithmetic identity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Polynomial<0,0> zero{0.0f};
    math::Polynomial<0,0> one {1.0f};

    SECTION("p+I must equal p"){
        CHECK(math::mean_square_difference(p+zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p*one , p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/one , p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-p, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q*one , q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/one , q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-q, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r*one , r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/one , r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-r, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s*one , s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/one , s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/monomial arithmetic commutativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Polynomial<0,0> m0{2.0f};
    math::Polynomial<2,2> m1{2.0f};
    math::Polynomial<-2,-2> m2{-2.0f};

    SECTION("p+k must equal k+p"){
        CHECK(math::mean_square_difference(p+m0, m0+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+m1, m1+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+m2, m2+p, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+m0, m0+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+m1, m1+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+m2, m2+q, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+m0, m0+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+m1, m1+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+m2, m2+r, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+m0, m0+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+m1, m1+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+m2, m2+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(math::mean_square_difference(p*m0, m0*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*m1, m1*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*m2, m2*p, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*m0, m0*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*m1, m1*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*m2, m2*q, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*m0, m0*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*m1, m1*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*m2, m2*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*m0, m0*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*m1, m1*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*m2, m2*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/monomial arithmetic associativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Polynomial<0,0> m0{2.0f};
    math::Polynomial<2,2> m1{2.0f};
    math::Polynomial<-2,-2> m2{-2.0f};

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+m0, p+(q+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+m1, p+(q+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+m2, p+(q+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+m0, p+(r+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+m1, p+(r+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+m2, p+(r+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+m0, p+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+m1, p+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+m2, p+(s+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+m0, q+(r+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+m1, q+(r+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+m2, q+(r+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+m0, q+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+m1, q+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+m2, q+(s+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+m0, r+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+m1, r+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+m2, r+(s+m2), lo, hi) < threshold);
    }

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+m0, p+(q+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+m1, p+(q+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+m2, p+(q+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+m0, p+(r+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+m1, p+(r+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+m2, p+(r+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+m0, p+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+m1, p+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+m2, p+(s+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+m0, q+(r+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+m1, q+(r+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+m2, q+(r+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+m0, q+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+m1, q+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+m2, q+(s+m2), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+m0, r+(s+m0), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+m1, r+(s+m1), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+m2, r+(s+m2), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/monomial arithmetic distributivity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Polynomial<0,0> m0{2.0f};
    math::Polynomial<2,2> m1{2.0f};
    math::Polynomial<-2,-2> m2{-2.0f};

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(math::mean_square_difference((p+q)*m0, (p*m0+q*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m0)*q, (p*q+m0*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*m1, (p*m1+q*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m1)*q, (p*q+m1*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*m2, (p*m2+q*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m2)*q, (p*q+m2*q), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+r)*m0, (p*m0+r*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m0)*r, (p*r+m0*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*m1, (p*m1+r*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m1)*r, (p*r+m1*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*m2, (p*m2+r*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m2)*r, (p*r+m2*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+s)*m0, (p*m0+s*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m0)*s, (p*s+m0*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*m1, (p*m1+s*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m1)*s, (p*s+m1*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*m2, (p*m2+s*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+m2)*s, (p*s+m2*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+r)*m0, (q*m0+r*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m0)*r, (q*r+m0*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*m1, (q*m1+r*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m1)*r, (q*r+m1*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*m2, (q*m2+r*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m2)*r, (q*r+m2*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+s)*m0, (q*m0+s*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m0)*s, (q*s+m0*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*m1, (q*m1+s*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m1)*s, (q*s+m1*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*m2, (q*m2+s*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+m2)*s, (q*s+m2*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((r+s)*m0, (r*m0+s*m0), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+m0)*s, (r*s+m0*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*m1, (r*m1+s*m1), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+m1)*s, (r*s+m1*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*m2, (r*m2+s*m2), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+m2)*s, (r*s+m2*s), midlo, midhi) < threshold);
    }
}










TEST_CASE( "Polynomial/Shifting arithmetic purity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting f(2.0f);
    math::Shifting g(-2.0f);
    math::Shifting h(0.0f);

    SECTION("p+g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p+f, p+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+g, p+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+h, p+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+f, q+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+g, q+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+h, q+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+f, r+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+g, r+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+h, r+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+f, s+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+g, s+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+h, s+h, lo, hi) < threshold);
    }

    SECTION("p*g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p*f, p*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*g, p*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*h, p*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*f, q*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*g, q*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*h, q*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*f, r*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*g, r*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*h, r*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*f, s*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*g, s*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*h, s*h, midlo, midhi) < threshold);
    }

    SECTION("p-g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p-f, p-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-g, p-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-h, p-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q-f, q-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-g, q-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-h, q-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r-f, r-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-g, r-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-h, r-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s-f, s-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-g, s-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-h, s-h, lo, hi) < threshold);
    }

    // SECTION("p/g must be called repeatedly without changing the output"){
    //     // CHECK(math::mean_square_difference(p/f, p/f, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(p/g, p/g, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(p/h, p/h, lo, hi) < threshold);

    //     // CHECK(math::mean_square_difference(q/f, q/f, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(q/g, q/g, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(q/h, q/h, lo, hi) < threshold);

    //     // CHECK(math::mean_square_difference(r/f, r/f, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(r/g, r/g, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(r/h, r/h, lo, hi) < threshold);

    //     // CHECK(math::mean_square_difference(s/f, s/f, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(s/g, s/g, lo, hi) < threshold);
    //     CHECK(math::mean_square_difference(s/h, s/h, lo, hi) < threshold);
    // }
}

TEST_CASE( "Polynomial/Shifting arithmetic identity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting zero(0.0f);

    SECTION("p+I must equal p"){
        CHECK(math::mean_square_difference(p+zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-zero, p, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(p-p, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-zero, q, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(q-q, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-zero, r, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(r-r, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-zero, s, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/Shifting arithmetic commutativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting f(2.0f);
    math::Shifting g(-2.0f);
    math::Shifting h(0.0f);

    SECTION("p+k must equal k+p"){
        CHECK(math::mean_square_difference(p+f, f+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+g, g+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+h, h+p, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+f, f+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+g, g+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+h, h+q, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+f, f+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+g, g+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+h, h+r, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+f, f+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+g, g+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+h, h+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(math::mean_square_difference(p*f, f*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*g, g*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*h, h*p, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*f, f*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*g, g*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*h, h*q, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*f, f*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*g, g*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*h, h*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*f, f*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*g, g*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*h, h*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/Shifting arithmetic associativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting f(2.0f);
    math::Shifting g(-2.0f);
    math::Shifting h(0.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+h, r+(s+h), lo, hi) < threshold);
    }

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+h, r+(s+h), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/Shifting arithmetic distributivity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting f(2.0f);
    math::Shifting g(-2.0f);
    math::Shifting h(0.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(math::mean_square_difference((p+q)*f, (p*f+q*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*q, (p*q+f*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*g, (p*g+q*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*q, (p*q+g*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*h, (p*h+q*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*q, (p*q+h*q), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+r)*f, (p*f+r*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*r, (p*r+f*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*g, (p*g+r*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*r, (p*r+g*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*h, (p*h+r*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*r, (p*r+h*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+s)*f, (p*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*s, (p*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*g, (p*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*s, (p*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*h, (p*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*s, (p*s+h*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+r)*f, (q*f+r*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+f)*r, (q*r+f*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*g, (q*g+r*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+g)*r, (q*r+g*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*h, (q*h+r*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+h)*r, (q*r+h*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+s)*f, (q*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+f)*s, (q*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*g, (q*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+g)*s, (q*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*h, (q*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+h)*s, (q*s+h*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);
    }
}











TEST_CASE( "Polynomial/Scaling arithmetic purity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Scaling f(2.0f);
    math::Scaling g(-2.0f);
    math::Scaling h(0.0f);

    SECTION("p+g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p+f, p+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+g, p+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+h, p+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+f, q+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+g, q+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+h, q+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+f, r+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+g, r+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+h, r+h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+f, s+f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+g, s+g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+h, s+h, lo, hi) < threshold);
    }

    SECTION("p*g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p*f, p*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*g, p*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*h, p*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*f, q*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*g, q*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*h, q*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*f, r*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*g, r*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*h, r*h, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*f, s*f, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*g, s*g, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*h, s*h, midlo, midhi) < threshold);
    }

    SECTION("p-g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p-f, p-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-g, p-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-h, p-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q-f, q-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-g, q-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-h, q-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r-f, r-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-g, r-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-h, r-h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s-f, s-f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-g, s-g, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-h, s-h, lo, hi) < threshold);
    }

    SECTION("p/g must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(p/f, p/f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p/g, p/g, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(p/h, p/h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q/f, q/f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q/g, q/g, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(q/h, q/h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r/f, r/f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r/g, r/g, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(r/h, r/h, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s/f, s/f, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s/g, s/g, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(s/h, s/h, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/Scaling arithmetic identity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Shifting zero(0.0f);

    SECTION("p+I must equal p"){
        CHECK(math::mean_square_difference(p+zero, p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p-zero, p, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(p-p, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+zero, q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q-zero, q, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(q-q, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+zero, r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r-zero, r, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(r-r, zero, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+zero, s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s-zero, s, lo, hi) < threshold);
        // CHECK(math::mean_square_difference(s-s, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Polynomial/Scaling arithmetic commutativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Scaling f(2.0f);
    math::Scaling g(-2.0f);
    math::Scaling h(0.0f);

    SECTION("p+k must equal k+p"){
        CHECK(math::mean_square_difference(p+f, f+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+g, g+p, lo, hi) < threshold);
        CHECK(math::mean_square_difference(p+h, h+p, lo, hi) < threshold);

        CHECK(math::mean_square_difference(q+f, f+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+g, g+q, lo, hi) < threshold);
        CHECK(math::mean_square_difference(q+h, h+q, lo, hi) < threshold);

        CHECK(math::mean_square_difference(r+f, f+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+g, g+r, lo, hi) < threshold);
        CHECK(math::mean_square_difference(r+h, h+r, lo, hi) < threshold);

        CHECK(math::mean_square_difference(s+f, f+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+g, g+s, lo, hi) < threshold);
        CHECK(math::mean_square_difference(s+h, h+s, lo, hi) < threshold);
    }

    SECTION("p*k must equal k*p"){
        CHECK(math::mean_square_difference(p*f, f*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*g, g*p, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(p*h, h*p, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(q*f, f*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*g, g*q, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(q*h, h*q, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(r*f, f*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*g, g*r, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(r*h, h*r, midlo, midhi) < threshold);

        CHECK(math::mean_square_difference(s*f, f*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*g, g*s, midlo, midhi) < threshold);
        CHECK(math::mean_square_difference(s*h, h*s, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Polynomial/Scaling arithmetic associativity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Scaling f(2.0f);
    math::Scaling g(-2.0f);
    math::Scaling h(0.0f);

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+h, r+(s+h), lo, hi) < threshold);
    }

    SECTION("(p+q)+r must equal p+(q+r)"){
        CHECK(math::mean_square_difference((p+q)+f, p+(q+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+g, p+(q+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+q)+h, p+(q+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+r)+f, p+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+g, p+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+r)+h, p+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((p+s)+f, p+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+g, p+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((p+s)+h, p+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+r)+f, q+(r+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+g, q+(r+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+r)+h, q+(r+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((q+s)+f, q+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+g, q+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((q+s)+h, q+(s+h), lo, hi) < threshold);

        CHECK(math::mean_square_difference((r+s)+f, r+(s+f), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+g, r+(s+g), lo, hi) < threshold);
        CHECK(math::mean_square_difference((r+s)+h, r+(s+h), lo, hi) < threshold);
    }
}


TEST_CASE( "Polynomial/Scaling arithmetic distributivity", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const float midlo = -1e2;
    const float midhi =  1e2;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    math::Scaling f(2.0f);
    math::Scaling g(-2.0f);
    math::Scaling h(0.0f);

    SECTION("(p+q)*k must equal p*k + q*k"){
        CHECK(math::mean_square_difference((p+q)*f, (p*f+q*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*q, (p*q+f*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*g, (p*g+q*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*q, (p*q+g*q), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+q)*h, (p*h+q*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*q, (p*q+h*q), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+r)*f, (p*f+r*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*r, (p*r+f*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*g, (p*g+r*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*r, (p*r+g*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+r)*h, (p*h+r*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*r, (p*r+h*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((p+s)*f, (p*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+f)*s, (p*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*g, (p*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+g)*s, (p*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((p+s)*h, (p*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((p+h)*s, (p*s+h*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+r)*f, (q*f+r*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+f)*r, (q*r+f*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*g, (q*g+r*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+g)*r, (q*r+g*r), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+r)*h, (q*h+r*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+h)*r, (q*r+h*r), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((q+s)*f, (q*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+f)*s, (q*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*g, (q*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+g)*s, (q*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((q+s)*h, (q*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((q+h)*s, (q*s+h*s), midlo, midhi) < threshold);


        CHECK(math::mean_square_difference((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(math::mean_square_difference((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(math::mean_square_difference((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);
    }
}







TEST_CASE( "Polynomial integral/derivative invertibility", "[math]" ) {
    const float threshold = std::numeric_limits<float>::epsilon();
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const float lo = -1e3;
    const float hi =  1e3;
    
    math::Polynomial<0,4> p = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> q = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    // math::Polynomial<-2,2> r = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    // math::Polynomial<-2,2> s = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(math::mean_square_difference(p, derivative(integral(p)), lo, hi) < threshold);
        CHECK(math::mean_square_difference(q, derivative(integral(q)), lo, hi) < threshold);
        // CHECK(math::mean_square_difference(r, derivative(integral(r)), lo, hi) < threshold);
        // CHECK(math::mean_square_difference(s, derivative(integral(s)), lo, hi) < threshold);
    }
}
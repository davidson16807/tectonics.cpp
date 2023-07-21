#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "PolynomialTrain.hpp"

TEST_CASE( "PolynomialTrain arithmetic purity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});

    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    SECTION("t0+t1 must be called repeatedly without changing the output"){

        CHECK(math::distance(t0+t0, t0+t0, lo, hi) < threshold );
        CHECK(math::distance(t0+t1, t0+t1, lo, hi) < threshold );
        CHECK(math::distance(t0+t2, t0+t2, lo, hi) < threshold );
        CHECK(math::distance(t0+t3, t0+t3, lo, hi) < threshold );

        CHECK(math::distance(t1+t0, t1+t0, lo, hi) < threshold );
        CHECK(math::distance(t1+t1, t1+t1, lo, hi) < threshold );
        CHECK(math::distance(t1+t2, t1+t2, lo, hi) < threshold );
        CHECK(math::distance(t1+t3, t1+t3, lo, hi) < threshold );

        CHECK(math::distance(t2+t0, t2+t0, lo, hi) < threshold );
        CHECK(math::distance(t2+t1, t2+t1, lo, hi) < threshold );
        CHECK(math::distance(t2+t2, t2+t2, lo, hi) < threshold );
        CHECK(math::distance(t2+t3, t2+t3, lo, hi) < threshold );

        CHECK(math::distance(t3+t0, t3+t0, lo, hi) < threshold );
        CHECK(math::distance(t3+t1, t3+t1, lo, hi) < threshold );
        CHECK(math::distance(t3+t2, t3+t2, lo, hi) < threshold );
        CHECK(math::distance(t3+t3, t3+t3, lo, hi) < threshold );
    }

    SECTION("t0*t1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0*t0, t0*t0, midlo, midhi) < threshold );
        CHECK(math::distance(t0*t1, t0*t1, midlo, midhi) < threshold );
        CHECK(math::distance(t0*t2, t0*t2, midlo, midhi) < threshold );
        CHECK(math::distance(t0*t3, t0*t3, midlo, midhi) < threshold );

        CHECK(math::distance(t1*t0, t1*t0, midlo, midhi) < threshold );
        CHECK(math::distance(t1*t1, t1*t1, midlo, midhi) < threshold );
        CHECK(math::distance(t1*t2, t1*t2, midlo, midhi) < threshold );
        CHECK(math::distance(t1*t3, t1*t3, midlo, midhi) < threshold );

        CHECK(math::distance(t2*t0, t2*t0, midlo, midhi) < threshold );
        CHECK(math::distance(t2*t1, t2*t1, midlo, midhi) < threshold );
        CHECK(math::distance(t2*t2, t2*t2, midlo, midhi) < threshold );
        CHECK(math::distance(t2*t3, t2*t3, midlo, midhi) < threshold );

        CHECK(math::distance(t3*t0, t3*t0, midlo, midhi) < threshold );
        CHECK(math::distance(t3*t1, t3*t1, midlo, midhi) < threshold );
        CHECK(math::distance(t3*t2, t3*t2, midlo, midhi) < threshold );
        CHECK(math::distance(t3*t3, t3*t3, midlo, midhi) < threshold );
    }

    SECTION("t0-t1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0-t0, t0-t0, lo, hi) < threshold );
        CHECK(math::distance(t0-t1, t0-t1, lo, hi) < threshold );
        CHECK(math::distance(t0-t2, t0-t2, lo, hi) < threshold );
        CHECK(math::distance(t0-t3, t0-t3, lo, hi) < threshold );

        CHECK(math::distance(t1-t0, t1-t0, lo, hi) < threshold );
        CHECK(math::distance(t1-t1, t1-t1, lo, hi) < threshold );
        CHECK(math::distance(t1-t2, t1-t2, lo, hi) < threshold );
        CHECK(math::distance(t1-t3, t1-t3, lo, hi) < threshold );

        CHECK(math::distance(t2-t0, t2-t0, lo, hi) < threshold );
        CHECK(math::distance(t2-t1, t2-t1, lo, hi) < threshold );
        CHECK(math::distance(t2-t2, t2-t2, lo, hi) < threshold );
        CHECK(math::distance(t2-t3, t2-t3, lo, hi) < threshold );

        CHECK(math::distance(t3-t0, t3-t0, lo, hi) < threshold );
        CHECK(math::distance(t3-t1, t3-t1, lo, hi) < threshold );
        CHECK(math::distance(t3-t2, t3-t2, lo, hi) < threshold );
        CHECK(math::distance(t3-t3, t3-t3, lo, hi) < threshold );
    }
}

TEST_CASE( "PolynomialTrain arithmetic identity", "[math]" ) {
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

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    math::Polynomial<double,0,0> zero = math::Polynomial<double,0,0>({0.0});
    math::Polynomial<double,0,0> one  = math::Polynomial<double,0,0>({1.0});

    SECTION("t0+I must equal t0"){
        CHECK(math::distance(t0+zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0-zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0*one , t0, lo, hi) < threshold);
        CHECK(math::distance(t0-t0, zero, lo, hi) < threshold);

        CHECK(math::distance(t1+zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1-zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1*one , t1, lo, hi) < threshold);
        CHECK(math::distance(t1-t1, zero, lo, hi) < threshold);

        CHECK(math::distance(t2+zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2-zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2*one , t2, lo, hi) < threshold);
        CHECK(math::distance(t2-t2, zero, lo, hi) < threshold);

        CHECK(math::distance(t3+zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3-zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3*one , t3, lo, hi) < threshold);
        CHECK(math::distance(t3-t3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    SECTION("t0+t1 must equal t1+t0"){
        CHECK(math::distance(t0+t1, t1+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+t2, t2+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+t3, t3+t0, lo, hi) < threshold);

        CHECK(math::distance(t1+t0, t0+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+t2, t2+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+t3, t3+t1, lo, hi) < threshold);

        CHECK(math::distance(t2+t0, t0+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+t1, t1+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+t3, t3+t2, lo, hi) < threshold);

        CHECK(math::distance(t3+t0, t0+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+t1, t1+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+t2, t2+t3, lo, hi) < threshold);
    }
    SECTION("t0*t1 must equal t1*t0"){
        CHECK(math::distance(t0*t1, t1*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*t2, t2*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*t3, t3*t0, midlo, midhi) < threshold);

        CHECK(math::distance(t1*t0, t0*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*t2, t2*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*t3, t3*t1, midlo, midhi) < threshold);

        CHECK(math::distance(t2*t0, t0*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*t1, t1*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*t3, t3*t2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*t0, t0*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*t1, t1*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*t2, t2*t3, midlo, midhi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain arithmetic associativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+t2, t0+(t1+t2), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+t3, t0+(t1+t3), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+t3, t1+(t2+t3), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+t0, t1+(t2+t0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+t0, t2+(t3+t0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+t1, t2+(t3+t1), lo, hi) < threshold);
    }
    SECTION("(t0*t1)*t2 must equal t0*(t1*t2)"){
        CHECK(math::distance((t0*t1)*t2, t0*(t1*t2), midlo, midhi) < threshold);
        CHECK(math::distance((t0*t1)*t3, t0*(t1*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t1*t2)*t3, t1*(t2*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t1*t2)*t0, t1*(t2*t0), midlo, midhi) < threshold);
        CHECK(math::distance((t2*t3)*t0, t2*(t3*t0), midlo, midhi) < threshold);
        CHECK(math::distance((t2*t3)*t1, t2*(t3*t1), midlo, midhi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    // const double lo = -1e3;
    // const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});

    SECTION("t0+t1 must equal t1+t0"){
        CHECK(math::distance((t0+t1)*t2, t0*t2+t1*t2, midlo, midhi) < threshold);
        CHECK(math::distance((t0+t1)*t3, t0*t3+t1*t3, midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*t1, t0*t1+t2*t1, midlo, midhi) < threshold);
        CHECK(math::distance((t0+t2)*t3, t0*t3+t2*t3, midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*t1, t0*t1+t3*t1, midlo, midhi) < threshold);
        CHECK(math::distance((t0+t3)*t2, t0*t2+t3*t2, midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*t0, t1*t0+t2*t0, midlo, midhi) < threshold);
        CHECK(math::distance((t1+t2)*t3, t1*t3+t2*t3, midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*t0, t1*t0+t3*t0, midlo, midhi) < threshold);
        CHECK(math::distance((t1+t3)*t2, t1*t2+t3*t2, midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*t0, t2*t0+t3*t0, midlo, midhi) < threshold);
        CHECK(math::distance((t2+t3)*t1, t2*t1+t3*t1, midlo, midhi) < threshold);
    }
}







TEST_CASE( "PolynomialTrain/scalar arithmetic purity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    double k0(0.0);
    double k1(2.0);
    double k2(-2.0);

    SECTION("t0+k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0+k0, t0+k0, lo, hi) < threshold);
        CHECK(math::distance(t0+k1, t0+k1, lo, hi) < threshold);
        CHECK(math::distance(t0+k2, t0+k2, lo, hi) < threshold);

        CHECK(math::distance(t1+k0, t1+k0, lo, hi) < threshold);
        CHECK(math::distance(t1+k1, t1+k1, lo, hi) < threshold);
        CHECK(math::distance(t1+k2, t1+k2, lo, hi) < threshold);

        CHECK(math::distance(t2+k0, t2+k0, lo, hi) < threshold);
        CHECK(math::distance(t2+k1, t2+k1, lo, hi) < threshold);
        CHECK(math::distance(t2+k2, t2+k2, lo, hi) < threshold);

        CHECK(math::distance(t3+k0, t3+k0, lo, hi) < threshold);
        CHECK(math::distance(t3+k1, t3+k1, lo, hi) < threshold);
        CHECK(math::distance(t3+k2, t3+k2, lo, hi) < threshold);
    }

    SECTION("t0*k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0*k0, t0*k0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*k1, t0*k1, midlo, midhi) < threshold);
        CHECK(math::distance(t0*k2, t0*k2, midlo, midhi) < threshold);

        CHECK(math::distance(t1*k0, t1*k0, midlo, midhi) < threshold);
        CHECK(math::distance(t1*k1, t1*k1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*k2, t1*k2, midlo, midhi) < threshold);

        CHECK(math::distance(t2*k0, t2*k0, midlo, midhi) < threshold);
        CHECK(math::distance(t2*k1, t2*k1, midlo, midhi) < threshold);
        CHECK(math::distance(t2*k2, t2*k2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*k0, t3*k0, midlo, midhi) < threshold);
        CHECK(math::distance(t3*k1, t3*k1, midlo, midhi) < threshold);
        CHECK(math::distance(t3*k2, t3*k2, midlo, midhi) < threshold);
    }

    SECTION("t0-k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0-k0, t0-k0, lo, hi) < threshold);
        CHECK(math::distance(t0-k1, t0-k1, lo, hi) < threshold);
        CHECK(math::distance(t0-k2, t0-k2, lo, hi) < threshold);

        CHECK(math::distance(t1-k0, t1-k0, lo, hi) < threshold);
        CHECK(math::distance(t1-k1, t1-k1, lo, hi) < threshold);
        CHECK(math::distance(t1-k2, t1-k2, lo, hi) < threshold);

        CHECK(math::distance(t2-k0, t2-k0, lo, hi) < threshold);
        CHECK(math::distance(t2-k1, t2-k1, lo, hi) < threshold);
        CHECK(math::distance(t2-k2, t2-k2, lo, hi) < threshold);

        CHECK(math::distance(t3-k0, t3-k0, lo, hi) < threshold);
        CHECK(math::distance(t3-k1, t3-k1, lo, hi) < threshold);
        CHECK(math::distance(t3-k2, t3-k2, lo, hi) < threshold);
    }

    SECTION("t0/k1 must be called repeatedly without changing the output"){
        // CHECK(math::distance(t0/k0, t0/k0, lo, hi) < threshold);
        CHECK(math::distance(t0/k1, t0/k1, lo, hi) < threshold);
        CHECK(math::distance(t0/k2, t0/k2, lo, hi) < threshold);

        // CHECK(math::distance(t1/k0, t1/k0, lo, hi) < threshold);
        CHECK(math::distance(t1/k1, t1/k1, lo, hi) < threshold);
        CHECK(math::distance(t1/k2, t1/k2, lo, hi) < threshold);

        // CHECK(math::distance(t2/k0, t2/k0, lo, hi) < threshold);
        CHECK(math::distance(t2/k1, t2/k1, lo, hi) < threshold);
        CHECK(math::distance(t2/k2, t2/k2, lo, hi) < threshold);

        // CHECK(math::distance(t3/k0, t3/k0, lo, hi) < threshold);
        CHECK(math::distance(t3/k1, t3/k1, lo, hi) < threshold);
        CHECK(math::distance(t3/k2, t3/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/scalar arithmetic identity", "[math]" ) {
    const double threshold = 1e-2;

    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -1e2;
    // const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    double zero(0.0);
    double one (1.0);

    SECTION("t0+I must equal t0"){
        CHECK(math::distance(t0+zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0-zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0*one , t0, lo, hi) < threshold);
        CHECK(math::distance(t0/one , t0, lo, hi) < threshold);
        CHECK(math::distance(t0-t0, zero, lo, hi) < threshold);

        CHECK(math::distance(t1+zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1-zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1*one , t1, lo, hi) < threshold);
        CHECK(math::distance(t1/one , t1, lo, hi) < threshold);
        CHECK(math::distance(t1-t1, zero, lo, hi) < threshold);

        CHECK(math::distance(t2+zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2-zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2*one , t2, lo, hi) < threshold);
        CHECK(math::distance(t2/one , t2, lo, hi) < threshold);
        CHECK(math::distance(t2-t2, zero, lo, hi) < threshold);

        CHECK(math::distance(t3+zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3-zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3*one , t3, lo, hi) < threshold);
        CHECK(math::distance(t3/one , t3, lo, hi) < threshold);
        CHECK(math::distance(t3-t3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/scalar arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    double k0(0.0);
    double k1(2.0);
    double k2(-2.0);

    SECTION("t0+k must equal k+t0"){
        CHECK(math::distance(t0+k0, k0+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+k1, k1+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+k2, k2+t0, lo, hi) < threshold);

        CHECK(math::distance(t1+k0, k0+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+k1, k1+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+k2, k2+t1, lo, hi) < threshold);

        CHECK(math::distance(t2+k0, k0+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+k1, k1+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+k2, k2+t2, lo, hi) < threshold);

        CHECK(math::distance(t3+k0, k0+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+k1, k1+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+k2, k2+t3, lo, hi) < threshold);
    }

    SECTION("t0*k must equal k*t0"){
        CHECK(math::distance(t0*k0, k0*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*k1, k1*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*k2, k2*t0, midlo, midhi) < threshold);

        CHECK(math::distance(t1*k0, k0*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*k1, k1*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*k2, k2*t1, midlo, midhi) < threshold);

        CHECK(math::distance(t2*k0, k0*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*k1, k1*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*k2, k2*t2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*k0, k0*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*k1, k1*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*k2, k2*t3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/scalar arithmetic associativity", "[math]" ) {
    const double threshold = 1e-2;

    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -1e2;
    // const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    double k0(0.0);
    double k1(2.0);
    double k2(-2.0);

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+k0, t0+(t1+k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+k1, t0+(t1+k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+k2, t0+(t1+k2), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)+k0, t0+(t2+k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+k1, t0+(t2+k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+k2, t0+(t2+k2), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)+k0, t0+(t3+k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+k1, t0+(t3+k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+k2, t0+(t3+k2), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)+k0, t1+(t2+k0), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+k1, t1+(t2+k1), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+k2, t1+(t2+k2), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)+k0, t1+(t3+k0), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+k1, t1+(t3+k1), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+k2, t1+(t3+k2), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)+k0, t2+(t3+k0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+k1, t2+(t3+k1), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+k2, t2+(t3+k2), lo, hi) < threshold);
    }

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0*t1)*k0, t0*(t1*k0), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*k1, t0*(t1*k1), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*k2, t0*(t1*k2), lo, hi) < threshold);

        CHECK(math::distance((t0*t2)*k0, t0*(t2*k0), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*k1, t0*(t2*k1), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*k2, t0*(t2*k2), lo, hi) < threshold);

        CHECK(math::distance((t0*t3)*k0, t0*(t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*k1, t0*(t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*k2, t0*(t3*k2), lo, hi) < threshold);

        CHECK(math::distance((t1*t2)*k0, t1*(t2*k0), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*k1, t1*(t2*k1), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*k2, t1*(t2*k2), lo, hi) < threshold);

        CHECK(math::distance((t1*t3)*k0, t1*(t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*k1, t1*(t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*k2, t1*(t3*k2), lo, hi) < threshold);

        CHECK(math::distance((t2*t3)*k0, t2*(t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*k1, t2*(t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*k2, t2*(t3*k2), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/scalar arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3e1;
    const double midhi =  3e1;

    const double oo = std::numeric_limits<double>::max();

    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});

    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    double k0(0.0);
    double k1(2.0);
    double k2(-2.0);

    SECTION("(t0+t1)*k must equal t0*k + t1*k"){
        CHECK(math::distance((t0+t1)*k0, (t0*k0+t1*k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)*k1, (t0*k1+t1*k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)*k2, (t0*k2+t1*k2), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)*k0, (t0*k0+t2*k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)*k1, (t0*k1+t2*k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)*k2, (t0*k2+t2*k2), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)*k0, (t0*k0+t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)*k1, (t0*k1+t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)*k2, (t0*k2+t3*k2), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)*k0, (t1*k0+t2*k0), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)*k1, (t1*k1+t2*k1), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)*k2, (t1*k2+t2*k2), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)*k0, (t1*k0+t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)*k1, (t1*k1+t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)*k2, (t1*k2+t3*k2), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)*k0, (t2*k0+t3*k0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)*k1, (t2*k1+t3*k1), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)*k2, (t2*k2+t3*k2), lo, hi) < threshold);



        CHECK(math::distance((t0+k0)*t1, (t0*t1+k0*t1), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k1)*t1, (t0*t1+k1*t1), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k2)*t1, (t0*t1+k2*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+k0)*t2, (t0*t2+k0*t2), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k1)*t2, (t0*t2+k1*t2), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k2)*t2, (t0*t2+k2*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+k0)*t3, (t0*t3+k0*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k1)*t3, (t0*t3+k1*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t0+k2)*t3, (t0*t3+k2*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+k0)*t2, (t1*t2+k0*t2), midlo, midhi) < threshold);
        CHECK(math::distance((t1+k1)*t2, (t1*t2+k1*t2), midlo, midhi) < threshold);
        CHECK(math::distance((t1+k2)*t2, (t1*t2+k2*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+k0)*t3, (t1*t3+k0*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t1+k1)*t3, (t1*t3+k1*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t1+k2)*t3, (t1*t3+k2*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+k0)*t3, (t2*t3+k0*t3), midlo, midhi) < threshold);
        CHECK(math::distance((t2+k1)*t3, (t2*t3+k1*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+k2)*t3, (t2*t3+k2*t3), midlo, midhi) < threshold);

    }
}








TEST_CASE( "PolynomialTrain/monomial arithmetic purity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Polynomial<double,0,0> m0{2.0};
    math::Polynomial<double,2,2> m1{2.0};
    math::Polynomial<double,-2,-2> m2{2.0};

    SECTION("t0+m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0+m0, t0+m0, lo, hi) < threshold);
        CHECK(math::distance(t0+m1, t0+m1, lo, hi) < threshold);
        CHECK(math::distance(t0+m2, t0+m2, lo, hi) < threshold);

        CHECK(math::distance(t1+m0, t1+m0, lo, hi) < threshold);
        CHECK(math::distance(t1+m1, t1+m1, lo, hi) < threshold);
        CHECK(math::distance(t1+m2, t1+m2, lo, hi) < threshold);

        CHECK(math::distance(t2+m0, t2+m0, lo, hi) < threshold);
        CHECK(math::distance(t2+m1, t2+m1, lo, hi) < threshold);
        CHECK(math::distance(t2+m2, t2+m2, lo, hi) < threshold);

        CHECK(math::distance(t3+m0, t3+m0, lo, hi) < threshold);
        CHECK(math::distance(t3+m1, t3+m1, lo, hi) < threshold);
        CHECK(math::distance(t3+m2, t3+m2, lo, hi) < threshold);
    }

    SECTION("t0*m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0*m0, t0*m0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*m1, t0*m1, midlo, midhi) < threshold);
        CHECK(math::distance(t0*m2, t0*m2, midlo, midhi) < threshold);

        CHECK(math::distance(t1*m0, t1*m0, midlo, midhi) < threshold);
        CHECK(math::distance(t1*m1, t1*m1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*m2, t1*m2, midlo, midhi) < threshold);

        CHECK(math::distance(t2*m0, t2*m0, midlo, midhi) < threshold);
        CHECK(math::distance(t2*m1, t2*m1, midlo, midhi) < threshold);
        CHECK(math::distance(t2*m2, t2*m2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*m0, t3*m0, midlo, midhi) < threshold);
        CHECK(math::distance(t3*m1, t3*m1, midlo, midhi) < threshold);
        CHECK(math::distance(t3*m2, t3*m2, midlo, midhi) < threshold);
    }

    SECTION("t0-m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(t0-m0, t0-m0, lo, hi) < threshold);
        CHECK(math::distance(t0-m1, t0-m1, lo, hi) < threshold);
        CHECK(math::distance(t0-m2, t0-m2, lo, hi) < threshold);

        CHECK(math::distance(t1-m0, t1-m0, lo, hi) < threshold);
        CHECK(math::distance(t1-m1, t1-m1, lo, hi) < threshold);
        CHECK(math::distance(t1-m2, t1-m2, lo, hi) < threshold);

        CHECK(math::distance(t2-m0, t2-m0, lo, hi) < threshold);
        CHECK(math::distance(t2-m1, t2-m1, lo, hi) < threshold);
        CHECK(math::distance(t2-m2, t2-m2, lo, hi) < threshold);

        CHECK(math::distance(t3-m0, t3-m0, lo, hi) < threshold);
        CHECK(math::distance(t3-m1, t3-m1, lo, hi) < threshold);
        CHECK(math::distance(t3-m2, t3-m2, lo, hi) < threshold);
    }

    SECTION("t0/m1 must be called repeatedly without changing the output"){
        // CHECK(math::distance(t0/m0, t0/m0, lo, hi) < threshold);
        CHECK(math::distance(t0/m1, t0/m1, lo, hi) < threshold);
        CHECK(math::distance(t0/m2, t0/m2, lo, hi) < threshold);

        // CHECK(math::distance(t1/m0, t1/m0, lo, hi) < threshold);
        CHECK(math::distance(t1/m1, t1/m1, lo, hi) < threshold);
        CHECK(math::distance(t1/m2, t1/m2, lo, hi) < threshold);

        // CHECK(math::distance(t2/m0, t2/m0, lo, hi) < threshold);
        CHECK(math::distance(t2/m1, t2/m1, lo, hi) < threshold);
        CHECK(math::distance(t2/m2, t2/m2, lo, hi) < threshold);

        // CHECK(math::distance(t3/m0, t3/m0, lo, hi) < threshold);
        CHECK(math::distance(t3/m1, t3/m1, lo, hi) < threshold);
        CHECK(math::distance(t3/m2, t3/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/monomial arithmetic identity", "[math]" ) {
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
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Polynomial<double,0,0> zero{0.0};
    math::Polynomial<double,0,0> one {1.0};

    SECTION("t0+I must equal t0"){
        CHECK(math::distance(t0+zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0-zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0*one , t0, lo, hi) < threshold);
        CHECK(math::distance(t0/one , t0, lo, hi) < threshold);
        CHECK(math::distance(t0-t0, zero, lo, hi) < threshold);

        CHECK(math::distance(t1+zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1-zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1*one , t1, lo, hi) < threshold);
        CHECK(math::distance(t1/one , t1, lo, hi) < threshold);
        CHECK(math::distance(t1-t1, zero, lo, hi) < threshold);

        CHECK(math::distance(t2+zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2-zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2*one , t2, lo, hi) < threshold);
        CHECK(math::distance(t2/one , t2, lo, hi) < threshold);
        CHECK(math::distance(t2-t2, zero, lo, hi) < threshold);

        CHECK(math::distance(t3+zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3-zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3*one , t3, lo, hi) < threshold);
        CHECK(math::distance(t3/one , t3, lo, hi) < threshold);
        CHECK(math::distance(t3-t3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/monomial arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Polynomial<double,0,0> m0{2.0};
    math::Polynomial<double,2,2> m1{2.0};
    math::Polynomial<double,-2,-2> m2{-2.0};

    SECTION("t0+k must equal k+t0"){
        CHECK(math::distance(t0+m0, m0+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+m1, m1+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+m2, m2+t0, lo, hi) < threshold);

        CHECK(math::distance(t1+m0, m0+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+m1, m1+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+m2, m2+t1, lo, hi) < threshold);

        CHECK(math::distance(t2+m0, m0+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+m1, m1+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+m2, m2+t2, lo, hi) < threshold);

        CHECK(math::distance(t3+m0, m0+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+m1, m1+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+m2, m2+t3, lo, hi) < threshold);
    }

    SECTION("t0*k must equal k*t0"){
        CHECK(math::distance(t0*m0, m0*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*m1, m1*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*m2, m2*t0, midlo, midhi) < threshold);

        CHECK(math::distance(t1*m0, m0*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*m1, m1*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*m2, m2*t1, midlo, midhi) < threshold);

        CHECK(math::distance(t2*m0, m0*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*m1, m1*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*m2, m2*t2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*m0, m0*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*m1, m1*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*m2, m2*t3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/monomial arithmetic associativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -1e2;
    // const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Polynomial<double,0,0> m0{2.0};
    math::Polynomial<double,2,2> m1{2.0};
    math::Polynomial<double,-2,-2> m2{-2.0};

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+m0, t0+(t1+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+m1, t0+(t1+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+m2, t0+(t1+m2), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)+m0, t0+(t2+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+m1, t0+(t2+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+m2, t0+(t2+m2), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)+m0, t0+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+m1, t0+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+m2, t0+(t3+m2), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)+m0, t1+(t2+m0), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+m1, t1+(t2+m1), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+m2, t1+(t2+m2), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)+m0, t1+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+m1, t1+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+m2, t1+(t3+m2), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)+m0, t2+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+m1, t2+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+m2, t2+(t3+m2), lo, hi) < threshold);
    }

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+m0, t0+(t1+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+m1, t0+(t1+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+m2, t0+(t1+m2), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)+m0, t0+(t2+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+m1, t0+(t2+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+m2, t0+(t2+m2), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)+m0, t0+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+m1, t0+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+m2, t0+(t3+m2), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)+m0, t1+(t2+m0), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+m1, t1+(t2+m1), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+m2, t1+(t2+m2), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)+m0, t1+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+m1, t1+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+m2, t1+(t3+m2), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)+m0, t2+(t3+m0), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+m1, t2+(t3+m1), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+m2, t2+(t3+m2), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/monomial arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    // const double lo = -1e3;
    // const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Polynomial<double,0,0> m0{2.0};
    math::Polynomial<double,2,2> m1{2.0};
    math::Polynomial<double,-2,-2> m2{-2.0};

    SECTION("(t0+t1)*k must equal t0*k + t1*k"){
        CHECK(math::distance((t0+t1)*m0, (t0*m0+t1*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m0)*t1, (t0*t1+m0*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*m1, (t0*m1+t1*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m1)*t1, (t0*t1+m1*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*m2, (t0*m2+t1*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m2)*t1, (t0*t1+m2*t1), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t2)*m0, (t0*m0+t2*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m0)*t2, (t0*t2+m0*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*m1, (t0*m1+t2*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m1)*t2, (t0*t2+m1*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*m2, (t0*m2+t2*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m2)*t2, (t0*t2+m2*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t3)*m0, (t0*m0+t3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m0)*t3, (t0*t3+m0*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*m1, (t0*m1+t3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m1)*t3, (t0*t3+m1*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*m2, (t0*m2+t3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t0+m2)*t3, (t0*t3+m2*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t2)*m0, (t1*m0+t2*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m0)*t2, (t1*t2+m0*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*m1, (t1*m1+t2*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m1)*t2, (t1*t2+m1*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*m2, (t1*m2+t2*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m2)*t2, (t1*t2+m2*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t3)*m0, (t1*m0+t3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m0)*t3, (t1*t3+m0*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*m1, (t1*m1+t3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m1)*t3, (t1*t3+m1*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*m2, (t1*m2+t3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t1+m2)*t3, (t1*t3+m2*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t2+t3)*m0, (t2*m0+t3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((t2+m0)*t3, (t2*t3+m0*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*m1, (t2*m1+t3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((t2+m1)*t3, (t2*t3+m1*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*m2, (t2*m2+t3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((t2+m2)*t3, (t2*t3+m2*t3), midlo, midhi) < threshold);
    }
}










TEST_CASE( "PolynomialTrain/Shifting arithmetic purity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Shifting<double> f(2.0);
    math::Shifting<double> g(-2.0);
    math::Shifting<double> h(0.0);

    SECTION("t0+g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0+f, t0+f, lo, hi) < threshold);
        CHECK(math::distance(t0+g, t0+g, lo, hi) < threshold);
        CHECK(math::distance(t0+h, t0+h, lo, hi) < threshold);

        CHECK(math::distance(t1+f, t1+f, lo, hi) < threshold);
        CHECK(math::distance(t1+g, t1+g, lo, hi) < threshold);
        CHECK(math::distance(t1+h, t1+h, lo, hi) < threshold);

        CHECK(math::distance(t2+f, t2+f, lo, hi) < threshold);
        CHECK(math::distance(t2+g, t2+g, lo, hi) < threshold);
        CHECK(math::distance(t2+h, t2+h, lo, hi) < threshold);

        CHECK(math::distance(t3+f, t3+f, lo, hi) < threshold);
        CHECK(math::distance(t3+g, t3+g, lo, hi) < threshold);
        CHECK(math::distance(t3+h, t3+h, lo, hi) < threshold);
    }

    SECTION("t0*g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0*f, t0*f, midlo, midhi) < threshold);
        CHECK(math::distance(t0*g, t0*g, midlo, midhi) < threshold);
        CHECK(math::distance(t0*h, t0*h, midlo, midhi) < threshold);

        CHECK(math::distance(t1*f, t1*f, midlo, midhi) < threshold);
        CHECK(math::distance(t1*g, t1*g, midlo, midhi) < threshold);
        CHECK(math::distance(t1*h, t1*h, midlo, midhi) < threshold);

        CHECK(math::distance(t2*f, t2*f, midlo, midhi) < threshold);
        CHECK(math::distance(t2*g, t2*g, midlo, midhi) < threshold);
        CHECK(math::distance(t2*h, t2*h, midlo, midhi) < threshold);

        CHECK(math::distance(t3*f, t3*f, midlo, midhi) < threshold);
        CHECK(math::distance(t3*g, t3*g, midlo, midhi) < threshold);
        CHECK(math::distance(t3*h, t3*h, midlo, midhi) < threshold);
    }

    SECTION("t0-g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0-f, t0-f, lo, hi) < threshold);
        CHECK(math::distance(t0-g, t0-g, lo, hi) < threshold);
        CHECK(math::distance(t0-h, t0-h, lo, hi) < threshold);

        CHECK(math::distance(t1-f, t1-f, lo, hi) < threshold);
        CHECK(math::distance(t1-g, t1-g, lo, hi) < threshold);
        CHECK(math::distance(t1-h, t1-h, lo, hi) < threshold);

        CHECK(math::distance(t2-f, t2-f, lo, hi) < threshold);
        CHECK(math::distance(t2-g, t2-g, lo, hi) < threshold);
        CHECK(math::distance(t2-h, t2-h, lo, hi) < threshold);

        CHECK(math::distance(t3-f, t3-f, lo, hi) < threshold);
        CHECK(math::distance(t3-g, t3-g, lo, hi) < threshold);
        CHECK(math::distance(t3-h, t3-h, lo, hi) < threshold);
    }

    // SECTION("t0/g must be called repeatedly without changing the output"){
    //     // CHECK(math::distance(t0/f, t0/f, lo, hi) < threshold);
    //     CHECK(math::distance(t0/g, t0/g, lo, hi) < threshold);
    //     CHECK(math::distance(t0/h, t0/h, lo, hi) < threshold);

    //     // CHECK(math::distance(t1/f, t1/f, lo, hi) < threshold);
    //     CHECK(math::distance(t1/g, t1/g, lo, hi) < threshold);
    //     CHECK(math::distance(t1/h, t1/h, lo, hi) < threshold);

    //     // CHECK(math::distance(t2/f, t2/f, lo, hi) < threshold);
    //     CHECK(math::distance(t2/g, t2/g, lo, hi) < threshold);
    //     CHECK(math::distance(t2/h, t2/h, lo, hi) < threshold);

    //     // CHECK(math::distance(t3/f, t3/f, lo, hi) < threshold);
    //     CHECK(math::distance(t3/g, t3/g, lo, hi) < threshold);
    //     CHECK(math::distance(t3/h, t3/h, lo, hi) < threshold);
    // }
}

TEST_CASE( "PolynomialTrain/Shifting arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Shifting<double> f(2.0);
    math::Shifting<double> g(-2.0);
    math::Shifting<double> h(0.0);

    SECTION("t0+k must equal k+t0"){
        CHECK(math::distance(t0+f, f+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+g, g+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+h, h+t0, lo, hi) < threshold);

        CHECK(math::distance(t1+f, f+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+g, g+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+h, h+t1, lo, hi) < threshold);

        CHECK(math::distance(t2+f, f+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+g, g+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+h, h+t2, lo, hi) < threshold);

        CHECK(math::distance(t3+f, f+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+g, g+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+h, h+t3, lo, hi) < threshold);
    }

    SECTION("t0*k must equal k*t0"){
        CHECK(math::distance(t0*f, f*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*g, g*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*h, h*t0, midlo, midhi) < threshold);

        CHECK(math::distance(t1*f, f*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*g, g*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*h, h*t1, midlo, midhi) < threshold);

        CHECK(math::distance(t2*f, f*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*g, g*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*h, h*t2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*f, f*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*g, g*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*h, h*t3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -3e1;
    // const double midhi =  3e1;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Shifting<double> f(2.0);
    math::Shifting<double> g(-2.0);
    math::Shifting<double> h(0.0);

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+f, t0+(t1+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+g, t0+(t1+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+h, t0+(t1+h), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)+f, t0+(t2+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+g, t0+(t2+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+h, t0+(t2+h), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)+f, t0+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+g, t0+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+h, t0+(t3+h), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)+f, t1+(t2+f), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+g, t1+(t2+g), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+h, t1+(t2+h), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)+f, t1+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+g, t1+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+h, t1+(t3+h), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)+f, t2+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+g, t2+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+h, t2+(t3+h), lo, hi) < threshold);
    }

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0*t1)*f, t0*(t1*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*g, t0*(t1*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*h, t0*(t1*h), lo, hi) < threshold);

        CHECK(math::distance((t0*t2)*f, t0*(t2*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*g, t0*(t2*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*h, t0*(t2*h), lo, hi) < threshold);

        CHECK(math::distance((t0*t3)*f, t0*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*g, t0*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*h, t0*(t3*h), lo, hi) < threshold);

        CHECK(math::distance((t1*t2)*f, t1*(t2*f), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*g, t1*(t2*g), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*h, t1*(t2*h), lo, hi) < threshold);

        CHECK(math::distance((t1*t3)*f, t1*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*g, t1*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*h, t1*(t3*h), lo, hi) < threshold);

        CHECK(math::distance((t2*t3)*f, t2*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*g, t2*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*h, t2*(t3*h), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    // const double lo = -1e3;
    // const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3e1;
    const double midhi =  3e1;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Shifting<double> f(2.0);
    math::Shifting<double> g(-2.0);
    math::Shifting<double> h(0.0);

    SECTION("(t0+t1)*k must equal t0*k + t1*k"){
        CHECK(math::distance((t0+t1)*f, (t0*f+t1*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t1, (t0*t1+f*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*g, (t0*g+t1*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t1, (t0*t1+g*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*h, (t0*h+t1*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t1, (t0*t1+h*t1), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t2)*f, (t0*f+t2*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t2, (t0*t2+f*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*g, (t0*g+t2*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t2, (t0*t2+g*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*h, (t0*h+t2*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t2, (t0*t2+h*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t3)*f, (t0*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t3, (t0*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*g, (t0*g+t3*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t3, (t0*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*h, (t0*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t3, (t0*t3+h*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t2)*f, (t1*f+t2*f), midlo, midhi) < threshold);
        CHECK(math::distance((t1+f)*t2, (t1*t2+f*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*g, (t1*g+t2*g), midlo, midhi) < threshold);
        CHECK(math::distance((t1+g)*t2, (t1*t2+g*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*h, (t1*h+t2*h), midlo, midhi) < threshold);
        CHECK(math::distance((t1+h)*t2, (t1*t2+h*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t3)*f, (t1*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t1+f)*t3, (t1*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*g, (t1*g+t3*g), midlo, midhi) < threshold);
        CHECK(math::distance((t1+g)*t3, (t1*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*h, (t1*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t1+h)*t3, (t1*t3+h*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t2+t3)*f, (t2*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t2+f)*t3, (t2*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*g, (t2*g+t3*g), midlo, midhi) < threshold);
        CHECK(math::distance((t2+g)*t3, (t2*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*h, (t2*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t2+h)*t3, (t2*t3+h*t3), midlo, midhi) < threshold);
    }
}











TEST_CASE( "PolynomialTrain/Scaling arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Scaling<double> f(2.0);
    math::Scaling<double> g(-2.0);
    math::Scaling<double> h(0.0);

    SECTION("t0+g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0+f, t0+f, lo, hi) < threshold);
        CHECK(math::distance(t0+g, t0+g, lo, hi) < threshold);
        CHECK(math::distance(t0+h, t0+h, lo, hi) < threshold);

        CHECK(math::distance(t1+f, t1+f, lo, hi) < threshold);
        CHECK(math::distance(t1+g, t1+g, lo, hi) < threshold);
        CHECK(math::distance(t1+h, t1+h, lo, hi) < threshold);

        CHECK(math::distance(t2+f, t2+f, lo, hi) < threshold);
        CHECK(math::distance(t2+g, t2+g, lo, hi) < threshold);
        CHECK(math::distance(t2+h, t2+h, lo, hi) < threshold);

        CHECK(math::distance(t3+f, t3+f, lo, hi) < threshold);
        CHECK(math::distance(t3+g, t3+g, lo, hi) < threshold);
        CHECK(math::distance(t3+h, t3+h, lo, hi) < threshold);
    }

    SECTION("t0*g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0*f, t0*f, midlo, midhi) < threshold);
        CHECK(math::distance(t0*g, t0*g, midlo, midhi) < threshold);
        CHECK(math::distance(t0*h, t0*h, midlo, midhi) < threshold);

        CHECK(math::distance(t1*f, t1*f, midlo, midhi) < threshold);
        CHECK(math::distance(t1*g, t1*g, midlo, midhi) < threshold);
        CHECK(math::distance(t1*h, t1*h, midlo, midhi) < threshold);

        CHECK(math::distance(t2*f, t2*f, midlo, midhi) < threshold);
        CHECK(math::distance(t2*g, t2*g, midlo, midhi) < threshold);
        CHECK(math::distance(t2*h, t2*h, midlo, midhi) < threshold);

        CHECK(math::distance(t3*f, t3*f, midlo, midhi) < threshold);
        CHECK(math::distance(t3*g, t3*g, midlo, midhi) < threshold);
        CHECK(math::distance(t3*h, t3*h, midlo, midhi) < threshold);
    }

    SECTION("t0-g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0-f, t0-f, lo, hi) < threshold);
        CHECK(math::distance(t0-g, t0-g, lo, hi) < threshold);
        CHECK(math::distance(t0-h, t0-h, lo, hi) < threshold);

        CHECK(math::distance(t1-f, t1-f, lo, hi) < threshold);
        CHECK(math::distance(t1-g, t1-g, lo, hi) < threshold);
        CHECK(math::distance(t1-h, t1-h, lo, hi) < threshold);

        CHECK(math::distance(t2-f, t2-f, lo, hi) < threshold);
        CHECK(math::distance(t2-g, t2-g, lo, hi) < threshold);
        CHECK(math::distance(t2-h, t2-h, lo, hi) < threshold);

        CHECK(math::distance(t3-f, t3-f, lo, hi) < threshold);
        CHECK(math::distance(t3-g, t3-g, lo, hi) < threshold);
        CHECK(math::distance(t3-h, t3-h, lo, hi) < threshold);
    }

    SECTION("t0/g must be called repeatedly without changing the output"){
        CHECK(math::distance(t0/f, t0/f, lo, hi) < threshold);
        CHECK(math::distance(t0/g, t0/g, lo, hi) < threshold);
        // CHECK(math::distance(t0/h, t0/h, lo, hi) < threshold);

        CHECK(math::distance(t1/f, t1/f, lo, hi) < threshold);
        CHECK(math::distance(t1/g, t1/g, lo, hi) < threshold);
        // CHECK(math::distance(t1/h, t1/h, lo, hi) < threshold);

        CHECK(math::distance(t2/f, t2/f, lo, hi) < threshold);
        CHECK(math::distance(t2/g, t2/g, lo, hi) < threshold);
        // CHECK(math::distance(t2/h, t2/h, lo, hi) < threshold);

        CHECK(math::distance(t3/f, t3/f, lo, hi) < threshold);
        CHECK(math::distance(t3/g, t3/g, lo, hi) < threshold);
        // CHECK(math::distance(t3/h, t3/h, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/Scaling arithmetic identity", "[math]" ) {
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
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Scaling<double> zero(0.0);

    SECTION("t0+I must equal t0"){
        CHECK(math::distance(t0+zero, t0, lo, hi) < threshold);
        CHECK(math::distance(t0-zero, t0, lo, hi) < threshold);
        // CHECK(math::distance(t0-t0, zero, lo, hi) < threshold);

        CHECK(math::distance(t1+zero, t1, lo, hi) < threshold);
        CHECK(math::distance(t1-zero, t1, lo, hi) < threshold);
        // CHECK(math::distance(t1-t1, zero, lo, hi) < threshold);

        CHECK(math::distance(t2+zero, t2, lo, hi) < threshold);
        CHECK(math::distance(t2-zero, t2, lo, hi) < threshold);
        // CHECK(math::distance(t2-t2, zero, lo, hi) < threshold);

        CHECK(math::distance(t3+zero, t3, lo, hi) < threshold);
        CHECK(math::distance(t3-zero, t3, lo, hi) < threshold);
        // CHECK(math::distance(t3-t3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialTrain/Scaling arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Scaling<double> f(2.0);
    math::Scaling<double> g(-2.0);
    math::Scaling<double> h(0.0);

    SECTION("t0+k must equal k+t0"){
        CHECK(math::distance(t0+f, f+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+g, g+t0, lo, hi) < threshold);
        CHECK(math::distance(t0+h, h+t0, lo, hi) < threshold);

        CHECK(math::distance(t1+f, f+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+g, g+t1, lo, hi) < threshold);
        CHECK(math::distance(t1+h, h+t1, lo, hi) < threshold);

        CHECK(math::distance(t2+f, f+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+g, g+t2, lo, hi) < threshold);
        CHECK(math::distance(t2+h, h+t2, lo, hi) < threshold);

        CHECK(math::distance(t3+f, f+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+g, g+t3, lo, hi) < threshold);
        CHECK(math::distance(t3+h, h+t3, lo, hi) < threshold);
    }

    SECTION("t0*k must equal k*t0"){
        CHECK(math::distance(t0*f, f*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*g, g*t0, midlo, midhi) < threshold);
        CHECK(math::distance(t0*h, h*t0, midlo, midhi) < threshold);

        CHECK(math::distance(t1*f, f*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*g, g*t1, midlo, midhi) < threshold);
        CHECK(math::distance(t1*h, h*t1, midlo, midhi) < threshold);

        CHECK(math::distance(t2*f, f*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*g, g*t2, midlo, midhi) < threshold);
        CHECK(math::distance(t2*h, h*t2, midlo, midhi) < threshold);

        CHECK(math::distance(t3*f, f*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*g, g*t3, midlo, midhi) < threshold);
        CHECK(math::distance(t3*h, h*t3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialTrain/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    // const double midlo = -1e2;
    // const double midhi =  1e2;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Scaling<double> f(2.0);
    math::Scaling<double> g(-2.0);
    math::Scaling<double> h(0.0);

    SECTION("(t0+t1)+t2 must equal t0+(t1+t2)"){
        CHECK(math::distance((t0+t1)+f, t0+(t1+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+g, t0+(t1+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t1)+h, t0+(t1+h), lo, hi) < threshold);

        CHECK(math::distance((t0+t2)+f, t0+(t2+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+g, t0+(t2+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t2)+h, t0+(t2+h), lo, hi) < threshold);

        CHECK(math::distance((t0+t3)+f, t0+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+g, t0+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t0+t3)+h, t0+(t3+h), lo, hi) < threshold);

        CHECK(math::distance((t1+t2)+f, t1+(t2+f), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+g, t1+(t2+g), lo, hi) < threshold);
        CHECK(math::distance((t1+t2)+h, t1+(t2+h), lo, hi) < threshold);

        CHECK(math::distance((t1+t3)+f, t1+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+g, t1+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t1+t3)+h, t1+(t3+h), lo, hi) < threshold);

        CHECK(math::distance((t2+t3)+f, t2+(t3+f), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+g, t2+(t3+g), lo, hi) < threshold);
        CHECK(math::distance((t2+t3)+h, t2+(t3+h), lo, hi) < threshold);
    }

    SECTION("(t0*t1)*t2 must equal t0*(t1*t2)"){
        CHECK(math::distance((t0*t1)*f, t0*(t1*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*g, t0*(t1*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t1)*h, t0*(t1*h), lo, hi) < threshold);

        CHECK(math::distance((t0*t2)*f, t0*(t2*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*g, t0*(t2*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t2)*h, t0*(t2*h), lo, hi) < threshold);

        CHECK(math::distance((t0*t3)*f, t0*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*g, t0*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t0*t3)*h, t0*(t3*h), lo, hi) < threshold);

        CHECK(math::distance((t1*t2)*f, t1*(t2*f), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*g, t1*(t2*g), lo, hi) < threshold);
        CHECK(math::distance((t1*t2)*h, t1*(t2*h), lo, hi) < threshold);

        CHECK(math::distance((t1*t3)*f, t1*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*g, t1*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t1*t3)*h, t1*(t3*h), lo, hi) < threshold);

        CHECK(math::distance((t2*t3)*f, t2*(t3*f), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*g, t2*(t3*g), lo, hi) < threshold);
        CHECK(math::distance((t2*t3)*h, t2*(t3*h), lo, hi) < threshold);
    }

}


TEST_CASE( "PolynomialTrain/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-2;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    // const double lo = -1e3;
    // const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -3e1;
    const double midhi =  3e1;

    const double oo = std::numeric_limits<double>::max();
    
    using P0 = math::Polynomial<double,0,2>;
    using P1 = math::Polynomial<double,0,3>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});
    P1 p2 = P1({4.0,3.0,2.0,1.0});
    P1 p3 = P1({-1.0,1.0,-2.0,2.0});


    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    // laurent polynomials
    math::Train<double,P1> t2 = math::Train<double,P1>(std::vector<P1>{p2,p3},std::vector<double>{-oo,0.0,oo}); 
    // all polynomials
    math::Train<double,P1> t3 = math::Train<double,P1>(std::vector<P1>{p3,p2,p1,p0},std::vector<double>{-oo,-2.0,0.0,2.0,oo});
    math::Scaling<double> f(2.0);
    math::Scaling<double> g(-2.0);
    math::Scaling<double> h(0.0);

    SECTION("(t0+t1)*k must equal t0*k + t1*k"){
        CHECK(math::distance((t0+t1)*f, (t0*f+t1*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t1, (t0*t1+f*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*g, (t0*g+t1*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t1, (t0*t1+g*t1), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t1)*h, (t0*h+t1*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t1, (t0*t1+h*t1), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t2)*f, (t0*f+t2*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t2, (t0*t2+f*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*g, (t0*g+t2*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t2, (t0*t2+g*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t2)*h, (t0*h+t2*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t2, (t0*t2+h*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t0+t3)*f, (t0*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t0+f)*t3, (t0*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*g, (t0*g+t3*g), midlo, midhi) < threshold);
        CHECK(math::distance((t0+g)*t3, (t0*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t0+t3)*h, (t0*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t0+h)*t3, (t0*t3+h*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t2)*f, (t1*f+t2*f), midlo, midhi) < threshold);
        CHECK(math::distance((t1+f)*t2, (t1*t2+f*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*g, (t1*g+t2*g), midlo, midhi) < threshold);
        CHECK(math::distance((t1+g)*t2, (t1*t2+g*t2), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t2)*h, (t1*h+t2*h), midlo, midhi) < threshold);
        CHECK(math::distance((t1+h)*t2, (t1*t2+h*t2), midlo, midhi) < threshold);


        CHECK(math::distance((t1+t3)*f, (t1*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t1+f)*t3, (t1*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*g, (t1*g+t3*g), midlo, midhi) < threshold);
        CHECK(math::distance((t1+g)*t3, (t1*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t1+t3)*h, (t1*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t1+h)*t3, (t1*t3+h*t3), midlo, midhi) < threshold);


        CHECK(math::distance((t2+t3)*f, (t2*f+t3*f), midlo, midhi) < threshold);
        CHECK(math::distance((t2+f)*t3, (t2*t3+f*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*g, (t2*g+t3*g), midlo, midhi) < threshold);

        CHECK(math::distance((t2+g)*t3, (t2*t3+g*t3), midlo, midhi) < threshold);

        CHECK(math::distance((t2+t3)*h, (t2*h+t3*h), midlo, midhi) < threshold);
        CHECK(math::distance((t2+h)*t3, (t2*t3+h*t3), midlo, midhi) < threshold);
    }
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
    
    using P0 = math::Polynomial<double,0,2>;

    P0 p0 = P0({3.0,2.0,1.0});
    P0 p1 = P0({-1.0,0.0,1.0});

    // standard polynomials, no overlap
    math::Train<double,P0> t0 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,-1.0,oo}); 
    // standard polynomials, with overlap
    math::Train<double,P0> t1 = math::Train<double,P0>(std::vector<P0>{p0,p1},std::vector<double>{-oo,1.0,oo}); 
    SECTION("the derivative of a function's integral must equal the original function"){
        CHECK(math::distance(t0, derivative(integral(t0)), lo, hi) < threshold);
        CHECK(math::distance(t1, derivative(integral(t1)), lo, hi) < threshold);
        // CHECK(math::distance(t2, derivative(integral(t2)), lo, hi) < threshold);
        // CHECK(math::distance(t3, derivative(integral(t3)), lo, hi) < threshold);
    }
}


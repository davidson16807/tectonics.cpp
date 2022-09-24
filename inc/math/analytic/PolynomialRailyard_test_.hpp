#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/analytic/PolynomialRailyard.hpp>  
#include <math/analytic/PolynomialRailyard_to_string.hpp>  

TEST_CASE( "PolynomialRailyard arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi,  p1) 
    }; 

    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    SECTION("s0+s1 must be called repeatedly without changing the output"){

        CHECK(analytic::distance(s0+s0, s0+s0, lo, hi) < threshold );
        CHECK(analytic::distance(s0+s1, s0+s1, lo, hi) < threshold );
        CHECK(analytic::distance(s0+s2, s0+s2, lo, hi) < threshold );
        CHECK(analytic::distance(s0+s3, s0+s3, lo, hi) < threshold );

        CHECK(analytic::distance(s1+s0, s1+s0, lo, hi) < threshold );
        CHECK(analytic::distance(s1+s1, s1+s1, lo, hi) < threshold );
        CHECK(analytic::distance(s1+s2, s1+s2, lo, hi) < threshold );
        CHECK(analytic::distance(s1+s3, s1+s3, lo, hi) < threshold );

        CHECK(analytic::distance(s2+s0, s2+s0, lo, hi) < threshold );
        CHECK(analytic::distance(s2+s1, s2+s1, lo, hi) < threshold );
        CHECK(analytic::distance(s2+s2, s2+s2, lo, hi) < threshold );
        CHECK(analytic::distance(s2+s3, s2+s3, lo, hi) < threshold );

        CHECK(analytic::distance(s3+s0, s3+s0, lo, hi) < threshold );
        CHECK(analytic::distance(s3+s1, s3+s1, lo, hi) < threshold );
        CHECK(analytic::distance(s3+s2, s3+s2, lo, hi) < threshold );
        CHECK(analytic::distance(s3+s3, s3+s3, lo, hi) < threshold );
    }

    SECTION("s0*s1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0*s0, s0*s0, midlo, midhi) < threshold );
        CHECK(analytic::distance(s0*s1, s0*s1, midlo, midhi) < threshold );
        CHECK(analytic::distance(s0*s2, s0*s2, midlo, midhi) < threshold );
        CHECK(analytic::distance(s0*s3, s0*s3, midlo, midhi) < threshold );

        CHECK(analytic::distance(s1*s0, s1*s0, midlo, midhi) < threshold );
        CHECK(analytic::distance(s1*s1, s1*s1, midlo, midhi) < threshold );
        CHECK(analytic::distance(s1*s2, s1*s2, midlo, midhi) < threshold );
        CHECK(analytic::distance(s1*s3, s1*s3, midlo, midhi) < threshold );

        CHECK(analytic::distance(s2*s0, s2*s0, midlo, midhi) < threshold );
        CHECK(analytic::distance(s2*s1, s2*s1, midlo, midhi) < threshold );
        CHECK(analytic::distance(s2*s2, s2*s2, midlo, midhi) < threshold );
        CHECK(analytic::distance(s2*s3, s2*s3, midlo, midhi) < threshold );

        CHECK(analytic::distance(s3*s0, s3*s0, midlo, midhi) < threshold );
        CHECK(analytic::distance(s3*s1, s3*s1, midlo, midhi) < threshold );
        CHECK(analytic::distance(s3*s2, s3*s2, midlo, midhi) < threshold );
        CHECK(analytic::distance(s3*s3, s3*s3, midlo, midhi) < threshold );
    }

    SECTION("s0-s1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0-s0, s0-s0, lo, hi) < threshold );
        CHECK(analytic::distance(s0-s1, s0-s1, lo, hi) < threshold );
        CHECK(analytic::distance(s0-s2, s0-s2, lo, hi) < threshold );
        CHECK(analytic::distance(s0-s3, s0-s3, lo, hi) < threshold );

        CHECK(analytic::distance(s1-s0, s1-s0, lo, hi) < threshold );
        CHECK(analytic::distance(s1-s1, s1-s1, lo, hi) < threshold );
        CHECK(analytic::distance(s1-s2, s1-s2, lo, hi) < threshold );
        CHECK(analytic::distance(s1-s3, s1-s3, lo, hi) < threshold );

        CHECK(analytic::distance(s2-s0, s2-s0, lo, hi) < threshold );
        CHECK(analytic::distance(s2-s1, s2-s1, lo, hi) < threshold );
        CHECK(analytic::distance(s2-s2, s2-s2, lo, hi) < threshold );
        CHECK(analytic::distance(s2-s3, s2-s3, lo, hi) < threshold );

        CHECK(analytic::distance(s3-s0, s3-s0, lo, hi) < threshold );
        CHECK(analytic::distance(s3-s1, s3-s1, lo, hi) < threshold );
        CHECK(analytic::distance(s3-s2, s3-s2, lo, hi) < threshold );
        CHECK(analytic::distance(s3-s3, s3-s3, lo, hi) < threshold );
    }
}

TEST_CASE( "PolynomialRailyard arithmetic identity", "[math]" ) {
    const double threshold = 1e-4;

    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    analytic::Polynomial<double,0,0> zero = analytic::Polynomial<double,0,0>({0.0f});
    analytic::Polynomial<double,0,0> one  = analytic::Polynomial<double,0,0>({1.0f});

    SECTION("s0+I must equal s0"){
        CHECK(analytic::distance(s0+zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0*one , s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-s0, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s1+zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1*one , s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-s1, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s2+zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2*one , s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-s2, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s3+zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3*one , s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-s3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    SECTION("s0+s1 must equal s1+s0"){
        CHECK(analytic::distance(s0+s1, s1+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+s2, s2+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+s3, s3+s0, lo, hi) < threshold);

        CHECK(analytic::distance(s1+s0, s0+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+s2, s2+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+s3, s3+s1, lo, hi) < threshold);

        CHECK(analytic::distance(s2+s0, s0+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+s1, s1+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+s3, s3+s2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+s0, s0+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+s1, s1+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+s2, s2+s3, lo, hi) < threshold);
    }
    SECTION("s0*s1 must equal s1*s0"){
        CHECK(analytic::distance(s0*s1, s1*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*s2, s2*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*s3, s3*s0, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*s0, s0*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*s2, s2*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*s3, s3*s1, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*s0, s0*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*s1, s1*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*s3, s3*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*s0, s0*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*s1, s1*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*s2, s2*s3, midlo, midhi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard arithmetic associativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+s2, s0+(s1+s2), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+s3, s0+(s1+s3), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+s3, s1+(s2+s3), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+s0, s1+(s2+s0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+s0, s2+(s3+s0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+s1, s2+(s3+s1), lo, hi) < threshold);
    }
    SECTION("(s0*s1)*s2 must equal s0*(s1*s2)"){
        CHECK(analytic::distance((s0*s1)*s2, s0*(s1*s2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0*s1)*s3, s0*(s1*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1*s2)*s3, s1*(s2*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1*s2)*s0, s1*(s2*s0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2*s3)*s0, s2*(s3*s0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2*s3)*s1, s2*(s3*s1), midlo, midhi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    SECTION("s0+s1 must equal s1+s0"){
        CHECK(analytic::distance((s0+s1)*s2, s0*s2+s1*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+s1)*s3, s0*s3+s1*s3, midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*s1, s0*s1+s2*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+s2)*s3, s0*s3+s2*s3, midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*s1, s0*s1+s3*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+s3)*s2, s0*s2+s3*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*s0, s1*s0+s2*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+s2)*s3, s1*s3+s2*s3, midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*s0, s1*s0+s3*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+s3)*s2, s1*s2+s3*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*s0, s2*s0+s3*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+s3)*s1, s2*s1+s3*s1, midlo, midhi) < threshold);
    }
}







TEST_CASE( "PolynomialRailyard/scalar arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("s0+k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0+k0, s0+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+k1, s0+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s0+k2, s0+k2, lo, hi) < threshold);

        CHECK(analytic::distance(s1+k0, s1+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s1+k1, s1+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+k2, s1+k2, lo, hi) < threshold);

        CHECK(analytic::distance(s2+k0, s2+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s2+k1, s2+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s2+k2, s2+k2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+k0, s3+k0, lo, hi) < threshold);
        CHECK(analytic::distance(s3+k1, s3+k1, lo, hi) < threshold);
        CHECK(analytic::distance(s3+k2, s3+k2, lo, hi) < threshold);
    }

    SECTION("s0*k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0*k0, s0*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*k1, s0*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*k2, s0*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*k0, s1*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*k1, s1*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*k2, s1*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*k0, s2*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*k1, s2*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*k2, s2*k2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*k0, s3*k0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*k1, s3*k1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*k2, s3*k2, midlo, midhi) < threshold);
    }

    SECTION("s0-k1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0-k0, s0-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-k1, s0-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s0-k2, s0-k2, lo, hi) < threshold);

        CHECK(analytic::distance(s1-k0, s1-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s1-k1, s1-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-k2, s1-k2, lo, hi) < threshold);

        CHECK(analytic::distance(s2-k0, s2-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s2-k1, s2-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s2-k2, s2-k2, lo, hi) < threshold);

        CHECK(analytic::distance(s3-k0, s3-k0, lo, hi) < threshold);
        CHECK(analytic::distance(s3-k1, s3-k1, lo, hi) < threshold);
        CHECK(analytic::distance(s3-k2, s3-k2, lo, hi) < threshold);
    }

    SECTION("s0/k1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(s0/k0, s0/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s0/k1, s0/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s0/k2, s0/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(s1/k0, s1/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s1/k1, s1/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s1/k2, s1/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(s2/k0, s2/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s2/k1, s2/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s2/k2, s2/k2, lo, hi) < threshold);

        // CHECK(analytic::distance(s3/k0, s3/k0, lo, hi) < threshold);
        CHECK(analytic::distance(s3/k1, s3/k1, lo, hi) < threshold);
        CHECK(analytic::distance(s3/k2, s3/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/scalar arithmetic identity", "[math]" ) {
    const double threshold = 1e-4;

    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    double zero(0.0f);
    double one (1.0f);

    SECTION("s0+I must equal s0"){
        CHECK(analytic::distance(s0+zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0*one , s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0/one , s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-s0, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s1+zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1*one , s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1/one , s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-s1, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s2+zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2*one , s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2/one , s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-s2, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s3+zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3*one , s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3/one , s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-s3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/scalar arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("s0+k must equal k+s0"){
        CHECK(analytic::distance(s0+k0, k0+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+k1, k1+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+k2, k2+s0, lo, hi) < threshold);

        CHECK(analytic::distance(s1+k0, k0+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+k1, k1+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+k2, k2+s1, lo, hi) < threshold);

        CHECK(analytic::distance(s2+k0, k0+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+k1, k1+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+k2, k2+s2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+k0, k0+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+k1, k1+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+k2, k2+s3, lo, hi) < threshold);
    }

    SECTION("s0*k must equal k*s0"){
        CHECK(analytic::distance(s0*k0, k0*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*k1, k1*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*k2, k2*s0, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*k0, k0*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*k1, k1*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*k2, k2*s1, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*k0, k0*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*k1, k1*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*k2, k2*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*k0, k0*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*k1, k1*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*k2, k2*s3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/scalar arithmetic associativity", "[math]" ) {
    const double threshold = 1e-4;

    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+k0, s0+(s1+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+k1, s0+(s1+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+k2, s0+(s1+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+k0, s0+(s2+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+k1, s0+(s2+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+k2, s0+(s2+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+k0, s0+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+k1, s0+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+k2, s0+(s3+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+k0, s1+(s2+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+k1, s1+(s2+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+k2, s1+(s2+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+k0, s1+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+k1, s1+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+k2, s1+(s3+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+k0, s2+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+k1, s2+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+k2, s2+(s3+k2), lo, hi) < threshold);
    }

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+k0, s0+(s1+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+k1, s0+(s1+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+k2, s0+(s1+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+k0, s0+(s2+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+k1, s0+(s2+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+k2, s0+(s2+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+k0, s0+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+k1, s0+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+k2, s0+(s3+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+k0, s1+(s2+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+k1, s1+(s2+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+k2, s1+(s2+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+k0, s1+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+k1, s1+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+k2, s1+(s3+k2), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+k0, s2+(s3+k0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+k1, s2+(s3+k1), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+k2, s2+(s3+k2), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/scalar arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;

    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(s0+s1)*k must equal s0*k + s1*k"){
        CHECK(analytic::distance((s0+s1)*k0, (s0*k0+s1*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)*k1, (s0*k1+s1*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)*k2, (s0*k2+s1*k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)*k0, (s0*k0+s2*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)*k1, (s0*k1+s2*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)*k2, (s0*k2+s2*k2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)*k0, (s0*k0+s3*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)*k1, (s0*k1+s3*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)*k2, (s0*k2+s3*k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)*k0, (s1*k0+s2*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)*k1, (s1*k1+s2*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)*k2, (s1*k2+s2*k2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)*k0, (s1*k0+s3*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)*k1, (s1*k1+s3*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)*k2, (s1*k2+s3*k2), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)*k0, (s2*k0+s3*k0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)*k1, (s2*k1+s3*k1), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)*k2, (s2*k2+s3*k2), lo, hi) < threshold);



        CHECK(analytic::distance((s0+k0)*s1, (s0*s1+k0*s1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k1)*s1, (s0*s1+k1*s1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k2)*s1, (s0*s1+k2*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+k0)*s2, (s0*s2+k0*s2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k1)*s2, (s0*s2+k1*s2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k2)*s2, (s0*s2+k2*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+k0)*s3, (s0*s3+k0*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k1)*s3, (s0*s3+k1*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+k2)*s3, (s0*s3+k2*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+k0)*s2, (s1*s2+k0*s2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+k1)*s2, (s1*s2+k1*s2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+k2)*s2, (s1*s2+k2*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+k0)*s3, (s1*s3+k0*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+k1)*s3, (s1*s3+k1*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+k2)*s3, (s1*s3+k2*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+k0)*s3, (s2*s3+k0*s3), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+k1)*s3, (s2*s3+k1*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+k2)*s3, (s2*s3+k2*s3), midlo, midhi) < threshold);

    }
}








TEST_CASE( "PolynomialRailyard/monomial arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Polynomial<double,0,0> m0{2.0f};
    analytic::Polynomial<double,2,2> m1{2.0f};
    analytic::Polynomial<double,-2,-2> m2{2.0f};

    SECTION("s0+m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0+m0, s0+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+m1, s0+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s0+m2, s0+m2, lo, hi) < threshold);

        CHECK(analytic::distance(s1+m0, s1+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s1+m1, s1+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+m2, s1+m2, lo, hi) < threshold);

        CHECK(analytic::distance(s2+m0, s2+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s2+m1, s2+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s2+m2, s2+m2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+m0, s3+m0, lo, hi) < threshold);
        CHECK(analytic::distance(s3+m1, s3+m1, lo, hi) < threshold);
        CHECK(analytic::distance(s3+m2, s3+m2, lo, hi) < threshold);
    }

    SECTION("s0*m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0*m0, s0*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*m1, s0*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*m2, s0*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*m0, s1*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*m1, s1*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*m2, s1*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*m0, s2*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*m1, s2*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*m2, s2*m2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*m0, s3*m0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*m1, s3*m1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*m2, s3*m2, midlo, midhi) < threshold);
    }

    SECTION("s0-m1 must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0-m0, s0-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-m1, s0-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s0-m2, s0-m2, lo, hi) < threshold);

        CHECK(analytic::distance(s1-m0, s1-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s1-m1, s1-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-m2, s1-m2, lo, hi) < threshold);

        CHECK(analytic::distance(s2-m0, s2-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s2-m1, s2-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s2-m2, s2-m2, lo, hi) < threshold);

        CHECK(analytic::distance(s3-m0, s3-m0, lo, hi) < threshold);
        CHECK(analytic::distance(s3-m1, s3-m1, lo, hi) < threshold);
        CHECK(analytic::distance(s3-m2, s3-m2, lo, hi) < threshold);
    }

    SECTION("s0/m1 must be called repeatedly without changing the output"){
        // CHECK(analytic::distance(s0/m0, s0/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s0/m1, s0/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s0/m2, s0/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(s1/m0, s1/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s1/m1, s1/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s1/m2, s1/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(s2/m0, s2/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s2/m1, s2/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s2/m2, s2/m2, lo, hi) < threshold);

        // CHECK(analytic::distance(s3/m0, s3/m0, lo, hi) < threshold);
        CHECK(analytic::distance(s3/m1, s3/m1, lo, hi) < threshold);
        CHECK(analytic::distance(s3/m2, s3/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/monomial arithmetic identity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Polynomial<double,0,0> zero{0.0f};
    analytic::Polynomial<double,0,0> one {1.0f};

    SECTION("s0+I must equal s0"){
        CHECK(analytic::distance(s0+zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0*one , s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0/one , s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-s0, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s1+zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1*one , s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1/one , s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-s1, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s2+zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2*one , s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2/one , s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-s2, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s3+zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3*one , s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3/one , s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-s3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/monomial arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Polynomial<double,0,0> m0{2.0f};
    analytic::Polynomial<double,2,2> m1{2.0f};
    analytic::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("s0+k must equal k+s0"){
        CHECK(analytic::distance(s0+m0, m0+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+m1, m1+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+m2, m2+s0, lo, hi) < threshold);

        CHECK(analytic::distance(s1+m0, m0+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+m1, m1+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+m2, m2+s1, lo, hi) < threshold);

        CHECK(analytic::distance(s2+m0, m0+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+m1, m1+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+m2, m2+s2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+m0, m0+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+m1, m1+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+m2, m2+s3, lo, hi) < threshold);
    }

    SECTION("s0*k must equal k*s0"){
        CHECK(analytic::distance(s0*m0, m0*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*m1, m1*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*m2, m2*s0, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*m0, m0*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*m1, m1*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*m2, m2*s1, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*m0, m0*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*m1, m1*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*m2, m2*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*m0, m0*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*m1, m1*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*m2, m2*s3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/monomial arithmetic associativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Polynomial<double,0,0> m0{2.0f};
    analytic::Polynomial<double,2,2> m1{2.0f};
    analytic::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+m0, s0+(s1+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+m1, s0+(s1+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+m2, s0+(s1+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+m0, s0+(s2+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+m1, s0+(s2+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+m2, s0+(s2+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+m0, s0+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+m1, s0+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+m2, s0+(s3+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+m0, s1+(s2+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+m1, s1+(s2+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+m2, s1+(s2+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+m0, s1+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+m1, s1+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+m2, s1+(s3+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+m0, s2+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+m1, s2+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+m2, s2+(s3+m2), lo, hi) < threshold);
    }

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+m0, s0+(s1+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+m1, s0+(s1+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+m2, s0+(s1+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+m0, s0+(s2+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+m1, s0+(s2+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+m2, s0+(s2+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+m0, s0+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+m1, s0+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+m2, s0+(s3+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+m0, s1+(s2+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+m1, s1+(s2+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+m2, s1+(s2+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+m0, s1+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+m1, s1+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+m2, s1+(s3+m2), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+m0, s2+(s3+m0), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+m1, s2+(s3+m1), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+m2, s2+(s3+m2), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/monomial arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Polynomial<double,0,0> m0{2.0f};
    analytic::Polynomial<double,2,2> m1{2.0f};
    analytic::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("(s0+s1)*k must equal s0*k + s1*k"){
        CHECK(analytic::distance((s0+s1)*m0, (s0*m0+s1*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m0)*s1, (s0*s1+m0*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*m1, (s0*m1+s1*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m1)*s1, (s0*s1+m1*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*m2, (s0*m2+s1*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m2)*s1, (s0*s1+m2*s1), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s2)*m0, (s0*m0+s2*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m0)*s2, (s0*s2+m0*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*m1, (s0*m1+s2*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m1)*s2, (s0*s2+m1*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*m2, (s0*m2+s2*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m2)*s2, (s0*s2+m2*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s3)*m0, (s0*m0+s3*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m0)*s3, (s0*s3+m0*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*m1, (s0*m1+s3*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m1)*s3, (s0*s3+m1*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*m2, (s0*m2+s3*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+m2)*s3, (s0*s3+m2*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s2)*m0, (s1*m0+s2*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m0)*s2, (s1*s2+m0*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*m1, (s1*m1+s2*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m1)*s2, (s1*s2+m1*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*m2, (s1*m2+s2*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m2)*s2, (s1*s2+m2*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s3)*m0, (s1*m0+s3*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m0)*s3, (s1*s3+m0*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*m1, (s1*m1+s3*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m1)*s3, (s1*s3+m1*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*m2, (s1*m2+s3*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+m2)*s3, (s1*s3+m2*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s2+s3)*m0, (s2*m0+s3*m0), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+m0)*s3, (s2*s3+m0*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*m1, (s2*m1+s3*m1), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+m1)*s3, (s2*s3+m1*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*m2, (s2*m2+s3*m2), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+m2)*s3, (s2*s3+m2*s3), midlo, midhi) < threshold);
    }
}










TEST_CASE( "PolynomialRailyard/Shifting arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("s0+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0+f, s0+f, lo, hi) < threshold);
        CHECK(analytic::distance(s0+g, s0+g, lo, hi) < threshold);
        CHECK(analytic::distance(s0+h, s0+h, lo, hi) < threshold);

        CHECK(analytic::distance(s1+f, s1+f, lo, hi) < threshold);
        CHECK(analytic::distance(s1+g, s1+g, lo, hi) < threshold);
        CHECK(analytic::distance(s1+h, s1+h, lo, hi) < threshold);

        CHECK(analytic::distance(s2+f, s2+f, lo, hi) < threshold);
        CHECK(analytic::distance(s2+g, s2+g, lo, hi) < threshold);
        CHECK(analytic::distance(s2+h, s2+h, lo, hi) < threshold);

        CHECK(analytic::distance(s3+f, s3+f, lo, hi) < threshold);
        CHECK(analytic::distance(s3+g, s3+g, lo, hi) < threshold);
        CHECK(analytic::distance(s3+h, s3+h, lo, hi) < threshold);
    }

    SECTION("s0*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0*f, s0*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*g, s0*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*h, s0*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*f, s1*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*g, s1*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*h, s1*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*f, s2*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*g, s2*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*h, s2*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*f, s3*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*g, s3*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*h, s3*h, midlo, midhi) < threshold);
    }

    SECTION("s0-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0-f, s0-f, lo, hi) < threshold);
        CHECK(analytic::distance(s0-g, s0-g, lo, hi) < threshold);
        CHECK(analytic::distance(s0-h, s0-h, lo, hi) < threshold);

        CHECK(analytic::distance(s1-f, s1-f, lo, hi) < threshold);
        CHECK(analytic::distance(s1-g, s1-g, lo, hi) < threshold);
        CHECK(analytic::distance(s1-h, s1-h, lo, hi) < threshold);

        CHECK(analytic::distance(s2-f, s2-f, lo, hi) < threshold);
        CHECK(analytic::distance(s2-g, s2-g, lo, hi) < threshold);
        CHECK(analytic::distance(s2-h, s2-h, lo, hi) < threshold);

        CHECK(analytic::distance(s3-f, s3-f, lo, hi) < threshold);
        CHECK(analytic::distance(s3-g, s3-g, lo, hi) < threshold);
        CHECK(analytic::distance(s3-h, s3-h, lo, hi) < threshold);
    }

    // SECTION("s0/g must be called repeatedly without changing the output"){
    //     // CHECK(analytic::distance(s0/f, s0/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s0/g, s0/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s0/h, s0/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(s1/f, s1/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s1/g, s1/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s1/h, s1/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(s2/f, s2/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s2/g, s2/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s2/h, s2/h, lo, hi) < threshold);

    //     // CHECK(analytic::distance(s3/f, s3/f, lo, hi) < threshold);
    //     CHECK(analytic::distance(s3/g, s3/g, lo, hi) < threshold);
    //     CHECK(analytic::distance(s3/h, s3/h, lo, hi) < threshold);
    // }
}

TEST_CASE( "PolynomialRailyard/Shifting arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("s0+k must equal k+s0"){
        CHECK(analytic::distance(s0+f, f+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+g, g+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+h, h+s0, lo, hi) < threshold);

        CHECK(analytic::distance(s1+f, f+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+g, g+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+h, h+s1, lo, hi) < threshold);

        CHECK(analytic::distance(s2+f, f+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+g, g+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+h, h+s2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+f, f+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+g, g+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+h, h+s3, lo, hi) < threshold);
    }

    SECTION("s0*k must equal k*s0"){
        CHECK(analytic::distance(s0*f, f*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*g, g*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*h, h*s0, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*f, f*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*g, g*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*h, h*s1, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*f, f*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*g, g*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*h, h*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*f, f*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*g, g*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*h, h*s3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+f, s0+(s1+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+g, s0+(s1+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+h, s0+(s1+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+f, s0+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+g, s0+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+h, s0+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+f, s0+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+g, s0+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+h, s0+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+f, s1+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+g, s1+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+h, s1+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+f, s1+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+g, s1+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+h, s1+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+f, s2+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+g, s2+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+h, s2+(s3+h), lo, hi) < threshold);
    }

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+f, s0+(s1+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+g, s0+(s1+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+h, s0+(s1+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+f, s0+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+g, s0+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+h, s0+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+f, s0+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+g, s0+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+h, s0+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+f, s1+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+g, s1+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+h, s1+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+f, s1+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+g, s1+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+h, s1+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+f, s2+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+g, s2+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+h, s2+(s3+h), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Shifting<double> f(2.0f);
    analytic::Shifting<double> g(-2.0f);
    analytic::Shifting<double> h(0.0f);

    SECTION("(s0+s1)*k must equal s0*k + s1*k"){
        CHECK(analytic::distance((s0+s1)*f, (s0*f+s1*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s1, (s0*s1+f*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*g, (s0*g+s1*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s1, (s0*s1+g*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*h, (s0*h+s1*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s1, (s0*s1+h*s1), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s2)*f, (s0*f+s2*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s2, (s0*s2+f*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*g, (s0*g+s2*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s2, (s0*s2+g*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*h, (s0*h+s2*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s2, (s0*s2+h*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s3)*f, (s0*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s3, (s0*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*g, (s0*g+s3*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s3, (s0*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*h, (s0*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s3, (s0*s3+h*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s2)*f, (s1*f+s2*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+f)*s2, (s1*s2+f*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*g, (s1*g+s2*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+g)*s2, (s1*s2+g*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*h, (s1*h+s2*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+h)*s2, (s1*s2+h*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s3)*f, (s1*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+f)*s3, (s1*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*g, (s1*g+s3*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+g)*s3, (s1*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*h, (s1*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+h)*s3, (s1*s3+h*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s2+s3)*f, (s2*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+f)*s3, (s2*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*g, (s2*g+s3*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+g)*s3, (s2*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*h, (s2*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+h)*s3, (s2*s3+h*s3), midlo, midhi) < threshold);
    }
}











TEST_CASE( "PolynomialRailyard/Scaling arithmetic purity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("s0+g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0+f, s0+f, lo, hi) < threshold);
        CHECK(analytic::distance(s0+g, s0+g, lo, hi) < threshold);
        CHECK(analytic::distance(s0+h, s0+h, lo, hi) < threshold);

        CHECK(analytic::distance(s1+f, s1+f, lo, hi) < threshold);
        CHECK(analytic::distance(s1+g, s1+g, lo, hi) < threshold);
        CHECK(analytic::distance(s1+h, s1+h, lo, hi) < threshold);

        CHECK(analytic::distance(s2+f, s2+f, lo, hi) < threshold);
        CHECK(analytic::distance(s2+g, s2+g, lo, hi) < threshold);
        CHECK(analytic::distance(s2+h, s2+h, lo, hi) < threshold);

        CHECK(analytic::distance(s3+f, s3+f, lo, hi) < threshold);
        CHECK(analytic::distance(s3+g, s3+g, lo, hi) < threshold);
        CHECK(analytic::distance(s3+h, s3+h, lo, hi) < threshold);
    }

    SECTION("s0*g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0*f, s0*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*g, s0*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*h, s0*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*f, s1*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*g, s1*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*h, s1*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*f, s2*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*g, s2*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*h, s2*h, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*f, s3*f, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*g, s3*g, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*h, s3*h, midlo, midhi) < threshold);
    }

    SECTION("s0-g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0-f, s0-f, lo, hi) < threshold);
        CHECK(analytic::distance(s0-g, s0-g, lo, hi) < threshold);
        CHECK(analytic::distance(s0-h, s0-h, lo, hi) < threshold);

        CHECK(analytic::distance(s1-f, s1-f, lo, hi) < threshold);
        CHECK(analytic::distance(s1-g, s1-g, lo, hi) < threshold);
        CHECK(analytic::distance(s1-h, s1-h, lo, hi) < threshold);

        CHECK(analytic::distance(s2-f, s2-f, lo, hi) < threshold);
        CHECK(analytic::distance(s2-g, s2-g, lo, hi) < threshold);
        CHECK(analytic::distance(s2-h, s2-h, lo, hi) < threshold);

        CHECK(analytic::distance(s3-f, s3-f, lo, hi) < threshold);
        CHECK(analytic::distance(s3-g, s3-g, lo, hi) < threshold);
        CHECK(analytic::distance(s3-h, s3-h, lo, hi) < threshold);
    }

    SECTION("s0/g must be called repeatedly without changing the output"){
        CHECK(analytic::distance(s0/f, s0/f, lo, hi) < threshold);
        CHECK(analytic::distance(s0/g, s0/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s0/h, s0/h, lo, hi) < threshold);

        CHECK(analytic::distance(s1/f, s1/f, lo, hi) < threshold);
        CHECK(analytic::distance(s1/g, s1/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s1/h, s1/h, lo, hi) < threshold);

        CHECK(analytic::distance(s2/f, s2/f, lo, hi) < threshold);
        CHECK(analytic::distance(s2/g, s2/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s2/h, s2/h, lo, hi) < threshold);

        CHECK(analytic::distance(s3/f, s3/f, lo, hi) < threshold);
        CHECK(analytic::distance(s3/g, s3/g, lo, hi) < threshold);
        // CHECK(analytic::distance(s3/h, s3/h, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/Scaling arithmetic identity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Scaling<double> zero(0.0f);

    SECTION("s0+I must equal s0"){
        CHECK(analytic::distance(s0+zero, s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0-zero, s0, lo, hi) < threshold);
        // CHECK(analytic::distance(s0-s0, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s1+zero, s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1-zero, s1, lo, hi) < threshold);
        // CHECK(analytic::distance(s1-s1, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s2+zero, s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2-zero, s2, lo, hi) < threshold);
        // CHECK(analytic::distance(s2-s2, zero, lo, hi) < threshold);

        CHECK(analytic::distance(s3+zero, s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3-zero, s3, lo, hi) < threshold);
        // CHECK(analytic::distance(s3-s3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "PolynomialRailyard/Scaling arithmetic commutativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("s0+k must equal k+s0"){
        CHECK(analytic::distance(s0+f, f+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+g, g+s0, lo, hi) < threshold);
        CHECK(analytic::distance(s0+h, h+s0, lo, hi) < threshold);

        CHECK(analytic::distance(s1+f, f+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+g, g+s1, lo, hi) < threshold);
        CHECK(analytic::distance(s1+h, h+s1, lo, hi) < threshold);

        CHECK(analytic::distance(s2+f, f+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+g, g+s2, lo, hi) < threshold);
        CHECK(analytic::distance(s2+h, h+s2, lo, hi) < threshold);

        CHECK(analytic::distance(s3+f, f+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+g, g+s3, lo, hi) < threshold);
        CHECK(analytic::distance(s3+h, h+s3, lo, hi) < threshold);
    }

    SECTION("s0*k must equal k*s0"){
        CHECK(analytic::distance(s0*f, f*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*g, g*s0, midlo, midhi) < threshold);
        CHECK(analytic::distance(s0*h, h*s0, midlo, midhi) < threshold);

        CHECK(analytic::distance(s1*f, f*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*g, g*s1, midlo, midhi) < threshold);
        CHECK(analytic::distance(s1*h, h*s1, midlo, midhi) < threshold);

        CHECK(analytic::distance(s2*f, f*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*g, g*s2, midlo, midhi) < threshold);
        CHECK(analytic::distance(s2*h, h*s2, midlo, midhi) < threshold);

        CHECK(analytic::distance(s3*f, f*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*g, g*s3, midlo, midhi) < threshold);
        CHECK(analytic::distance(s3*h, h*s3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+f, s0+(s1+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+g, s0+(s1+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+h, s0+(s1+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+f, s0+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+g, s0+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+h, s0+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+f, s0+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+g, s0+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+h, s0+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+f, s1+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+g, s1+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+h, s1+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+f, s1+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+g, s1+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+h, s1+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+f, s2+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+g, s2+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+h, s2+(s3+h), lo, hi) < threshold);
    }

    SECTION("(s0+s1)+s2 must equal s0+(s1+s2)"){
        CHECK(analytic::distance((s0+s1)+f, s0+(s1+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+g, s0+(s1+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s1)+h, s0+(s1+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s2)+f, s0+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+g, s0+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s2)+h, s0+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s0+s3)+f, s0+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+g, s0+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s0+s3)+h, s0+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s2)+f, s1+(s2+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+g, s1+(s2+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s2)+h, s1+(s2+h), lo, hi) < threshold);

        CHECK(analytic::distance((s1+s3)+f, s1+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+g, s1+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s1+s3)+h, s1+(s3+h), lo, hi) < threshold);

        CHECK(analytic::distance((s2+s3)+f, s2+(s3+f), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+g, s2+(s3+g), lo, hi) < threshold);
        CHECK(analytic::distance((s2+s3)+h, s2+(s3+h), lo, hi) < threshold);
    }
}


TEST_CASE( "PolynomialRailyard/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = 1e-4;
    // `lo*` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1e3;
    const double hi =  1e3;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -1e2;
    const double midhi =  1e2;
    
    using Poly0 = analytic::Polynomial<double,0,2>;
    using Poly1 = analytic::Polynomial<double,-2,1>;
    using Poly2 = analytic::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});
    Poly1 p2 = Poly1({4.0f,3.0f,2.0f,1.0f});
    Poly1 p3 = Poly1({-1.0f,1.0f,-2.0f,2.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;
    using Piece1 = analytic::Railcar<double,Poly1>;
    using Piece2 = analytic::Railcar<double,Poly2>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // laurent polynomials
    analytic::PolynomialRailyard<double,-2,1> s2 = analytic::PolynomialRailyard<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials
    analytic::PolynomialRailyard<double,-2,2> s3 = analytic::PolynomialRailyard<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };
    analytic::Scaling<double> f(2.0f);
    analytic::Scaling<double> g(-2.0f);
    analytic::Scaling<double> h(0.0f);

    SECTION("(s0+s1)*k must equal s0*k + s1*k"){
        CHECK(analytic::distance((s0+s1)*f, (s0*f+s1*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s1, (s0*s1+f*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*g, (s0*g+s1*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s1, (s0*s1+g*s1), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s1)*h, (s0*h+s1*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s1, (s0*s1+h*s1), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s2)*f, (s0*f+s2*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s2, (s0*s2+f*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*g, (s0*g+s2*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s2, (s0*s2+g*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s2)*h, (s0*h+s2*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s2, (s0*s2+h*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s0+s3)*f, (s0*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+f)*s3, (s0*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*g, (s0*g+s3*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+g)*s3, (s0*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s0+s3)*h, (s0*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s0+h)*s3, (s0*s3+h*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s2)*f, (s1*f+s2*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+f)*s2, (s1*s2+f*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*g, (s1*g+s2*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+g)*s2, (s1*s2+g*s2), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s2)*h, (s1*h+s2*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+h)*s2, (s1*s2+h*s2), midlo, midhi) < threshold);


        CHECK(analytic::distance((s1+s3)*f, (s1*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+f)*s3, (s1*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*g, (s1*g+s3*g), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+g)*s3, (s1*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s1+s3)*h, (s1*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s1+h)*s3, (s1*s3+h*s3), midlo, midhi) < threshold);


        CHECK(analytic::distance((s2+s3)*f, (s2*f+s3*f), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+f)*s3, (s2*s3+f*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*g, (s2*g+s3*g), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+g)*s3, (s2*s3+g*s3), midlo, midhi) < threshold);

        CHECK(analytic::distance((s2+s3)*h, (s2*h+s3*h), midlo, midhi) < threshold);
        CHECK(analytic::distance((s2+h)*s3, (s2*s3+h*s3), midlo, midhi) < threshold);
    }
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
    
    using Poly0 = analytic::Polynomial<double,0,2>;

    Poly0 p0 = Poly0({3.0f,2.0f,1.0f});
    Poly0 p1 = Poly0({-1.0f,0.0f,1.0f});

    using Piece0 = analytic::Railcar<double,Poly0>;

    // standard polynomials, no overlap
    analytic::PolynomialRailyard<double,0,2> s0 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, -1.0f, p0), 
        Piece0(1.0f, hi, p1) 
    }; 
    // standard polynomials, with overlap
    analytic::PolynomialRailyard<double,0,2> s1 = analytic::PolynomialRailyard<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    SECTION("the derivative of a function's3 integral must equal the original function"){
        CHECK(analytic::distance(s0, derivative(integral(s0)), lo, hi) < threshold);
        CHECK(analytic::distance(s1, derivative(integral(s1)), lo, hi) < threshold);
        // CHECK(analytic::distance(s2, derivative(integral(s2)), lo, hi) < threshold);
        // CHECK(analytic::distance(s3, derivative(integral(s3)), lo, hi) < threshold);
    }
}

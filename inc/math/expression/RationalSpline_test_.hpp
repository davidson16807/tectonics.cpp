#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/expression/Spline.hpp>  
#include <math/expression/RationalSpline.hpp>  


TEST_CASE( "Rational arithmetic purity", "[math]" ) {
    const double threshold = 1e-7;
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0),
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    SECTION("r0+r1 must be called repeatedly without changing the output"){

        CHECK(math::distance(r0+r0, r0+r0, midlo, midhi) < threshold );
        CHECK(math::distance(r0+r1, r0+r1, midlo, midhi) < threshold );
        CHECK(math::distance(r0+r2, r0+r2, midlo, midhi) < threshold );
        CHECK(math::distance(r0+r3, r0+r3, midlo, midhi) < threshold );

        CHECK(math::distance(r1+r0, r1+r0, midlo, midhi) < threshold );
        CHECK(math::distance(r1+r1, r1+r1, midlo, midhi) < threshold );
        CHECK(math::distance(r1+r2, r1+r2, midlo, midhi) < threshold );
        CHECK(math::distance(r1+r3, r1+r3, midlo, midhi) < threshold );

        CHECK(math::distance(r2+r0, r2+r0, midlo, midhi) < threshold );
        CHECK(math::distance(r2+r1, r2+r1, midlo, midhi) < threshold );
        CHECK(math::distance(r2+r2, r2+r2, midlo, midhi) < threshold );
        CHECK(math::distance(r2+r3, r2+r3, midlo, midhi) < threshold );

        CHECK(math::distance(r3+r0, r3+r0, midlo, midhi) < threshold );
        CHECK(math::distance(r3+r1, r3+r1, midlo, midhi) < threshold );
        CHECK(math::distance(r3+r2, r3+r2, midlo, midhi) < threshold );
        CHECK(math::distance(r3+r3, r3+r3, midlo, midhi) < threshold );
    }

    SECTION("r0*r1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0*r0, r0*r0, midlo, midhi) < threshold );
        CHECK(math::distance(r0*r1, r0*r1, midlo, midhi) < threshold );
        CHECK(math::distance(r0*r2, r0*r2, midlo, midhi) < threshold );
        CHECK(math::distance(r0*r3, r0*r3, midlo, midhi) < threshold );

        CHECK(math::distance(r1*r0, r1*r0, midlo, midhi) < threshold );
        CHECK(math::distance(r1*r1, r1*r1, midlo, midhi) < threshold );
        CHECK(math::distance(r1*r2, r1*r2, midlo, midhi) < threshold );
        CHECK(math::distance(r1*r3, r1*r3, midlo, midhi) < threshold );

        CHECK(math::distance(r2*r0, r2*r0, midlo, midhi) < threshold );
        CHECK(math::distance(r2*r1, r2*r1, midlo, midhi) < threshold );
        CHECK(math::distance(r2*r2, r2*r2, midlo, midhi) < threshold );
        CHECK(math::distance(r2*r3, r2*r3, midlo, midhi) < threshold );

        CHECK(math::distance(r3*r0, r3*r0, midlo, midhi) < threshold );
        CHECK(math::distance(r3*r1, r3*r1, midlo, midhi) < threshold );
        CHECK(math::distance(r3*r2, r3*r2, midlo, midhi) < threshold );
        CHECK(math::distance(r3*r3, r3*r3, midlo, midhi) < threshold );
    }

    SECTION("r0-r1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0-r0, r0-r0, midlo, midhi) < threshold );
        CHECK(math::distance(r0-r1, r0-r1, midlo, midhi) < threshold );
        CHECK(math::distance(r0-r2, r0-r2, midlo, midhi) < threshold );
        CHECK(math::distance(r0-r3, r0-r3, midlo, midhi) < threshold );

        CHECK(math::distance(r1-r0, r1-r0, midlo, midhi) < threshold );
        CHECK(math::distance(r1-r1, r1-r1, midlo, midhi) < threshold );
        CHECK(math::distance(r1-r2, r1-r2, midlo, midhi) < threshold );
        CHECK(math::distance(r1-r3, r1-r3, midlo, midhi) < threshold );

        CHECK(math::distance(r2-r0, r2-r0, midlo, midhi) < threshold );
        CHECK(math::distance(r2-r1, r2-r1, midlo, midhi) < threshold );
        CHECK(math::distance(r2-r2, r2-r2, midlo, midhi) < threshold );
        CHECK(math::distance(r2-r3, r2-r3, midlo, midhi) < threshold );

        CHECK(math::distance(r3-r0, r3-r0, midlo, midhi) < threshold );
        CHECK(math::distance(r3-r1, r3-r1, midlo, midhi) < threshold );
        CHECK(math::distance(r3-r2, r3-r2, midlo, midhi) < threshold );
        CHECK(math::distance(r3-r3, r3-r3, midlo, midhi) < threshold );
    }
}

TEST_CASE( "Rational arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;


    auto zero = math::Rational<double,0,0,0,0>(0.0);
    auto one  = math::Rational<double,0,0,0,0>(1.0);

    SECTION("r0+I must equal r0"){
        CHECK(math::distance(r0+zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0-zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0*one , r0, lo, hi) < threshold);
        CHECK(math::distance(r0-r0, zero, lo, hi) < threshold);

        CHECK(math::distance(r1+zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1-zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1*one , r1, lo, hi) < threshold);
        CHECK(math::distance(r1-r1, zero, lo, hi) < threshold);

        CHECK(math::distance(r2+zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2-zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2*one , r2, lo, hi) < threshold);
        CHECK(math::distance(r2-r2, zero, lo, hi) < threshold);

        CHECK(math::distance(r3+zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3-zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3*one , r3, lo, hi) < threshold);
        CHECK(math::distance(r3-r3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;


    SECTION("r0+r1 must equal r1+r0"){
        CHECK(math::distance(r0+r1, r1+r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0+r2, r2+r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0+r3, r3+r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1+r0, r0+r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1+r2, r2+r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1+r3, r3+r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2+r0, r0+r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2+r1, r1+r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2+r3, r3+r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3+r0, r0+r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3+r1, r1+r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3+r2, r2+r3, midlo, midhi) < threshold);
    }
    SECTION("r0*r1 must equal r1*r0"){
        CHECK(math::distance(r0*r1, r1*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*r2, r2*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*r3, r3*r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1*r0, r0*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*r2, r2*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*r3, r3*r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2*r0, r0*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*r1, r1*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*r3, r3*r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3*r0, r0*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*r1, r1*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*r2, r2*r3, midlo, midhi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;


    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+r2, r0+(r1+r2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+r3, r0+(r1+r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+r3, r1+(r2+r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+r0, r1+(r2+r0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+r0, r2+(r3+r0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+r1, r2+(r3+r1), midlo, midhi) < threshold);
    }
    SECTION("(r0*r1)*r2 must equal r0*(r1*r2)"){
        CHECK(math::distance((r0*r1)*r2, r0*(r1*r2), midlo, midhi) < threshold);
        CHECK(math::distance((r0*r1)*r3, r0*(r1*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1*r2)*r3, r1*(r2*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1*r2)*r0, r1*(r2*r0), midlo, midhi) < threshold);
        CHECK(math::distance((r2*r3)*r0, r2*(r3*r0), midlo, midhi) < threshold);
        CHECK(math::distance((r2*r3)*r1, r2*(r3*r1), midlo, midhi) < threshold);
    }
}

TEST_CASE( "Rational arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;


    SECTION("r0+r1 must equal r1+r0"){
        CHECK(math::distance((r0+r1)*r2, r0*r2+r1*r2, midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)*r3, r0*r3+r1*r3, midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*r1, r0*r1+r2*r1, midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)*r3, r0*r3+r2*r3, midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*r1, r0*r1+r3*r1, midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)*r2, r0*r2+r3*r2, midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*r0, r1*r0+r2*r0, midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)*r3, r1*r3+r2*r3, midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*r0, r1*r0+r3*r0, midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)*r2, r1*r2+r3*r2, midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*r0, r2*r0+r3*r0, midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)*r1, r2*r1+r3*r1, midlo, midhi) < threshold);
    }
}







TEST_CASE( "Rational/scalar arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -30.0f;
    const double hi =  30.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("r0+k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0+k0, r0+k0, lo, hi) < threshold);
        CHECK(math::distance(r0+k1, r0+k1, lo, hi) < threshold);
        CHECK(math::distance(r0+k2, r0+k2, lo, hi) < threshold);

        CHECK(math::distance(r1+k0, r1+k0, lo, hi) < threshold);
        CHECK(math::distance(r1+k1, r1+k1, lo, hi) < threshold);
        CHECK(math::distance(r1+k2, r1+k2, lo, hi) < threshold);

        CHECK(math::distance(r2+k0, r2+k0, lo, hi) < threshold);
        CHECK(math::distance(r2+k1, r2+k1, lo, hi) < threshold);
        CHECK(math::distance(r2+k2, r2+k2, lo, hi) < threshold);

        CHECK(math::distance(r3+k0, r3+k0, lo, hi) < threshold);
        CHECK(math::distance(r3+k1, r3+k1, lo, hi) < threshold);
        CHECK(math::distance(r3+k2, r3+k2, lo, hi) < threshold);
    }

    SECTION("r0*k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0*k0, r0*k0, lo, hi) < threshold);
        CHECK(math::distance(r0*k1, r0*k1, lo, hi) < threshold);
        CHECK(math::distance(r0*k2, r0*k2, lo, hi) < threshold);

        CHECK(math::distance(r1*k0, r1*k0, lo, hi) < threshold);
        CHECK(math::distance(r1*k1, r1*k1, lo, hi) < threshold);
        CHECK(math::distance(r1*k2, r1*k2, lo, hi) < threshold);

        CHECK(math::distance(r2*k0, r2*k0, lo, hi) < threshold);
        CHECK(math::distance(r2*k1, r2*k1, lo, hi) < threshold);
        CHECK(math::distance(r2*k2, r2*k2, lo, hi) < threshold);

        CHECK(math::distance(r3*k0, r3*k0, lo, hi) < threshold);
        CHECK(math::distance(r3*k1, r3*k1, lo, hi) < threshold);
        CHECK(math::distance(r3*k2, r3*k2, lo, hi) < threshold);
    }

    SECTION("r0-k1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0-k0, r0-k0, lo, hi) < threshold);
        CHECK(math::distance(r0-k1, r0-k1, lo, hi) < threshold);
        CHECK(math::distance(r0-k2, r0-k2, lo, hi) < threshold);

        CHECK(math::distance(r1-k0, r1-k0, lo, hi) < threshold);
        CHECK(math::distance(r1-k1, r1-k1, lo, hi) < threshold);
        CHECK(math::distance(r1-k2, r1-k2, lo, hi) < threshold);

        CHECK(math::distance(r2-k0, r2-k0, lo, hi) < threshold);
        CHECK(math::distance(r2-k1, r2-k1, lo, hi) < threshold);
        CHECK(math::distance(r2-k2, r2-k2, lo, hi) < threshold);

        CHECK(math::distance(r3-k0, r3-k0, lo, hi) < threshold);
        CHECK(math::distance(r3-k1, r3-k1, lo, hi) < threshold);
        CHECK(math::distance(r3-k2, r3-k2, lo, hi) < threshold);
    }

    SECTION("r0/k1 must be called repeatedly without changing the output"){
        // CHECK(math::distance(r0/k0, r0/k0, lo, hi) < threshold);
        CHECK(math::distance(r0/k1, r0/k1, lo, hi) < threshold);
        CHECK(math::distance(r0/k2, r0/k2, lo, hi) < threshold);

        // CHECK(math::distance(r1/k0, r1/k0, lo, hi) < threshold);
        CHECK(math::distance(r1/k1, r1/k1, lo, hi) < threshold);
        CHECK(math::distance(r1/k2, r1/k2, lo, hi) < threshold);

        // CHECK(math::distance(r2/k0, r2/k0, lo, hi) < threshold);
        CHECK(math::distance(r2/k1, r2/k1, lo, hi) < threshold);
        CHECK(math::distance(r2/k2, r2/k2, lo, hi) < threshold);

        // CHECK(math::distance(r3/k0, r3/k0, lo, hi) < threshold);
        CHECK(math::distance(r3/k1, r3/k1, lo, hi) < threshold);
        CHECK(math::distance(r3/k2, r3/k2, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/scalar arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    double zero(0.0f);
    double one (1.0);

    SECTION("r0+I must equal r0"){
        CHECK(math::distance(r0+zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0-zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0*one , r0, lo, hi) < threshold);
        CHECK(math::distance(r0/one , r0, lo, hi) < threshold);
        CHECK(math::distance(r0-r0, zero, lo, hi) < threshold);

        CHECK(math::distance(r1+zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1-zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1*one , r1, lo, hi) < threshold);
        CHECK(math::distance(r1/one , r1, lo, hi) < threshold);
        CHECK(math::distance(r1-r1, zero, lo, hi) < threshold);

        CHECK(math::distance(r2+zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2-zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2*one , r2, lo, hi) < threshold);
        CHECK(math::distance(r2/one , r2, lo, hi) < threshold);
        CHECK(math::distance(r2-r2, zero, lo, hi) < threshold);

        CHECK(math::distance(r3+zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3-zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3*one , r3, lo, hi) < threshold);
        CHECK(math::distance(r3/one , r3, lo, hi) < threshold);
        CHECK(math::distance(r3-r3, zero, lo, hi) < threshold);
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
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("r0+k must equal k+r0"){
        CHECK(math::distance(r0+k0, k0+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+k1, k1+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+k2, k2+r0, lo, hi) < threshold);

        CHECK(math::distance(r1+k0, k0+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+k1, k1+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+k2, k2+r1, lo, hi) < threshold);

        CHECK(math::distance(r2+k0, k0+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+k1, k1+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+k2, k2+r2, lo, hi) < threshold);

        CHECK(math::distance(r3+k0, k0+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+k1, k1+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+k2, k2+r3, lo, hi) < threshold);
    }

    SECTION("r0*k must equal k*r0"){
        CHECK(math::distance(r0*k0, k0*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*k1, k1*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*k2, k2*r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1*k0, k0*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*k1, k1*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*k2, k2*r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2*k0, k0*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*k1, k1*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*k2, k2*r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3*k0, k0*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*k1, k1*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*k2, k2*r3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/scalar arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+k0, r0+(r1+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+k1, r0+(r1+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+k2, r0+(r1+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+k0, r0+(r2+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+k1, r0+(r2+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+k2, r0+(r2+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+k0, r0+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+k1, r0+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+k2, r0+(r3+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+k0, r1+(r2+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+k1, r1+(r2+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+k2, r1+(r2+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+k0, r1+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+k1, r1+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+k2, r1+(r3+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+k0, r2+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+k1, r2+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+k2, r2+(r3+k2), midlo, midhi) < threshold);
    }

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+k0, r0+(r1+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+k1, r0+(r1+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+k2, r0+(r1+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+k0, r0+(r2+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+k1, r0+(r2+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+k2, r0+(r2+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+k0, r0+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+k1, r0+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+k2, r0+(r3+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+k0, r1+(r2+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+k1, r1+(r2+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+k2, r1+(r2+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+k0, r1+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+k1, r1+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+k2, r1+(r3+k2), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+k0, r2+(r3+k0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+k1, r2+(r3+k1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+k2, r2+(r3+k2), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/scalar arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;

    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    double k0(0.0f);
    double k1(2.0f);
    double k2(-2.0f);

    SECTION("(r0+r1)*k must equal r0*k + r1*k"){
        CHECK(math::distance((r0+r1)*k0, (r0*k0+r1*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)*k1, (r0*k1+r1*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)*k2, (r0*k2+r1*k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*k0, (r0*k0+r2*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)*k1, (r0*k1+r2*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)*k2, (r0*k2+r2*k2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*k0, (r0*k0+r3*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)*k1, (r0*k1+r3*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)*k2, (r0*k2+r3*k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*k0, (r1*k0+r2*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)*k1, (r1*k1+r2*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)*k2, (r1*k2+r2*k2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*k0, (r1*k0+r3*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)*k1, (r1*k1+r3*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)*k2, (r1*k2+r3*k2), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*k0, (r2*k0+r3*k0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)*k1, (r2*k1+r3*k1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)*k2, (r2*k2+r3*k2), midlo, midhi) < threshold);



        CHECK(math::distance((r0+k0)*r1, (r0*r1+k0*r1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k1)*r1, (r0*r1+k1*r1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k2)*r1, (r0*r1+k2*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+k0)*r2, (r0*r2+k0*r2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k1)*r2, (r0*r2+k1*r2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k2)*r2, (r0*r2+k2*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+k0)*r3, (r0*r3+k0*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k1)*r3, (r0*r3+k1*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r0+k2)*r3, (r0*r3+k2*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+k0)*r2, (r1*r2+k0*r2), midlo, midhi) < threshold);
        CHECK(math::distance((r1+k1)*r2, (r1*r2+k1*r2), midlo, midhi) < threshold);
        CHECK(math::distance((r1+k2)*r2, (r1*r2+k2*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+k0)*r3, (r1*r3+k0*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1+k1)*r3, (r1*r3+k1*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r1+k2)*r3, (r1*r3+k2*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+k0)*r3, (r2*r3+k0*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r2+k1)*r3, (r2*r3+k1*r3), midlo, midhi) < threshold);
        CHECK(math::distance((r2+k2)*r3, (r2*r3+k2*r3), midlo, midhi) < threshold);

    }
}








TEST_CASE( "Rational/monomial arithmetic purity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double lo = -30.0f;
    const double hi =  30.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Polynomial<double,0,0> m0{2.0f};
    math::Polynomial<double,2,2> m1{2.0f};
    math::Polynomial<double,-2,-2> m2{2.0f};

    SECTION("r0+m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0+m0, r0+m0, lo, hi) < threshold);
        CHECK(math::distance(r0+m1, r0+m1, lo, hi) < threshold);
        CHECK(math::distance(r0+m2, r0+m2, lo, hi) < threshold);

        CHECK(math::distance(r1+m0, r1+m0, lo, hi) < threshold);
        CHECK(math::distance(r1+m1, r1+m1, lo, hi) < threshold);
        CHECK(math::distance(r1+m2, r1+m2, lo, hi) < threshold);

        CHECK(math::distance(r2+m0, r2+m0, lo, hi) < threshold);
        CHECK(math::distance(r2+m1, r2+m1, lo, hi) < threshold);
        CHECK(math::distance(r2+m2, r2+m2, lo, hi) < threshold);

        CHECK(math::distance(r3+m0, r3+m0, lo, hi) < threshold);
        CHECK(math::distance(r3+m1, r3+m1, lo, hi) < threshold);
        CHECK(math::distance(r3+m2, r3+m2, lo, hi) < threshold);
    }

    SECTION("r0*m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0*m0, r0*m0, lo, hi) < threshold);
        CHECK(math::distance(r0*m1, r0*m1, lo, hi) < threshold);
        CHECK(math::distance(r0*m2, r0*m2, lo, hi) < threshold);

        CHECK(math::distance(r1*m0, r1*m0, lo, hi) < threshold);
        CHECK(math::distance(r1*m1, r1*m1, lo, hi) < threshold);
        CHECK(math::distance(r1*m2, r1*m2, lo, hi) < threshold);

        CHECK(math::distance(r2*m0, r2*m0, lo, hi) < threshold);
        CHECK(math::distance(r2*m1, r2*m1, lo, hi) < threshold);
        CHECK(math::distance(r2*m2, r2*m2, lo, hi) < threshold);

        CHECK(math::distance(r3*m0, r3*m0, lo, hi) < threshold);
        CHECK(math::distance(r3*m1, r3*m1, lo, hi) < threshold);
        CHECK(math::distance(r3*m2, r3*m2, lo, hi) < threshold);
    }

    SECTION("r0-m1 must be called repeatedly without changing the output"){
        CHECK(math::distance(r0-m0, r0-m0, lo, hi) < threshold);
        CHECK(math::distance(r0-m1, r0-m1, lo, hi) < threshold);
        CHECK(math::distance(r0-m2, r0-m2, lo, hi) < threshold);

        CHECK(math::distance(r1-m0, r1-m0, lo, hi) < threshold);
        CHECK(math::distance(r1-m1, r1-m1, lo, hi) < threshold);
        CHECK(math::distance(r1-m2, r1-m2, lo, hi) < threshold);

        CHECK(math::distance(r2-m0, r2-m0, lo, hi) < threshold);
        CHECK(math::distance(r2-m1, r2-m1, lo, hi) < threshold);
        CHECK(math::distance(r2-m2, r2-m2, lo, hi) < threshold);

        CHECK(math::distance(r3-m0, r3-m0, lo, hi) < threshold);
        CHECK(math::distance(r3-m1, r3-m1, lo, hi) < threshold);
        CHECK(math::distance(r3-m2, r3-m2, lo, hi) < threshold);
    }

    SECTION("r0/m1 must be called repeatedly without changing the output"){
        // CHECK(math::distance(r0/m0, r0/m0, lo, hi) < threshold);
        CHECK(math::distance(r0/m1, r0/m1, lo, hi) < threshold);
        CHECK(math::distance(r0/m2, r0/m2, lo, hi) < threshold);

        // CHECK(math::distance(r1/m0, r1/m0, lo, hi) < threshold);
        CHECK(math::distance(r1/m1, r1/m1, lo, hi) < threshold);
        CHECK(math::distance(r1/m2, r1/m2, lo, hi) < threshold);

        // CHECK(math::distance(r2/m0, r2/m0, lo, hi) < threshold);
        CHECK(math::distance(r2/m1, r2/m1, lo, hi) < threshold);
        CHECK(math::distance(r2/m2, r2/m2, lo, hi) < threshold);

        // CHECK(math::distance(r3/m0, r3/m0, lo, hi) < threshold);
        CHECK(math::distance(r3/m1, r3/m1, lo, hi) < threshold);
        CHECK(math::distance(r3/m2, r3/m2, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/monomial arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Polynomial<double,0,0> zero{0.0f};
    math::Polynomial<double,0,0> one {1.0};

    SECTION("r0+I must equal r0"){
        CHECK(math::distance(r0+zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0-zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0*one , r0, lo, hi) < threshold);
        CHECK(math::distance(r0/one , r0, lo, hi) < threshold);
        CHECK(math::distance(r0-r0, zero, lo, hi) < threshold);

        CHECK(math::distance(r1+zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1-zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1*one , r1, lo, hi) < threshold);
        CHECK(math::distance(r1/one , r1, lo, hi) < threshold);
        CHECK(math::distance(r1-r1, zero, lo, hi) < threshold);

        CHECK(math::distance(r2+zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2-zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2*one , r2, lo, hi) < threshold);
        CHECK(math::distance(r2/one , r2, lo, hi) < threshold);
        CHECK(math::distance(r2-r2, zero, lo, hi) < threshold);

        CHECK(math::distance(r3+zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3-zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3*one , r3, lo, hi) < threshold);
        CHECK(math::distance(r3/one , r3, lo, hi) < threshold);
        CHECK(math::distance(r3-r3, zero, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/monomial arithmetic commutativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Polynomial<double,0,0> m0{2.0f};
    math::Polynomial<double,2,2> m1{2.0f};
    math::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("r0+k must equal k+r0"){
        CHECK(math::distance(r0+m0, m0+r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0+m1, m1+r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0+m2, m2+r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1+m0, m0+r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1+m1, m1+r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1+m2, m2+r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2+m0, m0+r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2+m1, m1+r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2+m2, m2+r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3+m0, m0+r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3+m1, m1+r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3+m2, m2+r3, midlo, midhi) < threshold);
    }

    SECTION("r0*k must equal k*r0"){
        CHECK(math::distance(r0*m0, m0*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*m1, m1*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*m2, m2*r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1*m0, m0*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*m1, m1*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*m2, m2*r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2*m0, m0*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*m1, m1*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*m2, m2*r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3*m0, m0*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*m1, m1*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*m2, m2*r3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/monomial arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Polynomial<double,0,0> m0{2.0f};
    math::Polynomial<double,2,2> m1{2.0f};
    math::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+m0, r0+(r1+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+m1, r0+(r1+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+m2, r0+(r1+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+m0, r0+(r2+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+m1, r0+(r2+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+m2, r0+(r2+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+m0, r0+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+m1, r0+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+m2, r0+(r3+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+m0, r1+(r2+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+m1, r1+(r2+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+m2, r1+(r2+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+m0, r1+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+m1, r1+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+m2, r1+(r3+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+m0, r2+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+m1, r2+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+m2, r2+(r3+m2), midlo, midhi) < threshold);
    }

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+m0, r0+(r1+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+m1, r0+(r1+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+m2, r0+(r1+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+m0, r0+(r2+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+m1, r0+(r2+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+m2, r0+(r2+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+m0, r0+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+m1, r0+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+m2, r0+(r3+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+m0, r1+(r2+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+m1, r1+(r2+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+m2, r1+(r2+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+m0, r1+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+m1, r1+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+m2, r1+(r3+m2), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+m0, r2+(r3+m0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+m1, r2+(r3+m1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+m2, r2+(r3+m2), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/monomial arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Polynomial<double,0,0> m0{2.0f};
    math::Polynomial<double,2,2> m1{2.0f};
    math::Polynomial<double,-2,-2> m2{-2.0f};

    SECTION("(r0+r1)*k must equal r0*k + r1*k"){
        CHECK(math::distance((r0+r1)*m0, (r0*m0+r1*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m0)*r1, (r0*r1+m0*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*m1, (r0*m1+r1*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m1)*r1, (r0*r1+m1*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*m2, (r0*m2+r1*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m2)*r1, (r0*r1+m2*r1), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r2)*m0, (r0*m0+r2*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m0)*r2, (r0*r2+m0*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*m1, (r0*m1+r2*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m1)*r2, (r0*r2+m1*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*m2, (r0*m2+r2*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m2)*r2, (r0*r2+m2*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r3)*m0, (r0*m0+r3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m0)*r3, (r0*r3+m0*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*m1, (r0*m1+r3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m1)*r3, (r0*r3+m1*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*m2, (r0*m2+r3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r0+m2)*r3, (r0*r3+m2*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r2)*m0, (r1*m0+r2*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m0)*r2, (r1*r2+m0*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*m1, (r1*m1+r2*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m1)*r2, (r1*r2+m1*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*m2, (r1*m2+r2*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m2)*r2, (r1*r2+m2*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r3)*m0, (r1*m0+r3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m0)*r3, (r1*r3+m0*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*m1, (r1*m1+r3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m1)*r3, (r1*r3+m1*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*m2, (r1*m2+r3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r1+m2)*r3, (r1*r3+m2*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r2+r3)*m0, (r2*m0+r3*m0), midlo, midhi) < threshold);
        CHECK(math::distance((r2+m0)*r3, (r2*r3+m0*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*m1, (r2*m1+r3*m1), midlo, midhi) < threshold);
        CHECK(math::distance((r2+m1)*r3, (r2*r3+m1*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*m2, (r2*m2+r3*m2), midlo, midhi) < threshold);
        CHECK(math::distance((r2+m2)*r3, (r2*r3+m2*r3), midlo, midhi) < threshold);
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
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("r0+g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0+f, r0+f, lo, hi) < threshold);
        CHECK(math::distance(r0+g, r0+g, lo, hi) < threshold);
        CHECK(math::distance(r0+h, r0+h, lo, hi) < threshold);

        CHECK(math::distance(r1+f, r1+f, lo, hi) < threshold);
        CHECK(math::distance(r1+g, r1+g, lo, hi) < threshold);
        CHECK(math::distance(r1+h, r1+h, lo, hi) < threshold);

        CHECK(math::distance(r2+f, r2+f, lo, hi) < threshold);
        CHECK(math::distance(r2+g, r2+g, lo, hi) < threshold);
        CHECK(math::distance(r2+h, r2+h, lo, hi) < threshold);

        CHECK(math::distance(r3+f, r3+f, lo, hi) < threshold);
        CHECK(math::distance(r3+g, r3+g, lo, hi) < threshold);
        CHECK(math::distance(r3+h, r3+h, lo, hi) < threshold);
    }

    SECTION("r0*g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0*f, r0*f, midlo, midhi) < threshold);
        CHECK(math::distance(r0*g, r0*g, midlo, midhi) < threshold);
        CHECK(math::distance(r0*h, r0*h, midlo, midhi) < threshold);

        CHECK(math::distance(r1*f, r1*f, midlo, midhi) < threshold);
        CHECK(math::distance(r1*g, r1*g, midlo, midhi) < threshold);
        CHECK(math::distance(r1*h, r1*h, midlo, midhi) < threshold);

        CHECK(math::distance(r2*f, r2*f, midlo, midhi) < threshold);
        CHECK(math::distance(r2*g, r2*g, midlo, midhi) < threshold);
        CHECK(math::distance(r2*h, r2*h, midlo, midhi) < threshold);

        CHECK(math::distance(r3*f, r3*f, midlo, midhi) < threshold);
        CHECK(math::distance(r3*g, r3*g, midlo, midhi) < threshold);
        CHECK(math::distance(r3*h, r3*h, midlo, midhi) < threshold);
    }

    SECTION("r0-g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0-f, r0-f, lo, hi) < threshold);
        CHECK(math::distance(r0-g, r0-g, lo, hi) < threshold);
        CHECK(math::distance(r0-h, r0-h, lo, hi) < threshold);

        CHECK(math::distance(r1-f, r1-f, lo, hi) < threshold);
        CHECK(math::distance(r1-g, r1-g, lo, hi) < threshold);
        CHECK(math::distance(r1-h, r1-h, lo, hi) < threshold);

        CHECK(math::distance(r2-f, r2-f, lo, hi) < threshold);
        CHECK(math::distance(r2-g, r2-g, lo, hi) < threshold);
        CHECK(math::distance(r2-h, r2-h, lo, hi) < threshold);

        CHECK(math::distance(r3-f, r3-f, lo, hi) < threshold);
        CHECK(math::distance(r3-g, r3-g, lo, hi) < threshold);
        CHECK(math::distance(r3-h, r3-h, lo, hi) < threshold);
    }

    // SECTION("r0/g must be called repeatedly without changing the output"){
    //     // CHECK(math::distance(r0/f, r0/f, lo, hi) < threshold);
    //     CHECK(math::distance(r0/g, r0/g, lo, hi) < threshold);
    //     CHECK(math::distance(r0/h, r0/h, lo, hi) < threshold);

    //     // CHECK(math::distance(r1/f, r1/f, lo, hi) < threshold);
    //     CHECK(math::distance(r1/g, r1/g, lo, hi) < threshold);
    //     CHECK(math::distance(r1/h, r1/h, lo, hi) < threshold);

    //     // CHECK(math::distance(r2/f, r2/f, lo, hi) < threshold);
    //     CHECK(math::distance(r2/g, r2/g, lo, hi) < threshold);
    //     CHECK(math::distance(r2/h, r2/h, lo, hi) < threshold);

    //     // CHECK(math::distance(r3/f, r3/f, lo, hi) < threshold);
    //     CHECK(math::distance(r3/g, r3/g, lo, hi) < threshold);
    //     CHECK(math::distance(r3/h, r3/h, lo, hi) < threshold);
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
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("r0+k must equal k+r0"){
        CHECK(math::distance(r0+f, f+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+g, g+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+h, h+r0, lo, hi) < threshold);

        CHECK(math::distance(r1+f, f+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+g, g+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+h, h+r1, lo, hi) < threshold);

        CHECK(math::distance(r2+f, f+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+g, g+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+h, h+r2, lo, hi) < threshold);

        CHECK(math::distance(r3+f, f+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+g, g+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+h, h+r3, lo, hi) < threshold);
    }

    SECTION("r0*k must equal k*r0"){
        CHECK(math::distance(r0*f, f*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*g, g*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*h, h*r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1*f, f*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*g, g*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*h, h*r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2*f, f*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*g, g*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*h, h*r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3*f, f*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*g, g*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*h, h*r3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+f, r0+(r1+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+g, r0+(r1+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+h, r0+(r1+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+f, r0+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+g, r0+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+h, r0+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+f, r0+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+g, r0+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+h, r0+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+f, r1+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+g, r1+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+h, r1+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+f, r1+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+g, r1+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+h, r1+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+f, r2+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+g, r2+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+h, r2+(r3+h), midlo, midhi) < threshold);
    }

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+f, r0+(r1+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+g, r0+(r1+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+h, r0+(r1+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+f, r0+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+g, r0+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+h, r0+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+f, r0+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+g, r0+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+h, r0+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+f, r1+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+g, r1+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+h, r1+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+f, r1+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+g, r1+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+h, r1+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+f, r2+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+g, r2+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+h, r2+(r3+h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("(r0+r1)*k must equal r0*k + r1*k"){
        CHECK(math::distance((r0+r1)*f, (r0*f+r1*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r1, (r0*r1+f*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*g, (r0*g+r1*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r1, (r0*r1+g*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*h, (r0*h+r1*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r1, (r0*r1+h*r1), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r2)*f, (r0*f+r2*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r2, (r0*r2+f*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*g, (r0*g+r2*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r2, (r0*r2+g*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*h, (r0*h+r2*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r2, (r0*r2+h*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r3)*f, (r0*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r3, (r0*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*g, (r0*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r3, (r0*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*h, (r0*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r3, (r0*r3+h*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r2)*f, (r1*f+r2*f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+f)*r2, (r1*r2+f*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*g, (r1*g+r2*g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+g)*r2, (r1*r2+g*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*h, (r1*h+r2*h), midlo, midhi) < threshold);
        CHECK(math::distance((r1+h)*r2, (r1*r2+h*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r3)*f, (r1*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+f)*r3, (r1*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*g, (r1*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+g)*r3, (r1*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*h, (r1*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r1+h)*r3, (r1*r3+h*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r2+r3)*f, (r2*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+f)*r3, (r2*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*g, (r2*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+g)*r3, (r2*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*h, (r2*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r2+h)*r3, (r2*r3+h*r3), midlo, midhi) < threshold);
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
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("r0+g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0+f, r0+f, lo, hi) < threshold);
        CHECK(math::distance(r0+g, r0+g, lo, hi) < threshold);
        CHECK(math::distance(r0+h, r0+h, lo, hi) < threshold);

        CHECK(math::distance(r1+f, r1+f, lo, hi) < threshold);
        CHECK(math::distance(r1+g, r1+g, lo, hi) < threshold);
        CHECK(math::distance(r1+h, r1+h, lo, hi) < threshold);

        CHECK(math::distance(r2+f, r2+f, lo, hi) < threshold);
        CHECK(math::distance(r2+g, r2+g, lo, hi) < threshold);
        CHECK(math::distance(r2+h, r2+h, lo, hi) < threshold);

        CHECK(math::distance(r3+f, r3+f, lo, hi) < threshold);
        CHECK(math::distance(r3+g, r3+g, lo, hi) < threshold);
        CHECK(math::distance(r3+h, r3+h, lo, hi) < threshold);
    }

    SECTION("r0*g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0*f, r0*f, midlo, midhi) < threshold);
        CHECK(math::distance(r0*g, r0*g, midlo, midhi) < threshold);
        CHECK(math::distance(r0*h, r0*h, midlo, midhi) < threshold);

        CHECK(math::distance(r1*f, r1*f, midlo, midhi) < threshold);
        CHECK(math::distance(r1*g, r1*g, midlo, midhi) < threshold);
        CHECK(math::distance(r1*h, r1*h, midlo, midhi) < threshold);

        CHECK(math::distance(r2*f, r2*f, midlo, midhi) < threshold);
        CHECK(math::distance(r2*g, r2*g, midlo, midhi) < threshold);
        CHECK(math::distance(r2*h, r2*h, midlo, midhi) < threshold);

        CHECK(math::distance(r3*f, r3*f, midlo, midhi) < threshold);
        CHECK(math::distance(r3*g, r3*g, midlo, midhi) < threshold);
        CHECK(math::distance(r3*h, r3*h, midlo, midhi) < threshold);
    }

    SECTION("r0-g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0-f, r0-f, lo, hi) < threshold);
        CHECK(math::distance(r0-g, r0-g, lo, hi) < threshold);
        CHECK(math::distance(r0-h, r0-h, lo, hi) < threshold);

        CHECK(math::distance(r1-f, r1-f, lo, hi) < threshold);
        CHECK(math::distance(r1-g, r1-g, lo, hi) < threshold);
        CHECK(math::distance(r1-h, r1-h, lo, hi) < threshold);

        CHECK(math::distance(r2-f, r2-f, lo, hi) < threshold);
        CHECK(math::distance(r2-g, r2-g, lo, hi) < threshold);
        CHECK(math::distance(r2-h, r2-h, lo, hi) < threshold);

        CHECK(math::distance(r3-f, r3-f, lo, hi) < threshold);
        CHECK(math::distance(r3-g, r3-g, lo, hi) < threshold);
        CHECK(math::distance(r3-h, r3-h, lo, hi) < threshold);
    }

    SECTION("r0/g must be called repeatedly without changing the output"){
        CHECK(math::distance(r0/f, r0/f, lo, hi) < threshold);
        CHECK(math::distance(r0/g, r0/g, lo, hi) < threshold);
        // CHECK(math::distance(r0/h, r0/h, lo, hi) < threshold);

        CHECK(math::distance(r1/f, r1/f, lo, hi) < threshold);
        CHECK(math::distance(r1/g, r1/g, lo, hi) < threshold);
        // CHECK(math::distance(r1/h, r1/h, lo, hi) < threshold);

        CHECK(math::distance(r2/f, r2/f, lo, hi) < threshold);
        CHECK(math::distance(r2/g, r2/g, lo, hi) < threshold);
        // CHECK(math::distance(r2/h, r2/h, lo, hi) < threshold);

        CHECK(math::distance(r3/f, r3/f, lo, hi) < threshold);
        CHECK(math::distance(r3/g, r3/g, lo, hi) < threshold);
        // CHECK(math::distance(r3/h, r3/h, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/Scaling arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Scaling<double> zero(0.0f);

    SECTION("r0+I must equal r0"){
        CHECK(math::distance(r0+zero, r0, lo, hi) < threshold);
        CHECK(math::distance(r0-zero, r0, lo, hi) < threshold);
        // CHECK(math::distance(r0-r0, zero, lo, hi) < threshold);

        CHECK(math::distance(r1+zero, r1, lo, hi) < threshold);
        CHECK(math::distance(r1-zero, r1, lo, hi) < threshold);
        // CHECK(math::distance(r1-r1, zero, lo, hi) < threshold);

        CHECK(math::distance(r2+zero, r2, lo, hi) < threshold);
        CHECK(math::distance(r2-zero, r2, lo, hi) < threshold);
        // CHECK(math::distance(r2-r2, zero, lo, hi) < threshold);

        CHECK(math::distance(r3+zero, r3, lo, hi) < threshold);
        CHECK(math::distance(r3-zero, r3, lo, hi) < threshold);
        // CHECK(math::distance(r3-r3, zero, lo, hi) < threshold);
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
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("r0+k must equal k+r0"){
        CHECK(math::distance(r0+f, f+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+g, g+r0, lo, hi) < threshold);
        CHECK(math::distance(r0+h, h+r0, lo, hi) < threshold);

        CHECK(math::distance(r1+f, f+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+g, g+r1, lo, hi) < threshold);
        CHECK(math::distance(r1+h, h+r1, lo, hi) < threshold);

        CHECK(math::distance(r2+f, f+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+g, g+r2, lo, hi) < threshold);
        CHECK(math::distance(r2+h, h+r2, lo, hi) < threshold);

        CHECK(math::distance(r3+f, f+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+g, g+r3, lo, hi) < threshold);
        CHECK(math::distance(r3+h, h+r3, lo, hi) < threshold);
    }

    SECTION("r0*k must equal k*r0"){
        CHECK(math::distance(r0*f, f*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*g, g*r0, midlo, midhi) < threshold);
        CHECK(math::distance(r0*h, h*r0, midlo, midhi) < threshold);

        CHECK(math::distance(r1*f, f*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*g, g*r1, midlo, midhi) < threshold);
        CHECK(math::distance(r1*h, h*r1, midlo, midhi) < threshold);

        CHECK(math::distance(r2*f, f*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*g, g*r2, midlo, midhi) < threshold);
        CHECK(math::distance(r2*h, h*r2, midlo, midhi) < threshold);

        CHECK(math::distance(r3*f, f*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*g, g*r3, midlo, midhi) < threshold);
        CHECK(math::distance(r3*h, h*r3, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+f, r0+(r1+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+g, r0+(r1+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+h, r0+(r1+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+f, r0+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+g, r0+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+h, r0+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+f, r0+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+g, r0+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+h, r0+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+f, r1+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+g, r1+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+h, r1+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+f, r1+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+g, r1+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+h, r1+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+f, r2+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+g, r2+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+h, r2+(r3+h), midlo, midhi) < threshold);
    }

    SECTION("(r0+r1)+r2 must equal r0+(r1+r2)"){
        CHECK(math::distance((r0+r1)+f, r0+(r1+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+g, r0+(r1+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r1)+h, r0+(r1+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)+f, r0+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+g, r0+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r2)+h, r0+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)+f, r0+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+g, r0+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+r3)+h, r0+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)+f, r1+(r2+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+g, r1+(r2+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r2)+h, r1+(r2+h), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)+f, r1+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+g, r1+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+r3)+h, r1+(r3+h), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)+f, r2+(r3+f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+g, r2+(r3+g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+r3)+h, r2+(r3+h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `lo/hi` variables are used as bounds to a square integral 
    // that is used to calculate deviation from the correct output.
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    using Poly0 = math::Polynomial<double,0,2>;
    using Poly1 = math::Polynomial<double,-2,1>;
    using Poly2 = math::Polynomial<double,-2,2>;

    Poly0 p0 = Poly0{-1.0f,0.0f,1.0f};
    Poly0 p1 = Poly0{3.0f,2.0f,1.0f};
    Poly1 p2 = Poly1{4.0f,3.0f,2.0f,1.0f};
    Poly1 p3 = Poly1{-1.0f,1.0f,-2.0f,2.0f};

    using Piece0 = math::Railcar<double,Poly0>;
    using Piece1 = math::Railcar<double,Poly1>;
    using Piece2 = math::Railcar<double,Poly2>;

    // standard polynomials, with zeros, no overlaps
    math::Spline<double,0,2> s0 = math::Spline<double,0,2>{
        Piece0(lo, midhi, p0), 
        Piece0(midlo, hi, p1) 
    }; 
    // standard polynomials, no zeros, with overlaps
    math::Spline<double,0,2> s1 = math::Spline<double,0,2>{
        Piece0(lo, midlo, p0),
        Piece0(midlo, midhi, p1), 
        Piece0(midhi, hi, p0) 
    }; 

    // laurent polynomials, with zeros
    math::Spline<double,-2,1> s2 = math::Spline<double,-2,1>{ 
        Piece1(midlo, -1.0f, p2), 
        Piece1(1.0f, midhi, p3) 
    }; 
    // all polynomials, with zeros
    math::Spline<double,-2,2> s3 = math::Spline<double,-2,2>{ 
        Piece2(lo, midlo, Poly2(p2)), 
        Piece2(midhi, hi, Poly2(p3)), 
        Piece2(midlo, -1.0f, Poly2(p2)), 
        Piece2(1.0f, midhi, Poly2(p3)) 
    };

    auto r0 = s0/s1;
    auto r1 = s1/s1;
    auto r2 = s2/s1;
    auto r3 = s3/s1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("(r0+r1)*k must equal r0*k + r1*k"){
        CHECK(math::distance((r0+r1)*f, (r0*f+r1*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r1, (r0*r1+f*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*g, (r0*g+r1*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r1, (r0*r1+g*r1), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r1)*h, (r0*h+r1*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r1, (r0*r1+h*r1), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r2)*f, (r0*f+r2*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r2, (r0*r2+f*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*g, (r0*g+r2*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r2, (r0*r2+g*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r2)*h, (r0*h+r2*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r2, (r0*r2+h*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r0+r3)*f, (r0*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r0+f)*r3, (r0*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*g, (r0*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r0+g)*r3, (r0*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r0+r3)*h, (r0*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r0+h)*r3, (r0*r3+h*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r2)*f, (r1*f+r2*f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+f)*r2, (r1*r2+f*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*g, (r1*g+r2*g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+g)*r2, (r1*r2+g*r2), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r2)*h, (r1*h+r2*h), midlo, midhi) < threshold);
        CHECK(math::distance((r1+h)*r2, (r1*r2+h*r2), midlo, midhi) < threshold);


        CHECK(math::distance((r1+r3)*f, (r1*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r1+f)*r3, (r1*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*g, (r1*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r1+g)*r3, (r1*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r1+r3)*h, (r1*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r1+h)*r3, (r1*r3+h*r3), midlo, midhi) < threshold);


        CHECK(math::distance((r2+r3)*f, (r2*f+r3*f), midlo, midhi) < threshold);
        CHECK(math::distance((r2+f)*r3, (r2*r3+f*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*g, (r2*g+r3*g), midlo, midhi) < threshold);
        CHECK(math::distance((r2+g)*r3, (r2*r3+g*r3), midlo, midhi) < threshold);

        CHECK(math::distance((r2+r3)*h, (r2*h+r3*h), midlo, midhi) < threshold);
        CHECK(math::distance((r2+h)*r3, (r2*r3+h*r3), midlo, midhi) < threshold);
    }
}







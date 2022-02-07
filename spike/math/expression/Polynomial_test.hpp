#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/expression/Polynomial.hpp>  


TEST_CASE( "Polynomial arithmetic purity", "[many]" ) {
    const float float_precision_of_error = 1e-7;
    const float lo = -3e3;
    const float hi =  3e3;

    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};


    SECTION("a+b must be called repeatedly without changing the output"){
        CHECK(!std::isnan(a(hi)));
        CHECK(!std::isnan(a(lo)));
        CHECK(!std::isnan(integral(a, lo, hi)));
        CHECK(!std::isnan(math::mean_square_difference(a, a, lo, hi)) );
        CHECK(!std::isnan(math::mean_square_difference((c+2.0f)*d, (c*d+2.0f*d), lo, hi)) );

        CHECK(math::mean_square_difference(a+a, a+a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a+b, a+b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a+c, a+c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a+d, a+d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(b+a, b+a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b+b, b+b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b+c, b+c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b+d, b+d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(c+a, c+a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c+b, c+b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c+c, c+c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c+d, c+d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(d+a, d+a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d+b, d+b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d+c, d+c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d+d, d+d, lo, hi) < float_precision_of_error );
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(a*a, a*a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a*b, a*b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a*c, a*c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a*d, a*d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(b*a, b*a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b*b, b*b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b*c, b*c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b*d, b*d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(c*a, c*a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c*b, c*b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c*c, c*c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c*d, c*d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(d*a, d*a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d*b, d*b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d*c, d*c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d*d, d*d, lo, hi) < float_precision_of_error );
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(a-a, a-a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a-b, a-b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a-c, a-c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(a-d, a-d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(b-a, b-a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b-b, b-b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b-c, b-c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(b-d, b-d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(c-a, c-a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c-b, c-b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c-c, c-c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(c-d, c-d, lo, hi) < float_precision_of_error );

        CHECK(math::mean_square_difference(d-a, d-a, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d-b, d-b, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d-c, d-c, lo, hi) < float_precision_of_error );
        CHECK(math::mean_square_difference(d-d, d-d, lo, hi) < float_precision_of_error );
    }
}

TEST_CASE( "Polynomial arithmetic identity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;

    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    math::Polynomial<0,0> zero = math::Polynomial<0,0>{0.0f};
    math::Polynomial<0,0> one  = math::Polynomial<0,0>{1.0f};

    SECTION("a+I must equal a"){
        CHECK(math::mean_square_difference(a+zero, a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-zero, a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*one , a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-a, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b+zero, b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-zero, b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*one , b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-b, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c+zero, c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-zero, c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*one , c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-c, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d+zero, d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-zero, d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*one , d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-d, zero, lo, hi) < float_precision_of_error);
    }
}

TEST_CASE( "Polynomial arithmetic commutativity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;

    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("a+b must equal b+a"){
        CHECK(math::mean_square_difference(a+b, b+a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+c, c+a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+d, d+a, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b+a, a+b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+c, c+b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+d, d+b, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c+a, a+c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+b, b+c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+d, d+c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d+a, a+d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+b, b+d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+c, c+d, lo, hi) < float_precision_of_error);
    }
    SECTION("a*b must equal b*a"){
        CHECK(math::mean_square_difference(a*b, b*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*c, c*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*d, d*a, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b*a, a*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*c, c*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*d, d*b, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c*a, a*c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*b, b*c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*d, d*c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d*a, a*d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*b, b*d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*c, c*d, lo, hi) < float_precision_of_error);
    }
}

TEST_CASE( "Polynomial arithmetic associativity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;

    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK(math::mean_square_difference((a+b)+c, a+(b+c), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)+d, a+(b+d), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+d, b+(c+d), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+a, b+(c+a), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+a, c+(d+a), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+b, c+(d+b), lo, hi) < float_precision_of_error);
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK(math::mean_square_difference((a*b)*c, a*(b*c), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a*b)*d, a*(b*d), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b*c)*d, b*(c*d), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b*c)*a, b*(c*a), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c*d)*a, c*(d*a), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c*d)*b, c*(d*b), lo, hi) < float_precision_of_error);
    }
}

TEST_CASE( "Polynomial arithmetic distributivity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;

    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};

    SECTION("a+b must equal b+a"){
        CHECK(math::mean_square_difference((a+b)*c, a*c+b*c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)*d, a*d+b*d, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+c)*b, a*b+c*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+c)*d, a*d+c*d, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+d)*b, a*b+d*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+d)*c, a*c+d*c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+c)*a, b*a+c*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)*d, b*d+c*d, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+d)*a, b*a+d*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+d)*c, b*c+d*c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((c+d)*a, c*a+d*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)*b, c*b+d*b, lo, hi) < float_precision_of_error);
    }
}







TEST_CASE( "Series<T>/scalar arithmetic purity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;
    
    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("a+k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(a+k0, a+k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+k1, a+k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+k2, a+k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b+k0, b+k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+k1, b+k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+k2, b+k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c+k0, c+k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+k1, c+k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+k2, c+k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d+k0, d+k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+k1, d+k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+k2, d+k2, lo, hi) < float_precision_of_error);
    }

    SECTION("a*k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(a*k0, a*k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*k1, a*k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*k2, a*k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b*k0, b*k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*k1, b*k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*k2, b*k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c*k0, c*k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*k1, c*k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*k2, c*k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d*k0, d*k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*k1, d*k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*k2, d*k2, lo, hi) < float_precision_of_error);
    }

    SECTION("a-k1 must be called repeatedly without changing the output"){
        CHECK(math::mean_square_difference(a-k0, a-k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-k1, a-k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-k2, a-k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b-k0, b-k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-k1, b-k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-k2, b-k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c-k0, c-k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-k1, c-k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-k2, c-k2, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d-k0, d-k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-k1, d-k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-k2, d-k2, lo, hi) < float_precision_of_error);
    }

    SECTION("a/k1 must be called repeatedly without changing the output"){
        // CHECK(math::mean_square_difference(a/k0, a/k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a/k1, a/k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a/k2, a/k2, lo, hi) < float_precision_of_error);

        // CHECK(math::mean_square_difference(b/k0, b/k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b/k1, b/k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b/k2, b/k2, lo, hi) < float_precision_of_error);

        // CHECK(math::mean_square_difference(c/k0, c/k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c/k1, c/k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c/k2, c/k2, lo, hi) < float_precision_of_error);

        // CHECK(math::mean_square_difference(d/k0, d/k0, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d/k1, d/k1, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d/k2, d/k2, lo, hi) < float_precision_of_error);
    }
}

TEST_CASE( "Series<T>/scalar arithmetic identity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;
    
    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float zero(0.0f);
    float one (1.0f);

    SECTION("a+I must equal a"){
        CHECK(math::mean_square_difference(a+zero, a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-zero, a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*one , a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a/one , a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a-a, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b+zero, b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-zero, b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*one , b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b/one , b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b-b, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c+zero, c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-zero, c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*one , c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c/one , c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c-c, zero, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d+zero, d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-zero, d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*one , d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d/one , d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d-d, zero, lo, hi) < float_precision_of_error);
    }
}

TEST_CASE( "Series<T>/scalar arithmetic commutativity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;
    
    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("a+k must equal k+a"){
        CHECK(math::mean_square_difference(a+k0, k0+a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+k1, k1+a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a+k2, k2+a, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b+k0, k0+b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+k1, k1+b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b+k2, k2+b, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c+k0, k0+c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+k1, k1+c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c+k2, k2+c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d+k0, k0+d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+k1, k1+d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d+k2, k2+d, lo, hi) < float_precision_of_error);
    }

    SECTION("a*k must equal k*a"){
        CHECK(math::mean_square_difference(a*k0, k0*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*k1, k1*a, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(a*k2, k2*a, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(b*k0, k0*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*k1, k1*b, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(b*k2, k2*b, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(c*k0, k0*c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*k1, k1*c, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(c*k2, k2*c, lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference(d*k0, k0*d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*k1, k1*d, lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference(d*k2, k2*d, lo, hi) < float_precision_of_error);
    }
}


TEST_CASE( "Series<T>/scalar arithmetic associativity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;
    
    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK(math::mean_square_difference((a+b)+k0, a+(b+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)+k1, a+(b+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)+k2, a+(b+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+c)+k0, a+(c+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+c)+k1, a+(c+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+c)+k2, a+(c+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+d)+k0, a+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+d)+k1, a+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+d)+k2, a+(d+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+c)+k0, b+(c+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+k1, b+(c+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+k2, b+(c+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+d)+k0, b+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+d)+k1, b+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+d)+k2, b+(d+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((c+d)+k0, c+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+k1, c+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+k2, c+(d+k2), lo, hi) < float_precision_of_error);
    }

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK(math::mean_square_difference((a+b)+k0, a+(b+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)+k1, a+(b+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+b)+k2, a+(b+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+c)+k0, a+(c+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+c)+k1, a+(c+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+c)+k2, a+(c+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+d)+k0, a+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+d)+k1, a+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+d)+k2, a+(d+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+c)+k0, b+(c+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+k1, b+(c+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+c)+k2, b+(c+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+d)+k0, b+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+d)+k1, b+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+d)+k2, b+(d+k2), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((c+d)+k0, c+(d+k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+k1, c+(d+k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+d)+k2, c+(d+k2), lo, hi) < float_precision_of_error);
    }
}


TEST_CASE( "Series<T>/scalar arithmetic distributivity", "[many]" ) {
    const float float_precision_of_error = std::numeric_limits<float>::epsilon();
    const float lo = -3e3;
    const float hi =  3e3;
    
    math::Polynomial<0,4> a = math::Polynomial<0,4>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<0,4> b = math::Polynomial<0,4>{-1.0f,0.0f,1.0f,2.0f,3.0f};
    math::Polynomial<-2,2> c = math::Polynomial<-2,2>{1.0f,2.0f,3.0f,4.0f,5.0f};
    math::Polynomial<-2,2> d = math::Polynomial<-2,2>{-1.0f,1.0f,-2.0f,2.0f,3.0f};
    float k0(0.0f);
    float k1(2.0f);
    float k2(-2.0f);

    SECTION("(a+b)*k must equal a*k + b*k"){
        CHECK(math::mean_square_difference((a+b)*k0, (a*k0+b*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k0)*b, (a*b+k0*b), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+b)*k1, (a*k1+b*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k1)*b, (a*b+k1*b), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+b)*k2, (a*k2+b*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k2)*b, (a*b+k2*b), lo, hi) < float_precision_of_error);


        CHECK(math::mean_square_difference((a+c)*k0, (a*k0+c*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k0)*c, (a*c+k0*c), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+c)*k1, (a*k1+c*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k1)*c, (a*c+k1*c), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+c)*k2, (a*k2+c*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k2)*c, (a*c+k2*c), lo, hi) < float_precision_of_error);


        CHECK(math::mean_square_difference((a+d)*k0, (a*k0+d*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k0)*d, (a*d+k0*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+d)*k1, (a*k1+d*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k1)*d, (a*d+k1*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((a+d)*k2, (a*k2+d*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((a+k2)*d, (a*d+k2*d), lo, hi) < float_precision_of_error);


        CHECK(math::mean_square_difference((b+c)*k0, (b*k0+c*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k0)*c, (b*c+k0*c), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+c)*k1, (b*k1+c*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k1)*c, (b*c+k1*c), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+c)*k2, (b*k2+c*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k2)*c, (b*c+k2*c), lo, hi) < float_precision_of_error);


        CHECK(math::mean_square_difference((b+d)*k0, (b*k0+d*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k0)*d, (b*d+k0*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+d)*k1, (b*k1+d*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k1)*d, (b*d+k1*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((b+d)*k2, (b*k2+d*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((b+k2)*d, (b*d+k2*d), lo, hi) < float_precision_of_error);


        CHECK(math::mean_square_difference((c+d)*k0, (c*k0+d*k0), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+k0)*d, (c*d+k0*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((c+d)*k1, (c*k1+d*k1), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+k1)*d, (c*d+k1*d), lo, hi) < float_precision_of_error);

        CHECK(math::mean_square_difference((c+d)*k2, (c*k2+d*k2), lo, hi) < float_precision_of_error);
        CHECK(math::mean_square_difference((c+k2)*d, (c*d+k2*d), lo, hi) < float_precision_of_error);
    }
}


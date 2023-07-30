#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide r main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/analytic/Rational.hpp>  



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
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("r+g must be called repeatedly without changing the output"){
        CHECK(math::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(math::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(math::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(math::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(math::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(math::distance(s+h, s+h, lo, hi) < threshold);

        CHECK(math::distance(t+f, t+f, lo, hi) < threshold);
        CHECK(math::distance(t+g, t+g, lo, hi) < threshold);
        CHECK(math::distance(t+h, t+h, lo, hi) < threshold);

        CHECK(math::distance(u+f, u+f, lo, hi) < threshold);
        CHECK(math::distance(u+g, u+g, lo, hi) < threshold);
        CHECK(math::distance(u+h, u+h, lo, hi) < threshold);
    }

    SECTION("r*g must be called repeatedly without changing the output"){
        CHECK(math::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(math::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(math::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(math::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(math::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(math::distance(s*h, s*h, midlo, midhi) < threshold);

        CHECK(math::distance(t*f, t*f, midlo, midhi) < threshold);
        CHECK(math::distance(t*g, t*g, midlo, midhi) < threshold);
        CHECK(math::distance(t*h, t*h, midlo, midhi) < threshold);

        CHECK(math::distance(u*f, u*f, midlo, midhi) < threshold);
        CHECK(math::distance(u*g, u*g, midlo, midhi) < threshold);
        CHECK(math::distance(u*h, u*h, midlo, midhi) < threshold);
    }

    SECTION("r-g must be called repeatedly without changing the output"){
        CHECK(math::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(math::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(math::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(math::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(math::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(math::distance(s-h, s-h, lo, hi) < threshold);

        CHECK(math::distance(t-f, t-f, lo, hi) < threshold);
        CHECK(math::distance(t-g, t-g, lo, hi) < threshold);
        CHECK(math::distance(t-h, t-h, lo, hi) < threshold);

        CHECK(math::distance(u-f, u-f, lo, hi) < threshold);
        CHECK(math::distance(u-g, u-g, lo, hi) < threshold);
        CHECK(math::distance(u-h, u-h, lo, hi) < threshold);
    }

    // SECTION("r/g must be called repeatedly without changing the output"){
    //     // CHECK(math::distance(r/f, r/f, lo, hi) < threshold);
    //     CHECK(math::distance(r/g, r/g, lo, hi) < threshold);
    //     CHECK(math::distance(r/h, r/h, lo, hi) < threshold);

    //     // CHECK(math::distance(s/f, s/f, lo, hi) < threshold);
    //     CHECK(math::distance(s/g, s/g, lo, hi) < threshold);
    //     CHECK(math::distance(s/h, s/h, lo, hi) < threshold);

    //     // CHECK(math::distance(t/f, t/f, lo, hi) < threshold);
    //     CHECK(math::distance(t/g, t/g, lo, hi) < threshold);
    //     CHECK(math::distance(t/h, t/h, lo, hi) < threshold);

    //     // CHECK(math::distance(u/f, u/f, lo, hi) < threshold);
    //     CHECK(math::distance(u/g, u/g, lo, hi) < threshold);
    //     CHECK(math::distance(u/h, u/h, lo, hi) < threshold);
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
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("r+k must equal k+r"){
        CHECK(math::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(math::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(math::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(math::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(math::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(math::distance(s+h, h+s, lo, hi) < threshold);

        CHECK(math::distance(t+f, f+t, lo, hi) < threshold);
        CHECK(math::distance(t+g, g+t, lo, hi) < threshold);
        CHECK(math::distance(t+h, h+t, lo, hi) < threshold);

        CHECK(math::distance(u+f, f+u, lo, hi) < threshold);
        CHECK(math::distance(u+g, g+u, lo, hi) < threshold);
        CHECK(math::distance(u+h, h+u, lo, hi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(math::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(math::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(math::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(math::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(math::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(math::distance(s*h, h*s, midlo, midhi) < threshold);

        CHECK(math::distance(t*f, f*t, midlo, midhi) < threshold);
        CHECK(math::distance(t*g, g*t, midlo, midhi) < threshold);
        CHECK(math::distance(t*h, h*t, midlo, midhi) < threshold);

        CHECK(math::distance(u*f, f*u, midlo, midhi) < threshold);
        CHECK(math::distance(u*g, g*u, midlo, midhi) < threshold);
        CHECK(math::distance(u*h, h*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(math::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(math::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(math::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }

    SECTION("(r*s)*t must equal r*(s*t)"){
        CHECK(math::distance((r*s)*f, r*(s*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*s)*g, r*(s*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*s)*h, r*(s*h), midlo, midhi) < threshold);

        CHECK(math::distance((r*t)*f, r*(t*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*t)*g, r*(t*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*t)*h, r*(t*h), midlo, midhi) < threshold);

        CHECK(math::distance((r*u)*f, r*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*u)*g, r*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*u)*h, r*(u*h), midlo, midhi) < threshold);

        CHECK(math::distance((s*t)*f, s*(t*f), midlo, midhi) < threshold);
        CHECK(math::distance((s*t)*g, s*(t*g), midlo, midhi) < threshold);
        CHECK(math::distance((s*t)*h, s*(t*h), midlo, midhi) < threshold);

        CHECK(math::distance((s*u)*f, s*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((s*u)*g, s*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((s*u)*h, s*(u*h), midlo, midhi) < threshold);

        CHECK(math::distance((t*u)*f, t*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((t*u)*g, t*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((t*u)*h, t*(u*h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Shifting arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Shifting<double> f(2.0f);
    math::Shifting<double> g(-2.0f);
    math::Shifting<double> h(0.0f);

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(math::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(math::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(math::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);


        CHECK(math::distance((r+t)*f, (r*f+t*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*t, (r*t+f*t), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)*g, (r*g+t*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*t, (r*t+g*t), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)*h, (r*h+t*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*t, (r*t+h*t), midlo, midhi) < threshold);


        CHECK(math::distance((r+u)*f, (r*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*u, (r*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)*g, (r*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*u, (r*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)*h, (r*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*u, (r*u+h*u), midlo, midhi) < threshold);


        CHECK(math::distance((s+t)*f, (s*f+t*f), midlo, midhi) < threshold);
        CHECK(math::distance((s+f)*t, (s*t+f*t), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)*g, (s*g+t*g), midlo, midhi) < threshold);
        CHECK(math::distance((s+g)*t, (s*t+g*t), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)*h, (s*h+t*h), midlo, midhi) < threshold);
        CHECK(math::distance((s+h)*t, (s*t+h*t), midlo, midhi) < threshold);


        CHECK(math::distance((s+u)*f, (s*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((s+f)*u, (s*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)*g, (s*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((s+g)*u, (s*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)*h, (s*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((s+h)*u, (s*u+h*u), midlo, midhi) < threshold);


        CHECK(math::distance((t+u)*f, (t*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((t+f)*u, (t*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)*g, (t*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((t+g)*u, (t*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)*h, (t*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((t+h)*u, (t*u+h*u), midlo, midhi) < threshold);
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
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("r+g must be called repeatedly without changing the output"){
        CHECK(math::distance(r+f, r+f, lo, hi) < threshold);
        CHECK(math::distance(r+g, r+g, lo, hi) < threshold);
        CHECK(math::distance(r+h, r+h, lo, hi) < threshold);

        CHECK(math::distance(s+f, s+f, lo, hi) < threshold);
        CHECK(math::distance(s+g, s+g, lo, hi) < threshold);
        CHECK(math::distance(s+h, s+h, lo, hi) < threshold);

        CHECK(math::distance(t+f, t+f, lo, hi) < threshold);
        CHECK(math::distance(t+g, t+g, lo, hi) < threshold);
        CHECK(math::distance(t+h, t+h, lo, hi) < threshold);

        CHECK(math::distance(u+f, u+f, lo, hi) < threshold);
        CHECK(math::distance(u+g, u+g, lo, hi) < threshold);
        CHECK(math::distance(u+h, u+h, lo, hi) < threshold);
    }

    SECTION("r*g must be called repeatedly without changing the output"){
        CHECK(math::distance(r*f, r*f, midlo, midhi) < threshold);
        CHECK(math::distance(r*g, r*g, midlo, midhi) < threshold);
        CHECK(math::distance(r*h, r*h, midlo, midhi) < threshold);

        CHECK(math::distance(s*f, s*f, midlo, midhi) < threshold);
        CHECK(math::distance(s*g, s*g, midlo, midhi) < threshold);
        CHECK(math::distance(s*h, s*h, midlo, midhi) < threshold);

        CHECK(math::distance(t*f, t*f, midlo, midhi) < threshold);
        CHECK(math::distance(t*g, t*g, midlo, midhi) < threshold);
        CHECK(math::distance(t*h, t*h, midlo, midhi) < threshold);

        CHECK(math::distance(u*f, u*f, midlo, midhi) < threshold);
        CHECK(math::distance(u*g, u*g, midlo, midhi) < threshold);
        CHECK(math::distance(u*h, u*h, midlo, midhi) < threshold);
    }

    SECTION("r-g must be called repeatedly without changing the output"){
        CHECK(math::distance(r-f, r-f, lo, hi) < threshold);
        CHECK(math::distance(r-g, r-g, lo, hi) < threshold);
        CHECK(math::distance(r-h, r-h, lo, hi) < threshold);

        CHECK(math::distance(s-f, s-f, lo, hi) < threshold);
        CHECK(math::distance(s-g, s-g, lo, hi) < threshold);
        CHECK(math::distance(s-h, s-h, lo, hi) < threshold);

        CHECK(math::distance(t-f, t-f, lo, hi) < threshold);
        CHECK(math::distance(t-g, t-g, lo, hi) < threshold);
        CHECK(math::distance(t-h, t-h, lo, hi) < threshold);

        CHECK(math::distance(u-f, u-f, lo, hi) < threshold);
        CHECK(math::distance(u-g, u-g, lo, hi) < threshold);
        CHECK(math::distance(u-h, u-h, lo, hi) < threshold);
    }

    SECTION("r/g must be called repeatedly without changing the output"){
        CHECK(math::distance(r/f, r/f, lo, hi) < threshold);
        CHECK(math::distance(r/g, r/g, lo, hi) < threshold);
        // CHECK(math::distance(r/h, r/h, lo, hi) < threshold);

        CHECK(math::distance(s/f, s/f, lo, hi) < threshold);
        CHECK(math::distance(s/g, s/g, lo, hi) < threshold);
        // CHECK(math::distance(s/h, s/h, lo, hi) < threshold);

        CHECK(math::distance(t/f, t/f, lo, hi) < threshold);
        CHECK(math::distance(t/g, t/g, lo, hi) < threshold);
        // CHECK(math::distance(t/h, t/h, lo, hi) < threshold);

        CHECK(math::distance(u/f, u/f, lo, hi) < threshold);
        CHECK(math::distance(u/g, u/g, lo, hi) < threshold);
        // CHECK(math::distance(u/h, u/h, lo, hi) < threshold);
    }
}

TEST_CASE( "Rational/Scaling arithmetic identity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double lo = -1000.0f;
    const double hi =  1000.0f;
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Scaling<double> zero(0.0f);

    SECTION("r+I must equal r"){
        CHECK(math::distance(r+zero, r, lo, hi) < threshold);
        CHECK(math::distance(r-zero, r, lo, hi) < threshold);
        // CHECK(math::distance(r-r, zero, lo, hi) < threshold);

        CHECK(math::distance(s+zero, s, lo, hi) < threshold);
        CHECK(math::distance(s-zero, s, lo, hi) < threshold);
        // CHECK(math::distance(s-s, zero, lo, hi) < threshold);

        CHECK(math::distance(t+zero, t, lo, hi) < threshold);
        CHECK(math::distance(t-zero, t, lo, hi) < threshold);
        // CHECK(math::distance(t-t, zero, lo, hi) < threshold);

        CHECK(math::distance(u+zero, u, lo, hi) < threshold);
        CHECK(math::distance(u-zero, u, lo, hi) < threshold);
        // CHECK(math::distance(u-u, zero, lo, hi) < threshold);
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
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("r+k must equal k+r"){
        CHECK(math::distance(r+f, f+r, lo, hi) < threshold);
        CHECK(math::distance(r+g, g+r, lo, hi) < threshold);
        CHECK(math::distance(r+h, h+r, lo, hi) < threshold);

        CHECK(math::distance(s+f, f+s, lo, hi) < threshold);
        CHECK(math::distance(s+g, g+s, lo, hi) < threshold);
        CHECK(math::distance(s+h, h+s, lo, hi) < threshold);

        CHECK(math::distance(t+f, f+t, lo, hi) < threshold);
        CHECK(math::distance(t+g, g+t, lo, hi) < threshold);
        CHECK(math::distance(t+h, h+t, lo, hi) < threshold);

        CHECK(math::distance(u+f, f+u, lo, hi) < threshold);
        CHECK(math::distance(u+g, g+u, lo, hi) < threshold);
        CHECK(math::distance(u+h, h+u, lo, hi) < threshold);
    }

    SECTION("r*k must equal k*r"){
        CHECK(math::distance(r*f, f*r, midlo, midhi) < threshold);
        CHECK(math::distance(r*g, g*r, midlo, midhi) < threshold);
        CHECK(math::distance(r*h, h*r, midlo, midhi) < threshold);

        CHECK(math::distance(s*f, f*s, midlo, midhi) < threshold);
        CHECK(math::distance(s*g, g*s, midlo, midhi) < threshold);
        CHECK(math::distance(s*h, h*s, midlo, midhi) < threshold);

        CHECK(math::distance(t*f, f*t, midlo, midhi) < threshold);
        CHECK(math::distance(t*g, g*t, midlo, midhi) < threshold);
        CHECK(math::distance(t*h, h*t, midlo, midhi) < threshold);

        CHECK(math::distance(u*f, f*u, midlo, midhi) < threshold);
        CHECK(math::distance(u*g, g*u, midlo, midhi) < threshold);
        CHECK(math::distance(u*h, h*u, midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic associativity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("(r+s)+t must equal r+(s+t)"){
        CHECK(math::distance((r+s)+f, r+(s+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+s)+g, r+(s+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+s)+h, r+(s+h), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)+f, r+(t+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+t)+g, r+(t+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+t)+h, r+(t+h), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)+f, r+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((r+u)+g, r+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((r+u)+h, r+(u+h), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)+f, s+(t+f), midlo, midhi) < threshold);
        CHECK(math::distance((s+t)+g, s+(t+g), midlo, midhi) < threshold);
        CHECK(math::distance((s+t)+h, s+(t+h), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)+f, s+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((s+u)+g, s+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((s+u)+h, s+(u+h), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)+f, t+(u+f), midlo, midhi) < threshold);
        CHECK(math::distance((t+u)+g, t+(u+g), midlo, midhi) < threshold);
        CHECK(math::distance((t+u)+h, t+(u+h), midlo, midhi) < threshold);
    }

    SECTION("(r*s)*t must equal r*(s*t)"){
        CHECK(math::distance((r*s)*f, r*(s*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*s)*g, r*(s*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*s)*h, r*(s*h), midlo, midhi) < threshold);

        CHECK(math::distance((r*t)*f, r*(t*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*t)*g, r*(t*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*t)*h, r*(t*h), midlo, midhi) < threshold);

        CHECK(math::distance((r*u)*f, r*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((r*u)*g, r*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((r*u)*h, r*(u*h), midlo, midhi) < threshold);

        CHECK(math::distance((s*t)*f, s*(t*f), midlo, midhi) < threshold);
        CHECK(math::distance((s*t)*g, s*(t*g), midlo, midhi) < threshold);
        CHECK(math::distance((s*t)*h, s*(t*h), midlo, midhi) < threshold);

        CHECK(math::distance((s*u)*f, s*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((s*u)*g, s*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((s*u)*h, s*(u*h), midlo, midhi) < threshold);

        CHECK(math::distance((t*u)*f, t*(u*f), midlo, midhi) < threshold);
        CHECK(math::distance((t*u)*g, t*(u*g), midlo, midhi) < threshold);
        CHECK(math::distance((t*u)*h, t*(u*h), midlo, midhi) < threshold);
    }
}


TEST_CASE( "Rational/Scaling arithmetic distributivity", "[math]" ) {
    const double threshold = std::numeric_limits<double>::epsilon();
    // `mid*` variables are used when the degree of a polynomial is so large 
    // that a square integral of it will produce nans for all but the smallest input.
    const double midlo = -100.0f;
    const double midhi =  100.0f;
    
    math::Polynomial<double,0,4> p0 = math::Polynomial<double,0,4>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,0,4> q0 = math::Polynomial<double,0,4>({-1.0,0.0,1.0,2.0,3.0});
    math::Polynomial<double,-2,2> p1 = math::Polynomial<double,-2,2>({1.0,2.0,3.0,4.0,5.0});
    math::Polynomial<double,-2,2> q1 = math::Polynomial<double,-2,2>({-1.0,1.0,-2.0,2.0,3.0});

    auto r = p0/q0;
    auto s = p1/q0;
    auto t = p0/q1;
    auto u = p1/q1;

    math::Scaling<double> f(2.0f);
    math::Scaling<double> g(-2.0f);
    math::Scaling<double> h(0.0f);

    SECTION("(r+s)*k must equal r*k + s*k"){
        CHECK(math::distance((r+s)*f, (r*f+s*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*s, (r*s+f*s), midlo, midhi) < threshold);

        CHECK(math::distance((r+s)*g, (r*g+s*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*s, (r*s+g*s), midlo, midhi) < threshold);

        CHECK(math::distance((r+s)*h, (r*h+s*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*s, (r*s+h*s), midlo, midhi) < threshold);


        CHECK(math::distance((r+t)*f, (r*f+t*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*t, (r*t+f*t), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)*g, (r*g+t*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*t, (r*t+g*t), midlo, midhi) < threshold);

        CHECK(math::distance((r+t)*h, (r*h+t*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*t, (r*t+h*t), midlo, midhi) < threshold);


        CHECK(math::distance((r+u)*f, (r*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((r+f)*u, (r*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)*g, (r*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((r+g)*u, (r*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((r+u)*h, (r*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((r+h)*u, (r*u+h*u), midlo, midhi) < threshold);


        CHECK(math::distance((s+t)*f, (s*f+t*f), midlo, midhi) < threshold);
        CHECK(math::distance((s+f)*t, (s*t+f*t), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)*g, (s*g+t*g), midlo, midhi) < threshold);
        CHECK(math::distance((s+g)*t, (s*t+g*t), midlo, midhi) < threshold);

        CHECK(math::distance((s+t)*h, (s*h+t*h), midlo, midhi) < threshold);
        CHECK(math::distance((s+h)*t, (s*t+h*t), midlo, midhi) < threshold);


        CHECK(math::distance((s+u)*f, (s*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((s+f)*u, (s*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)*g, (s*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((s+g)*u, (s*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((s+u)*h, (s*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((s+h)*u, (s*u+h*u), midlo, midhi) < threshold);


        CHECK(math::distance((t+u)*f, (t*f+u*f), midlo, midhi) < threshold);
        CHECK(math::distance((t+f)*u, (t*u+f*u), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)*g, (t*g+u*g), midlo, midhi) < threshold);
        CHECK(math::distance((t+g)*u, (t*u+g*u), midlo, midhi) < threshold);

        CHECK(math::distance((t+u)*h, (t*h+u*h), midlo, midhi) < threshold);
        CHECK(math::distance((t+h)*u, (t*u+h*u), midlo, midhi) < threshold);
    }
}







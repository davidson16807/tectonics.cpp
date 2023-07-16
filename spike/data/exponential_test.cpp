

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"


#include "Series.hpp"
#include "arithmetic.hpp"
#include "relational.hpp"
#include "exponential.hpp"


TEST_CASE( "Series<T> sqrt purity", "[data]" ) {
    auto a  = data::series({1.0,2.0,3.0,4.0,5.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        data::sqrt(a,c1);
        data::sqrt(a,c2);
        CHECK(data::equal(c1,c2, 1e-7));
    }
}
TEST_CASE( "Series<T> sqrt consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto sqrt1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto sqrt2 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log1_ = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        data::sqrt(a,    sqrt1);
        data::pow (a,data::uniform(0.5f),sqrt2);
        CHECK(data::equal(sqrt1 , sqrt2, 1e-7));

        data::sqrt(b,    sqrt1);
        data::pow (b,data::uniform(0.5f),sqrt2);
        CHECK(data::equal(sqrt1 , sqrt2, 1e-7));
    }

}
TEST_CASE( "Series<T> log consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto log1_ = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        data::log2(a, log1_);
        data::log (a, log2_);
        data::div (log2_, data::uniform(std::log(2.f)), log2_);
        CHECK(data::equal(log1_ , log2_, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto loga  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto logb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto elalb = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ab    = data::series({0.0,0.0,0.0,0.0,0.0});
    auto out   = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        data::log(a, loga);
        data::log(b, logb);
        data::add(loga, logb, lalb);
        data::exp(lalb, elalb);
        data::mult(a, b, ab);
        CHECK(data::equal(elalb, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log(exp(a)) must equal a"){
        data::log (a,   out);
        data::exp (out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
}

TEST_CASE( "Series<T> log2/exp2 consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto loga  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto logb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ab    = data::series({0.0,0.0,0.0,0.0,0.0});
    auto out   = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        data::log2(a, loga);
        data::log2(b, logb);
        data::add(loga, logb, lalb);
        data::exp2(lalb, out);
        data::mult(a, b, ab);
        CHECK(data::equal(out, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log2/exp2 invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(exp2(a)) must equal a"){
        data::log2 (a,   out);
        data::exp2 (out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
}


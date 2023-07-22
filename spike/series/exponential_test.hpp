

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <series/series.hpp>  
#include <series/common.hpp>  


TEST_CASE( "many<T> sqrt purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        series::sqrt(a,c1);
        series::sqrt(a,c2);
        CHECK(series::equal(c1,c2));
    }
}
TEST_CASE( "many<T> sqrt consistency", "[many]" ) {
    series::floats a     = series::floats({1,2,3,4,5});
    series::floats b     = series::floats({1,1,2,3,5});
    series::floats sqrt1 = series::floats({0,0,0,0,0});
    series::floats sqrt2 = series::floats({0,0,0,0,0});
    series::floats log1_ = series::floats({0,0,0,0,0});
    series::floats log2_ = series::floats({0,0,0,0,0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        series::sqrt(a,    sqrt1);
        series::pow (a,0.5f,sqrt2);
        CHECK(series::equal(sqrt1 , sqrt2));

        sqrt(b,    sqrt1);
        pow (b,0.5f,sqrt2);
        CHECK(series::equal(sqrt1 , sqrt2));
    }

}
TEST_CASE( "many<T> log consistency", "[many]" ) {
    series::floats a     = series::floats({1,2,3,4,5});
    series::floats b     = series::floats({1,1,2,3,5});
    series::floats log1_ = series::floats({0,0,0,0,0});
    series::floats log2_ = series::floats({0,0,0,0,0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        series::log2(a, log1_);
        series::log (a, log2_);
        div (log2_, std::log(2.f), log2_);
        CHECK(series::equal(log1_ , log2_));
    }
}
TEST_CASE( "many<T> log/exp consistency", "[many]" ) {
    series::floats a     = series::floats({1,2,3,4,5});
    series::floats b     = series::floats({1,1,2,3,5});
    series::floats loga  = series::floats({0,0,0,0,0});
    series::floats logb  = series::floats({0,0,0,0,0});
    series::floats out   = series::floats({0,0,0,0,0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        series::log(a, loga);
        series::log(b, logb);
        series::exp(loga+logb, out);
        CHECK(series::equal(out, a*b));
    }
}
TEST_CASE( "many<T> log/exp invertibility", "[many]" ) {
    series::floats a   = series::floats({1,2,3,4,5});
    series::floats b   = series::floats({1,1,2,3,5});
    series::floats out = series::floats({0,0,0,0,0});

    SECTION("log(exp(a)) must equal a"){
        series::log (a,   out);
        series::exp (out, out);
        CHECK(series::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        series::log2(a,   out);
        series::exp2(out, out);
        CHECK(series::equal(a, out));
    }
}

TEST_CASE( "many<T> log2/exp2 consistency", "[many]" ) {
    series::floats a     = series::floats({1,2,3,4,5});
    series::floats b     = series::floats({1,1,2,3,5});
    series::floats loga  = series::floats({0,0,0,0,0});
    series::floats logb  = series::floats({0,0,0,0,0});
    series::floats out   = series::floats({0,0,0,0,0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        series::log2(a, loga);
        series::log2(b, logb);
        series::exp2(loga+logb, out);
        CHECK(series::equal(out, a*b));
    }
}
TEST_CASE( "many<T> log2/exp2 invertibility", "[many]" ) {
    series::floats a   = series::floats({1,2,3,4,5});
    series::floats b   = series::floats({1,1,2,3,5});
    series::floats out = series::floats({0,0,0,0,0});

    SECTION("log2(exp2(a)) must equal a"){
        series::log2 (a,   out);
        series::exp2 (out, out);
        CHECK(series::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        series::log2(a,   out);
        series::exp2(out, out);
        CHECK(series::equal(a, out));
    }
}


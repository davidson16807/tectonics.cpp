

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <many/many.hpp>  
#include <many/common.hpp>  


TEST_CASE( "many<T> sqrt purity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats c1 = many::floats({0,0,0,0,0});
    many::floats c2 = many::floats({0,0,0,0,0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        many::sqrt(a,c1);
        many::sqrt(a,c2);
        CHECK(many::equal(c1,c2));
    }
}
TEST_CASE( "many<T> sqrt consistency", "[many]" ) {
    many::floats a     = many::floats({1,2,3,4,5});
    many::floats b     = many::floats({1,1,2,3,5});
    many::floats sqrt1 = many::floats({0,0,0,0,0});
    many::floats sqrt2 = many::floats({0,0,0,0,0});
    many::floats log1_ = many::floats({0,0,0,0,0});
    many::floats log2_ = many::floats({0,0,0,0,0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        many::sqrt(a,    sqrt1);
        many::pow (a,0.5f,sqrt2);
        CHECK(many::equal(sqrt1 , sqrt2));

        sqrt(b,    sqrt1);
        pow (b,0.5f,sqrt2);
        CHECK(many::equal(sqrt1 , sqrt2));
    }

}
TEST_CASE( "many<T> log consistency", "[many]" ) {
    many::floats a     = many::floats({1,2,3,4,5});
    many::floats b     = many::floats({1,1,2,3,5});
    many::floats log1_ = many::floats({0,0,0,0,0});
    many::floats log2_ = many::floats({0,0,0,0,0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        many::log2(a, log1_);
        many::log (a, log2_);
        div (log2_, std::log(2.f), log2_);
        CHECK(many::equal(log1_ , log2_));
    }
}
TEST_CASE( "many<T> log/exp consistency", "[many]" ) {
    many::floats a     = many::floats({1,2,3,4,5});
    many::floats b     = many::floats({1,1,2,3,5});
    many::floats loga  = many::floats({0,0,0,0,0});
    many::floats logb  = many::floats({0,0,0,0,0});
    many::floats out   = many::floats({0,0,0,0,0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        many::log(a, loga);
        many::log(b, logb);
        many::exp(loga+logb, out);
        CHECK(many::equal(out, a*b));
    }
}
TEST_CASE( "many<T> log/exp invertibility", "[many]" ) {
    many::floats a   = many::floats({1,2,3,4,5});
    many::floats b   = many::floats({1,1,2,3,5});
    many::floats out = many::floats({0,0,0,0,0});

    SECTION("log(exp(a)) must equal a"){
        many::log (a,   out);
        many::exp (out, out);
        CHECK(many::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        many::log2(a,   out);
        many::exp2(out, out);
        CHECK(many::equal(a, out));
    }
}

TEST_CASE( "many<T> log2/exp2 consistency", "[many]" ) {
    many::floats a     = many::floats({1,2,3,4,5});
    many::floats b     = many::floats({1,1,2,3,5});
    many::floats loga  = many::floats({0,0,0,0,0});
    many::floats logb  = many::floats({0,0,0,0,0});
    many::floats out   = many::floats({0,0,0,0,0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        many::log2(a, loga);
        many::log2(b, logb);
        many::exp2(loga+logb, out);
        CHECK(many::equal(out, a*b));
    }
}
TEST_CASE( "many<T> log2/exp2 invertibility", "[many]" ) {
    many::floats a   = many::floats({1,2,3,4,5});
    many::floats b   = many::floats({1,1,2,3,5});
    many::floats out = many::floats({0,0,0,0,0});

    SECTION("log2(exp2(a)) must equal a"){
        many::log2 (a,   out);
        many::exp2 (out, out);
        CHECK(many::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        many::log2(a,   out);
        many::exp2(out, out);
        CHECK(many::equal(a, out));
    }
}


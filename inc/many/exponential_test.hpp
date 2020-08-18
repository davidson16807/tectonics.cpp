

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


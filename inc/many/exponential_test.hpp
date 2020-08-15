

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <many/many.hpp>  
#include <many/common.hpp>  

using namespace many;


TEST_CASE( "many<T> sqrt purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats c1 = floats({0,0,0,0,0});
    floats c2 = floats({0,0,0,0,0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        sqrt(a,c1);
        sqrt(a,c2);
        CHECK(equal(c1,c2));
    }
}
TEST_CASE( "many<T> sqrt consistency", "[many]" ) {
    floats a     = floats({1,2,3,4,5});
    floats b     = floats({1,1,2,3,5});
    floats sqrt1 = floats({0,0,0,0,0});
    floats sqrt2 = floats({0,0,0,0,0});
    floats log1_ = floats({0,0,0,0,0});
    floats log2_ = floats({0,0,0,0,0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        sqrt(a,    sqrt1);
        pow (a,0.5f,sqrt2);
        CHECK(equal(sqrt1 , sqrt2));

        sqrt(b,    sqrt1);
        pow (b,0.5f,sqrt2);
        CHECK(equal(sqrt1 , sqrt2));
    }

}
TEST_CASE( "many<T> log consistency", "[many]" ) {
    floats a     = floats({1,2,3,4,5});
    floats b     = floats({1,1,2,3,5});
    floats log1_ = floats({0,0,0,0,0});
    floats log2_ = floats({0,0,0,0,0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        log2(a, log1_);
        log (a, log2_);
        div (log2_, std::log(2.f), log2_);
        CHECK(equal(log1_ , log2_));
    }
}


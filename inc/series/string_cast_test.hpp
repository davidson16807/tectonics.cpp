

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <series/series.hpp>  
#include <series/string_cast.hpp>  

TEST_CASE( "many<T> string cast purity", "[many]" ) {
    series::floats a   ({1,2,3,4,5});
    SECTION("to_string(a) must be called repeatedly without changing the output"){
        CHECK(series::to_string(a) == series::to_string(a));
    }
}
TEST_CASE( "many<T> string cast correctness", "[many]" ) {
    series::floats a   ({1,2,3,4,5,6});
    series::floats b   ({1,1,2,3,5,8});
    series::floats c   ({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
                       4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});
    std::string stra = series::to_string(a);
    std::string strb = series::to_string(b);
    std::string strc = series::to_string(c);
    SECTION("to_string() must render correct representation"){
        REQUIRE_THAT(stra, Catch::Contains("█"));
        REQUIRE_THAT(strb, Catch::Contains("█"));
        REQUIRE_THAT(series::to_string(series::floats({INFINITY})), Catch::Contains("∞"));
        // REQUIRE_THAT(to_string(series::floats({std::sqrt(-1)})), Catch::Contains("N"));
    }
}

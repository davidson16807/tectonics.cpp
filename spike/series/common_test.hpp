

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <series/types.hpp>  
#include <series/morphologic.hpp>  
#include <series/relational.hpp>  

#include "common.hpp" // diamond_grid

using namespace series;

TEST_CASE( "Series<T> abs purity", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("abs(a) must generate the same output when called repeatedly"){
        abs(a, out1);
        abs(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> sign purity", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("sign(a) must generate the same output when called repeatedly"){
        sign(a, out1);
        sign(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> abs/sign consistency", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats abs_a = series::floats({0,0,0,0,0});
    series::floats sign_a = series::floats({0,0,0,0,0});
    SECTION("sign(a)*abs(a) must reconstruct a"){
        abs(a, abs_a);
        sign(a, sign_a);
        CHECK(equal(sign_a*abs_a, a));
    }
}
TEST_CASE( "Series<T> floor purity", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("floor(a) must generate the same output when called repeatedly"){
        floor(a, out1);
        floor(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> fract purity", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("fract(a) must generate the same output when called repeatedly"){
        fract(a, out1);
        fract(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> floor/fract consistency", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats floor_a = series::floats({0,0,0,0,0});
    series::floats fract_a = series::floats({0,0,0,0,0});
    SECTION("fract(a)+floor(a) must reconstruct a"){
        floor(a, floor_a);
        fract(a, fract_a);
        CHECK(equal(floor_a + fract_a, a));
    }
}

TEST_CASE( "Series<T> mod purity", "[many]" ) {
    series::floats a = series::floats({-1,1,-2,2,3});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("mod(a,b) must generate the same output when called repeatedly"){
        mod(a,b, out1);
        mod(a,b, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> mod/fract consistency", "[many]" ) {
    series::floats a = series::floats({1.0,2.1,3.1,4.2,5.3});
    series::floats ones = series::floats({1,1,1,1,1});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("mod(a,1) must generate the same output as fract(a) for positive numbers"){
        mod(a,1, out1);
        fract(a, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("mod(a,ones) must generate the same output as fract(a) for positive numbers"){
        mod(a,ones, out1);
        fract(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min purity", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("min(a,b) must generate the same output when called repeatedly"){
        min(a,b, out1);
        min(a,b, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min decreasing", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats c = series::floats({10,0,0,0,-10});
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats abc = series::floats({0,0,0,0,0});
    SECTION("sum(min(a,b)) must always return a value less than or equal to the starting value"){
        min(a,b, ab);
        min(ab,c, abc);
        CHECK(sum(ab) >= sum(abc));
    }
}
TEST_CASE( "Series<T> min idempotence", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats abb = series::floats({0,0,0,0,0});
    SECTION("subsequent calls to min(*,b) must return the same value"){
        min(a,b, ab);
        min(ab,b, abb);
        CHECK(equal(ab, abb));
    }
}

TEST_CASE( "Series<T> max purity", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats out1 = series::floats({0,0,0,0,0});
    series::floats out2 = series::floats({0,0,0,0,0});
    SECTION("max(a,b) must generate the same output when called repeatedly"){
        max(a,b, out1);
        max(a,b, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Series<T> max decreasing", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats c = series::floats({10,0,0,0,-10});
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats abc = series::floats({0,0,0,0,0});
    SECTION("sum(max(a,b)) must always return a value greater than or equal to the starting value"){
        max(a,b, ab);
        max(ab,c, abc);
        CHECK(sum(ab) <= sum(abc));
    }
}
TEST_CASE( "Series<T> max idempotence", "[many]" ) {
    series::floats a = series::floats({3,2,-2,1,-1});
    series::floats b = series::floats({1,2,3,4,5});
    series::floats ab = series::floats({0,0,0,0,0});
    series::floats abb = series::floats({0,0,0,0,0});
    SECTION("subsequent calls to max(*,b) must return the same value"){
        max(a,b, ab);
        max(ab,b, abb);
        CHECK(equal(ab, abb));
    }
}

TEST_CASE( "Series<T> min/max/clamp consistency", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats lo = series::floats({-1,1,-2,2,3});
    series::floats hi = series::floats({7,6,5,4,3});
    series::floats alo = series::floats({0,0,0,0,0});
    series::floats alohi1 = series::floats({0,0,0,0,0});
    series::floats alohi2 = series::floats({0,0,0,0,0});
    SECTION("min(max(a,lo),hi) must return the same value as clamp(a,lo,hi)"){
        max(a,lo, alo);
        min(alo,hi, alohi1);
        clamp(a,lo,hi, alohi2);
        CHECK(equal(alohi1, alohi2));
    }
}

/*
TEST_CASE( "Raster dilation purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats b = series::floats({-1,1,-2,2,3});
    series::floats c = series::floats({1,1,2,3,5});
    series::floats d = series::floats({0,0,0,0,0});
    auto upper_half =  series::floats({true,  true,  true,  true,  false });
    auto top_only   =  series::floats({false, false, true,  false, false });
    auto out1       =  series::floats({false, false, false, false, false });
    auto out2       =  series::floats({false, false, false, false, false });
    auto scratch    =  series::floats({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must generate the same output when called repeatedly"){
        dilate(top_only, out1);
        dilate(top_only, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("dilate(grid, top_only, radius) must generate the same output when called repeatedly"){
        dilate(top_only, out1, 2);
        dilate(top_only, out2, 2);
        CHECK(equal(out1, out2));
    }
    SECTION("dilate(grid, top_only, radius, scratch) must generate the same output when called repeatedly"){
        dilate(top_only, out1, 2, scratch);
        dilate(top_only, out2, 2, scratch);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "Raster dilation increasing", "[many]" ) {
    auto upper_half =  series::floats({true,  true,  true,  true,  false });
    auto top_only   =  series::floats({false, false, true,  false, false });
    auto out1       =  series::floats({false, false, false, false, false });
    auto out2       =  series::floats({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must increase the number of flagged vertices"){
        dilate(top_only, out1);
        dilate(out1,     out2);
        CHECK(sum(out1) >= sum(top_only));
        CHECK(sum(out2) >= sum(out1));
    }
}
TEST_CASE( "Raster dilation associative", "[many]" ) {
    auto A   =  series::floats({false, false, true,  false, false });
    auto AB  =  series::floats({false, false, false, false, false });
    auto ABC =  series::floats({false, false, false, false, false });
    auto BC  =  series::floats({false, false, false, false, false });
    auto BCA =  series::floats({false, false, false, false, false });
    SECTION("dilation (A+B)+C must equal A+(B+C) where B and C are circles of radius 1 and 2"){
        dilate(A,  AB,  1);
        dilate(AB, ABC, 2);
        dilate(A,  BCA, 3);
        CHECK(equal(ABC,  BCA));
    }
}
TEST_CASE( "Raster dilation distributive over union", "[many]" ) {
    auto A       =  series::floats({false, false, true,  false, false });
    auto B       =  series::floats({true,  false, true,  false, false });
    auto AC      =  series::floats({false, false, false, false, false });
    auto BC      =  series::floats({false, false, false, false, false });
    auto AB      =  series::floats({false, false, false, false, false });
    auto AB_C    =  series::floats({false, false, false, false, false });
    auto AC_BC   =  series::floats({false, false, false, false, false });
    SECTION("dilation (A ∪ B)+C must equal (A+C) ∪ (B+C)"){
        unite (A,  B,  AB);
        dilate(AB, AB_C, 1);

        dilate(A,   AC,  1);
        dilate(B,   BC,  1);
        unite (AC, BC, AC_BC);

        CHECK(equal(AB_C,  AC_BC));
    }
}
TEST_CASE( "Raster dilation happy path", "[many]" ) {
    auto upper_half =  series::floats({true,  true,  true,  true,  false });
    auto top_only   =  series::floats({false, false, true,  false, false });
    auto empty      =  series::floats({false, false, false, false, false });
    auto full       =  series::floats({true,  true,  true,  true,  true  });
    auto out1       =  series::floats({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must return predictable results"){
        dilate(top_only, out1);
        CHECK(equal(out1,  upper_half));
    }
    SECTION("dilate(grid, upper_half) must return predictable results"){
        dilate(upper_half, out1);
        CHECK(equal(out1,  full));
    }
    SECTION("dilate(grid, empty) must return predictable results"){
        dilate(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("dilate(grid, full) must return predictable results"){
        dilate(full, out1);
        CHECK(equal(out1,  full));
    }
}
*/


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// HACK: order of tests is arbitrary and glm each must be loaded first to permit template specialization
// so we include it here despite not necessarily needing it.
#include "glm/each.hpp"

#include "Series.hpp"
#include "each.hpp"
#include "whole.hpp"

TEST_CASE( "Series<T> abs purity", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("abs(a) must generate the same output when called repeatedly"){
        each::abs(a, out1);
        each::abs(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> sign purity", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("sign(a) must generate the same output when called repeatedly"){
        each::sign(a, out1);
        each::sign(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> abs/sign consistency", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto abs_a = each::series({0,0,0,0,0});
    auto sign_a = each::series({0,0,0,0,0});
    auto a2 = each::series({0,0,0,0,0});
    SECTION("sign(a)*abs(a) must reconstruct a"){
        each::abs(a, abs_a);
        each::sign(a, sign_a);
        each::mult(sign_a, abs_a, a2);
        CHECK(whole::equal(a2, a));
    }
}
TEST_CASE( "Series<T> floor purity", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("floor(a) must generate the same output when called repeatedly"){
        each::floor(a, out1);
        each::floor(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> fract purity", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("fract(a) must generate the same output when called repeatedly"){
        each::fract(a, out1);
        each::fract(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> floor/fract consistency", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto floor_a = each::series({0,0,0,0,0});
    auto fract_a = each::series({0,0,0,0,0});
    auto a2 = each::series({0,0,0,0,0});
    SECTION("fract(a)+floor(a) must reconstruct a"){
        each::floor(a, floor_a);
        each::fract(a, fract_a);
        each::add(floor_a, fract_a, a2);
        CHECK(whole::equal(a2, a));
    }
}

TEST_CASE( "Series<T> mod purity", "[whole]" ) {
    auto a = each::series({-1,1,-2,2,3});
    auto b = each::series({1,2,3,4,5});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("mod(a,b) must generate the same output when called repeatedly"){
        each::mod(a,b, out1);
        each::mod(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> mod/fract consistency", "[whole]" ) {
    auto a = each::series({1.0,2.1,3.1,4.2,5.3});
    auto ones = each::series({1,1,1,1,1});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("mod(a,1) must generate the same output as fract(a) for positive numbers"){
        each::mod(a, each::uniform(1), out1);
        each::fract(a, out2);
        CHECK(whole::equal(out1, out2));
    }
    SECTION("mod(a,ones) must generate the same output as fract(a) for positive numbers"){
        each::mod(a,ones, out1);
        each::fract(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min purity", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("min(a,b) must generate the same output when called repeatedly"){
        each::min(a,b, out1);
        each::min(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min decreasing", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto c = each::series({10,0,0,0,-10});
    auto ab = each::series({0,0,0,0,0});
    auto abc = each::series({0,0,0,0,0});
    SECTION("sum(min(a,b)) must always return a value less than or equal to the starting value"){
        each::min(a,b, ab);
        each::min(ab,c, abc);
        CHECK(whole::sum(ab) >= whole::sum(abc));
    }
}
TEST_CASE( "Series<T> min idempotence", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto ab = each::series({0,0,0,0,0});
    auto abb = each::series({0,0,0,0,0});
    SECTION("subsequent calls to min(*,b) must return the same value"){
        each::min(a,b, ab);
        each::min(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

TEST_CASE( "Series<T> max purity", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto out1 = each::series({0,0,0,0,0});
    auto out2 = each::series({0,0,0,0,0});
    SECTION("max(a,b) must generate the same output when called repeatedly"){
        each::max(a,b, out1);
        each::max(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> max decreasing", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto c = each::series({10,0,0,0,-10});
    auto ab = each::series({0,0,0,0,0});
    auto abc = each::series({0,0,0,0,0});
    SECTION("sum(max(a,b)) must always return a value greater than or equal to the starting value"){
        each::max(a,b, ab);
        each::max(ab,c, abc);
        CHECK(whole::sum(ab) <= whole::sum(abc));
    }
}
TEST_CASE( "Series<T> max idempotence", "[whole]" ) {
    auto a = each::series({3,2,-2,1,-1});
    auto b = each::series({1,2,3,4,5});
    auto ab = each::series({0,0,0,0,0});
    auto abb = each::series({0,0,0,0,0});
    SECTION("subsequent calls to max(*,b) must return the same value"){
        each::max(a,b, ab);
        each::max(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

TEST_CASE( "Series<T> min/max/clamp consistency", "[whole]" ) {
    auto a = each::series({1,2,3,4,5});
    auto lo = each::series({-1,1,-2,2,3});
    auto hi = each::series({7,6,5,4,3});
    auto alo = each::series({0,0,0,0,0});
    auto alohi1 = each::series({0,0,0,0,0});
    auto alohi2 = each::series({0,0,0,0,0});
    SECTION("min(max(a,lo),hi) must return the same value as clamp(a,lo,hi)"){
        each::max(a,lo, alo);
        each::min(alo,hi, alohi1);
        each::clamp(a,lo,hi, alohi2);
        CHECK(whole::equal(alohi1, alohi2));
    }
}





TEST_CASE( "Series<T> string cast purity", "[whole]" ) {
    auto a = each::series({1,2,3,4,5});
    SECTION("to_string(a) must be called repeatedly without changing the output"){
        CHECK(whole::to_string(a) == whole::to_string(a));
    }
}

TEST_CASE( "Series<T> string cast correctness", "[whole]" ) {
    auto a = each::series({1,2,3,4,5,6});
    auto b = each::series({1,1,2,3,5,8});
    auto c = each::series<float>({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                           4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
                           1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                           4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
                           4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                           4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
                           1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                           4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});
    std::string stra = whole::to_string(a);
    std::string strb = whole::to_string(b);
    std::string strc = whole::to_string(c);
    SECTION("to_string() must render correct representation"){
        REQUIRE_THAT(stra, Catch::Contains("█"));
        REQUIRE_THAT(strb, Catch::Contains("█"));
        REQUIRE_THAT(whole::to_string(each::series({INFINITY})), Catch::Contains("∞"));
        REQUIRE_THAT(whole::to_string(each::series({std::sqrt(-1)})), Catch::Contains("N"));
    }
}

TEST_CASE( "Series<T> string cast representation", "[whole]" ) {
    auto a = each::series({1,2,3,4,5,6});
    SECTION("to_string() must capture appreciable differences in the content of Series<T>"){
        for(std::size_t i=0; i<a.size(); ++i){
            auto b = each::series({1,2,3,4,5,6});
            b[i] = -1;
            std::string stra = whole::to_string(a);
            std::string strb = whole::to_string(b);
            CHECK(stra != strb);
        }
    }
    SECTION("to_string() must capture appreciable differences in the length of Series<T>"){
        auto c = each::series({1,2,3,4,5,6,7});
        std::string stra = whole::to_string(a);
        std::string strc = whole::to_string(c);
        CHECK(stra != strc);
    }
}

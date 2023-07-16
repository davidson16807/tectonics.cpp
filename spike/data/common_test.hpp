

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3


#include "Series.hpp"
#include "arithmetic.hpp"
#include "relational.hpp"
#include "common.hpp"
#include "statistic.hpp"

TEST_CASE( "Series<T> abs purity", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("abs(a) must generate the same output when called repeatedly"){
        data::abs(a, out1);
        data::abs(a, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> sign purity", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("sign(a) must generate the same output when called repeatedly"){
        data::sign(a, out1);
        data::sign(a, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> abs/sign consistency", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto abs_a = data::series({0,0,0,0,0});
    auto sign_a = data::series({0,0,0,0,0});
    auto a2 = data::series({0,0,0,0,0});
    SECTION("sign(a)*abs(a) must reconstruct a"){
        data::abs(a, abs_a);
        data::sign(a, sign_a);
        data::mult(sign_a, abs_a, a2);
        CHECK(data::equal(a2, a));
    }
}
TEST_CASE( "Series<T> floor purity", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("floor(a) must generate the same output when called repeatedly"){
        data::floor(a, out1);
        data::floor(a, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> fract purity", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("fract(a) must generate the same output when called repeatedly"){
        data::fract(a, out1);
        data::fract(a, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> floor/fract consistency", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto floor_a = data::series({0,0,0,0,0});
    auto fract_a = data::series({0,0,0,0,0});
    auto a2 = data::series({0,0,0,0,0});
    SECTION("fract(a)+floor(a) must reconstruct a"){
        data::floor(a, floor_a);
        data::fract(a, fract_a);
        data::add(floor_a, fract_a, a2);
        CHECK(data::equal(a2, a));
    }
}

TEST_CASE( "Series<T> mod purity", "[many]" ) {
    auto a = data::series({-1,1,-2,2,3});
    auto b = data::series({1,2,3,4,5});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("mod(a,b) must generate the same output when called repeatedly"){
        data::mod(a,b, out1);
        data::mod(a,b, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> mod/fract consistency", "[many]" ) {
    auto a = data::series({1.0,2.1,3.1,4.2,5.3});
    auto ones = data::series({1,1,1,1,1});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("mod(a,1) must generate the same output as fract(a) for positive numbers"){
        data::mod(a, data::uniform(1), out1);
        data::fract(a, out2);
        CHECK(data::equal(out1, out2));
    }
    SECTION("mod(a,ones) must generate the same output as fract(a) for positive numbers"){
        data::mod(a,ones, out1);
        data::fract(a, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min purity", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("min(a,b) must generate the same output when called repeatedly"){
        data::min(a,b, out1);
        data::min(a,b, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> min decreasing", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto c = data::series({10,0,0,0,-10});
    auto ab = data::series({0,0,0,0,0});
    auto abc = data::series({0,0,0,0,0});
    SECTION("sum(min(a,b)) must always return a value less than or equal to the starting value"){
        data::min(a,b, ab);
        data::min(ab,c, abc);
        CHECK(data::sum(ab) >= data::sum(abc));
    }
}
TEST_CASE( "Series<T> min idempotence", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto ab = data::series({0,0,0,0,0});
    auto abb = data::series({0,0,0,0,0});
    SECTION("subsequent calls to min(*,b) must return the same value"){
        data::min(a,b, ab);
        data::min(ab,b, abb);
        CHECK(data::equal(ab, abb));
    }
}

TEST_CASE( "Series<T> max purity", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto out1 = data::series({0,0,0,0,0});
    auto out2 = data::series({0,0,0,0,0});
    SECTION("max(a,b) must generate the same output when called repeatedly"){
        data::max(a,b, out1);
        data::max(a,b, out2);
        CHECK(data::equal(out1, out2));
    }
}
TEST_CASE( "Series<T> max decreasing", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto c = data::series({10,0,0,0,-10});
    auto ab = data::series({0,0,0,0,0});
    auto abc = data::series({0,0,0,0,0});
    SECTION("sum(max(a,b)) must always return a value greater than or equal to the starting value"){
        data::max(a,b, ab);
        data::max(ab,c, abc);
        CHECK(data::sum(ab) <= data::sum(abc));
    }
}
TEST_CASE( "Series<T> max idempotence", "[many]" ) {
    auto a = data::series({3,2,-2,1,-1});
    auto b = data::series({1,2,3,4,5});
    auto ab = data::series({0,0,0,0,0});
    auto abb = data::series({0,0,0,0,0});
    SECTION("subsequent calls to max(*,b) must return the same value"){
        data::max(a,b, ab);
        data::max(ab,b, abb);
        CHECK(data::equal(ab, abb));
    }
}

TEST_CASE( "Series<T> min/max/clamp consistency", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto lo = data::series({-1,1,-2,2,3});
    auto hi = data::series({7,6,5,4,3});
    auto alo = data::series({0,0,0,0,0});
    auto alohi1 = data::series({0,0,0,0,0});
    auto alohi2 = data::series({0,0,0,0,0});
    SECTION("min(max(a,lo),hi) must return the same value as clamp(a,lo,hi)"){
        data::max(a,lo, alo);
        data::min(alo,hi, alohi1);
        data::clamp(a,lo,hi, alohi2);
        CHECK(data::equal(alohi1, alohi2));
    }
}

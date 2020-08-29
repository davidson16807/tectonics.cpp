

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <many/types.hpp>  
#include <many/arithmetic.hpp>  
#include <many/relational.hpp>  
#include <many/statistic.hpp>  
#include <many/operators.hpp>  

#include "types.hpp" // vec3s
#include "relational.hpp" 
#include "common.hpp" 
#include "operators.hpp" 

#include "types_test_utils.hpp" // get_random_vec3s

using namespace many;

TEST_CASE( "vec3s abs purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("abs(a) must generate the same output when called repeatedly"){
        abs(a, out1);
        abs(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "vec3s sign purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("sign(a) must generate the same output when called repeatedly"){
        sign(a, out1);
        sign(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "vec3s abs/sign consistency", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s abs_a = many::get_random_vec3s(5, generator);
    many::vec3s sign_a = many::get_random_vec3s(5, generator);
    SECTION("sign(a)*abs(a) must reconstruct a"){
        abs(a, abs_a);
        sign(a, sign_a);
        CHECK(equal(sign_a*abs_a, a));
    }
}
TEST_CASE( "vec3s floor purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("floor(a) must generate the same output when called repeatedly"){
        floor(a, out1);
        floor(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "vec3s fract purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("fract(a) must generate the same output when called repeatedly"){
        fract(a, out1);
        fract(a, out2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "vec3s floor/fract consistency", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s floor_a = many::get_random_vec3s(5, generator);
    many::vec3s fract_a = many::get_random_vec3s(5, generator);
    SECTION("fract(a)+floor(a) must reconstruct a"){
        floor(a, floor_a);
        fract(a, fract_a);
        CHECK(equal(floor_a + fract_a, a));
    }
}

// TEST_CASE( "vec3s mod purity", "[many]" ) {
//     std::mt19937 generator(2);
//     many::vec3s a = many::get_random_vec3s(5, generator);
//     many::vec3s b = many::get_random_vec3s(5, generator);
//     many::vec3s out1 = many::get_random_vec3s(5, generator);
//     many::vec3s out2 = many::get_random_vec3s(5, generator);
//     SECTION("mod(a,b) must generate the same output when called repeatedly"){
//         mod(a,b, out1);
//         mod(a,b, out2);
//         CHECK(equal(out1, out2));
//     }
// }
// TEST_CASE( "vec3s mod/fract consistency", "[many]" ) {
//     std::mt19937 generator(2);
//     many::vec3s a = many::get_random_vec3s(5, generator);
//     many::vec3s ones = many::get_random_vec3s(5, generator);
//     many::vec3s out1 = many::get_random_vec3s(5, generator);
//     many::vec3s out2 = many::get_random_vec3s(5, generator);
//     SECTION("mod(a,1) must generate the same output as fract(a) for positive numbers"){
//         mod(a,1, out1);
//         fract(a, out2);
//         CHECK(equal(out1, out2));
//     }
//     SECTION("mod(a,ones) must generate the same output as fract(a) for positive numbers"){
//         mod(a,ones, out1);
//         fract(a, out2);
//         CHECK(equal(out1, out2));
//     }
// }
TEST_CASE( "vec3s min purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s b = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("min(a,b) must generate the same output when called repeatedly"){
        min(a,b, out1);
        min(a,b, out2);
        CHECK(equal(out1, out2));
    }
}
// TEST_CASE( "vec3s min decreasing", "[many]" ) {
//     std::mt19937 generator(2);
//     many::vec3s a = many::get_random_vec3s(5, generator);
//     many::vec3s b = many::get_random_vec3s(5, generator);
//     many::vec3s c = many::get_random_vec3s(5, generator);
//     many::vec3s ab = many::get_random_vec3s(5, generator);
//     many::vec3s abc = many::get_random_vec3s(5, generator);
//     SECTION("sum(min(a,b)) must always return a value less than or equal to the starting value"){
//         min(a,b, ab);
//         min(ab,c, abc);
//         CHECK(sum(ab) >= sum(abc));
//     }
// }
TEST_CASE( "vec3s min idempotence", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s b = many::get_random_vec3s(5, generator);
    many::vec3s ab = many::get_random_vec3s(5, generator);
    many::vec3s abb = many::get_random_vec3s(5, generator);
    SECTION("subsequent calls to min(*,b) must return the same value"){
        min(a,b, ab);
        min(ab,b, abb);
        CHECK(equal(ab, abb));
    }
}

TEST_CASE( "vec3s max purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s b = many::get_random_vec3s(5, generator);
    many::vec3s out1 = many::get_random_vec3s(5, generator);
    many::vec3s out2 = many::get_random_vec3s(5, generator);
    SECTION("max(a,b) must generate the same output when called repeatedly"){
        max(a,b, out1);
        max(a,b, out2);
        CHECK(equal(out1, out2));
    }
}
// TEST_CASE( "vec3s max decreasing", "[many]" ) {
//     std::mt19937 generator(2);
//     many::vec3s a = many::get_random_vec3s(5, generator);
//     many::vec3s b = many::get_random_vec3s(5, generator);
//     many::vec3s c = many::get_random_vec3s(5, generator);
//     many::vec3s ab = many::get_random_vec3s(5, generator);
//     many::vec3s abc = many::get_random_vec3s(5, generator);
//     SECTION("sum(max(a,b)) must always return a value greater than or equal to the starting value"){
//         max(a,b, ab);
//         max(ab,c, abc);
//         CHECK(sum(ab) <= sum(abc));
//     }
// }
TEST_CASE( "vec3s max idempotence", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s b = many::get_random_vec3s(5, generator);
    many::vec3s ab = many::get_random_vec3s(5, generator);
    many::vec3s abb = many::get_random_vec3s(5, generator);
    SECTION("subsequent calls to max(*,b) must return the same value"){
        max(a,b, ab);
        max(ab,b, abb);
        CHECK(equal(ab, abb));
    }
}

// TEST_CASE( "vec3s min/max/clamp consistency", "[many]" ) {
//     std::mt19937 generator(2);
//     many::vec3s a = many::get_random_vec3s(5, generator);
//     many::vec3s lo = many::get_random_vec3s(5, generator);
//     many::vec3s hi = many::get_random_vec3s(5, generator);
//     many::vec3s alo = many::get_random_vec3s(5, generator);
//     many::vec3s alohi1 = many::get_random_vec3s(5, generator);
//     many::vec3s alohi2 = many::get_random_vec3s(5, generator);
//     SECTION("min(max(a,lo),hi) must return the same value as clamp(a,lo,hi)"){
//         max(a,lo, alo);
//         min(alo,hi, alohi1);
//         clamp(a,lo,hi, alohi2);
//         CHECK(alohi1 == alohi2);
//     }
// }


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include "each.hpp" 

#include "../Series.hpp" 
#include "../whole.hpp" 
#include "_test_utils.cpp"


TEST_CASE( "Must be able to test each::series equivalence for boolean vectors using the catch framework", "[each]" ) {
    std::mt19937 generator(2);
    auto ref       = each::get_random_vec3s(4, generator);
    auto ref_copy  = each::series(ref);
    auto ref_tweak = each::series(ref); ref_tweak[1].z = 1.0f;
    auto ref_fewer = each::get_random_vec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(whole::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(whole::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(whole::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(whole::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test each::series equivalence for vectors using the catch framework", "[each]" ) {
    std::mt19937 generator(2);
    auto ref       = each::get_random_bvec3s(4, generator);
    auto ref_tweak = each::series(ref); ref_tweak[1].z = true; ref[1].z = false;
    auto ref_copy  = each::series(ref);
    auto ref_fewer = each::get_random_bvec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(whole::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(whole::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(whole::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(whole::notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "Series<vec3> comparator purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c1 = each::get_random_bvec3s(5, generator);
    auto c2 = each::get_random_bvec3s(5, generator);
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        each::greaterThanEqual(a,b,c1);
        each::greaterThanEqual(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        each::greaterThan(a,b,c1);
        each::greaterThan(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        each::lessThanEqual(a,b,c1);
        each::lessThanEqual(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        each::lessThan(a,b,c1);
        each::lessThan(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        each::equal(a,b,c1);
        each::equal(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
}
TEST_CASE( "Series<vec3> comparator consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a    = each::get_random_vec3s(5, generator);
    auto b    = each::get_random_vec3s(5, generator);
    auto gt   = each::get_random_bvec3s(5, generator);
    auto lt   = each::get_random_bvec3s(5, generator);
    auto eq   = each::get_random_bvec3s(5, generator);
    auto ne1  = each::get_random_bvec3s(5, generator);
    auto ne2  = each::get_random_bvec3s(5, generator);
    auto gte1 = each::get_random_bvec3s(5, generator);
    auto gte2 = each::get_random_bvec3s(5, generator);
    auto lte1 = each::get_random_bvec3s(5, generator);
    auto lte2 = each::get_random_bvec3s(5, generator);

    SECTION("a>=b must equal a>b || a==b"){
        each::greaterThanEqual(a,b,   gte1);
        each::greaterThan     (a,b,   gt);
        each::equal           (a,b,   eq);
        each::unite           (gt,eq, gte2);
        CHECK(whole::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        each::greaterThanEqual(a,b,   lte1);
        each::greaterThan     (a,b,   lt);
        each::equal           (a,b,   eq);
        each::unite           (lt,eq, lte2);
        CHECK(whole::equal(lte1, lte2));
    }

    // NOTE: bvec3 does not have an overload for ! 
    // SECTION("a!=b must equal !(a==b)"){
    //     each::equal           (a,b, eq);
    //     each::negate          (eq,  ne1);
    //     each::notEqual        (a,b, ne2);
    //     CHECK(whole::equal(ne1, ne2));
    // }
}









TEST_CASE( "vec3s abs purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("abs(a) must generate the same output when called repeatedly"){
        each::abs(a, out1);
        each::abs(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s sign purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("sign(a) must generate the same output when called repeatedly"){
        each::sign(a, out1);
        each::sign(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s abs/sign consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto abs_a = each::get_random_vec3s(5, generator);
    auto sign_a = each::get_random_vec3s(5, generator);
    auto sign_a_abs_a = each::get_random_vec3s(5, generator);
    SECTION("sign(a)*abs(a) must reconstruct a"){
        each::abs(a, abs_a);
        each::sign(a, sign_a);
        each::mult(sign_a, abs_a, sign_a_abs_a);
        CHECK(whole::equal(sign_a_abs_a, a));
    }
}
TEST_CASE( "vec3s floor purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("floor(a) must generate the same output when called repeatedly"){
        each::floor(a, out1);
        each::floor(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s fract purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("fract(a) must generate the same output when called repeatedly"){
        each::fract(a, out1);
        each::fract(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s floor/fract consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto floor_a = each::get_random_vec3s(5, generator);
    auto fract_a = each::get_random_vec3s(5, generator);
    auto floor_a_fract_a = each::get_random_vec3s(5, generator);
    SECTION("fract(a)+floor(a) must reconstruct a"){
        each::floor(a, floor_a);
        each::fract(a, fract_a);
        each::add(floor_a, fract_a, floor_a_fract_a);
        CHECK(whole::equal(floor_a_fract_a, a, 1e-7));
    }
}

// TEST_CASE( "vec3s mod purity", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto out1 = each::get_random_vec3s(5, generator);
//  auto out2 = each::get_random_vec3s(5, generator);
//     SECTION("each::mod(a,b) must generate the same output when called repeatedly"){
//         each::mod(a,b, out1);
//         each::mod(a,b, out2);
//         CHECK(whole::equal(out1, out2));
//     }
// }
// TEST_CASE( "vec3s mod/fract consistency", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto ones = each::get_random_vec3s(5, generator);
//  auto out1 = each::get_random_vec3s(5, generator);
//  auto out2 = each::get_random_vec3s(5, generator);
//     SECTION("each::mod(a,1) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,1, out1);
//         each::fract(a, out2);
//         CHECK(whole::equal(out1, out2));
//     }
//     SECTION("each::mod(a,ones) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,ones, out1);
//         each::fract(a, out2);
//         CHECK(whole::equal(out1, out2));
//     }
// }
TEST_CASE( "vec3s min purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("each::min(a,b) must generate the same output when called repeatedly"){
        each::min(a,b, out1);
        each::min(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
// TEST_CASE( "vec3s min decreasing", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto c = each::get_random_vec3s(5, generator);
//  auto ab = each::get_random_vec3s(5, generator);
//  auto abc = each::get_random_vec3s(5, generator);
//     SECTION("sum(each::min(a,b)) must always return a value less than or equal to the starting value"){
//         each::min(a,b, ab);
//         min(ab,c, abc);
//         CHECK(sum(ab) >= sum(abc));
//     }
// }
TEST_CASE( "vec3s min idempotence", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto abb = each::get_random_vec3s(5, generator);
    SECTION("subsequent calls to min(*,b) must return the same value"){
        each::min(a,b, ab);
        min(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

TEST_CASE( "vec3s max purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("each::max(a,b) must generate the same output when called repeatedly"){
        each::max(a,b, out1);
        each::max(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
// TEST_CASE( "vec3s max decreasing", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto c = each::get_random_vec3s(5, generator);
//  auto ab = each::get_random_vec3s(5, generator);
//  auto abc = each::get_random_vec3s(5, generator);
//     SECTION("sum(each::max(a,b)) must always return a value greater than or equal to the starting value"){
//         each::max(a,b, ab);
//         max(ab,c, abc);
//         CHECK(sum(ab) <= sum(abc));
//     }
// }
TEST_CASE( "vec3s max idempotence", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto abb = each::get_random_vec3s(5, generator);
    SECTION("subsequent calls to max(*,b) must return the same value"){
        each::max(a,b, ab);
        max(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

// TEST_CASE( "vec3s min/max/clamp consistency", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto lo = each::get_random_vec3s(5, generator);
//  auto hi = each::get_random_vec3s(5, generator);
//  auto alo = each::get_random_vec3s(5, generator);
//  auto alohi1 = each::get_random_vec3s(5, generator);
//  auto alohi2 = each::get_random_vec3s(5, generator);
//     SECTION("min(each::max(a,lo),hi) must return the same value as each::clamp(a,lo,hi)"){
//         each::max(a,lo, alo);
//         min(alo,hi, alohi1);
//         each::clamp(a,lo,hi, alohi2);
//         CHECK(alohi1 == alohi2);
//     }
// }

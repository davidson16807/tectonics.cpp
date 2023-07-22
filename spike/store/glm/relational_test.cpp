
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include "../Series.hpp" 
#include "../each.hpp" 
#include "relational.hpp" 
#include "_test_utils.cpp"


TEST_CASE( "Must be able to test each::series equivalence for boolean vectors using the catch framework", "[data]" ) {
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

TEST_CASE( "Must be able to test each::series equivalence for vectors using the catch framework", "[data]" ) {
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



TEST_CASE( "Series<vec3> comparator purity", "[data]" ) {
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
TEST_CASE( "Series<vec3> comparator consistency", "[data]" ) {
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

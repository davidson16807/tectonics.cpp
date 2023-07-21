
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include "../Series.hpp" 
#include "../morphologic.hpp" 
#include "relational.hpp" 
#include "_test_utils.cpp"


TEST_CASE( "Must be able to test data::series equivalence for boolean vectors using the catch framework", "[data]" ) {
    std::mt19937 generator(2);
    auto ref       = data::get_random_vec3s(4, generator);
    auto ref_copy  = data::series(ref);
    auto ref_tweak = data::series(ref); ref_tweak[1].z = 1.0f;
    auto ref_fewer = data::get_random_vec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(data::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(data::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(data::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(data::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test data::series equivalence for vectors using the catch framework", "[data]" ) {
    std::mt19937 generator(2);
    auto ref       = data::get_random_bvec3s(4, generator);
    auto ref_tweak = data::series(ref); ref_tweak[1].z = true; ref[1].z = false;
    auto ref_copy  = data::series(ref);
    auto ref_fewer = data::get_random_bvec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(data::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(data::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(data::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(data::notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "Series<vec3> comparator purity", "[data]" ) {
    std::mt19937 generator(2);
    auto a = data::get_random_vec3s(5, generator);
    auto b = data::get_random_vec3s(5, generator);
    auto c1 = data::get_random_bvec3s(5, generator);
    auto c2 = data::get_random_bvec3s(5, generator);
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        data::vec::greaterThanEqual(a,b,c1);
        data::vec::greaterThanEqual(a,b,c2);
        CHECK(data::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        data::vec::greaterThan(a,b,c1);
        data::vec::greaterThan(a,b,c2);
        CHECK(data::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        data::vec::lessThanEqual(a,b,c1);
        data::vec::lessThanEqual(a,b,c2);
        CHECK(data::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        data::vec::lessThan(a,b,c1);
        data::vec::lessThan(a,b,c2);
        CHECK(data::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        data::vec::equal(a,b,c1);
        data::vec::equal(a,b,c2);
        CHECK(data::equal(c1,c2));
    }
}
TEST_CASE( "Series<vec3> comparator consistency", "[data]" ) {
    std::mt19937 generator(2);
    auto a    = data::get_random_vec3s(5, generator);
    auto b    = data::get_random_vec3s(5, generator);
    auto gt   = data::get_random_bvec3s(5, generator);
    auto lt   = data::get_random_bvec3s(5, generator);
    auto eq   = data::get_random_bvec3s(5, generator);
    auto ne1  = data::get_random_bvec3s(5, generator);
    auto ne2  = data::get_random_bvec3s(5, generator);
    auto gte1 = data::get_random_bvec3s(5, generator);
    auto gte2 = data::get_random_bvec3s(5, generator);
    auto lte1 = data::get_random_bvec3s(5, generator);
    auto lte2 = data::get_random_bvec3s(5, generator);

    SECTION("a>=b must equal a>b || a==b"){
        data::vec::greaterThanEqual(a,b,   gte1);
        data::vec::greaterThan     (a,b,   gt);
        data::vec::equal           (a,b,   eq);
        data::unite           (gt,eq, gte2);
        CHECK(data::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        data::vec::greaterThanEqual(a,b,   lte1);
        data::vec::greaterThan     (a,b,   lt);
        data::vec::equal           (a,b,   eq);
        data::unite           (lt,eq, lte2);
        CHECK(data::equal(lte1, lte2));
    }

    // NOTE: bvec3 does not have an overload for ! 
    // SECTION("a!=b must equal !(a==b)"){
    //     data::vec::equal           (a,b, eq);
    //     data::vec::negate          (eq,  ne1);
    //     data::vec::notEqual        (a,b, ne2);
    //     CHECK(data::equal(ne1, ne2));
    // }
}

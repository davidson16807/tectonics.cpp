
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include <series/types.hpp>  
#include <series/relational.hpp>  

#include "relational.hpp" 


TEST_CASE( "Must be able to test series::vec3s equivalence using the catch framework", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s ref       = series::get_random_vec3s(4, generator);
    series::vec3s ref_copy  = series::vec3s(ref);
    series::vec3s ref_tweak = series::vec3s(ref); ref_tweak[1].z = 1.0f;
    series::vec3s ref_fewer = series::get_random_vec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(series::equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(series::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(series::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(series::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test series::bvec3s equivalence using the catch framework", "[many]" ) {
    std::mt19937 generator(2);
    series::bvec3s ref       = series::get_random_bvec3s(4, generator);
    series::bvec3s ref_tweak = series::bvec3s(ref); ref_tweak[1].z = true; ref[1].z = false;
    series::bvec3s ref_copy  = series::bvec3s(ref);
    series::bvec3s ref_fewer = series::get_random_bvec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(series::equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(series::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(series::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(series::notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "many<vec3> comparator purity", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a = series::get_random_vec3s(5, generator);
    series::vec3s b = series::get_random_vec3s(5, generator);
    series::bvec3s c1 = series::get_random_bvec3s(5, generator);
    series::bvec3s c2 = series::get_random_bvec3s(5, generator);
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        series::greaterThanEqual(a,b,c1);
        series::greaterThanEqual(a,b,c2);
        CHECK(series::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        greaterThan(a,b,c1);
        greaterThan(a,b,c2);
        CHECK(series::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        lessThanEqual(a,b,c1);
        lessThanEqual(a,b,c2);
        CHECK(series::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        lessThan(a,b,c1);
        lessThan(a,b,c2);
        CHECK(series::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        equal(a,b,c1);
        equal(a,b,c2);
        CHECK(series::equal(c1,c2));
    }
}
TEST_CASE( "many<vec3> comparator consistency", "[many]" ) {
    std::mt19937 generator(2);
    series::vec3s a    = series::get_random_vec3s(5, generator);
    series::vec3s b    = series::get_random_vec3s(5, generator);
    series::bvec3s  gt   = series::get_random_bvec3s(5, generator);
    series::bvec3s  lt   = series::get_random_bvec3s(5, generator);
    series::bvec3s  eq   = series::get_random_bvec3s(5, generator);
    series::bvec3s  ne1  = series::get_random_bvec3s(5, generator);
    series::bvec3s  ne2  = series::get_random_bvec3s(5, generator);
    series::bvec3s  gte1 = series::get_random_bvec3s(5, generator);
    series::bvec3s  gte2 = series::get_random_bvec3s(5, generator);
    series::bvec3s  lte1 = series::get_random_bvec3s(5, generator);
    series::bvec3s  lte2 = series::get_random_bvec3s(5, generator);

    SECTION("a>=b must equal a>b || a==b"){
        series::greaterThanEqual(a,b,   gte1);
        series::greaterThan     (a,b,   gt);
        series::equal           (a,b,   eq);
        series::unite           (gt,eq, gte2);
        CHECK(series::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        series::greaterThanEqual(a,b,   lte1);
        series::greaterThan     (a,b,   lt);
        series::equal           (a,b,   eq);
        series::unite           (lt,eq, lte2);
        CHECK(series::equal(lte1, lte2));
    }

    // NOTE: bvec3 does not have an overload for ! 
    // SECTION("a!=b must equal !(a==b)"){
    //     series::equal           (a,b, eq);
    //     series::negate          (eq,  ne1);
    //     series::notEqual        (a,b, ne2);
    //     CHECK(series::equal(ne1, ne2));
    // }
}

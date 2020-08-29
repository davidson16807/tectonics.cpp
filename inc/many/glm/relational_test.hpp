
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include <many/types.hpp>  
#include <many/relational.hpp>  

#include "relational.hpp" 


TEST_CASE( "Must be able to test many::vec3s equivalence using the catch framework", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s ref       = many::get_random_vec3s(4, generator);
    many::vec3s ref_copy  = many::vec3s(ref);
    many::vec3s ref_tweak = many::vec3s(ref); ref_tweak[1].z = 1.0f;
    many::vec3s ref_fewer = many::get_random_vec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(many::equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(many::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(many::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(many::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test many::bvec3s equivalence using the catch framework", "[many]" ) {
    std::mt19937 generator(2);
    many::bvec3s ref       = many::get_random_bvec3s(4, generator);
    many::bvec3s ref_tweak = many::bvec3s(ref); ref_tweak[1].z = true; ref[1].z = false;
    many::bvec3s ref_copy  = many::bvec3s(ref);
    many::bvec3s ref_fewer = many::get_random_bvec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(many::equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(many::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(many::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(many::notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "many<T> comparator purity", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a = many::get_random_vec3s(5, generator);
    many::vec3s b = many::get_random_vec3s(5, generator);
    many::bvec3s c1 = many::get_random_bvec3s(5, generator);
    many::bvec3s c2 = many::get_random_bvec3s(5, generator);
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        many::greaterThanEqual(a,b,c1);
        many::greaterThanEqual(a,b,c2);
        CHECK(many::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        greaterThan(a,b,c1);
        greaterThan(a,b,c2);
        CHECK(many::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        lessThanEqual(a,b,c1);
        lessThanEqual(a,b,c2);
        CHECK(many::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        lessThan(a,b,c1);
        lessThan(a,b,c2);
        CHECK(many::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        equal(a,b,c1);
        equal(a,b,c2);
        CHECK(many::equal(c1,c2));
    }
}
TEST_CASE( "many<T> comparator consistency", "[many]" ) {
    std::mt19937 generator(2);
    many::vec3s a    = many::get_random_vec3s(5, generator);
    many::vec3s b    = many::get_random_vec3s(5, generator);
    many::bvec3s  gt   = many::get_random_bvec3s(5, generator);
    many::bvec3s  lt   = many::get_random_bvec3s(5, generator);
    many::bvec3s  eq   = many::get_random_bvec3s(5, generator);
    many::bvec3s  ne1  = many::get_random_bvec3s(5, generator);
    many::bvec3s  ne2  = many::get_random_bvec3s(5, generator);
    many::bvec3s  gte1 = many::get_random_bvec3s(5, generator);
    many::bvec3s  gte2 = many::get_random_bvec3s(5, generator);
    many::bvec3s  lte1 = many::get_random_bvec3s(5, generator);
    many::bvec3s  lte2 = many::get_random_bvec3s(5, generator);

    SECTION("a>=b must equal a>b || a==b"){
        many::greaterThanEqual(a,b,   gte1);
        many::greaterThan     (a,b,   gt);
        many::equal           (a,b,   eq);
        many::unite           (gt,eq, gte2);
        CHECK(many::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        many::greaterThanEqual(a,b,   lte1);
        many::greaterThan     (a,b,   lt);
        many::equal           (a,b,   eq);
        many::unite           (lt,eq, lte2);
        CHECK(many::equal(lte1, lte2));
    }

    // NOTE: bvec3 does not have an overload for ! 
    // SECTION("a!=b must equal !(a==b)"){
    //     many::equal           (a,b, eq);
    //     many::negate          (eq,  ne1);
    //     many::notEqual        (a,b, ne2);
    //     CHECK(many::equal(ne1, ne2));
    // }
}

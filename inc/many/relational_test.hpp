
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/types.hpp>  
#include <many/relational.hpp>  

using namespace many;

TEST_CASE( "Must be able to test many<float> equivalence using the catch framework", "[many]" ) {
    floats ref       = floats({1,2,3,4,5});
    floats ref_copy  = floats({1,2,3,4,5});
    floats ref_tweak = floats({1,2,3,4,0});
    floats ref_fewer = floats({1,2,3,4});

    SECTION("Must be able to equate object with itself"){
        CHECK(equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test many<bool> equivalence using the catch framework", "[many]" ) {
    bools ref       = bools({true,false,true,false,true});
    bools ref_copy  = bools({true,false,true,false,true});
    bools ref_tweak = bools({true,false,true,false,false});
    bools ref_fewer = bools({true,false,true,false});

    SECTION("Must be able to equate object with itself"){
        CHECK(equal(ref, ref));
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "many<T> comparator purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    bools c1 = bools({0,0,0,0,0});
    bools c2 = bools({0,0,0,0,0});
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        greaterThanEqual(a,b,c1);
        greaterThanEqual(a,b,c2);
        CHECK(equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        greaterThan(a,b,c1);
        greaterThan(a,b,c2);
        CHECK(equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        lessThanEqual(a,b,c1);
        lessThanEqual(a,b,c2);
        CHECK(equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        lessThan(a,b,c1);
        lessThan(a,b,c2);
        CHECK(equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        equal(a,b,c1);
        equal(a,b,c2);
        CHECK(equal(c1,c2));
    }
}
TEST_CASE( "many<T> comparator consistency", "[many]" ) {
    floats a    = floats({1,2,3,4,5});
    floats b    = floats({1,1,2,3,5});
    bools  gt   = bools({0,0,0,0,0});
    bools  lt   = bools({0,0,0,0,0});
    bools  eq   = bools({0,0,0,0,0});
    bools  ne1  = bools({0,0,0,0,0});
    bools  ne2  = bools({0,0,0,0,0});
    bools  gte1 = bools({0,0,0,0,0});
    bools  gte2 = bools({0,0,0,0,0});
    bools  lte1 = bools({0,0,0,0,0});
    bools  lte2 = bools({0,0,0,0,0});

    SECTION("a>=b must equal a>b || a==b"){
        greaterThanEqual(a,b,   gte1);
        greaterThan     (a,b,   gt);
        equal           (a,b,   eq);
        unite           (gt,eq, gte2);
        CHECK(equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        greaterThanEqual(a,b,   lte1);
        greaterThan     (a,b,   lt);
        equal           (a,b,   eq);
        unite           (lt,eq, lte2);
        CHECK(equal(lte1, lte2));
    }

    SECTION("a!=b must equal !(a==b)"){
        equal           (a,b, eq);
        negate          (eq,  ne1);
        notEqual        (a,b, ne2);
        CHECK(equal(ne1, ne2));
    }
}

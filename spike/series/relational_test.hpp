
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <series/types.hpp>  
#include <series/relational.hpp>  


TEST_CASE( "Must be able to test many<float> equivalence using the catch framework", "[many]" ) {
    series::floats ref       ({1,2,3,4,5});
    series::floats ref_copy  ({1,2,3,4,5});
    series::floats ref_tweak ({1,2,3,4,0});
    series::floats ref_fewer ({1,2,3,4});

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

TEST_CASE( "Must be able to test many<bool> equivalence using the catch framework", "[many]" ) {
    series::bools ref       ({true,false,true,false,true});
    series::bools ref_copy  ({true,false,true,false,true});
    series::bools ref_tweak ({true,false,true,false,false});
    series::bools ref_fewer ({true,false,true,false});

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



TEST_CASE( "many<T> comparator purity", "[many]" ) {
    series::floats a ({1,2,3,4,5});
    series::floats b ({1,1,2,3,5});
    series::bools c1 ({0,0,0,0,0});
    series::bools c2 ({0,0,0,0,0});
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
TEST_CASE( "many<T> comparator consistency", "[many]" ) {
    series::floats a    ({1,2,3,4,5});
    series::floats b    ({1,1,2,3,5});
    series::bools  gt   ({0,0,0,0,0});
    series::bools  lt   ({0,0,0,0,0});
    series::bools  eq   ({0,0,0,0,0});
    series::bools  ne1  ({0,0,0,0,0});
    series::bools  ne2  ({0,0,0,0,0});
    series::bools  gte1 ({0,0,0,0,0});
    series::bools  gte2 ({0,0,0,0,0});
    series::bools  lte1 ({0,0,0,0,0});
    series::bools  lte2 ({0,0,0,0,0});

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

    SECTION("a!=b must equal !(a==b)"){
        series::equal           (a,b, eq);
        series::negate          (eq,  ne1);
        series::notEqual        (a,b, ne2);
        CHECK(series::equal(ne1, ne2));
    }
}

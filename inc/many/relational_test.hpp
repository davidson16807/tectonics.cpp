
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/types.hpp>  
#include <many/relational.hpp>  


TEST_CASE( "Must be able to test many<float> equivalence using the catch framework", "[many]" ) {
    many::floats ref       ({1,2,3,4,5});
    many::floats ref_copy  ({1,2,3,4,5});
    many::floats ref_tweak ({1,2,3,4,0});
    many::floats ref_fewer ({1,2,3,4});

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

TEST_CASE( "Must be able to test many<bool> equivalence using the catch framework", "[many]" ) {
    many::bools ref       ({true,false,true,false,true});
    many::bools ref_copy  ({true,false,true,false,true});
    many::bools ref_tweak ({true,false,true,false,false});
    many::bools ref_fewer ({true,false,true,false});

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
    many::floats a ({1,2,3,4,5});
    many::floats b ({1,1,2,3,5});
    many::bools c1 ({0,0,0,0,0});
    many::bools c2 ({0,0,0,0,0});
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
    many::floats a    ({1,2,3,4,5});
    many::floats b    ({1,1,2,3,5});
    many::bools  gt   ({0,0,0,0,0});
    many::bools  lt   ({0,0,0,0,0});
    many::bools  eq   ({0,0,0,0,0});
    many::bools  ne1  ({0,0,0,0,0});
    many::bools  ne2  ({0,0,0,0,0});
    many::bools  gte1 ({0,0,0,0,0});
    many::bools  gte2 ({0,0,0,0,0});
    many::bools  lte1 ({0,0,0,0,0});
    many::bools  lte2 ({0,0,0,0,0});

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

    SECTION("a!=b must equal !(a==b)"){
        many::equal           (a,b, eq);
        many::negate          (eq,  ne1);
        many::notEqual        (a,b, ne2);
        CHECK(many::equal(ne1, ne2));
    }
}

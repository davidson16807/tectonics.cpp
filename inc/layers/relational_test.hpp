
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Layers.hpp"
#include "relational.hpp"

TEST_CASE( "Must be able to test Layers<float> equivalence using the catch framework", "[layers]" ) {
    layers::Layers<float,5> ref = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> ref_copy = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> ref_tweak = layers::Layers<float,5>({1,2,3,4,0});
    layers::Layers<float,5> ref_fewer = layers::Layers<float,5>({1,2,3,4});

    SECTION("Must be able to equate object with itself"){
        CHECK(layers::equal(ref, ref));
    }
    SECTION("Must be able to equate layers of the same content and size"){
        CHECK(layers::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish layers of slightly different content"){
        CHECK(layers::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish layers of slightly different size"){
        CHECK(layers::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test Layers<bool> equivalence using the catch framework", "[layers]" ) {
    layers::Layers<float,5> ref = layers::Layers<float,5>       ({true,false,true,false,true});
    layers::Layers<float,5> ref_copy = layers::Layers<float,5>  ({true,false,true,false,true});
    layers::Layers<float,5> ref_tweak = layers::Layers<float,5> ({true,false,true,false,false});
    layers::Layers<float,5> ref_fewer = layers::Layers<float,5> ({true,false,true,false});

    SECTION("Must be able to equate object with itself"){
        CHECK(layers::equal(ref, ref));
    }
    SECTION("Must be able to equate layers of the same content and size"){
        CHECK(layers::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish layers of slightly different content"){
        CHECK(layers::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish layers of slightly different size"){
        CHECK(layers::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Layers<T> comparator purity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({1,1,2,3,5});
    layers::Layers<bool,5> c1 = layers::Layers<bool,5> ({0,0,0,0,0});
    layers::Layers<bool,5> c2 = layers::Layers<bool,5> ({0,0,0,0,0});
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        layers::greaterThanEqual(a,b,c1);
        layers::greaterThanEqual(a,b,c2);
        CHECK(layers::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        layers::greaterThan(a,b,c1);
        layers::greaterThan(a,b,c2);
        CHECK(layers::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        layers::lessThanEqual(a,b,c1);
        layers::lessThanEqual(a,b,c2);
        CHECK(layers::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        layers::lessThan(a,b,c1);
        layers::lessThan(a,b,c2);
        CHECK(layers::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        layers::equal(a,b,c1);
        layers::equal(a,b,c2);
        CHECK(layers::equal(c1,c2));
    }
}

TEST_CASE( "Layers<T> comparator consistency", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({1,1,2,3,5});
    layers::Layers<bool,5> gt = layers::Layers<bool,5>   ({0,0,0,0,0});
    layers::Layers<bool,5> lt = layers::Layers<bool,5>   ({0,0,0,0,0});
    layers::Layers<bool,5> eq = layers::Layers<bool,5>   ({0,0,0,0,0});
    layers::Layers<bool,5> ne1 = layers::Layers<bool,5>  ({0,0,0,0,0});
    layers::Layers<bool,5> ne2 = layers::Layers<bool,5>  ({0,0,0,0,0});
    layers::Layers<bool,5> gte1 = layers::Layers<bool,5> ({0,0,0,0,0});
    layers::Layers<bool,5> gte2 = layers::Layers<bool,5> ({0,0,0,0,0});
    layers::Layers<bool,5> lte1 = layers::Layers<bool,5> ({0,0,0,0,0});
    layers::Layers<bool,5> lte2 = layers::Layers<bool,5> ({0,0,0,0,0});

    SECTION("a>=b must equal a>b || a==b"){
        layers::greaterThanEqual(a,b,   gte1);
        layers::greaterThan     (a,b,   gt);
        layers::equal           (a,b,   eq);
        layers::unite           (gt,eq, gte2);
        CHECK(layers::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        layers::greaterThanEqual(a,b,   lte1);
        layers::greaterThan     (a,b,   lt);
        layers::equal           (a,b,   eq);
        layers::unite           (lt,eq, lte2);
        CHECK(layers::equal(lte1, lte2));
    }

    SECTION("a!=b must equal !(a==b)"){
        layers::equal           (a,b, eq);
        layers::negate          (eq,  ne1);
        layers::notEqual        (a,b, ne2);
        CHECK(layers::equal(ne1, ne2));
    }
}

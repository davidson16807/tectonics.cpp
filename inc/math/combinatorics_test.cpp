#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <math/combinatorics.hpp>  

TEST_CASE( "combination purity", "[math]" ) {
    SECTION("combination when called repeatedly on the same input will always provide the same output"){
        for (int x = 0; x < 10; ++x)
        {
            for (int y = 0; y <= x; ++y)
            {
                CHECK(math::combination(x,y)==math::combination(x,y));
            }
        }
    }
}

TEST_CASE( "combination degeneracy", "[math]" ) {
    SECTION("combination(x,0) always equals 1"){
        for (int x = 0; x < 10; ++x)
        {
            CHECK(math::combination(x,0)==1);
        }
    }
    SECTION("combination(x,x) always equals 1"){
        for (int x = 0; x < 10; ++x)
        {
            CHECK(math::combination(x,x)==1);
        }
    }
}

TEST_CASE( "combination happy path", "[math]" ) {
    SECTION("combination for given input must equal known values"){
        CHECK(math::combination(0,0)==1);

        CHECK(math::combination(1,0)==1);
        CHECK(math::combination(1,1)==1);

        CHECK(math::combination(2,0)==1);
        CHECK(math::combination(2,1)==2);
        CHECK(math::combination(2,2)==1);

        CHECK(math::combination(3,0)==1);
        CHECK(math::combination(3,1)==3);
        CHECK(math::combination(3,2)==3);
        CHECK(math::combination(3,3)==1);
    }
}
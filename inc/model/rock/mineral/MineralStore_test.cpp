
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include "MineralStore.hpp"
#include "_test_utils.hpp"

#include <test/properties.hpp>

TEST_CASE( "MineralStore pack/unpack invertibility", "[mineral]" ) {
    rock::MineralAdapter inexact;

  	std::mt19937 generator(2);
	rock::Mineral e;
  	std::vector<rock::Mineral> minerals{
		rock::get_random(generator), 
		rock::get_random(generator), 
		e
	};

    REQUIRE(test::left_invertibility(inexact,
    	"unpack", [=](auto x){ rock::Mineral      out; x.unpack(out); return out; }, 
    	"pack",   [=](auto x){ rock::MineralStore out; out.pack(x);   return out; }, 
    	minerals));

}

TEST_CASE( "MineralStore memory constraints", "[strata]" ) {
    SECTION("a MineralStore must fit within an expected memory footprint"){
    	std::cout << "sizeof(MineralStore): " << sizeof(rock::MineralStore) << std::endl;
		CHECK(sizeof(rock::MineralStore) <= 8 );
	}
}



#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumStore.hpp"
#include "_test_tools.hpp"

#include <test/structures/grouplike.hpp>

TEST_CASE( "StratumStore pack/unpack invertibility", "[rock]" ) {
  	const std::size_t M = 15;

    rock::StratumAdapter<M> inexact;

  	std::mt19937 generator(2);
	rock::Stratum<M> e;
  	std::vector<rock::Stratum<M>> stratums{
		rock::get_random<M>(generator), 
		rock::get_random<M>(generator), 
		e
	};

    REQUIRE(test::left_invertibility(inexact,
    	"unpack", [=](auto x){ rock::Stratum<M>      out; x.unpack(out); return out; }, 
    	"pack",   [=](auto x){ rock::StratumStore<M> out; out.pack(x);   return out; }, 
    	stratums));

}

TEST_CASE( "StratumStore memory constraints", "[rock]" ) {
    SECTION("a StratumStore must fit within an expected memory footprint for a given number of layers and mass pools"){
		CHECK(sizeof(rock::StratumStore<15>) <= 188); //bytes
	}
}

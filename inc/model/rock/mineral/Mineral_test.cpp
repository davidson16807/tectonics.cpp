
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// C libraries
#include <cmath>

// std libraries
#include <numeric>

// in house libraries
#include "Mineral.hpp"
#include "_test_utils.hpp"

#include <test/structures/grouplike.hpp>

TEST_CASE( "Mineral combine() commutative monoid", "[mineral]" ) {

    rock::MineralAdapter inexact;

  	std::mt19937 generator(2);
  	rock::Mineral e;
  	std::vector<rock::Mineral> minerals{
  		rock::get_random(generator), 
  		rock::get_random(generator), 
  		e
  	};

    test::CommutativeMonoid commutative_monoid(
        "the empty mineral", e,
        "combine",           [=](auto x, auto y){ rock::Mineral out; (rock::combine(x,y,out)); return out; }
    );

    REQUIRE(commutative_monoid.valid(inexact, minerals));

}

TEST_CASE( "Mineral combine() closure", "[mineral]" ) {
  rock::MineralAdapter inexact;

  std::mt19937 generator(2);
  rock::Mineral e;
  std::vector<rock::Mineral> minerals{
    rock::get_random(generator), 
    rock::get_random(generator), 
    e
  };

  REQUIRE(test::predicate(inexact, 
        "the result of passing two valid Mineral objects to combine() must itself produce a valid Mineral",
        [=](auto a, auto b){
            rock::Mineral fab;
            rock::combine(a,b,fab);
            return test::Results(inexact.is_valid(fab),
                "combine(a,b): " + inexact.print(fab)
            );
        }, minerals, minerals));
}

TEST_CASE( "Mineral combine() mass conservation", "[mineral]" ) {
  	std::mt19937 generator(2);
	rock::Mineral a = rock::get_random(generator);
	rock::Mineral b = rock::get_random(generator);

	rock::Mineral ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid Mineral objects to combine() must produce a new Mineral object with equal mass"){
    	CHECK(ab.mass == a.mass + b.mass);
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/


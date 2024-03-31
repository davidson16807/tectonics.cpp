
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Stratum.hpp"
#include "Stratum_operators.hpp"
#include "_test_utils.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>

TEST_CASE( "Stratum scale() closure", "[rock]" ) {
	const std::size_t M = 15;

	rock::StratumAdapter<M> inexact;

	std::mt19937 generator(2);
	rock::Stratum<M> e;
	std::vector<rock::Stratum<M>> strata{
		rock::get_random<M>(generator), 
		rock::get_random<M>(generator), 
		e
	};

    std::vector<double> positives { 1e-3, 2.0 };

	REQUIRE(test::predicate(inexact, 
	    "the result of passing Stratum objects to scale() must itself produce a valid Stratum for positive scalars",
	    [=](auto a, auto b){
	        rock::Stratum<M> fab;
	        rock::scale(a,b,fab);
	        return test::Results(inexact.is_valid(fab),
	            "scale(a,b): " + test::indent(inexact.print(fab), "  ")
	        );
	    }, strata, positives));
}

TEST_CASE( "Stratum scale() monoid", "[rock]" ) {
  	const std::size_t M = 15;

    rock::StratumAdapter<M> inexact;

  	std::mt19937 generator(2);
	// default constructor is required to be identity
  	rock::Stratum<M> e;
	// test to make sure that any stratum with zero mass is an identity, 
	// regardless of grain type distribution within mass pools 
	e.minerals[0].grain_type_relative_volume[0] = 1000.0f;
  	std::vector<rock::Stratum<M>> strata{
  		rock::get_random<M>(generator), 
  		rock::get_random<M>(generator), 
  		e
  	};
    std::vector<double> scalars { -2.0, 0.0, 2.0 };

  	auto scale = [=](auto x, auto y){ rock::Stratum<M> out; (rock::scale(x,y,out)); return out; };
  	auto mult  = [=](auto x, auto y){ return x*y; };

    REQUIRE(test::right_identity(
    	inexact,
        "1",     1.0f,
        "scale", scale,
        strata
    ));

    REQUIRE(test::associativity(
    	inexact,
        "scale",    scale,
        "scale",    scale,
        "multiply", mult,
        strata, scalars, scalars
    ));

}

TEST_CASE( "Stratum combine() commutative monoid", "[rock]" ) {
	const std::size_t M = 15;

    rock::StratumAdapter<M> inexact;

  	std::mt19937 generator(2);
	// default constructor is required to be identity
  	rock::Stratum<M> e;
	// test to make sure that any stratum with zero mass is an identity, 
	// regardless of grain type distribution within mass pools 
	e.minerals[0].grain_type_relative_volume[0] = 1000.0f;
  	std::vector<rock::Stratum<M>> strata{
  		rock::get_random<M>(generator), 
  		rock::get_random<M>(generator), 
  		e
  	};

    test::CommutativeMonoid commutative_monoid(
        "the empty stratum", e,
        "combine",           [=](auto x, auto y){ rock::Stratum<M> out; (rock::combine(x,y,out)); return out; }
    );

    REQUIRE(commutative_monoid.valid(inexact, strata));

}

TEST_CASE( "Stratum combine() closure", "[rock]" ) {
	const std::size_t M = 15;

	rock::StratumAdapter<M> inexact;

	std::mt19937 generator(2);
	rock::Stratum<M> e;
	std::vector<rock::Stratum<M>> strata{
		rock::get_random<M>(generator), 
		rock::get_random<M>(generator), 
		e
	};

  	REQUIRE(test::predicate(inexact, 
        "the result of passing two valid Stratum objects to combine() must itself produce a valid Stratum",
        [=](auto a, auto b){
            rock::Stratum<M> fab;
            rock::combine(a,b,fab);
            return test::Results(inexact.is_valid(fab),
                "combine(a,b): " + test::indent(inexact.print(fab), "  ")
            );
        }, strata, strata));
}

TEST_CASE( "Stratum combine() mass conservation", "[rock]" ) {
  	const std::size_t M = 15;

  	std::mt19937 generator(2);
	rock::Stratum<M> a = rock::get_random<M>(generator);
	rock::Stratum<M> b = rock::get_random<M>(generator);

	rock::Stratum<M> ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid Stratum objects to combine() must produce a valid Stratum with equivalent mass"){
    	CHECK(ab.mass() == a.mass() + b.mass());
    }
}


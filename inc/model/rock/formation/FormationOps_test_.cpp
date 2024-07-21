
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "StratumStore.hpp"
#include "Formation.hpp"
#include "FormationOps.hpp"
#include "_test_tools.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

TEST_CASE( "Formation combine() commutative monoid", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::StratumSummaryAdapter strata_testing;
    rock::FormationOps tools();
    rock::FormationAdapter testing(strata_testing);

    float oo = std::numeric_limits<float>::max();
    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::Formation e1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::Formation e2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::Formation e3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    std::vector<rock::Formation> formations ({
        rock::Formation(std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer)),
        rock::Formation(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer)),
        rock::Formation(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer)),
        e1
    });

    test::CommutativeMonoid commutative_monoid1(
        "an empty Formation", e1,
        "combine",                 TEST_BINARY(tools.combine)
    );

    test::CommutativeMonoid commutative_monoid2(
        "an empty Formation", e2,
        "combine",                 TEST_BINARY(tools.combine)
    );

    REQUIRE(commutative_monoid1.valid(testing, formations));
    REQUIRE(commutative_monoid2.valid(testing, formations));

}

TEST_CASE( "Formation combine() mass conservation", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::StratumSummaryAdapter strata_testing;
    rock::FormationOps tools();
    rock::FormationAdapter testing(strata_testing);

    rock::Formation a(std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length(4.0*si::kilometer));
    rock::Formation b(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length(5.0*si::kilometer));

    SECTION("the result of passing two valid Formation objects to combine() must produce a valid Formation with equivalent area_density"){
        CHECK(si::distance(tools.combine(a, b).area_density(), a.area_density() + b.area_density()) <=
            0.001f * si::max(tools.combine(a, b).area_density(), a.area_density() + b.area_density()));
    }
}


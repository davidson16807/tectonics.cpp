
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "StratumStore.hpp"
#include "FormationSummary.hpp"
#include "StratumSummarization.hpp"
#include "FormationSummaryTools.hpp"
#include "_test_utils.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

TEST_CASE( "FormationSummary combine() commutative monoid", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::StratumSummaryOps strata_tools;
    rock::StratumSummaryAdapter strata_testing;
    rock::FormationSummaryTools tools(strata_tools);
    rock::FormationSummaryAdapter testing;

    float oo = std::numeric_limits<float>::max();
    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::FormationSummary e1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::FormationSummary e2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::FormationSummary e3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    std::vector<rock::FormationSummary> strata ({
        rock::FormationSummary(std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer)),
        rock::FormationSummary(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer)),
        rock::FormationSummary(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer)),
        e1
    });

    test::CommutativeMonoid commutative_monoid1(
        "an empty FormationSummary", e1,
        "combine",                 TEST_BINARY(tools.combine)
    );

    test::CommutativeMonoid commutative_monoid2(
        "an empty FormationSummary", e2,
        "combine",                 TEST_BINARY(tools.combine)
    );

    REQUIRE(commutative_monoid1.valid(testing, strata));
    REQUIRE(commutative_monoid2.valid(testing, strata));

}

TEST_CASE( "FormationSummary combine() mass conservation", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::StratumSummaryOps strata_tools;
    rock::StratumSummaryAdapter strata_testing;
    rock::FormationSummaryTools tools(strata_tools);
    rock::FormationSummaryAdapter testing;

    rock::FormationSummary a(std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length(4.0*si::kilometer));
    rock::FormationSummary b(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length(5.0*si::kilometer));

    SECTION("the result of passing two valid FormationSummary objects to combine() must produce a valid FormationSummary with equivalent area_density"){
        CHECK(si::distance(tools.combine(a, b).area_density(), a.area_density() + b.area_density()) <=
            0.001f * si::max(tools.combine(a, b).area_density(), a.area_density() + b.area_density()));
    }
}


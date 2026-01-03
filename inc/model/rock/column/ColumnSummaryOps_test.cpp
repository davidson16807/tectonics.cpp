
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "ColumnSummary.hpp"
#include "ColumnSummaryOps.hpp"
#include "_test_tools.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

TEST_CASE( "ColumnSummary absorb() monoid", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::ColumnSummaryOps ops(length(si::centimeter));
    rock::ColumnSummaryAdapter inexact;

    float oo = std::numeric_limits<float>::max();

    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::StratumSummary empty1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary empty2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary empty3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    rock::StratumSummary subducting(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary continental(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));
    rock::StratumSummary oceanic(std::bitset<8>(4), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));

    rock::ColumnSummary e1(empty1, empty1);
    rock::ColumnSummary e2(empty2, empty3);

    rock::ColumnSummary column1(continental, empty1);
    rock::ColumnSummary column2(oceanic, empty1);
    rock::ColumnSummary column3(subducting, empty1);
    rock::ColumnSummary column4(continental, oceanic);
    rock::ColumnSummary column5(oceanic, oceanic);
    rock::ColumnSummary column6(continental, subducting);
    rock::ColumnSummary column7(oceanic, subducting);
    rock::ColumnSummary column8(subducting, subducting);

    std::vector<rock::ColumnSummary> columns ({
        e1, e2, column1, column2, column3, column4, column5, column6, column7, column8
    });

    std::vector<rock::StratumSummary> strata ({
        empty1, empty2, empty3, oceanic, subducting, continental
    });

    test::Monoid monoid1(
        "an empty ColumnSummary", e1,
        "absorb",                 TEST_BINARY(ops.absorb)
    );

    test::Monoid monoid2(
        "an empty ColumnSummary", e2,
        "absorb",                 TEST_BINARY(ops.absorb)
    );

    REQUIRE(monoid1.valid(inexact, columns, strata));
    REQUIRE(monoid2.valid(inexact, columns, strata));

}

TEST_CASE( "ColumnSummary absorb() mass conservation", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::ColumnSummaryOps ops(length(si::centimeter));
    rock::ColumnSummaryAdapter inexact;

    float oo = std::numeric_limits<float>::max();

    rock::StratumSummary empty1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary empty2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary empty3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    rock::StratumSummary subducting(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary continental(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));
    rock::StratumSummary oceanic(std::bitset<8>(4), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));

    rock::ColumnSummary a(continental, oceanic);
    rock::StratumSummary b(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length(5.0*si::kilometer));

    SECTION("the result of passing two valid objects to absorb() must produce a valid ColumnSummary with equivalent area_density"){
        CHECK(si::distance(ops.absorb(a, b).area_density(), a.area_density() + b.area_density()) <=
            0.001f * si::max(ops.absorb(a, b).area_density(), a.area_density() + b.area_density()));
    }
}


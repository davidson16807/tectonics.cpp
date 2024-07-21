
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries

#include "_test_tools.hpp"
#include "CrustSummary.hpp"
#include "CrustSummaryOps.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

#define ROCK_TEST_UNARY_OUT_PARAMETER(TYPE,SIZE,F)   [=](auto x){ std::vector<TYPE> out(SIZE); (F(x,out)); return out; }
#define ROCK_TEST_BINARY_OUT_PARAMETER(TYPE,SIZE,F)  [=](auto x, auto y){ std::vector<TYPE> out(SIZE); (F(x,y,out)); return out; }
#define ROCK_TEST_TRINARY_OUT_PARAMETER(TYPE,SIZE,F) [=](auto x, auto y, auto z){ std::vector<TYPE> out(SIZE); (F(x,y,z,out)); return out; }

TEST_CASE( "ColumnSummary absorb() commutative monoid", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::CrustSummaryOps ops(
        rock::ColumnSummaryOps(
            rock::StratumSummaryOps(density(3075.0*si::kilogram/si::meter3)), 
            length(si::centimeter)));
    rock::ColumnSummaryAdapter inexact;

    float oo = std::numeric_limits<float>::max();

    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::StratumSummary strataless1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary strataless2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary strataless3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    rock::StratumSummary subducting (std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary oceanic    (std::bitset<8>(4), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));
    rock::StratumSummary continental(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(7.0*si::kilometer));
    rock::StratumSummary mountain   (std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));

    rock::ColumnSummary columnless11(strataless1, strataless1);
    rock::ColumnSummary columnless12(strataless1, strataless2);
    rock::ColumnSummary columnless13(strataless1, strataless3);
    rock::ColumnSummary columnless21(strataless2, strataless1);
    rock::ColumnSummary columnless22(strataless2, strataless2);
    rock::ColumnSummary columnless23(strataless2, strataless3);
    rock::ColumnSummary columnless31(strataless3, strataless1);
    rock::ColumnSummary columnless32(strataless3, strataless2);
    rock::ColumnSummary columnless33(strataless3, strataless3);

    rock::ColumnSummary column1(continental, strataless1);
    rock::ColumnSummary column2(oceanic, strataless1);
    rock::ColumnSummary column3(subducting, strataless1);
    rock::ColumnSummary column4(continental, oceanic);
    rock::ColumnSummary column5(oceanic, oceanic);
    rock::ColumnSummary column6(continental, subducting);
    rock::ColumnSummary column7(oceanic, subducting);
    rock::ColumnSummary column8(subducting, subducting);

    rock::CrustSummary crustless ({
        columnless11, columnless12, columnless13, 
        columnless21, columnless22, columnless23, 
        columnless31, columnless32, columnless33
    });
    rock::CrustSummary crust1 ({
        columnless11, column1, column2, 
        column3, column4, column5, 
        column6, column7, column8
    });
    rock::CrustSummary crust2 ({
        column1, column2, column3, 
        column4, column5, column6, 
        column7, column8, columnless22
    });

    rock::FormationSummary formless ({
        strataless1, strataless2, strataless3, 
        strataless3, strataless1, strataless2, 
        strataless2, strataless3, strataless1
    });

    rock::FormationSummary formation1 ({
        subducting, continental, oceanic, 
        subducting, continental, oceanic, 
        subducting, continental, oceanic 
    });

    rock::FormationSummary formation2 ({
        subducting, continental, oceanic,
        oceanic, subducting, continental, 
        continental, oceanic, subducting
    });

    std::vector<rock::CrustSummary> crusts ({crustless, crust1, crust2});
    std::vector<rock::FormationSummary> formations ({formless, formation1, formation2});

    test::Monoid monoid1(
        "an empty Crust", crustless,
        "absorb",         ROCK_TEST_BINARY_OUT_PARAMETER(rock::ColumnSummary, 9, ops.absorb)
    );

    REQUIRE(monoid1.valid(inexact, crusts, formations));

}

TEST_CASE( "ColumnSummary absorb() mass conservation", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::CrustSummaryOps ops(
        rock::ColumnSummaryOps(
            rock::StratumSummaryOps(density(3075.0*si::kilogram/si::meter3)), 
            length(si::centimeter)));
    rock::ColumnSummaryAdapter inexact;

    float oo = std::numeric_limits<float>::max();

    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::StratumSummary strataless1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary strataless2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary strataless3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));
    rock::StratumSummary subducting(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary continental(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));
    rock::StratumSummary oceanic(std::bitset<8>(4), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));

    rock::ColumnSummary columnless11(strataless1, strataless1);
    rock::ColumnSummary columnless12(strataless1, strataless2);
    rock::ColumnSummary columnless13(strataless1, strataless3);
    rock::ColumnSummary columnless21(strataless2, strataless1);
    rock::ColumnSummary columnless22(strataless2, strataless2);
    rock::ColumnSummary columnless23(strataless2, strataless3);
    rock::ColumnSummary columnless31(strataless3, strataless1);
    rock::ColumnSummary columnless32(strataless3, strataless2);
    rock::ColumnSummary columnless33(strataless3, strataless3);

    rock::ColumnSummary column1(continental, strataless1);
    rock::ColumnSummary column2(oceanic, strataless1);
    rock::ColumnSummary column3(subducting, strataless1);
    rock::ColumnSummary column4(continental, oceanic);
    rock::ColumnSummary column5(oceanic, oceanic);
    rock::ColumnSummary column6(continental, subducting);
    rock::ColumnSummary column7(oceanic, subducting);
    rock::ColumnSummary column8(subducting, subducting);

    rock::CrustSummary a ({
        columnless11, column1, column2, 
        column3, column4, column5, 
        column6, column7, column8
    });
    rock::CrustSummary b ({
        column1, column2, column3, 
        column4, column5, column6, 
        column7, column8, columnless22
    });

    ColumnSummaryProperties

    rock::CrustSummary ab(9);
    ops.absorb(a, b, ab);

    SECTION("the result of passing two valid objects to absorb() must produce a valid ColumnSummary with equivalent area_density"){
        CHECK(si::distance(ab.area_density(), a.area_density() + b.area_density()) <=
            0.001f * si::max(ab.area_density(), a.area_density() + b.area_density()));
    }
}


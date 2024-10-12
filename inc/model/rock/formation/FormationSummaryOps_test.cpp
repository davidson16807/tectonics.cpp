
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <index/whole.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>

#include <model/rock/stratum/StratumSummary.hpp>
#include <model/rock/stratum/StratumSummaryProperties.hpp>

#include "FormationSummary.hpp"
#include "FormationSummaryOps.hpp"
#include "_test_tools.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

#define FORMATION_TEST_BINARY_OUT_PARAMETER(TYPE,SIZE,F)  [=](auto x, auto y){ std::vector<TYPE> out(SIZE); (F(x,y,out)); return out; }

TEST_CASE( "FormationSummary combine() commutative monoid", "[rock]" ) {

    using density = si::density<float>;
    using length = si::length<float>;

    rock::FormationSummaryOps ops {rock::StratumSummaryOps{density(3075.0*si::kilogram/si::meter3)}};
    rock::FormationSummaryAdapter testing;

    float oo = std::numeric_limits<float>::max();

    rock::StratumSummary continent(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));
    rock::StratumSummary subducted(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary rifted   (std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));

    rock::StratumSummary empty1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary empty2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary empty3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));

    // test to make sure that any stratum with zero mass is an identity, 
    // regardless of grain type distribution within mass pools 
    rock::FormationSummary identity1{empty1, empty2};
    rock::FormationSummary identity2{empty2, empty3};
    rock::FormationSummary identity3{empty3, empty1};
    std::vector<rock::FormationSummary> strata ({
        rock::FormationSummary{continent, subducted},
        rock::FormationSummary{subducted, rifted},
        rock::FormationSummary{rifted, continent},
        // identity3
    });

    test::CommutativeMonoid commutative_monoid1(
        "an empty FormationSummary", identity1,
        "combine",                 FORMATION_TEST_BINARY_OUT_PARAMETER(rock::StratumSummary, 2, ops.combine)
    );

    test::CommutativeMonoid commutative_monoid2(
        "an empty FormationSummary", identity2,
        "combine",                 FORMATION_TEST_BINARY_OUT_PARAMETER(rock::StratumSummary, 2, ops.combine)
    );

    REQUIRE(commutative_monoid1.valid(testing, strata));
    REQUIRE(commutative_monoid2.valid(testing, strata));

}

TEST_CASE( "FormationSummary combine() mass conservation", "[rock]" ) {

    using area_density = si::area_density<float>;
    using density = si::density<float>;
    using length = si::length<float>;

    using floats = std::vector<float>;
    using area_densities = std::vector<area_density>;

    rock::FormationSummaryOps ops {rock::StratumSummaryOps{density(3075.0*si::kilogram/si::meter3)}};
    iterated::Unary formation_area_density(rock::StratumSummaryAreaDensity{});
    iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic(0.0*si::kilogram/si::meter2, 1.0*si::kilogram/si::meter2)};
    rock::FormationSummaryAdapter testing;

    float oo = std::numeric_limits<float>::max();

    rock::StratumSummary continent(std::bitset<8>(2), density(2700.0*si::kilogram/si::meter3), length(70.0*si::kilometer));
    rock::StratumSummary subducted(std::bitset<8>(1), density(3300.0*si::kilogram/si::meter3), length( 5.0*si::kilometer));
    rock::StratumSummary rifted   (std::bitset<8>(3), density(3000.0*si::kilogram/si::meter3), length( 4.0*si::kilometer));

    rock::StratumSummary empty1(std::bitset<8>(0), density(oo*si::kilogram/si::meter3),     length(0.0*si::kilometer));
    rock::StratumSummary empty2(std::bitset<8>(0), density(0.0*si::kilogram/si::meter3),    length(0.0*si::kilometer));
    rock::StratumSummary empty3(std::bitset<8>(0), density(3000.0*si::kilogram/si::meter3), length(0.0*si::kilometer));

    rock::FormationSummary a{continent, rifted,    empty1,    empty2, empty3   };
    rock::FormationSummary b{subducted, subducted, subducted, rifted, continent};
    rock::FormationSummary ab(5, empty1);

    area_densities arho      (5,area_density(0.0f));
    area_densities brho      (5,area_density(0.0f));
    area_densities abrho     (5,area_density(0.0f));
    area_densities arho_brho (5,area_density(0.0f));
    area_densities offset    (5,area_density(0.0f));
    floats distance (5, 0.0f);

    ops.combine(a, b, ab);
    formation_area_density(ab, abrho);
    formation_area_density(a, arho);
    formation_area_density(b, brho);
    arithmetic.add(arho, brho, arho_brho);
    arithmetic.subtract(arho_brho, abrho, offset);
    arithmetic.divide(offset, procedural::uniform(area_density(1.0f)), distance);
    arithmetic.multiply(distance, distance, distance);

    SECTION("the result of passing two valid FormationSummary objects to combine() must produce a valid FormationSummary with equivalent area_density"){
        CHECK(whole::sum<float>(distance) <= 1e8f);
    }
}


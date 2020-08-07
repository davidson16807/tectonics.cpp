#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "StratumTypes_to_RockType.hpp"

using namespace strata;

TEST_CASE( "get_rock_type() happy path", "[strata]" ) {
    SECTION("get_rock_type() must recognize sand"){
    	CHECK(get_rock_type(StratumTypes(
                MetamorphicGrades::igneous_or_sediment,
                MetamorphicFacies::igneous_or_sediment,
                IgneousCompositionTypes::felsic,
                IgneousFormationTypes::intrusive,
                ParticleSizeBins::sand,
                SoilTypes::sand,
                RockCompositionTypes {false, false, true, false, true, false, false}
            )) == RockType::sand);
    }
    SECTION("get_rock_type() must recognize granite"){
        CHECK(get_rock_type(StratumTypes(
                MetamorphicGrades::igneous_or_sediment,
                MetamorphicFacies::igneous_or_sediment,
                IgneousCompositionTypes::felsic,
                IgneousFormationTypes::intrusive,
                ParticleSizeBins::boulder,
                SoilTypes::sand,
                RockCompositionTypes {false, false, false, true, true, false, false}
            )) == RockType::granite);
    }
    SECTION("get_rock_type() must recognize basalt"){
        CHECK(get_rock_type(StratumTypes(
                MetamorphicGrades::igneous_or_sediment,
                MetamorphicFacies::igneous_or_sediment,
                IgneousCompositionTypes::mafic,
                IgneousFormationTypes::extrusive,
                ParticleSizeBins::boulder,
                SoilTypes::sand,
                RockCompositionTypes {false, false, false, false, true, false, false}
            )) == RockType::basalt);
    }
    SECTION("get_rock_type() must recognize limestone"){
        CHECK(get_rock_type(StratumTypes(
                MetamorphicGrades::sedimentary,
                MetamorphicFacies::sedimentary,
                IgneousCompositionTypes::other,
                IgneousFormationTypes::intrusive,
                ParticleSizeBins::clay,
                SoilTypes::sand,
                RockCompositionTypes {true, true, false, false, false, false, false}
            )) == RockType::limestone);
    }
    SECTION("get_rock_type() must recognize marble"){
        CHECK(get_rock_type(StratumTypes(
                MetamorphicGrades::low,
                MetamorphicFacies::zeolite,
                IgneousCompositionTypes::other,
                IgneousFormationTypes::intrusive,
                ParticleSizeBins::clay,
                SoilTypes::sand,
                RockCompositionTypes {true, true, false, false, false, false, false}
            )) == RockType::marble);
    }
}

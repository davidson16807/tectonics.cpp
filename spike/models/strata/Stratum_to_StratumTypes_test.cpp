#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Stratum_to_StratumTypes.hpp"

using namespace strata;

TEST_CASE( "get_stratum_types() happy path", "[strata]" ) {
    const int M = 15;
    SECTION("get_stratum_types() must recognize sand"){
		StratumMassPool pool;
		pool.mass = 1.0f;
		pool.grain_type_relative_volume[int(GrainType::chemically_weathered_intrusive)] = 1.0f;

		Stratum<M> stratum(300*units::kelvin, 1e5*units::pascal, 0);
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)] = pool;

		StratumTypes stratum_types = get_stratum_types(stratum);
    	CHECK(stratum_types.dominant_particle_size_bin == ParticleSizeBins::sand);
    	CHECK(stratum_types.igneous_formation_type == IgneousFormationTypes::intrusive);
    	CHECK(stratum_types.igneous_composition_type == IgneousCompositionTypes::felsic);
    	CHECK(stratum_types.metamorphic_grade == MetamorphicGrades::igneous_or_sediment);
    	CHECK(!stratum_types.rock_composition_types.partly_calcareous);
    	CHECK(!stratum_types.rock_composition_types.calcareous);
    	CHECK(stratum_types.rock_composition_types.silicaceous);
    	CHECK(!stratum_types.rock_composition_types.feldspathic);
    	CHECK(stratum_types.rock_composition_types.volcanic);
    	CHECK(!stratum_types.rock_composition_types.ferrous);
    	CHECK(!stratum_types.rock_composition_types.organic);
    }
    SECTION("get_stratum_types() must recognize granite"){
		StratumMassPool quartz;
		quartz.mass = 0.7f;
		quartz.grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = 1.0f;
		StratumMassPool orthoclase;
		orthoclase.mass = 0.3f;
		orthoclase.grain_type_relative_volume[int(GrainType::unweathered_intrusive)] = 1.0f;

		Stratum<M> stratum(300*units::kelvin, 1e5*units::pascal, 0);
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)] = quartz;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::orthoclase)] = orthoclase;

		StratumTypes stratum_types = get_stratum_types(stratum);
    	CHECK(stratum_types.dominant_particle_size_bin == ParticleSizeBins::boulder);
    	CHECK(stratum_types.igneous_formation_type == IgneousFormationTypes::intrusive);
    	CHECK(stratum_types.igneous_composition_type == IgneousCompositionTypes::felsic);
    	CHECK(stratum_types.metamorphic_grade == MetamorphicGrades::igneous_or_sediment);
    	CHECK(!stratum_types.rock_composition_types.partly_calcareous);
    	CHECK(!stratum_types.rock_composition_types.calcareous);
    	CHECK(!stratum_types.rock_composition_types.silicaceous);
    	CHECK(stratum_types.rock_composition_types.feldspathic);
    	CHECK(stratum_types.rock_composition_types.volcanic);
    	CHECK(!stratum_types.rock_composition_types.ferrous);
    	CHECK(!stratum_types.rock_composition_types.organic);
    }
    SECTION("get_stratum_types() must recognize basalt"){
		StratumMassPool quartz;
		quartz.mass = 0.5f;
		quartz.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] = 1.0f;
		StratumMassPool orthoclase;
		orthoclase.mass = 0.03f;
		orthoclase.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] = 1.0f;
		StratumMassPool plagioclase;
		plagioclase.mass = 0.07f;
		plagioclase.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] = 1.0f;
		StratumMassPool pyroxene;
		pyroxene.mass = 0.4f;
		pyroxene.grain_type_relative_volume[int(GrainType::unweathered_extrusive)] = 1.0f;

		Stratum<M> stratum(300*units::kelvin, 1e5*units::pascal, 0);
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)] = quartz;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::orthoclase)] = orthoclase;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::plagioclase)] = plagioclase;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::pyroxene)] = pyroxene;

		StratumTypes stratum_types = get_stratum_types(stratum);
    	CHECK(stratum_types.dominant_particle_size_bin == ParticleSizeBins::boulder);
    	CHECK(stratum_types.igneous_formation_type == IgneousFormationTypes::extrusive);
    	CHECK(stratum_types.igneous_composition_type == IgneousCompositionTypes::mafic);
    	CHECK(stratum_types.metamorphic_grade == MetamorphicGrades::igneous_or_sediment);
    	CHECK(!stratum_types.rock_composition_types.partly_calcareous);
    	CHECK(!stratum_types.rock_composition_types.calcareous);
    	CHECK(!stratum_types.rock_composition_types.silicaceous);
    	CHECK(!stratum_types.rock_composition_types.feldspathic);
    	CHECK(stratum_types.rock_composition_types.volcanic);
    	CHECK(!stratum_types.rock_composition_types.ferrous);
    	CHECK(!stratum_types.rock_composition_types.organic);
    }
    SECTION("get_stratum_types() must recognize limestone"){
		StratumMassPool calcite;
		calcite.mass = 0.8f;
		calcite.grain_type_relative_volume[int(GrainType::chemically_weathered_intrusive)] = 1.0f;
		StratumMassPool quartz;
		quartz.mass = 0.2f;
		quartz.grain_type_relative_volume[int(GrainType::chemically_weathered_intrusive)] = 1.0f;

		Stratum<M> stratum(300*units::kelvin, 2e8*units::pascal, 0);
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::calcite)] = calcite;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)] = quartz;

		StratumTypes stratum_types = get_stratum_types(stratum);
    	CHECK(stratum_types.dominant_particle_size_bin == ParticleSizeBins::clay);
    	CHECK(stratum_types.igneous_formation_type == IgneousFormationTypes::intrusive);
    	CHECK(stratum_types.igneous_composition_type == IgneousCompositionTypes::other);
    	CHECK(stratum_types.metamorphic_grade == MetamorphicGrades::sedimentary);
    	CHECK(stratum_types.rock_composition_types.partly_calcareous);
    	CHECK(stratum_types.rock_composition_types.calcareous);
    	CHECK(!stratum_types.rock_composition_types.silicaceous);
    	CHECK(!stratum_types.rock_composition_types.feldspathic);
    	CHECK(!stratum_types.rock_composition_types.volcanic);
    	CHECK(!stratum_types.rock_composition_types.ferrous);
    	CHECK(!stratum_types.rock_composition_types.organic);
    }
    SECTION("get_stratum_types() must recognize marble"){
		StratumMassPool calcite;
		calcite.mass = 0.8f;
		calcite.grain_type_relative_volume[int(GrainType::chemically_weathered_intrusive)] = 1.0f;
		StratumMassPool quartz;
		quartz.mass = 0.2f;
		quartz.grain_type_relative_volume[int(GrainType::chemically_weathered_intrusive)] = 1.0f;

		Stratum<M> stratum(300*units::kelvin, 4e8*units::pascal, 0);
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::calcite)] = calcite;
		stratum.mass_pools[int(OxygenPlanetStratumMassPoolTypes::quartz)] = quartz;

		StratumTypes stratum_types = get_stratum_types(stratum);
    	CHECK(stratum_types.dominant_particle_size_bin == ParticleSizeBins::clay);
    	CHECK(stratum_types.igneous_formation_type == IgneousFormationTypes::intrusive);
    	CHECK(stratum_types.igneous_composition_type == IgneousCompositionTypes::other);
    	CHECK(stratum_types.metamorphic_grade == MetamorphicGrades::low);
    	CHECK(stratum_types.rock_composition_types.partly_calcareous);
    	CHECK(stratum_types.rock_composition_types.calcareous);
    	CHECK(!stratum_types.rock_composition_types.silicaceous);
    	CHECK(!stratum_types.rock_composition_types.feldspathic);
    	CHECK(!stratum_types.rock_composition_types.volcanic);
    	CHECK(!stratum_types.rock_composition_types.ferrous);
    	CHECK(!stratum_types.rock_composition_types.organic);
    }
}

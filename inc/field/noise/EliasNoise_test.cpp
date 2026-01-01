
// std libraries
#include <limits>
#include <string>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3

// in-house libraries
#include <index/whole.hpp>  
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>

#include "EliasNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "EliasNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-3);

    auto positions = procedural::get(
        procedural::vector_interleave<3>(
            procedural::UnitIntervalNoise<double>()), 
        procedural::Range(6000));

    REQUIRE(test::determinism(adapter,
        "EliasNoise(…)", 
        TEST_UNARY(
            field::elias_noise<double>(
                procedural::unit_vector_noise<3>(10.0, 1e4), 
                procedural::gaussian(11.0, 1.1e4), 
                100)),
        positions
    ));

    auto noise = field::elias_noise<double>(
        procedural::unit_vector_noise<3>(10.0, 1e4), 
        procedural::gaussian(11.0, 1.1e4), 
        100);

    REQUIRE(test::continuity(adapter,
        "EliasNoise(…)", TEST_UNARY(noise),
        "continuity",    TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = procedural::map(noise, positions);
    CHECK(whole::standard_deviation(out) > 4.0);
}


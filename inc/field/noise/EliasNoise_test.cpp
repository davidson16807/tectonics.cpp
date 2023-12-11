
// std libraries
#include <limits>
#include <string>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3

// in-house libraries
#include <index/each.hpp>  
#include <index/whole.hpp>  
#include <index/series/Get.hpp>
#include <index/series/Map.hpp>
#include <index/series/Range.hpp>
#include <index/series/glm/VectorInterleave.hpp>
#include <index/series/noise/GaussianNoise.hpp>
#include <index/series/noise/glm/UnitVectorNoise.hpp>

#include "EliasNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "EliasNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-3);

    auto positions = series::get(
        series::vector_interleave<3>(
            series::UnitIntervalNoise<double>()), 
        series::Range(6000));

    REQUIRE(test::determinism(adapter,
        "EliasNoise(…)", 
        TEST_UNARY(
            field::elias_noise<double>(
                series::unit_vector_noise<3>(10.0, 1e4), 
                series::gaussian(11.0, 1.1e4), 
                100)),
        positions
    ));

    auto noise = field::elias_noise<double>(
        series::unit_vector_noise<3>(10.0, 1e4), 
        series::gaussian(11.0, 1.1e4), 
        100);

    REQUIRE(test::continuity(adapter,
        "EliasNoise(…)", TEST_UNARY(noise),
        "continuity",    TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = series::map(noise, positions);
    CHECK(whole::standard_deviation(out) > 4.0);
}


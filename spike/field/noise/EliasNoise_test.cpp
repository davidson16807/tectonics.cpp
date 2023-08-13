
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
#include <store/each.hpp>  
#include <store/whole.hpp>  
#include <store/series/Get.hpp>
#include <store/series/Map.hpp>
#include <store/series/Range.hpp>
#include <store/series/glm/VectorInterleave.hpp>
#include <store/series/noise/GaussianNoise.hpp>
#include <store/series/noise/glm/UnitVectorNoise.hpp>

#include "EliasNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "EliasNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-5);

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


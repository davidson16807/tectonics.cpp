
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
#include <store/known.hpp>  
#include <store/series/Uniform.hpp>
#include <store/series/Get.hpp>
#include <store/series/Map.hpp>
#include <store/series/Range.hpp>
#include <store/series/glm/VectorInterleave.hpp>
#include <store/series/noise/UnitIntervalNoise.hpp>
#include <store/series/noise/glm/UnitVectorNoise.hpp>

#include "SquareNoise.hpp"
#include "WorleyNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "WorleyNoise()", "[series]" ) {
    test::GlmAdapter<int, double> adapter(1e-5);

    auto positions = known::mult(
        series::get(
            series::vector_interleave<3>(
                series::UnitIntervalNoise<double>()), 
            series::Range(10000)),
        series::uniform(100.0)
    );

    REQUIRE(test::determinism(adapter,
        "WorleyNoise(…)", 
        TEST_UNARY(
            field::worley_noise3(
                field::square_noise(
                    series::vector_interleave<3>(
                        series::unit_interval_noise(11.0, 1.1e4)
                    )))),
        positions
    ));

    auto noise = 
        field::worley_noise3(
            field::square_noise(
                series::vector_interleave<3>(
                    series::unit_interval_noise(11.0, 1.1e4)
                )));


    REQUIRE(test::continuity(adapter,
        "WorleyNoise(…)", TEST_UNARY(noise),
        "continuity",     TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = series::map(noise, positions);
    CHECK(whole::min(out) <  0.05);
    CHECK(whole::min(out) >= 0.00);
    CHECK(whole::max(out) >  0.1);
    CHECK(whole::standard_deviation(out) > 0.1);
}



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
#include <index/known.hpp>  
#include <index/series/Uniform.hpp>
#include <index/series/Get.hpp>
#include <index/series/Map.hpp>
#include <index/series/Range.hpp>
#include <index/series/glm/VectorInterleave.hpp>
#include <index/series/noise/UnitIntervalNoise.hpp>
#include <index/series/noise/glm/UnitVectorNoise.hpp>

#include "MosaicNoise.hpp"
#include "WorleyNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "WorleyNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-5);

    auto positions = known::mult(
        series::get(
            series::vector_interleave<3>(
                series::UnitIntervalNoise<double>()), 
            series::Range(10000)),
        series::uniform(100.0)
    );

    auto noise = 
        field::worley_noise<3,double>(
            field::mosaic_noise(
                series::vector_interleave<3>(series::unit_interval_noise(11.0, 1.1e4))),
            field::vector_mosaic_ops<int,double>()
        );

    REQUIRE(test::determinism(adapter,
        "WorleyNoise(…)", 
        TEST_UNARY(noise),
        positions
    ));

    REQUIRE(test::continuity(adapter,
        "WorleyNoise(…)", TEST_UNARY(noise),
        "continuity",     TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = series::map(noise, positions);
    CHECK(whole::min(out) <  0.1);
    CHECK(whole::min(out) >= 0.00);
    CHECK(whole::max(out) >  1.0);
    CHECK(whole::standard_deviation(out) > 0.1);
}


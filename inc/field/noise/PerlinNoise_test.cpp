
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
#include <index/whole.hpp>  
#include <index/known.hpp>  
#include <index/procedural/Uniform.hpp>
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>

#include "MosaicNoise.hpp"
#include "PerlinNoise.hpp"
#include "MosaicOps.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "PerlinNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-5);

    auto positions = known::mult(
        procedural::get(
            procedural::vector_interleave<3>(
                procedural::UnitIntervalNoise<double>()), 
            procedural::Range(10000)),
        procedural::uniform(100.0)
    );

    auto noise = 
        field::perlin_noise<3,double>(
            field::mosaic_noise(
                procedural::vector_interleave<3>(procedural::gaussian(11.0, 1.1e4))),
            field::vector_mosaic_ops<3,int,double>()
        );

    REQUIRE(test::determinism(adapter,
        "PerlinNoise(…)", 
        TEST_UNARY(noise),
        positions
    ));

    REQUIRE(test::continuity(adapter,
        "PerlinNoise(…)", TEST_UNARY(noise),
        "continuity",     TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = procedural::map(noise, positions);
    CHECK(std::abs(whole::mean(out)) < 0.05);
    CHECK(whole::standard_deviation(out) > 0.1);
}


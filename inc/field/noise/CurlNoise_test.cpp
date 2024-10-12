
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
#include <index/glm/known.hpp>  
#include <index/procedural/Uniform.hpp>
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/UnitIntervalNoise.hpp>
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>

#include <field/VectorZip.hpp>

#include "MosaicNoise.hpp"
#include "SimplexNoise.hpp"
#include "CurlNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

TEST_CASE( "CurlNoise()", "[field]" ) {
    test::GlmAdapter<int, double> adapter(1e-4);

    auto positions = known::mult(
        procedural::get(
            procedural::vector_interleave<3>(
                procedural::UnitIntervalNoise<double>()), 
            procedural::Range(10000)),
        procedural::uniform(100.0)
    );

    REQUIRE(test::determinism(adapter,
        "CurlNoise(…)", 
        TEST_UNARY(
            field::curl_noise3(
                field::vector3_zip(
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(10.0, 1.0e4)
                            )
                        )
                    ),
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(11.0, 1.1e4)
                            )
                        )
                    ),
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(12.0, 1.2e4)
                            )
                        )
                    )
                )
            )
        ),
        positions
    ));

    auto noise = 
            field::curl_noise3(
                field::vector3_zip(
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(10.0, 1.0e4)
                            )
                        )
                    ),
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(11.0, 1.1e4)
                            )
                        )
                    ),
                    field::simplex_noise3(
                        field::mosaic_noise(
                            procedural::vector_interleave<3>(
                                procedural::unit_interval_noise(12.0, 1.2e4)
                            )
                        )
                    )
                )
            )
    ;


    REQUIRE(test::continuity(adapter,
        "CurlNoise(…)", TEST_UNARY(noise),
        "continuity",   TEST_NUDGE(glm::dvec3(1e-6,1e-6,1e-6)),
        positions
    ));

    auto out = procedural::map(noise, positions);
    CHECK(whole::min(known::length<double>(out)) <  0.05);
    CHECK(whole::min(known::length<double>(out)) >= 0.00);
    CHECK(whole::max(known::length<double>(out)) >  0.1);
    CHECK(whole::standard_deviation(known::length<double>(out)) > 0.1);
}


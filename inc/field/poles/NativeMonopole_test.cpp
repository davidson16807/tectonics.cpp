// std libraries
#include <limits>
#include <string>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// in-house libraries
#include <index/whole.hpp>
#include <index/known.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/UnitIntervalNoise.hpp>

#include "NaiveMultipole.hpp"

#include <test/properties.hpp>
#include <test/equality.hpp>
#include <test/macros.hpp>
#include <test/glm/adapter.hpp>

TEST_CASE( "NaiveMultipole()", "[field]" ) {
    
    using vec3 = glm::dvec3;

    test::GlmAdapter<int, double> adapter(1e-5);

    auto sample_positions = known::mult(
        procedural::get(
            procedural::vector_interleave<3>(
                procedural::UnitIntervalNoise<double>()),
            procedural::Range(1000)),
        procedural::uniform(100.0)
    );

    auto particle_positions = known::mult(
        procedural::get(
            procedural::vector_interleave<3>(
                procedural::UnitIntervalNoise<double>()),
            procedural::Range(200)),
        procedural::uniform(100.0)
    );

    auto particle_weights = known::mult(
        procedural::get(
            procedural::unit_interval_noise<double>(17.0, 9.0e3),
            procedural::Range(200)),
        procedural::uniform(10.0)
    );

    field::NaiveMultipole<double, glm::dvec3> naive(1.0);

    for (int i = 0; i < 200; ++i) {
        naive.add(particle_positions[i], particle_weights[i]);
    }

    REQUIRE(test::determinism(adapter,
        "NaiveMultipole(…)", TEST_UNARY(naive),
        sample_positions
    ));

    naive.clear();
    REQUIRE(test::equality(adapter,
        "NaiveMultipole.clear() produces zero field",
        "NaiveMultipole(…)", TEST_UNARY(naive),
        "zero",
        [](const auto& x) { return glm::dvec3(0.0); },
        sample_positions
    ));

    field::NaiveMultipole<double, vec3> forward(1.0);
    field::NaiveMultipole<double, vec3> reverse(1.0);

    for (int i = 0; i < 200; ++i) {
        forward.add(particle_positions[i], particle_weights[i]);
    }
    for (int i = 199; i >= 0; --i) {
        reverse.add(particle_positions[i], particle_weights[i]);
    }

    REQUIRE(test::equality(adapter,
        "NaiveMultipole insertion order does not matter",
        "forward(…)", TEST_UNARY(forward),
        "reverse(…)", TEST_UNARY(reverse),
        sample_positions
    ));

    field::NaiveMultipole<double, vec3> base(1.0);
    field::NaiveMultipole<double, vec3> scaled(1.0);

    const double k = 7.0;

    for (int i = 0; i < 200; ++i) {
        base.add(particle_positions[i], particle_weights[i]);
        scaled.add(particle_positions[i], k * particle_weights[i]);
    }

    REQUIRE(test::equality(adapter,
        "Scaling weights scales NaiveMultipole output",
        "scaled",
        [&](const auto& x) { return scaled(x); },
        "k*base",
        [&](const auto& x) { return k * base(x); },
        sample_positions
    ));

}


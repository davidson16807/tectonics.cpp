
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
#include <store/series/noise/UnitIntervalNoise.hpp>
#include <store/series/noise/GaussianNoise.hpp>
#include <store/series/noise/glm/UnitVectorNoise.hpp>

#include "EliasNoise.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  
#include <test/glm/adapter.hpp>


// determinism
// nonzero standard deviation
// smoothness (small changes to input cause small to output)

namespace field {

    template<typename T>
    struct Adapter{
        T threshold;

        Adapter(const T threshold):
            threshold(threshold)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            return whole::distance(a,b) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            return whole::to_string(a);
        }

    };

}

TEST_CASE( "EliasNoise()", "[series]" ) {
    test::GlmAdapter<int, double> adapter(1e-5);
    // field::Adapter<double> adapter (1e-5);

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
    CHECK(std::abs(whole::standard_deviation(out)-1.0) > 3.3);
}


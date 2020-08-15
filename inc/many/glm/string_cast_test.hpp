

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/string_cast.hpp>  

using namespace glm;
using namespace many;

TEST_CASE( "many<vec2> string cast correctness", "[many]" ) {
    SECTION("to_string() must produce obvious results for straight forward 2d vectors"){
        REQUIRE_THAT(to_string(vec2s({vec2( 0,   2),vec2( 0,   5)})), Catch::Contains("↑") && Catch::Contains("⬆"));
        REQUIRE_THAT(to_string(vec2s({vec2( 0,  -2),vec2( 0,  -5)})), Catch::Contains("↓") && Catch::Contains("⬇"));
        REQUIRE_THAT(to_string(vec2s({vec2( 2,   0),vec2( 5,   0)})), Catch::Contains("→") && Catch::Contains("➡"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   0),vec2(-5,   0)})), Catch::Contains("←") && Catch::Contains("⬅"));
    }
    SECTION("to_string() must behave correctly around edge case for atan2()"){
        // WARNING: white box testing, we need to test left arrows extra carefully since they cause a discontinuity in atan2()
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   2),vec2(-5,   5)})), Catch::Contains("↖") && Catch::Contains("⬉"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,-0.1),vec2(-5,-0.1)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   0),vec2(-5,   0)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2, 0.1),vec2(-5,-0.1)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,  -2),vec2(-5,  -5)})), Catch::Contains("↙") && Catch::Contains("⬋"));
    }
}

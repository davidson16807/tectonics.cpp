
#include <iostream> //std::cout
#include <sstream>  //std::stringstream

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include "../LayeredGrid/LayeredRaster.hpp"
#include "../LayeredGrid/layering.hpp"
#include "LayeredSpheroidGrid.hpp"
#include "string_cast.hpp"

#include "LayeredSpheroidGrid_test_utils.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

TEST_CASE( "LayeredSpheroidRaster string cast purity", "[rasters]" ) {
    auto a = make_LayeredRaster(layered_octahedron_grid, 
        { 1, 2, 3, 4, 5, 6,
          7, 8, 9,10,11,12 });
    auto v2 = make_LayeredRaster(layered_octahedron_grid, 
        {
            vec2(-1,-1),
            vec2(-1, 0),
            vec2(-1, 1),

            vec2( 0,-1),
            vec2( 0, 0),
            vec2( 0, 1),

            vec2( 1,-1),
            vec2( 1, 0),
            vec2( 1, 1),

            vec2(-1,-1),
            vec2(-1, 0),
            vec2(-1, 1),
        });
    auto v3 = make_LayeredRaster(layered_octahedron_grid, 
        {
            vec3(-1,-1,-1),
            vec3(-1,-1, 0),
            vec3(-1,-1, 1),

            vec3(-1, 0,-1),
            vec3(-1, 0, 0),
            vec3(-1, 0, 1),

            vec3(-1, 1,-1),
            vec3(-1, 1, 0),
            vec3(-1, 1, 1),

            vec3( 0,-1,-1),
            vec3( 0,-1, 0),
            vec3( 0,-1, 1),
        });
    SECTION("to_string(grid, floats) must be called repeatedly without changing the output"){
        CHECK(to_string(a) == to_string(a));
    }
    SECTION("to_string(grid, vec2s) must be called repeatedly without changing the output"){
        CHECK(to_string(v2) == to_string(v2));
    }
    SECTION("to_string(grid, vec3s) must be called repeatedly without changing the output"){
        CHECK(to_string(v3) == to_string(v3));
    }
}
TEST_CASE( "LayeredSpheroidRaster string cast correctness", "[rasters]" ) {
    auto a = make_LayeredRaster(layered_octahedron_grid, 
        { 1, 2, 3, 4, 5, 6,
          7, 8, 9,10,11,12 });
    auto v2 = make_LayeredRaster(layered_octahedron_grid, 
        {
            vec2(-1,-1),
            vec2(-1, 0),
            vec2(-1, 1),

            vec2( 0,-1),
            vec2( 0, 0),
            vec2( 0, 1),

            vec2( 1,-1),
            vec2( 1, 0),
            vec2( 1, 1),

            vec2(-1,-1),
            vec2(-1, 0),
            vec2(-1, 1),
        });
    auto v3 = make_LayeredRaster(layered_octahedron_grid, {
            vec3( 0,-1,-1),
            vec3( 0,-1, 0),
            vec3( 0,-1, 1),

            vec3( 0, 0,-1),
            vec3( 0, 0, 0),
            vec3( 0, 0, 1),

            vec3( 0, 1,-1),
            vec3( 0, 1, 0),
            vec3( 0, 1, 1),

            vec3(-1,-1,-1),
            vec3(-1,-1, 0),
            vec3(-1,-1, 1),
        });
    SECTION("to_string(grid, floats) must contain appropriate characters"){
        REQUIRE_THAT(to_string(a), 
                    Catch::Contains(" ") && 
                    Catch::Contains("░") && 
                    Catch::Contains("▒") && 
                    Catch::Contains("▓") && 
                    Catch::Contains("█")
                );
    }
    SECTION("to_string(grid, floats) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(a), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string(grid, vec2s) must depict 2d vector Raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(v2), 
                    (Catch::Contains(" ")                        ) && 
                    (Catch::Contains("←") || Catch::Contains("⬅")) && 
                    (Catch::Contains("↑") || Catch::Contains("⬆")) && 
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                    (Catch::Contains("↓") || Catch::Contains("⬇")) &&
                    (Catch::Contains("↖") || Catch::Contains("⬉")) &&
                    (Catch::Contains("↙") || Catch::Contains("⬋")) &&
                    (Catch::Contains("↘") || Catch::Contains("⬊")) &&
                    (Catch::Contains("↗") || Catch::Contains("⬈"))
                );
    }
    SECTION("to_string(grid, vec2s) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(v2), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string() must depict uniform 3d vector Raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(v3), 
                    (Catch::Contains(" ")                        ) && 
                   !(Catch::Contains("←") || Catch::Contains("⬅")) &&    
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                    (Catch::Contains("↑") || Catch::Contains("⬆")) && 
                    (Catch::Contains("↓") || Catch::Contains("⬇")) &&
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                    (Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                    (Catch::Contains("↗") || Catch::Contains("⬈"))
                );
    }
    SECTION("to_string(grid, vec2s) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(v3), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
}
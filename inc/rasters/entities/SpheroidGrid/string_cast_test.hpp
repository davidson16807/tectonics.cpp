
// std libraries
#include <iostream>  //std::cout

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/series.hpp>  
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

#include "../Grid/Raster.hpp"
#include "SpheroidGrid.hpp"
#include "string_cast.hpp"
#include "SpheroidGrid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "SpheroidRaster string cast purity", "[rasters]" ) {
    auto a = make_Raster(octahedron_grid, {1,2,3,4,5,6});
    auto v2 = make_Raster(octahedron_grid, {
                    glm::vec2( 0,-1),
                    glm::vec2( 0, 1),
                    glm::vec2(-1, 0),
                    glm::vec2( 1, 0),
                    glm::vec2( 0, 0),
                    glm::vec2( 0, 0),
                });
    auto v3 = make_Raster(octahedron_grid, {
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0)
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
TEST_CASE( "SpheroidRaster string cast correctness", "[rasters]" ) {
    auto a = make_Raster(octahedron_grid, {1,2,3,4,5,6});
    auto v2 = make_Raster(octahedron_grid, {
                    glm::vec2( 0,-1),
                    glm::vec2( 0, 1),
                    glm::vec2(-1, 0),
                    glm::vec2( 1, 0),
                    glm::vec2( 0, 0),
                    glm::vec2( 0, 0),
                });
    auto v3 = make_Raster(octahedron_grid, {
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0),
                    glm::vec3( 1, 0, 0)
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
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                   !(Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                   !(Catch::Contains("↗") || Catch::Contains("⬈"))
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
                    (Catch::Contains("←") || Catch::Contains("⬅")) && 
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                   !(Catch::Contains("↑") || Catch::Contains("⬆")) && 
                   !(Catch::Contains("↓") || Catch::Contains("⬇")) &&
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                   !(Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                   !(Catch::Contains("↗") || Catch::Contains("⬈"))
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

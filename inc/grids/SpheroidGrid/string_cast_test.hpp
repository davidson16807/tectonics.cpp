
// std libraries
#include <iostream>  //std::cout

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>
#include "string_cast.hpp"

using namespace glm;
using namespace many;
using namespace rasters;


/*
"octahedron" is a simple 3d grid for testing raster operations that require 
something comparable to a unit sphere (e.g. nearest neighbor lookups using SpheroidVoronoi)
*/
SpheroidGrid octahedron = SpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces);

/*
"icosahedron" is a simple 3d grid for testing rasters with a relatively large number of vertices
*/
SpheroidGrid icosahedron = SpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

TEST_CASE( "raster string cast purity", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec2s v2 = vec2s({
                    vec2( 0,-1),
                    vec2( 0, 1),
                    vec2(-1, 0),
                    vec2( 1, 0),
                    vec2( 0, 0),
                    vec2( 0, 0),
                });
    vec3s v3 = vec3s({
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0)
                });
    SECTION("to_string(grid, floats) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, a) == to_string(octahedron, a));
    }
    SECTION("to_string(grid, vec2s) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, v2) == to_string(octahedron, v2));
    }
    SECTION("to_string(grid, vec3s) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, v3) == to_string(octahedron, v3));
    }
}
TEST_CASE( "raster string cast correctness", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec2s v2 = vec2s({
                    vec2( 0,-1),
                    vec2( 0, 1),
                    vec2(-1, 0),
                    vec2( 1, 0),
                    vec2( 0, 0),
                    vec2( 0, 0),
                });
    vec3s v3 = vec3s({
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0)
                });
    SECTION("to_string(grid, floats) must contain appropriate characters"){
        REQUIRE_THAT(to_string(octahedron, a), 
                    Catch::Contains(" ") && 
                    Catch::Contains("░") && 
                    Catch::Contains("▒") && 
                    Catch::Contains("▓") && 
                    Catch::Contains("█")
                );
    }
    SECTION("to_string(grid, floats) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(octahedron, a), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string(grid, vec2s) must depict 2d vector raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(octahedron, v2), 
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
        REQUIRE_THAT(to_string(octahedron, v2), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string() must depict uniform 3d vector raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(octahedron, v3), 
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
        REQUIRE_THAT(to_string(octahedron, v3), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
}

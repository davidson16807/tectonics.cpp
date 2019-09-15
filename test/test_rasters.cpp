

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/types.hpp>
#include <rasters/mesh.hpp>
#include <rasters/string_cast.hpp>  
#include <rasters/glm/glm.hpp>
#include <rasters/glm/string_cast.hpp>  
#include <rasters/glm/vector_calculus.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

/* 
"diamond" is a simple 2d grid for testing raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
std::shared_ptr<Grid> diamond = 
    std::make_shared<Grid>(
        vec3s({
                vec3( 0, 0, 0),
                vec3( 1, 0, 0),
                vec3( 0, 1, 0),
                vec3(-1, 0, 0),
                vec3( 0,-1, 0)
            }),
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,4),
                uvec3(0,3,2),
                uvec3(0,3,4)
            })
    );
/*
"tetrahedron" is a simple 3d grid for testing basic raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
std::shared_ptr<Grid> tetrahedron = 
    std::make_shared<Grid>(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
/*
"octahedron" is a simple 3d grid for testing raster operations that require 
something comparable to a unit sphere (e.g. nearest neighbor lookups using SpheroidGridVoronoi)
*/
SpheroidGrid octahedron = 
    SpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces);

/*
"icosahedron" is a simple 3d grid for testing rasters with a large number of vertices
*/
SpheroidGrid icosahedron = 
    SpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

SpheroidGridVoronoi voronoi_test(
        normalize(vec3s({
                        vec3( 1, 0, 0),
                        vec3( 0, 1, 0),
                        vec3( 0, 0, 1),
                        vec3(-1, 0, 0),
                        vec3( 0,-1, 0),
                        vec3( 0, 0,-1),
                        vec3(-1,-1,-1),
                        vec3( 1,-1,-1),
                        vec3(-1, 1,-1),
                        vec3( 1, 1,-1),
                        vec3(-1,-1, 1),
                        vec3( 1,-1, 1),
                        vec3(-1, 1, 1),
                        vec3( 1, 1, 1),
                        vec3( 1, 1, 1)
                    })),
        1./100.,
        10./100.
    );


TEST_CASE( "raster string cast must render correct representation", "[many]" ) {

    SECTION("to_string() must use all shades for equally distributed raster values"){
        REQUIRE_THAT(to_string(octahedron, floats({1,2,3,4,5,6})), 
                    Catch::Contains(" ") && 
                    Catch::Contains("░") && 
                    Catch::Contains("▒") && 
                    Catch::Contains("▓") && 
                    Catch::Contains("█")
                );
    }
    SECTION("to_string() must depict 2d vector raster using only arrows that are appropriate"){
        REQUIRE_THAT(to_string(octahedron, vec2s({
                            vec2( 0,-1),
                            vec2( 0, 1),
                            vec2(-1, 0),
                            vec2( 1, 0),
                            vec2( 0, 0),
                            vec2( 0, 0),
                        })), 
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
    SECTION("to_string() must depict uniform 3d vector raster using only arrows that are appropriate"){
        REQUIRE_THAT(to_string(octahedron, vec3s({
                            vec3( 1, 0, 0),
                            vec3( 1, 0, 0),
                            vec3( 1, 0, 0),
                            vec3( 1, 0, 0),
                            vec3( 1, 0, 0),
                            vec3( 1, 0, 0)
                        })), 
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
}

TEST_CASE( "Must be able to test equivalence of rasters using the catch framework", "[rasters]" ) {
    raster ref       = raster(diamond, {1,2,3,4,5});
    raster ref_copy  = raster(diamond, {1,2,3,4,5});
    raster ref_tweak = raster(diamond, {1,2,3,4,0});
    raster ref_fewer = raster(tetrahedron, {1,2,3,4});

    floats a = floats({1,2,3,4,5,6});

    SECTION("Must be able to equate object with itself"){
        CHECK(ref == ref);
    }
    SECTION("Must be able to equate rasters of the same content and size"){
        CHECK(ref == ref_copy);
    }
    SECTION("Must be able to distinguish rasters of slightly different content"){
        CHECK(ref != ref_tweak);
    }
    SECTION("Must be able to distinguish rasters of slightly different size"){
        CHECK(ref != ref_fewer);
    }
}

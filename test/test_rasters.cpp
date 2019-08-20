

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <composites/many.hpp>        //  floats, etc.
#include <composites/common.hpp>      //  floats, etc.
#include <composites/glm/vecs.hpp>    // *vec*s

#include "rasters/raster.hpp"         
#include "rasters/glm/vec_raster.hpp"
#include "rasters/glm/vector_calculus.hpp"

// #include "academics/tectonics.hpp"

using namespace composites;
using namespace rasters;

std::shared_ptr<Grid> tetrahedron = 
    std::make_shared<Grid>(
        vec3s({
                vec3(0,0,0),
                vec3(1,0,0),
                vec3(0,1,0),
                vec3(0,0,1)
            }),
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,3),
                uvec3(0,2,3),
                uvec3(1,2,3)
            })
    );
/* "diamond" is a 2d grid, it looks like this:
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

SphereGridVoronoi3d voronoi_test(
        vec3s({
                normalize(vec3( 1, 0, 0)),
                normalize(vec3( 0, 1, 0)),
                normalize(vec3( 0, 0, 1)),
                normalize(vec3(-1, 0, 0)),
                normalize(vec3( 0,-1, 0)),
                normalize(vec3( 0, 0,-1)),
                normalize(vec3(-1,-1,-1)),
                normalize(vec3( 1,-1,-1)),
                normalize(vec3(-1, 1,-1)),
                normalize(vec3( 1, 1,-1)),
                normalize(vec3(-1,-1, 1)),
                normalize(vec3( 1,-1, 1)),
                normalize(vec3(-1, 1, 1)),
                normalize(vec3( 1, 1, 1)),
                normalize(vec3( 1, 1, 1))
            }),
        1./100.
    );


TEST_CASE( "make sure rasters work", "[rasters]" ) {

    // stratum_mass_pool test = stratum_mass_pool();

    // SECTION("stratum_mass_pool.max_pressure_received() must have the inverse property within the valid range"){
    //     test.max_pressure_received(1e9);
    //     REQUIRE(test.max_pressure_received() == Approx(1e9).epsilon(0.12));
    // }
}


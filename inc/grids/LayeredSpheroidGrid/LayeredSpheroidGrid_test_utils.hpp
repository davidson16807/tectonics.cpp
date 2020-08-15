#pragma once


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/glm/glm.hpp>         // *vec*s

#include <grids/LayeredSpheroidGrid/LayeredSpheroidGrid.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

/* 
"layered_diamond" is a simple 2d grid for testing raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
LayeredSpheroidGrid layered_diamond = 
    LayeredSpheroidGrid(
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
            }),
        1.f, 0.f, 2u
    );
/*
"layered_tetrahedron" is a simple 3d LayeredSpheroidGrid for testing basic raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
LayeredSpheroidGrid layered_tetrahedron = LayeredSpheroidGrid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces, 1.f, 0.f, 2u);

/*
"layered_octahedron" is a simple 3d grid for testing raster operations that require 
something comparable to a unit sphere (e.g. nearest neighbor lookups using SpheroidVoronoi)
*/
LayeredSpheroidGrid layered_octahedron = LayeredSpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces, 1.f, 0.f, 2u);

/*
"layered_icosahedron" is a simple 3d grid for testing rasters with a relatively large number of vertices
*/
LayeredSpheroidGrid layered_icosahedron = LayeredSpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces, 1.f, 0.f, 2u);

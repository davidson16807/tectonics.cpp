#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/types.hpp>
#include <many/glm/types.hpp>

#include <meshes/mesh.hpp>

#include "Grid.hpp"

/* 
"diamond_grid" is a simple 2d grid for testing Raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
rasters::Grid<int,float> diamond_grid(
        many::vec3s({
                glm::vec3( 0, 0, 0),
                glm::vec3( 1, 0, 0),
                glm::vec3( 0, 1, 0),
                glm::vec3(-1, 0, 0),
                glm::vec3( 0,-1, 0)
            }),
        many::uvec3s({
                glm::uvec3(0,1,2),
                glm::uvec3(0,1,4),
                glm::uvec3(0,3,2),
                glm::uvec3(0,3,4)
            })
    );

/*
"tetrahedron_grid" is a simple 3d grid for testing basic Raster operations
 that require spatial awareness without requiring a particular shape or resolution
 (e.g. gradient, divergence, laplacian)
*/
rasters::Grid<int,float> tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);

rasters::Grid<int,float> nonspheroid_octahedron_grid(meshes::octahedron.vertices, meshes::octahedron.faces);

rasters::Grid<int,float> nonspheroid_icosahedron_grid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

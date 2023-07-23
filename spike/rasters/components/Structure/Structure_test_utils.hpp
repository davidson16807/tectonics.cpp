#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include "Structure.hpp" 

using namespace glm;
using namespace series;
using namespace rasters;
/* 
"diamond_structure" is a simple 2d grid for testing Raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
Structure diamond_structure = 
    Structure(
        5u, 
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,4),
                uvec3(0,3,2),
                uvec3(0,3,4)
            })
    );

/*
"tetrahedron_structure" is a simple 3d grid for testing basic Raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
Structure tetrahedron_structure = Structure(4u, meshes::tetrahedron.faces);

#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/types.hpp>
#include <many/glm/types.hpp>

#include <meshes/mesh.hpp>

#include "SpheroidGrid.hpp"

#include <rasters/entities/Grid/Grid_test_utils.hpp>

rasters::SpheroidGrid<int,float> octahedron_grid(meshes::octahedron.vertices, meshes::octahedron.faces);

rasters::SpheroidGrid<int,float> icosahedron_grid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

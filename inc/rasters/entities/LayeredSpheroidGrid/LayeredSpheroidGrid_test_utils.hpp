#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

// in-house libraries
#include "LayeredSpheroidGrid.hpp"

#include <rasters/entities/SpheroidGrid/SpheroidGrid_test_utils.hpp>

rasters::LayeredSpheroidGrid<int,float> layered_octahedron_grid(octahedron_grid, 1.0f, 0.0f, 2u);

rasters::LayeredSpheroidGrid<int,float> layered_icosahedron_grid(icosahedron_grid, 1.0f, 0.0f, 2u);

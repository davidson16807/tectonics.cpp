

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly

#include "Grid/Grid.hpp"  

using namespace rasters;

TEST_CASE( "Raster consistency 2", "[rasters]" ) {
    auto a = make_raster<float>(tetrahedron_grid);
    SECTION("Elements of raster of grid must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.cell_count(rasters::mapping::cell) );
    }
}
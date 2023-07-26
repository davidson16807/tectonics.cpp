

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly

#include "Raster.hpp"  

using namespace rasters;

TEST_CASE( "Raster consistency", "[rasters]" ) {
    auto a = make_Raster<float>(tetrahedron_grid);
    auto b = make_Raster<float, mapping::arrow>(tetrahedron_grid);
    SECTION("Elements of Raster of layered grid must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.raster_size(rasters::mapping::cell) );
        CHECK(b.size() == b.grid.raster_size(rasters::mapping::arrow) );
    }
}
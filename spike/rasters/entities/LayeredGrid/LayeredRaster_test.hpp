

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/series.hpp>  
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>
#include "../Grid/Raster.hpp"

#include "LayeredGrid.hpp"

#include "LayeredGrid_test_utils.hpp"

using namespace rasters;


//More thorough tests are available in components/*_test.hpp files,
//these tests simply establish that the components have been instantiated
//and with the correct paramater values

TEST_CASE( "LayeredRaster consistency", "[rasters]" ) {
    auto a = make_Raster<float>(layered_tetrahedron_grid);
    auto b = make_Raster<float, mapping::arrow>(layered_tetrahedron_grid);
    SECTION("Elements of Raster of layered grid must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.raster_size(mapping::cell) );
        CHECK(b.size() == b.grid.raster_size(mapping::arrow) );
    }
}
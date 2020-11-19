
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE     // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/mat3x3.hpp>             // *mat3
#include <glm/mat4x4.hpp>             // *mat4
#include <glm/gtc/matrix_transform.hpp> // rotate

#include <series/types.hpp>
#include <series/glm/types.hpp>
#include <series/glm/random.hpp>  

#include <rasters/entities/LayeredGrid/LayeredRaster.hpp>
#include <rasters/entities/LayeredSpheroidGrid/LayeredSpheroidGrid.hpp>  
#include <rasters/entities/LayeredSpheroidGrid/interpolation.hpp>

#include "LayeredSpheroidGrid_test_utils.hpp"

TEST_CASE( "LayeredSpheroidGrid nearest_neighbor_interpolation purity", "[many]" ) {
    SECTION("nearest_neighbor_interpolation must be called repeatedly without changing the output"){
        // initialize mesh
        auto mesh1 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        auto mesh2 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        mult(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)))), mesh2.vertices, mesh2.vertices);

        // initialize grids
        auto grid1 = rasters::LayeredSpheroidGrid<int,float>(rasters::SpheroidGrid<int,float>(mesh1.vertices, mesh1.faces), 1.0f, 0.0f, 2u);
        auto grid2 = rasters::LayeredSpheroidGrid<int,float>(rasters::SpheroidGrid<int,float>(mesh2.vertices, mesh2.faces), 1.0f, 0.0f, 2u);

        // initialize rasters
        std::mt19937 generator(2);
        auto raster1 = get_random_LayeredSpheroidRaster(grid1, generator);
        auto raster2 = get_random_LayeredSpheroidRaster(grid2, generator);
        auto raster3 = get_random_LayeredSpheroidRaster(grid2, generator);

        // populate rasters
        rasters::nearest_neighbor_interpolation(raster1, raster2);
        rasters::nearest_neighbor_interpolation(raster1, raster3);

        CHECK(raster2 == raster3);
    }
}

TEST_CASE( "LayeredSpheroidGrid nearest_neighbor_interpolation identity", "[many]" ) {
    SECTION("a set of input must exist for nearest_neighbor_interpolation that when fed produces the original input"){
        // initialize rasters
        std::mt19937 generator(2);
        auto raster1 = get_random_LayeredSpheroidRaster(layered_icosahedron_grid, generator);
        auto raster2 = get_random_LayeredSpheroidRaster(layered_icosahedron_grid, generator);

        rasters::nearest_neighbor_interpolation(raster1, raster2);

        CHECK(raster1 == raster2);
    }
}


TEST_CASE( "LayeredSpheroidGrid nearest_neighbor_interpolation invertibility", "[many]" ) {
    SECTION("nearest_neighbor_interpolation can be reverted by interpolating back to original grid"){
        // initialize mesh
        auto mesh1 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        auto mesh2 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        mult(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)))), mesh2.vertices, mesh2.vertices);

        // initialize grids
        auto grid1 = rasters::LayeredSpheroidGrid<int,float>(rasters::SpheroidGrid<int,float>(mesh1.vertices, mesh1.faces), 1.0f, 0.0f, 2u);
        auto grid2 = rasters::LayeredSpheroidGrid<int,float>(rasters::SpheroidGrid<int,float>(mesh2.vertices, mesh2.faces), 1.0f, 0.0f, 2u);

        // initialize rasters
        std::mt19937 generator(2);
        auto raster1 = get_random_LayeredSpheroidRaster(grid1, generator);
        auto raster2 = get_random_LayeredSpheroidRaster(grid2, generator);
        auto raster3 = get_random_LayeredSpheroidRaster(grid1, generator);

        // populate rasters
        rasters::nearest_neighbor_interpolation(raster1, raster2);
        rasters::nearest_neighbor_interpolation(raster2, raster3);

        CHECK(raster1 == raster3);
    }
}
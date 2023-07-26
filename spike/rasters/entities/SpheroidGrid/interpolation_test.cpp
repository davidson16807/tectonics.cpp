
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

#include <rasters/entities/Grid/Raster.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>  
#include <rasters/entities/SpheroidGrid/interpolation.hpp>

#include "SpheroidGrid_test_utils.hpp"

TEST_CASE( "SpheroidGrid nearest_neighbor_interpolation purity", "[many]" ) {
    SECTION("nearest_neighbor_interpolation must be called repeatedly without changing the output"){
        // initialize mesh
        auto mesh1 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        auto mesh2 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        mult(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)))), mesh2.vertices, mesh2.vertices);

        // initialize grids
        auto grid1 = rasters::SpheroidGrid<uint,float>(mesh1.vertices, mesh1.faces);
        auto grid2 = rasters::SpheroidGrid<uint,float>(mesh2.vertices, mesh2.faces);

        // initialize rasters
        auto raster1 = rasters::make_Raster<float>(grid1);
        auto raster2 = rasters::make_Raster<float>(grid2);
        auto raster3 = rasters::make_Raster<float>(grid2);

        // populate rasters
        std::mt19937 generator(2);
        each::get_elias_noise(icosahedron_grid.metrics->vertex_positions, generator, raster1);
        rasters::nearest_neighbor_interpolation(raster1, raster2);
        rasters::nearest_neighbor_interpolation(raster1, raster3);

        CHECK(raster2 == raster3);
    }
}

TEST_CASE( "SpheroidGrid nearest_neighbor_interpolation identity", "[many]" ) {
    SECTION("a set of input must exist for nearest_neighbor_interpolation that when fed produces the original input"){
        // initialize rasters
        auto raster1 = rasters::make_Raster<float>(icosahedron_grid);
        auto raster2 = rasters::make_Raster<float>(icosahedron_grid);

        // populate rasters
        std::mt19937 generator(2);
        each::get_elias_noise(icosahedron_grid.metrics->vertex_positions, generator, raster1);
        rasters::nearest_neighbor_interpolation(raster1, raster2);

        CHECK(raster1 == raster2);
    }
}


TEST_CASE( "SpheroidGrid nearest_neighbor_interpolation invertibility", "[many]" ) {
    SECTION("nearest_neighbor_interpolation can be reverted by interpolating back to original grid"){
        // initialize mesh
        auto mesh1 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        auto mesh2 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        mult(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)))), mesh2.vertices, mesh2.vertices);

        // initialize grids
        auto grid1 = rasters::SpheroidGrid<uint,float>(mesh1.vertices, mesh1.faces);
        auto grid2 = rasters::SpheroidGrid<uint,float>(mesh2.vertices, mesh2.faces);

        // initialize rasters
        auto raster1 = rasters::make_Raster<float>(grid1);
        auto raster2 = rasters::make_Raster<float>(grid2);
        auto raster3 = rasters::make_Raster<float>(grid1);

        // populate rasters
        std::mt19937 generator(2);
        each::get_elias_noise(icosahedron_grid.metrics->vertex_positions, generator, raster1);
        rasters::nearest_neighbor_interpolation(raster1, raster2);
        rasters::nearest_neighbor_interpolation(raster2, raster3);

        CHECK(raster1 == raster3);
    }
}
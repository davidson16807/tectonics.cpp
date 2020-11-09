
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/types.hpp>
#include <many/glm/types.hpp>
#include <many/glm/random.hpp>  

#include <rasters/entities/Grid/Raster.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>  
#include <rasters/entities/SpheroidGrid/interpolation.hpp>

TEST_CASE( "nearest_neighbor_interpolation purity", "[many]" ) {
    SECTION("nearest_neighbor_interpolation must be called repeatedly without changing the output"){
        // initialize mesh
        auto mesh1 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        auto mesh2 = meshes::mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
        mult(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f))), mesh2.vertices, mesh2.vertices);

        // initialize grids
        auto grid1 = rasters::SpheroidGrid<int,float>(mesh1.vertices, mesh1.faces);
        auto grid2 = rasters::SpheroidGrid<int,float>(mesh2.vertices, mesh2.faces);

        // initialize rasters
        auto raster1 = make_Raster<float>(grid1);
        auto raster2 = make_Raster<float>(grid2);
        auto raster3 = make_Raster<float>(grid2);

        // populate rasters
        std::mt19937 generator(2);
        many::get_elias_noise(icosahedron_grid.cache->vertex_positions, generator, raster1);
        rasters::nearest_neighbor_interpolation(raster1, raster2);
        rasters::nearest_neighbor_interpolation(raster1, raster3);

        CHECK(raster2 == raster3);
    }
}

TEST_CASE( "nearest_neighbor_interpolation identity", "[many]" ) {
    SECTION("a set of input must exist for nearest_neighbor_interpolation that when fed produces the original input"){
        // initialize rasters
        auto raster1 = make_Raster<float>(icosahedron_grid);
        auto raster2 = make_Raster<float>(icosahedron_grid);

        // populate rasters
        std::mt19937 generator(2);
        many::get_elias_noise(icosahedron_grid.cache->vertex_positions, generator, raster1);
        rasters::nearest_neighbor_interpolation(raster1, raster2);

        CHECK(raster1 == raster2);
    }
}

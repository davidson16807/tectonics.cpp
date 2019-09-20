
#include <sstream>  //std::stringstream

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/types.hpp>
#include <rasters/mesh.hpp>
#include <rasters/string_cast.hpp>  
#include <rasters/random.hpp>  
#include <rasters/glm/glm.hpp>
#include <rasters/glm/string_cast.hpp>  
#include <rasters/glm/vector_calculus.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

TEST_CASE( "mesh subdivision purity", "[rasters]" ) {
    SECTION("subdivide(mesh) must be called repeatedly without changing the output"){
        meshes::mesh icosphere1 = meshes::subdivide(meshes::tetrahedron);
        meshes::mesh icosphere2 = meshes::subdivide(meshes::tetrahedron);
        CHECK(icosphere1.vertices==icosphere2.vertices);
        CHECK(icosphere1.faces==icosphere2.faces);
    }
}
TEST_CASE( "mesh subdivision correctness", "[rasters]" ) {
    SECTION("subdivide(mesh) must generate a sensible number of vertices and faces"){
        meshes::mesh icosphere = meshes::subdivide(meshes::tetrahedron);
        CHECK(icosphere.vertices.size() == 10);
        CHECK(icosphere.faces.size() == 4 * meshes::tetrahedron.faces.size());
    }
}
/* 
"diamond" is a simple 2d grid for testing raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
Grid diamond = 
    Grid(
        vec3s({
                vec3( 0, 0, 0),
                vec3( 1, 0, 0),
                vec3( 0, 1, 0),
                vec3(-1, 0, 0),
                vec3( 0,-1, 0)
            }),
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,4),
                uvec3(0,3,2),
                uvec3(0,3,4)
            })
    );
TEST_CASE( "Grid correctness", "[Grid]" ) {
    SECTION("Grid must have the appropriate counts for vertex, edge, arrow, and face attributes"){
        CHECK(diamond.buffer_array_vertex_ids.size() == 12);

        CHECK(diamond.vertex_count == 5);
        // CHECK(diamond.vertex_neighbor_ids.size() == 5);
        CHECK(diamond.vertex_neighbor_counts.size() == 5);
        CHECK(diamond.vertex_positions.size() == 5);
        CHECK(diamond.vertex_normals.size() == 5);
        CHECK(diamond.vertex_areas.size() == 5);
        // CHECK(diamond.vertex_average_area.size() == );

        CHECK(diamond.face_count == 4);
        CHECK(diamond.face_vertex_ids.size() == 4);
        CHECK(diamond.face_vertex_id_a.size() == 4);
        CHECK(diamond.face_vertex_id_b.size() == 4);
        CHECK(diamond.face_vertex_id_c.size() == 4);
        // CHECK(diamond.face_edge_id_a.size() == 4);
        // CHECK(diamond.face_edge_id_b.size() == 4);
        // CHECK(diamond.face_edge_id_c.size() == 4);
        CHECK(diamond.face_endpoint_a.size() == 4);
        CHECK(diamond.face_endpoint_b.size() == 4);
        CHECK(diamond.face_endpoint_c.size() == 4);
        CHECK(diamond.face_midpoints.size() == 4);
        CHECK(diamond.face_normals.size() == 4);
        CHECK(diamond.face_areas.size() == 4);
        // CHECK(0.49f < diamond.face_average_area && diamond.face_average_area < 0.51f);

        CHECK(diamond.edge_count == 8);
        CHECK(diamond.edge_vertex_ids.size() == 8);
        CHECK(diamond.edge_vertex_id_a.size() == 8);
        CHECK(diamond.edge_vertex_id_b.size() == 8);
        // CHECK(diamond.edge_face_id_a.size() == 8);
        // CHECK(diamond.edge_face_id_b.size() == 8);
        CHECK(diamond.edge_endpoint_a.size() == 8);
        CHECK(diamond.edge_endpoint_b.size() == 8);
        CHECK(diamond.edge_midpoints.size() == 8);
        CHECK(diamond.edge_distances.size() == 8);
        CHECK(diamond.edge_normals.size() == 8);
        // CHECK(diamond.edge_areas.size() == );
        // CHECK(1.f < diamond.edge_average_distance && diamond.edge_average_distance < sqrt(2.f));
        
        CHECK(diamond.arrow_count == 16);
        CHECK(diamond.arrow_vertex_ids.size() == 16);
        CHECK(diamond.arrow_vertex_id_from.size() == 16);
        CHECK(diamond.arrow_vertex_id_to.size() == 16);
        // CHECK(diamond.arrow_face_id_a.size() == 16);
        // CHECK(diamond.arrow_face_id_b.size() == 16);
        CHECK(diamond.arrow_endpoint_from.size() == 16);
        CHECK(diamond.arrow_endpoint_to.size() == 16);
        CHECK(diamond.arrow_midpoints.size() == 16);
        CHECK(diamond.arrow_offsets.size() == 16);
        CHECK(diamond.arrow_distances.size() == 16);
        CHECK(diamond.arrow_normals.size() == 16);
        // CHECK(diamond.arrow_areas.size() == );
        // CHECK(1.f < diamond.arrow_average_distance && diamond.arrow_average_distance < sqrt(2.f));
    }
}
/*
"tetrahedron" is a simple 3d grid for testing basic raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
Grid tetrahedron = Grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);

TEST_CASE( "Grid nontriviality", "[Grid]" ) {
    SECTION("Grid attributes must contain nonzero elements"){
        CHECK(many::sum(many::abs(tetrahedron.buffer_array_vertex_ids)) > 0.01f);

        // CHECK(many::sum(many::abs(tetrahedron.vertex_neighbor_ids)) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.vertex_neighbor_counts)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.vertex_positions))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.vertex_normals))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.vertex_areas)) > 0.01f);

        CHECK(many::sum(many::abs(many::get_x(tetrahedron.face_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.face_vertex_id_a)) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.face_vertex_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.face_vertex_id_c)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.face_edge_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.face_edge_id_b)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.face_edge_id_c)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.face_endpoint_a))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.face_endpoint_b))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_y(tetrahedron.face_endpoint_c))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.face_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.face_normals))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.face_areas)) > 0.01f);

        CHECK(many::sum(many::abs(many::get_x(tetrahedron.edge_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.edge_vertex_id_a)) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.edge_vertex_id_b)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.edge_face_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.edge_face_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.edge_endpoint_a))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.edge_endpoint_b))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.edge_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.edge_distances)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.edge_normals))) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.edge_areas)) > 0.01f);
        
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.arrow_vertex_id_from)) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.arrow_vertex_id_to)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.arrow_face_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(tetrahedron.arrow_face_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_endpoint_from))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_endpoint_to))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_offsets))) > 0.01f);
        CHECK(many::sum(many::abs(tetrahedron.arrow_distances)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron.arrow_normals))) > 0.01f);
        // CHECK(tetrahedron.arrow_areas[2] > 0.01f);

    }
}
/*
"octahedron" is a simple 3d grid for testing raster operations that require 
something comparable to a unit sphere (e.g. nearest neighbor lookups using SpheroidGridVoronoi)
*/
SpheroidGrid octahedron = SpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces);

/*
"icosahedron" is a simple 3d grid for testing rasters with a relatively large number of vertices
*/
SpheroidGrid icosahedron = SpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces);

CartesianGridCellList cell_list(
        normalize(vec3s({
                        vec3( 1, 0, 0),
                        vec3( 0, 1, 0),
                        vec3( 0, 0, 1),
                        vec3(-1, 0, 0),
                        vec3( 0,-1, 0),
                        vec3( 0, 0,-1),
                        vec3(-1,-1,-1),
                        vec3( 1,-1,-1),
                        vec3(-1, 1,-1),
                        vec3( 1, 1,-1),
                        vec3(-1,-1, 1),
                        vec3( 1,-1, 1),
                        vec3(-1, 1, 1),
                        vec3( 1, 1, 1),
                        vec3( 1, 1, 1)
                    })),
        10./100.
    );
TEST_CASE( "CartesianGridCellList.nearest_id() purity", "[rasters]" ) {
    SECTION("CartesianGridCellList.nearest_id() must be called repeatedly without changing the output"){
        CHECK(cell_list.nearest_id(vec3(1,0,0)) == cell_list.nearest_id(vec3(1,0,0)));
    }
}
TEST_CASE( "CartesianGridCellList.nearest_id() happy path", "[rasters]" ) {
    SECTION("CartesianGridCellList.nearest_id() must return the appropriate id when answer is obvious"){
        CHECK( cell_list.nearest_id(normalize(vec3( 1, 0, 0))) == 0  );
        CHECK( cell_list.nearest_id(normalize(vec3( 0, 1, 0))) == 1  );
        CHECK( cell_list.nearest_id(normalize(vec3( 0, 0, 1))) == 2  );
        CHECK( cell_list.nearest_id(normalize(vec3(-1, 0, 0))) == 3  );
        CHECK( cell_list.nearest_id(normalize(vec3( 0,-1, 0))) == 4  );
        CHECK( cell_list.nearest_id(normalize(vec3( 0, 0,-1))) == 5  );
        CHECK( cell_list.nearest_id(normalize(vec3(-1,-1,-1))) == 6  );
        CHECK( cell_list.nearest_id(normalize(vec3( 1,-1,-1))) == 7  );
        CHECK( cell_list.nearest_id(normalize(vec3(-1, 1,-1))) == 8  );
        CHECK( cell_list.nearest_id(normalize(vec3( 1, 1,-1))) == 9  );
        CHECK( cell_list.nearest_id(normalize(vec3(-1,-1, 1))) == 10 );
        CHECK( cell_list.nearest_id(normalize(vec3( 1,-1, 1))) == 11 );
        CHECK( cell_list.nearest_id(normalize(vec3(-1, 1, 1))) == 12 );
        CHECK( cell_list.nearest_id(normalize(vec3( 1, 1, 1))) == 13 );
    }
}
SpheroidGridVoronoi voronoi(
        normalize(vec3s({
                        vec3( 1, 0, 0),
                        vec3( 0, 1, 0),
                        vec3( 0, 0, 1),
                        vec3(-1, 0, 0),
                        vec3( 0,-1, 0),
                        vec3( 0, 0,-1),
                        vec3(-1,-1,-1),
                        vec3( 1,-1,-1),
                        vec3(-1, 1,-1),
                        vec3( 1, 1,-1),
                        vec3(-1,-1, 1),
                        vec3( 1,-1, 1),
                        vec3(-1, 1, 1),
                        vec3( 1, 1, 1),
                        vec3( 1, 1, 1)
                    })),
        1./100.,
        10./100.
    );
TEST_CASE( "SpheroidGridVoronoi.nearest_id() purity", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.nearest_id() must be called repeatedly without changing the output"){
        CHECK(voronoi.nearest_id(vec3(1,0,0)) == voronoi.nearest_id(vec3(1,0,0)));
    }
}
TEST_CASE( "SpheroidGridVoronoi.nearest_id() happy path", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.nearest_id() must return the appropriate id when answer is obvious"){
        CHECK(  voronoi.nearest_id(normalize(vec3( 1, 0, 0))) == 0  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 0, 1, 0))) == 1  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 0, 0, 1))) == 2  );
        CHECK(  voronoi.nearest_id(normalize(vec3(-1, 0, 0))) == 3  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 0,-1, 0))) == 4  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 0, 0,-1))) == 5  );
        CHECK(  voronoi.nearest_id(normalize(vec3(-1,-1,-1))) == 6  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 1,-1,-1))) == 7  );
        CHECK(  voronoi.nearest_id(normalize(vec3(-1, 1,-1))) == 8  );
        CHECK(  voronoi.nearest_id(normalize(vec3( 1, 1,-1))) == 9  );
        CHECK(  voronoi.nearest_id(normalize(vec3(-1,-1, 1))) == 10 );
        CHECK(  voronoi.nearest_id(normalize(vec3( 1,-1, 1))) == 11 );
        CHECK(  voronoi.nearest_id(normalize(vec3(-1, 1, 1))) == 12 );
        CHECK(  voronoi.nearest_id(normalize(vec3( 1, 1, 1))) == 13 );
    }
}
TEST_CASE( "SpheroidGridVoronoi.nearest_ids() purity", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.nearest_id() must be called repeatedly without changing the output"){
        // CHECK(voronoi.nearest_ids(vec3(1,0,0)) == voronoi.nearest_ids(vec3(1,0,0)));
    }
}
TEST_CASE( "SpheroidGridVoronoi.nearest_ids() happy path", "[rasters]" ) {

}

TEST_CASE( "raster string cast purity", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec2s v2 = vec2s({
                    vec2( 0,-1),
                    vec2( 0, 1),
                    vec2(-1, 0),
                    vec2( 1, 0),
                    vec2( 0, 0),
                    vec2( 0, 0),
                });
    vec3s v3 = vec3s({
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0)
                });
    SECTION("to_string(grid, floats) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, a) == to_string(octahedron, a));
    }
    SECTION("to_string(grid, vec2s) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, v2) == to_string(octahedron, v2));
    }
    SECTION("to_string(grid, vec3s) must be called repeatedly without changing the output"){
        CHECK(to_string(octahedron, v3) == to_string(octahedron, v3));
    }
}
TEST_CASE( "raster string cast correctness", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec2s v2 = vec2s({
                    vec2( 0,-1),
                    vec2( 0, 1),
                    vec2(-1, 0),
                    vec2( 1, 0),
                    vec2( 0, 0),
                    vec2( 0, 0),
                });
    vec3s v3 = vec3s({
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0),
                    vec3( 1, 0, 0)
                });
    SECTION("to_string(grid, floats) must contain appropriate characters"){
        REQUIRE_THAT(to_string(octahedron, a), 
                    Catch::Contains(" ") && 
                    Catch::Contains("░") && 
                    Catch::Contains("▒") && 
                    Catch::Contains("▓") && 
                    Catch::Contains("█")
                );
    }
    SECTION("to_string(grid, floats) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(octahedron, a), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string(grid, vec2s) must depict 2d vector raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(octahedron, v2), 
                    (Catch::Contains(" ")                        ) && 
                    (Catch::Contains("←") || Catch::Contains("⬅")) && 
                    (Catch::Contains("↑") || Catch::Contains("⬆")) && 
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                    (Catch::Contains("↓") || Catch::Contains("⬇")) &&
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                   !(Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                   !(Catch::Contains("↗") || Catch::Contains("⬈"))
                );
    }
    SECTION("to_string(grid, vec2s) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(octahedron, v2), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
    SECTION("to_string() must depict uniform 3d vector raster using only appropriate arrows"){
        REQUIRE_THAT(to_string(octahedron, v3), 
                    (Catch::Contains(" ")                        ) && 
                    (Catch::Contains("←") || Catch::Contains("⬅")) && 
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                   !(Catch::Contains("↑") || Catch::Contains("⬆")) && 
                   !(Catch::Contains("↓") || Catch::Contains("⬇")) &&
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                   !(Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                   !(Catch::Contains("↗") || Catch::Contains("⬈"))
                );
    }
    SECTION("to_string(grid, vec2s) must not contain inappropriate characters"){
        REQUIRE_THAT(to_string(octahedron, v3), 
                    !Catch::Contains("∞") && 
                    !Catch::Contains("N") && 
                    !Catch::Contains("X") 
                );
    }
}
TEST_CASE( "raster random generation determinism", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,1,2,3,5,8});
    SECTION("random(grid, generator) must generate the same raster when given the same state of generator"){
        std::stringstream ss;
        std::mt19937 generator(time(0));
        ss << generator;
        random(octahedron, generator, a);
        ss >> generator;
        random(octahedron, generator, b);
        CHECK(a==b);
    }
}
TEST_CASE( "raster random generation nonpurity", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,2,3,4,5,6});
    SECTION("random(grid, generator) must generate different output when called repeatedly"){
        std::mt19937 generator(time(0));
        random(octahedron, generator, a);
        random(octahedron, generator, b);
        CHECK(a!=b);
    }
}
TEST_CASE( "raster random generation nontriviality", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    SECTION("random(grid, generator) must generate nontrivial output"){
        std::mt19937 generator(time(0));
        random(octahedron, generator, a);
        CHECK(sum(a) > 0.f);
    }
}
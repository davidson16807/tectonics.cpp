
#include <sstream>  //std::stringstream
#include <iostream>  //std::cout

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/mesh.hpp>
#include <rasters/morphologic.hpp>  
#include <rasters/random.hpp>  
#include <rasters/string_cast.hpp>  
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
        CHECK(diamond.edge_lengths.size() == 8);
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
        CHECK(diamond.arrow_lengths.size() == 16);
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
        CHECK(many::sum(many::abs(tetrahedron.edge_lengths)) > 0.01f);
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
        CHECK(many::sum(many::abs(tetrahedron.arrow_lengths)) > 0.01f);
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
TEST_CASE( "SpheroidGridVoronoi.get_value() purity", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.get_value() must be called repeatedly without changing the output"){
        CHECK(voronoi.get_value(vec3(1,0,0)) == voronoi.get_value(vec3(1,0,0)));
    }
}
TEST_CASE( "SpheroidGridVoronoi.get_value() happy path", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.get_value() must return the appropriate id when answer is obvious"){
        CHECK(  voronoi.get_value(normalize(vec3( 1, 0, 0))) == 0  );
        CHECK(  voronoi.get_value(normalize(vec3( 0, 1, 0))) == 1  );
        CHECK(  voronoi.get_value(normalize(vec3( 0, 0, 1))) == 2  );
        CHECK(  voronoi.get_value(normalize(vec3(-1, 0, 0))) == 3  );
        CHECK(  voronoi.get_value(normalize(vec3( 0,-1, 0))) == 4  );
        CHECK(  voronoi.get_value(normalize(vec3( 0, 0,-1))) == 5  );
        CHECK(  voronoi.get_value(normalize(vec3(-1,-1,-1))) == 6  );
        CHECK(  voronoi.get_value(normalize(vec3( 1,-1,-1))) == 7  );
        CHECK(  voronoi.get_value(normalize(vec3(-1, 1,-1))) == 8  );
        CHECK(  voronoi.get_value(normalize(vec3( 1, 1,-1))) == 9  );
        CHECK(  voronoi.get_value(normalize(vec3(-1,-1, 1))) == 10 );
        CHECK(  voronoi.get_value(normalize(vec3( 1,-1, 1))) == 11 );
        CHECK(  voronoi.get_value(normalize(vec3(-1, 1, 1))) == 12 );
        CHECK(  voronoi.get_value(normalize(vec3( 1, 1, 1))) == 13 );
    }
}
TEST_CASE( "SpheroidGridVoronoi.get_values() purity", "[rasters]" ) {
    SECTION("SpheroidGridVoronoi.get_value() must be called repeatedly without changing the output"){
        // CHECK(voronoi.get_values(vec3(1,0,0)) == voronoi.get_values(vec3(1,0,0)));
    }
}
TEST_CASE( "SpheroidGridVoronoi.get_values() happy path", "[rasters]" ) {

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
        std::mt19937 generator(2);
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
        std::mt19937 generator(2);
        random(octahedron, generator, a);
        random(octahedron, generator, b);
        CHECK(a!=b);
    }
}
TEST_CASE( "raster random generation nontriviality", "[rasters]" ) {
    floats a = floats({1,2,3,4,5,6});
    SECTION("random(grid, generator) must generate nontrivial output"){
        std::mt19937 generator(2);
        random(octahedron, generator, a);
        CHECK(sum(a) > 0.f);
    }
}
TEST_CASE( "raster dilation purity", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch    = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1);
        dilate(diamond, top_only, out2);
        CHECK(out1==out2);
    }
    SECTION("dilate(grid, top_only, radius) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1, 2);
        dilate(diamond, top_only, out2, 2);
        CHECK(out1==out2);
    }
    SECTION("dilate(grid, top_only, radius, scratch) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1, 2, scratch);
        dilate(diamond, top_only, out2, 2, scratch);
        CHECK(out1==out2);
    }
}
TEST_CASE( "raster dilation increasing", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must increase the number of flagged vertices"){
        dilate(diamond, top_only, out1);
        dilate(diamond, out1,     out2);
        CHECK(sum(out1) >= sum(top_only));
        CHECK(sum(out2) >= sum(out1));
    }
}
TEST_CASE( "raster dilation associative", "[rasters]" ) {
    bools A   = bools({false, false, true,  false, false });
    bools AB  = bools({false, false, false, false, false });
    bools ABC = bools({false, false, false, false, false });
    bools BC  = bools({false, false, false, false, false });
    bools BCA = bools({false, false, false, false, false });
    SECTION("dilation (A+B)+C must equal A+(B+C) where B and C are circles of radius 1 and 2"){
        dilate(diamond, A,  AB,  1);
        dilate(diamond, AB, ABC, 2);
        dilate(diamond, A,  BCA, 3);
        CHECK(ABC == BCA);
    }
}
TEST_CASE( "raster dilation distributive over union", "[rasters]" ) {
    bools A       = bools({false, false, true,  false, false });
    bools B       = bools({true,  false, true,  false, false });
    bools AC      = bools({false, false, false, false, false });
    bools BC      = bools({false, false, false, false, false });
    bools AB      = bools({false, false, false, false, false });
    bools AB_C    = bools({false, false, false, false, false });
    bools AC_BC   = bools({false, false, false, false, false });
    SECTION("dilation (A ∪ B)+C must equal (A+C) ∪ (B+C)"){
        unite (A,  B,  AB);
        dilate(diamond, AB, AB_C, 1);

        dilate(diamond, A,   AC,  1);
        dilate(diamond, B,   BC,  1);
        unite (AC, BC, AC_BC);

        CHECK(AB_C == AC_BC);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster dilation happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must return predictable results"){
        dilate(diamond, top_only, out1);
        CHECK(out1 == upper_half);
    }
    SECTION("dilate(grid, upper_half) must return predictable results"){
        dilate(diamond, upper_half, out1);
        CHECK(out1 == full);
    }
    SECTION("dilate(grid, empty) must return predictable results"){
        dilate(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("dilate(grid, full) must return predictable results"){
        dilate(diamond, full, out1);
        CHECK(out1 == full);
    }
}
TEST_CASE( "raster erosion purity", "[rasters]" ) {
    bools lower_half = bools({true,  true,  false,  true,  true });
    bools bottom_only= bools({true,  true,  false,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch    = bools({false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1);
        erode(diamond, bottom_only, out2);
        CHECK(out1==out2);
    }
    SECTION("erode(grid, bottom_only, radius) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1, 2);
        erode(diamond, bottom_only, out2, 2);
        CHECK(out1==out2);
    }
    SECTION("erode(grid, bottom_only, radius, scratch) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1, 2, scratch);
        erode(diamond, bottom_only, out2, 2, scratch);
        CHECK(out1==out2);
    }
}
TEST_CASE( "raster erosion decreasing", "[rasters]" ) {
    bools lower_half = bools({true,  true,  false,  true,  true });
    bools bottom_only= bools({true,  true,  false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must increase the number of flagged vertices"){
        erode(diamond, bottom_only, out1);
        erode(diamond, out1,        out2);
        CHECK(sum(out1) <= sum(bottom_only));
        CHECK(sum(out2) <= sum(out1));
    }
}
TEST_CASE( "raster erosion distributive over intersection", "[rasters]" ) {
    bools A       = bools({false, false, true,  false, false });
    bools B       = bools({true,  false, true,  false, false });
    bools AC      = bools({false, false, false, false, false });
    bools BC      = bools({false, false, false, false, false });
    bools AB      = bools({false, false, false, false, false });
    bools AB_C    = bools({false, false, false, false, false });
    bools AC_BC   = bools({false, false, false, false, false });
    SECTION("erosion (A ∩ B)+C must equal (A+C) ∩ (B+C)"){
        intersect (A,  B,  AB);
        erode     (diamond, AB, AB_C, 1);

        erode     (diamond, A,   AC,    1);
        erode     (diamond, B,   BC,    1);
        intersect (AC, BC, AC_BC);

        CHECK(AC_BC == AC_BC);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster erosion happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("erode(grid, top_only) must return predictable results"){
        erode(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("erode(grid, upper_half) must return predictable results"){
        erode(diamond, upper_half, out1);
        CHECK(out1 == top_only);
    }
    SECTION("erode(grid, none) must return predictable results"){
        erode(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("erode(grid, full) must return predictable results"){
        erode(diamond, full, out1);
        CHECK(out1 == full);
    }
}


TEST_CASE( "opening purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("opening(grid, bottom_edge) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1);
        opening(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("opening(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1, 1);
        opening(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("opening(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        opening(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
TEST_CASE( "opening idempotence", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("subsequent calls to opening(grid, bottom_edge) must generate the same output"){
        opening(diamond, bottom_edge, out1);
        opening(diamond, out1,        out2);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius) must generate the same output"){
        opening(diamond, bottom_edge, out1, 1);
        opening(diamond, out1,        out2, 1);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius, scratch) must generate the same output"){
        opening(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        opening(diamond, out1,        out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "opening happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("opening(grid, top_only) must return predictable results"){
        opening(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("opening(grid, upper_half) must return predictable results"){
        opening(diamond, upper_half, out1);
        CHECK(out1 == upper_half);
    }
    SECTION("opening(grid, none) must return predictable results"){
        opening(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("opening(grid, full) must return predictable results"){
        opening(diamond, full, out1);
        CHECK(out1 == full);
    }
}

TEST_CASE( "closing purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("closing(grid, bottom_edge) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1);
        closing(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("closing(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1, 1);
        closing(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("closing(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        closing(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
TEST_CASE( "closing idempotence", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("subsequent calls to closing(grid, bottom_edge) must generate the same output"){
        closing(diamond, bottom_edge, out1);
        closing(diamond, out1,        out2);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius) must generate the same output"){
        closing(diamond, bottom_edge, out1, 1);
        closing(diamond, out1,        out2, 1);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius, scratch) must generate the same output"){
        closing(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        closing(diamond, out1,        out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "closing happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("closing(grid, top_only) must return predictable results"){
        closing(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("closing(grid, upper_half) must return predictable results"){
        closing(diamond, upper_half, out1);
        CHECK(out1 == full);
    }
    SECTION("closing(grid, none) must return predictable results"){
        closing(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("closing(grid, full) must return predictable results"){
        closing(diamond, full, out1);
        CHECK(out1 == full);
    }
}


TEST_CASE( "white top hat purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("white_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1);
        white_top_hat(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("white_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1, 1);
        white_top_hat(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("white_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        white_top_hat(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster white_top_hat happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools bottom_only= bools({false, false, false, false, true  });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("white_top_hat(grid, top_only) must return predictable results"){
        white_top_hat(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("white_top_hat(grid, upper_half) must return predictable results"){
        white_top_hat(diamond, upper_half, out1);
        CHECK(out1 == bottom_only);
    }
    SECTION("white_top_hat(grid, none) must return predictable results"){
        white_top_hat(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("white_top_hat(grid, full) must return predictable results"){
        white_top_hat(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "black top hat purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("black_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1);
        black_top_hat(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("black_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1, 1);
        black_top_hat(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("black_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        black_top_hat(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster black_top_hat happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("black_top_hat(grid, top_only) must return predictable results"){
        black_top_hat(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("black_top_hat(grid, upper_half) must return predictable results"){
        black_top_hat(diamond, upper_half, out1);
        CHECK(out1 == empty);
    }
    SECTION("black_top_hat(grid, none) must return predictable results"){
        black_top_hat(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("black_top_hat(grid, full) must return predictable results"){
        black_top_hat(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "margin purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("margin(grid, bottom_edge) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1);
        margin(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("margin(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1, 1);
        margin(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("margin(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1, 1, scratch1);
        margin(diamond, bottom_edge, out2, 1, scratch1);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "margin happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools bottom_only= bools({false, false, false, false, true  });
    bools center     = bools({true,  true,  false, true,  false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("margin(grid, top_only) must return predictable results"){
        margin(diamond, top_only,   out1);
        CHECK(out1 == center);
    }
    SECTION("margin(grid, upper_half) must return predictable results"){
        margin(diamond, upper_half, out1);
        CHECK(out1 == bottom_only);
    }
    SECTION("margin(grid, none) must return predictable results"){
        margin(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("margin(grid, full) must return predictable results"){
        margin(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "padding purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("padding(grid, bottom_edge) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1);
        padding(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("padding(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1, 1);
        padding(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("padding(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1, 1, scratch1);
        padding(diamond, bottom_edge, out2, 1, scratch1);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "padding happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools center     = bools({true,  true,  false, true,  false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("padding(grid, top_only) must return predictable results"){
        padding(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("padding(grid, upper_half) must return predictable results"){
        padding(diamond, upper_half, out1);
        CHECK(out1 == center);
    }
    SECTION("padding(grid, none) must return predictable results"){
        padding(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("padding(grid, full) must return predictable results"){
        padding(diamond, full, out1);
        CHECK(out1 == empty);
    }
}


TEST_CASE( "gradient determinism", "[rasters]" ) {
    floats a   = floats({1,2,3,4,5,6,7,8,9,10,11,12});
    // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
    vec3s out1 = vec3s (icosahedron.vertex_count);
    vec3s out2 = vec3s (icosahedron.vertex_count);
    SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
        gradient(icosahedron, a, out1);
        gradient(icosahedron, a, out2);
        CHECK(out1==out2);
    }
}

// TEST_CASE( "gradient translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a           (icosphere.vertex_count);
//     uints  A_ids       (icosphere.vertex_count);
//     uints  Ai_ids      (icosphere.vertex_count);
//     vec3s  A_pos       (icosphere.vertex_count);
//     vec3s  Ai_pos      (icosphere.vertex_count);
//     floats A_a         (icosphere.vertex_count);
//     vec3s  grad_A_a    (icosphere.vertex_count);
//     vec3s  Ai_grad_A_a (icosphere.vertex_count);
//     vec3s  grad_a      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere, generator, a);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( icosphere, a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( icosphere, A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }

// TEST_CASE( "gradient resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); many::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     SpheroidGrid icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); many::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     SpheroidGrid icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     floats a           (icosphere1.vertex_count);
//     uints  A_ids       (icosphere2.vertex_count);
//     uints  Ai_ids      (icosphere1.vertex_count);
//     floats A_a         (icosphere2.vertex_count);
//     vec3s  grad_A_a    (icosphere2.vertex_count);
//     vec3s  Ai_grad_A_a (icosphere1.vertex_count);
//     vec3s  grad_a      (icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere1, generator, a);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( icosphere1,a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( icosphere2,A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }


TEST_CASE( "gradient distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats a           (icosphere.vertex_count);
    floats b           (icosphere.vertex_count);
    vec3s  grad_a      (icosphere.vertex_count);
    vec3s  grad_b      (icosphere.vertex_count);
    vec3s  grad_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    random(icosphere, generator, a);
    random(icosphere, generator, b);

    SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
        gradient ( icosphere, a,      grad_a         );
        gradient ( icosphere, b,      grad_b         );
        gradient ( icosphere, a+b,    grad_a_b       );

        CHECK(equal( grad_a_b,  grad_a + grad_b, 0.01f, 0.01f ));
    }
}

TEST_CASE( "gradient multiplication relation", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats a           (icosphere.vertex_count);
    floats b           (icosphere.vertex_count);
    vec3s  grad_a      (icosphere.vertex_count);
    vec3s  grad_b      (icosphere.vertex_count);
    vec3s  grad_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    random(icosphere, generator, a);
    random(icosphere, generator, b);

    SECTION("gradient(a+b) must generate the same output as a*gradient(b) + b*gradient(a)"){
        gradient ( icosphere, a,      grad_a                  );
        gradient ( icosphere, b,      grad_b                  );
        gradient ( icosphere, a*b,    grad_a_b                );
        CHECK(equal( grad_a_b,  grad_b*a + grad_a*b, 0.7f, 0.1f ));
    }
}

TEST_CASE( "divergence determinism", "[rasters]" ) {
    vec3s a    = vec3s ({
        vec3(1, 2, 3 ),
        vec3(4, 5, 6 ),
        vec3(7, 8, 9 ),
        vec3(10,11,12),
        vec3(13,14,15),
        vec3(16,17,18),
        vec3(19,20,21),
        vec3(22,23,24),
        vec3(25,26,27),
        vec3(28,29,30),
        vec3(31,32,33),
        vec3(34,35,36)
    });
    floats out1 = floats(icosahedron.vertex_count);
    floats out2 = floats(icosahedron.vertex_count);

    SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
        divergence(icosahedron, a, out1);
        divergence(icosahedron, a, out2);
        CHECK(out1==out2);
    }
}
// TEST_CASE( "divergence translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar      (icosphere.vertex_count);
//     vec3s  a           (icosphere.vertex_count);
//     uints  A_ids       (icosphere.vertex_count);
//     uints  Ai_ids      (icosphere.vertex_count);
//     vec3s  A_pos       (icosphere.vertex_count);
//     vec3s  Ai_pos      (icosphere.vertex_count);
//     vec3s  A_a         (icosphere.vertex_count);
//     floats div_A_a     (icosphere.vertex_count);
//     floats Ai_div_A_a  (icosphere.vertex_count);
//     floats div_a       (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random  ( icosphere, generator, scalar   );
//     gradient( icosphere, scalar,    a        );

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere, a,      div_a      );
//         get        ( a,         A_ids,  A_a        );
//         divergence ( icosphere, A_a,    div_A_a    );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }

// TEST_CASE( "divergence resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); many::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     SpheroidGrid icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); many::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     SpheroidGrid icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     floats scalar     (icosphere1.vertex_count);
//     vec3s  a          (icosphere1.vertex_count);
//     uints  A_ids      (icosphere2.vertex_count);
//     uints  Ai_ids     (icosphere1.vertex_count);
//     vec3s  A_a        (icosphere2.vertex_count);
//     floats div_A_a    (icosphere2.vertex_count);
//     floats Ai_div_A_a (icosphere1.vertex_count);
//     floats div_a      (icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     random  ( icosphere1, generator, scalar );
//     gradient( icosphere1, scalar,    a      );

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere1,a,     div_a       );
//         get        ( a,         A_ids, A_a         );
//         divergence ( icosphere2,A_a,   div_A_a     );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }


TEST_CASE( "divergence distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats scalar     (icosphere.vertex_count);
    vec3s  a          (icosphere.vertex_count);
    vec3s  b          (icosphere.vertex_count);
    floats div_a      (icosphere.vertex_count);
    floats div_b      (icosphere.vertex_count);
    floats div_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    random(icosphere, generator, scalar);
    gradient( icosphere, scalar,a     );
    random(icosphere, generator, scalar);
    gradient( icosphere, scalar,b     );

    SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
        divergence ( icosphere, a,      div_a         );
        divergence ( icosphere, b,      div_b         );
        divergence ( icosphere, a+b,    div_a_b       );
        CHECK(div_a + div_b == div_a_b);
    }
}

// TEST_CASE( "divergence multiplication relation", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     floats a          (icosphere.vertex_count);
//     vec3s  b          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     floats div_b      (icosphere.vertex_count);
//     floats div_a_b    (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere, generator, a);
//     random(icosphere, generator, scalar);
//     gradient( icosphere, scalar, b     );

//     SECTION("divergence(a+b) must generate the same output as a*divergence(b) + b*divergence(a)"){
//         gradient   ( icosphere, a,      grad_a                 );
//         divergence ( icosphere, b,      div_b                  );
//         divergence ( icosphere, b*a,    div_a_b                );
//         CHECK(equal( div_a_b,  div_b*a + dot(b, grad_a), 1e-0f));
//     }
// }
TEST_CASE( "curl determinism", "[rasters]" ) {
    vec3s a    = vec3s ({
        vec3(1, 2, 3 ),
        vec3(4, 5, 6 ),
        vec3(7, 8, 9 ),
        vec3(10,11,12),
        vec3(13,14,15),
        vec3(16,17,18),
        vec3(19,20,21),
        vec3(22,23,24),
        vec3(25,26,27),
        vec3(28,29,30),
        vec3(31,32,33),
        vec3(34,35,36)
    });
    // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
    vec3s  out1 = vec3s (icosahedron.vertex_count);
    vec3s  out2 = vec3s (icosahedron.vertex_count);

    SECTION("curl(grid, a) must generate the same output when called repeatedly"){
        curl(icosahedron, a, out1);
        curl(icosahedron, a, out2);
        CHECK(out1==out2);
    }
}

TEST_CASE( "curl distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats scalar     (icosphere.vertex_count);
    vec3s  a          (icosphere.vertex_count);
    vec3s  b          (icosphere.vertex_count);
    vec3s  curl_a      (icosphere.vertex_count);
    vec3s  curl_b      (icosphere.vertex_count);
    vec3s  curl_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    random(icosphere, generator, scalar);
    gradient( icosphere, scalar,a     );
    random(icosphere, generator, scalar);
    gradient( icosphere, scalar,b     );

    SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
        curl ( icosphere, a,      curl_a         );
        curl ( icosphere, b,      curl_b         );
        curl ( icosphere, a+b,    curl_a_b       );
        CHECK(curl_a + curl_b == curl_a_b);
    }
}

// TEST_CASE( "curl of gradient is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     vec3s  curl_grad_a(icosphere.vertex_count);
//     vec3s  zeros      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere, generator, a);
//     fill  (zeros, vec3(0));

//     gradient  (icosphere, a,      grad_a      );
//     curl      (icosphere, grad_a, curl_grad_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         // CHECK(curl_grad_a == zeros);
//         CHECK(equal(curl_grad_a, zeros));
//     }
// }
// TEST_CASE( "divergence of curl is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     vec3s  a          (icosphere.vertex_count);
//     vec3s  curl_a     (icosphere.vertex_count);
//     floats div_curl_a (icosphere.vertex_count);
//     floats zeros      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere, generator, scalar);
//     gradient(icosphere, scalar,  a);
//     fill  (zeros, 0.f);

//     curl       (icosphere, a,      curl_a     );
//     divergence (icosphere, curl_a, div_curl_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         CHECK(div_curl_a == zeros);
//         // CHECK(equal(div_curl_a, zeros));
//     }
// }
TEST_CASE( "laplacian determinism", "[rasters]" ) {
    floats a    = floats({1,2,3,4,5,6,7,8,9,10,11,12});
    // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
    floats out1 = floats (icosahedron.vertex_count);
    floats out2 = floats (icosahedron.vertex_count);
    SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
        laplacian(icosahedron, a, out1);
        laplacian(icosahedron, a, out2);
        CHECK(out1==out2);
    }
}


// TEST_CASE( "laplacian is divergence of gradient", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     floats div_grad_a (icosphere.vertex_count);
//     floats laplacian_a(icosphere.vertex_count);

//     std::mt19937 generator(2);
//     random(icosphere, generator, a);

//     laplacian (icosphere, a,      laplacian_a);
//     gradient  (icosphere, a,      grad_a     );
//     divergence(icosphere, grad_a, div_grad_a );
//     SECTION("laplacian(a) must generate the same output as div(grad(a))"){
//         CHECK(equal(laplacian_a, div_grad_a));
//     }
// }

#include <sstream>  //std::stringstream
#include <iostream>  //std::cout

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/random.hpp>      // get_elias_noise
#include <many/glm/convenience.hpp> //  operators, etc.

#include <grids/mesh.hpp>
#include <grids/Grid/morphologic.hpp>  
#include <grids/Grid/vector_calculus.hpp>
#include <grids/SpheroidGrid/string_cast.hpp>  
#include <grids/LayeredSpheroidGrid/LayeredSpheroidGrid.hpp>
#include <grids/LayeredSpheroidGrid/vector_calculus.hpp>
#include <grids/LayeredSpheroidGrid/string_cast.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

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
LayeredSpheroidGrid diamond = 
    LayeredSpheroidGrid(
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
            }),
        1.f, 2u
    );
TEST_CASE( "LayeredSpheroidGrid correctness", "[LayeredSpheroidGrid]" ) {
    SECTION("LayeredSpheroidGrid must have the appropriate counts for vertex, edge, arrow, and face attributes"){
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
"tetrahedron" is a simple 3d LayeredSpheroidGrid for testing basic raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
LayeredSpheroidGrid tetrahedron = LayeredSpheroidGrid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces, 1.f, 2u);

TEST_CASE( "LayeredSpheroidGrid nontriviality", "[LayeredSpheroidGrid]" ) {
    SECTION("LayeredSpheroidGrid attributes must contain nonzero elements"){
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
LayeredSpheroidGrid octahedron = LayeredSpheroidGrid(meshes::octahedron.vertices, meshes::octahedron.faces, 1.f, 2u);

/*
"icosahedron" is a simple 3d grid for testing rasters with a relatively large number of vertices
*/
LayeredSpheroidGrid icosahedron = LayeredSpheroidGrid(meshes::icosahedron.vertices, meshes::icosahedron.faces, 1.f, 2u);

// SpheroidGridVoronoi voronoi(
//         normalize(vec3s({
//                         vec3( 1, 0, 0),
//                         vec3( 0, 1, 0),
//                         vec3( 0, 0, 1),
//                         vec3(-1, 0, 0),
//                         vec3( 0,-1, 0),
//                         vec3( 0, 0,-1),
//                         vec3(-1,-1,-1),
//                         vec3( 1,-1,-1),
//                         vec3(-1, 1,-1),
//                         vec3( 1, 1,-1),
//                         vec3(-1,-1, 1),
//                         vec3( 1,-1, 1),
//                         vec3(-1, 1, 1),
//                         vec3( 1, 1, 1),
//                         vec3( 1, 1, 1)
//                     })),
//         1./100.,
//         10./100.
//     );
// TEST_CASE( "SpheroidGridVoronoi.get_value() purity", "[rasters]" ) {
//     SECTION("SpheroidGridVoronoi.get_value() must be called repeatedly without changing the output"){
//         CHECK(voronoi.get_value(vec3(1,0,0)) == voronoi.get_value(vec3(1,0,0)));
//     }
// }
// TEST_CASE( "SpheroidGridVoronoi.get_value() happy path", "[rasters]" ) {
//     SECTION("SpheroidGridVoronoi.get_value() must return the appropriate id when answer is obvious"){
//         CHECK(  voronoi.get_value(normalize(vec3( 1, 0, 0))) == 0  );
//         CHECK(  voronoi.get_value(normalize(vec3( 0, 1, 0))) == 1  );
//         CHECK(  voronoi.get_value(normalize(vec3( 0, 0, 1))) == 2  );
//         CHECK(  voronoi.get_value(normalize(vec3(-1, 0, 0))) == 3  );
//         CHECK(  voronoi.get_value(normalize(vec3( 0,-1, 0))) == 4  );
//         CHECK(  voronoi.get_value(normalize(vec3( 0, 0,-1))) == 5  );
//         CHECK(  voronoi.get_value(normalize(vec3(-1,-1,-1))) == 6  );
//         CHECK(  voronoi.get_value(normalize(vec3( 1,-1,-1))) == 7  );
//         CHECK(  voronoi.get_value(normalize(vec3(-1, 1,-1))) == 8  );
//         CHECK(  voronoi.get_value(normalize(vec3( 1, 1,-1))) == 9  );
//         CHECK(  voronoi.get_value(normalize(vec3(-1,-1, 1))) == 10 );
//         CHECK(  voronoi.get_value(normalize(vec3( 1,-1, 1))) == 11 );
//         CHECK(  voronoi.get_value(normalize(vec3(-1, 1, 1))) == 12 );
//         CHECK(  voronoi.get_value(normalize(vec3( 1, 1, 1))) == 13 );
//     }
// }
// TEST_CASE( "SpheroidGridVoronoi.get_values() purity", "[rasters]" ) {
//     SECTION("SpheroidGridVoronoi.get_value() must be called repeatedly without changing the output"){
//         // CHECK(voronoi.get_values(vec3(1,0,0)) == voronoi.get_values(vec3(1,0,0)));
//     }
// }
// TEST_CASE( "SpheroidGridVoronoi.get_values() happy path", "[rasters]" ) {

// }

TEST_CASE( "raster string cast purity", "[rasters]" ) {
    floats a = floats({ 1, 2, 3, 4, 5, 6,
                        7, 8, 9,10,11,12 });
    vec2s v2 = vec2s({
                    vec2(-1,-1),
                    vec2(-1, 0),
                    vec2(-1, 1),

                    vec2( 0,-1),
                    vec2( 0, 0),
                    vec2( 0, 1),

                    vec2( 1,-1),
                    vec2( 1, 0),
                    vec2( 1, 1),

                    vec2(-1,-1),
                    vec2(-1, 0),
                    vec2(-1, 1),
                });
    vec3s v3 = vec3s({
                    vec3(-1,-1,-1),
                    vec3(-1,-1, 0),
                    vec3(-1,-1, 1),

                    vec3(-1, 0,-1),
                    vec3(-1, 0, 0),
                    vec3(-1, 0, 1),

                    vec3(-1, 1,-1),
                    vec3(-1, 1, 0),
                    vec3(-1, 1, 1),

                    vec3( 0,-1,-1),
                    vec3( 0,-1, 0),
                    vec3( 0,-1, 1),
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
    floats a = floats({ 1, 2, 3, 4, 5, 6,
                        7, 8, 9,10,11,12 });
    vec2s v2 = vec2s({
                    vec2(-1,-1),
                    vec2(-1, 0),
                    vec2(-1, 1),

                    vec2( 0,-1),
                    vec2( 0, 0),
                    vec2( 0, 1),

                    vec2( 1,-1),
                    vec2( 1, 0),
                    vec2( 1, 1),

                    vec2(-1,-1),
                    vec2(-1, 0),
                    vec2(-1, 1),
                });
    vec3s v3 = vec3s({
                    vec3( 0,-1,-1),
                    vec3( 0,-1, 0),
                    vec3( 0,-1, 1),

                    vec3( 0, 0,-1),
                    vec3( 0, 0, 0),
                    vec3( 0, 0, 1),

                    vec3( 0, 1,-1),
                    vec3( 0, 1, 0),
                    vec3( 0, 1, 1),

                    vec3(-1,-1,-1),
                    vec3(-1,-1, 0),
                    vec3(-1,-1, 1),
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
                    (Catch::Contains("↖") || Catch::Contains("⬉")) &&
                    (Catch::Contains("↙") || Catch::Contains("⬋")) &&
                    (Catch::Contains("↘") || Catch::Contains("⬊")) &&
                    (Catch::Contains("↗") || Catch::Contains("⬈"))
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
                   !(Catch::Contains("←") || Catch::Contains("⬅")) &&    
                    (Catch::Contains("→") || Catch::Contains("➡")) && 
                    (Catch::Contains("↑") || Catch::Contains("⬆")) && 
                    (Catch::Contains("↓") || Catch::Contains("⬇")) &&
                   !(Catch::Contains("↖") || Catch::Contains("⬉")) &&
                    (Catch::Contains("↙") || Catch::Contains("⬋")) &&
                   !(Catch::Contains("↘") || Catch::Contains("⬊")) &&
                    (Catch::Contains("↗") || Catch::Contains("⬈"))
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
// TEST_CASE( "raster get_elias_noise generation determinism", "[rasters]" ) {
//     floats a = floats({1,2,3,4,5,6});
//     floats b = floats({1,1,2,3,5,8});
//     SECTION("get_elias_noise(positions, generator) must generate the same raster when given the same state of generator"){
//         std::stringstream ss;
//         std::mt19937 generator(2);
//         ss << generator;
//         get_elias_noise(octahedron.vertex_positions, generator, a);
//         ss >> generator;
//         get_elias_noise(octahedron.vertex_positions, generator, b);
//         CHECK(a==b);
//     }
// }
// TEST_CASE( "raster get_elias_noise generation nonpurity", "[rasters]" ) {
//     floats a = floats({1,2,3,4,5,6});
//     floats b = floats({1,2,3,4,5,6});
//     SECTION("get_elias_noise(positions, generator) must generate different output when called repeatedly"){
//         std::mt19937 generator(2);
//         get_elias_noise(octahedron.vertex_positions, generator, a);
//         get_elias_noise(octahedron.vertex_positions, generator, b);
//         CHECK(a!=b);
//     }
// }
// TEST_CASE( "raster get_elias_noise generation nontriviality", "[rasters]" ) {
//     floats a = floats({1,2,3,4,5,6});
//     SECTION("get_elias_noise(positions, generator) must generate nontrivial output"){
//         std::mt19937 generator(2);
//         get_elias_noise(octahedron.vertex_positions, generator, a);
//         CHECK(sum(a) > 0.f);
//     }
// }


// TEST_CASE( "gradient determinism", "[rasters]" ) {
//     floats a   = floats({1,2,3,4,5,6,7,8,9,10,11,12});
//     SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
//         CHECK(gradient(icosahedron, a)==gradient(icosahedron, a));
//     }
// }

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
//     get_elias_noise(icosphere.vertex_positions, generator, a);

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
//     get_elias_noise(icosphere1.vertex_positions, generator, a);

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


// TEST_CASE( "gradient distributive over addition", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a           (icosphere.vertex_count);
//     floats b           (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere.vertex_positions, generator, a);
//     get_elias_noise(icosphere.vertex_positions, generator, b);

//     SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
//         CHECK(equal( gradient(icosphere, a+b),  gradient(icosphere, a) + gradient(icosphere, b), 0.01f, 0.01f ));
//     }
// }

// TEST_CASE( "gradient multiplication relation", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a           (icosphere.vertex_count);
//     floats b           (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere.vertex_positions, generator, a);
//     get_elias_noise(icosphere.vertex_positions, generator, b);

//     SECTION("gradient(a*b) must generate the same output as a*gradient(b) + b*gradient(a)"){
//         CHECK(equal( gradient(icosphere, a*b),  gradient(icosphere, b)*a + gradient(icosphere, a)*b, 0.7f, 0.1f ));
//     }
// }

// TEST_CASE( "divergence determinism", "[rasters]" ) {
//     vec3s a    = vec3s ({
//         vec3(1, 2, 3 ),
//         vec3(4, 5, 6 ),
//         vec3(7, 8, 9 ),
//         vec3(10,11,12),
//         vec3(13,14,15),
//         vec3(16,17,18),
//         vec3(19,20,21),
//         vec3(22,23,24),
//         vec3(25,26,27),
//         vec3(28,29,30),
//         vec3(31,32,33),
//         vec3(34,35,36)
//     });
//     floats out1 = floats(icosahedron.vertex_count);
//     floats out2 = floats(icosahedron.vertex_count);

//     SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
//         CHECK(divergence(icosahedron, a)==divergence(icosahedron, a));
//     }
// }
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
//     get_elias_noise  ( icosphere, generator, scalar   );
//     a = gradient( icosphere, scalar);

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
//     get_elias_noise  ( icosphere1, generator, scalar );
//     a = gradient( icosphere1, scalar);

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


// TEST_CASE( "divergence distributive over addition", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     vec3s  a          (icosphere.vertex_count);
//     vec3s  b          (icosphere.vertex_count);
//     floats div_a      (icosphere.vertex_count);
//     floats div_b      (icosphere.vertex_count);
//     floats div_a_b    (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     a = gradient( icosphere, scalar);
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     b = gradient( icosphere, scalar);

//     SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
//         CHECK(divergence( icosphere, a) + divergence( icosphere, b) == divergence(icosphere, a+b));
//     }
// }

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
//     get_elias_noise(icosphere.vertex_positions, generator, a);
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     b = gradient( icosphere, scalar);
//     grad_a = gradient( icosphere, a     );

//     SECTION("divergence(a+b) must generate the same output as a*divergence(b) + b*divergence(a)"){
//         divergence ( icosphere, b,      div_b                  );
//         divergence ( icosphere, b*a,    div_a_b                );
//         CHECK(equal( div_a_b,  div_b*a + dot(b, grad_a), 1e-0f));
//     }
// }
// TEST_CASE( "curl determinism", "[rasters]" ) {
//     vec3s a    = vec3s ({
//         vec3(1, 2, 3 ),
//         vec3(4, 5, 6 ),
//         vec3(7, 8, 9 ),
//         vec3(10,11,12),
//         vec3(13,14,15),
//         vec3(16,17,18),
//         vec3(19,20,21),
//         vec3(22,23,24),
//         vec3(25,26,27),
//         vec3(28,29,30),
//         vec3(31,32,33),
//         vec3(34,35,36)
//     });
//     // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
//     vec3s  out1 = vec3s (icosahedron.vertex_count);
//     vec3s  out2 = vec3s (icosahedron.vertex_count);

//     SECTION("curl(grid, a) must generate the same output when called repeatedly"){
//         CHECK(curl(icosahedron, a)==curl(icosahedron, a));
//     }
// }

// TEST_CASE( "curl distributive over addition", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     vec3s  a          (icosphere.vertex_count);
//     vec3s  b          (icosphere.vertex_count);
//     vec3s  curl_a      (icosphere.vertex_count);
//     vec3s  curl_b      (icosphere.vertex_count);
//     vec3s  curl_a_b    (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     a = gradient( icosphere, scalar);
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     b = gradient( icosphere, scalar);

//     SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
//         CHECK(curl(icosphere, a) + curl(icosphere, b) == curl(icosphere, a+b));
//     }
// }

// TEST_CASE( "curl of gradient is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     vec3s  curl_grad_a(icosphere.vertex_count);
//     vec3s  zeros      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere.vertex_positions, generator, a);
//     fill  (zeros, vec3(0));

//     grad_a = gradient  (icosphere, a);
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
//     get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     a = gradient(icosphere, scalar);
//     fill  (zeros, 0.f);

//     curl       (icosphere, a,      curl_a     );
//     divergence (icosphere, curl_a, div_curl_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         CHECK(div_curl_a == zeros);
//         // CHECK(equal(div_curl_a, zeros));
//     }
// }
// TEST_CASE( "laplacian determinism", "[rasters]" ) {
//     floats a    = floats({1,2,3,4,5,6,7,8,9,10,11,12});
//     // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
//     floats out1 = floats (icosahedron.vertex_count);
//     floats out2 = floats (icosahedron.vertex_count);
//     SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
//         CHECK(laplacian(icosahedron, a)==laplacian(icosahedron, a));
//     }
// }


// TEST_CASE( "laplacian is divergence of gradient", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     SpheroidGrid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     floats div_grad_a (icosphere.vertex_count);
//     floats laplacian_a(icosphere.vertex_count);

//     std::mt19937 generator(2);
//     get_elias_noise(icosphere, generator, a);

//     laplacian (icosphere, a,      laplacian_a);
//     grad_a = gradient(icosphere, a);
//     divergence(icosphere, grad_a, div_grad_a );
//     SECTION("laplacian(a) must generate the same output as div(grad(a))"){
//         CHECK(equal(laplacian_a, div_grad_a));
//     }
// }
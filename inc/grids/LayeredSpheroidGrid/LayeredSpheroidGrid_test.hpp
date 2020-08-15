

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>


#include "LayeredSpheroidGrid.hpp"

#include "LayeredSpheroidGrid_test_utils.hpp"

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

TEST_CASE( "LayeredSpheroidGrid correctness", "[LayeredSpheroidGrid]" ) {
    SECTION("LayeredSpheroidGrid must have the appropriate counts for vertex, edge, arrow, and face attributes"){
        CHECK(layered_diamond.flattened_face_vertex_ids.size() == 12);

        CHECK(layered_diamond.vertex_count == 5);
        // CHECK(layered_diamond.vertex_neighbor_ids.size() == 5);
        CHECK(layered_diamond.vertex_neighbor_counts.size() == 5);
        CHECK(layered_diamond.vertex_positions.size() == 5);
        CHECK(layered_diamond.vertex_normals.size() == 5);
        CHECK(layered_diamond.vertex_areas.size() == 5);
        // CHECK(layered_diamond.vertex_average_area.size() == );

        CHECK(layered_diamond.face_count == 4);
        CHECK(layered_diamond.face_vertex_ids.size() == 4);
        CHECK(layered_diamond.face_vertex_id_a.size() == 4);
        CHECK(layered_diamond.face_vertex_id_b.size() == 4);
        CHECK(layered_diamond.face_vertex_id_c.size() == 4);
        // CHECK(layered_diamond.face_edge_id_a.size() == 4);
        // CHECK(layered_diamond.face_edge_id_b.size() == 4);
        // CHECK(layered_diamond.face_edge_id_c.size() == 4);
        CHECK(layered_diamond.face_endpoint_a.size() == 4);
        CHECK(layered_diamond.face_endpoint_b.size() == 4);
        CHECK(layered_diamond.face_endpoint_c.size() == 4);
        CHECK(layered_diamond.face_midpoints.size() == 4);
        CHECK(layered_diamond.face_normals.size() == 4);
        CHECK(layered_diamond.face_areas.size() == 4);
        // CHECK(0.49f < layered_diamond.face_average_area && layered_diamond.face_average_area < 0.51f);

        CHECK(layered_diamond.edge_count == 8);
        CHECK(layered_diamond.edge_vertex_ids.size() == 8);
        CHECK(layered_diamond.edge_vertex_id_a.size() == 8);
        CHECK(layered_diamond.edge_vertex_id_b.size() == 8);
        // CHECK(layered_diamond.edge_face_id_a.size() == 8);
        // CHECK(layered_diamond.edge_face_id_b.size() == 8);
        CHECK(layered_diamond.edge_endpoint_a.size() == 8);
        CHECK(layered_diamond.edge_endpoint_b.size() == 8);
        CHECK(layered_diamond.edge_midpoints.size() == 8);
        CHECK(layered_diamond.edge_lengths.size() == 8);
        CHECK(layered_diamond.edge_normals.size() == 8);
        // CHECK(layered_diamond.edge_areas.size() == );
        // CHECK(1.f < layered_diamond.edge_average_distance && layered_diamond.edge_average_distance < sqrt(2.f));
        
        CHECK(layered_diamond.arrow_count == 16);
        CHECK(layered_diamond.arrow_vertex_ids.size() == 16);
        CHECK(layered_diamond.arrow_vertex_id_from.size() == 16);
        CHECK(layered_diamond.arrow_vertex_id_to.size() == 16);
        // CHECK(layered_diamond.arrow_face_id_a.size() == 16);
        // CHECK(layered_diamond.arrow_face_id_b.size() == 16);
        CHECK(layered_diamond.arrow_endpoint_from.size() == 16);
        CHECK(layered_diamond.arrow_endpoint_to.size() == 16);
        CHECK(layered_diamond.arrow_midpoints.size() == 16);
        CHECK(layered_diamond.arrow_offsets.size() == 16);
        CHECK(layered_diamond.arrow_lengths.size() == 16);
        CHECK(layered_diamond.arrow_normals.size() == 16);
        // CHECK(layered_diamond.arrow_areas.size() == );
        // CHECK(1.f < layered_diamond.arrow_average_distance && layered_diamond.arrow_average_distance < sqrt(2.f));
    }
}
TEST_CASE( "LayeredSpheroidGrid nontriviality", "[LayeredSpheroidGrid]" ) {
    SECTION("LayeredSpheroidGrid attributes must contain nonzero elements"){
        CHECK(many::sum(many::abs(layered_tetrahedron.flattened_face_vertex_ids)) > 0.01f);

        // CHECK(many::sum(many::abs(layered_tetrahedron.vertex_neighbor_ids)) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.vertex_neighbor_counts)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.vertex_positions))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.vertex_normals))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.vertex_areas)) > 0.01f);

        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.face_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.face_vertex_id_a)) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.face_vertex_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.face_vertex_id_c)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.face_edge_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.face_edge_id_b)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.face_edge_id_c)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.face_endpoint_a))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.face_endpoint_b))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_y(layered_tetrahedron.face_endpoint_c))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.face_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.face_normals))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.face_areas)) > 0.01f);

        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.edge_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.edge_vertex_id_a)) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.edge_vertex_id_b)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.edge_face_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.edge_face_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.edge_endpoint_a))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.edge_endpoint_b))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.edge_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.edge_lengths)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.edge_normals))) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.edge_areas)) > 0.01f);
        
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_vertex_ids))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.arrow_vertex_id_from)) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.arrow_vertex_id_to)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.arrow_face_id_a)) > 0.01f);
        // CHECK(many::sum(many::abs(layered_tetrahedron.arrow_face_id_b)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_endpoint_from))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_endpoint_to))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_midpoints))) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_offsets))) > 0.01f);
        CHECK(many::sum(many::abs(layered_tetrahedron.arrow_lengths)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(layered_tetrahedron.arrow_normals))) > 0.01f);
        // CHECK(layered_tetrahedron.arrow_areas[2] > 0.01f);

    }
}
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

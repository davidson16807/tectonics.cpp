

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <meshes/mesh.hpp>  

#include "MeshCache.hpp"  

#include "MeshCache_test_utils.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

TEST_CASE( "MeshCache correctness", "[MeshCache]" ) {
    SECTION("MeshCache must have the appropriate counts for vertex, edge, arrow, and face attributes"){
        CHECK(diamond.flattened_face_vertex_ids.size() == 12);

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

TEST_CASE( "MeshCache nontriviality", "[MeshCache]" ) {
    SECTION("MeshCache attributes must contain nonzero elements"){
        CHECK(many::sum(many::abs(tetrahedron.flattened_face_vertex_ids)) > 0.01f);

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

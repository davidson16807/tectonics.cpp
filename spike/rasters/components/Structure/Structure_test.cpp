

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <meshes/mesh.hpp>  

#include "Structure.hpp"  

#include "Structure_test_utils.hpp"

using namespace glm;
using namespace series;
using namespace rasters;

TEST_CASE( "Structure correctness", "[Structure]" ) {
    SECTION("Structure must have the appropriate counts for vertex, edge, arrow, and face attributes"){
        CHECK(diamond_structure.flattened_face_vertex_ids.size() == 12);

        CHECK(diamond_structure.vertex_count == 5);
        // CHECK(diamond_structure.vertex_neighbor_ids.size() == 5);
        CHECK(diamond_structure.vertex_neighbor_counts.size() == 5);
        // CHECK(diamond_structure.vertex_average_area.size() == );

        CHECK(diamond_structure.face_count == 4);
        CHECK(diamond_structure.face_vertex_ids.size() == 4);
        CHECK(diamond_structure.face_vertex_id_a.size() == 4);
        CHECK(diamond_structure.face_vertex_id_b.size() == 4);
        CHECK(diamond_structure.face_vertex_id_c.size() == 4);
        // CHECK(diamond_structure.face_edge_id_a.size() == 4);
        // CHECK(diamond_structure.face_edge_id_b.size() == 4);
        // CHECK(diamond_structure.face_edge_id_c.size() == 4);
        // CHECK(0.49f < diamond_structure.face_average_area && diamond_structure.face_average_area < 0.51f);

        CHECK(diamond_structure.edge_count == 8);
        CHECK(diamond_structure.edge_vertex_ids.size() == 8);
        CHECK(diamond_structure.edge_vertex_id_a.size() == 8);
        CHECK(diamond_structure.edge_vertex_id_b.size() == 8);
        // CHECK(diamond_structure.edge_face_id_a.size() == 8);
        // CHECK(diamond_structure.edge_face_id_b.size() == 8);
        // CHECK(1.f < diamond_structure.edge_average_distance && diamond_structure.edge_average_distance < sqrt(2.f));
        
        CHECK(diamond_structure.arrow_count == 16);
        CHECK(diamond_structure.arrow_vertex_ids.size() == 16);
        CHECK(diamond_structure.arrow_vertex_id_from.size() == 16);
        CHECK(diamond_structure.arrow_vertex_id_to.size() == 16);
        // CHECK(diamond_structure.arrow_face_id_a.size() == 16);
        // CHECK(diamond_structure.arrow_face_id_b.size() == 16);
        // CHECK(1.f < diamond_structure.arrow_average_distance && diamond_structure.arrow_average_distance < sqrt(2.f));
    }
}

TEST_CASE( "Structure nontriviality", "[Structure]" ) {
    SECTION("Structure attributes must contain nonzero elements"){
        CHECK(series::sum(series::abs(tetrahedron_structure.flattened_face_vertex_ids)) > 0.01f);

        // CHECK(series::sum(series::abs(tetrahedron_structure.vertex_neighbor_ids)) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.vertex_neighbor_counts)) > 0.01f);

        CHECK(series::sum(series::abs(series::get_x(tetrahedron_structure.face_vertex_ids))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.face_vertex_id_a)) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.face_vertex_id_b)) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.face_vertex_id_c)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.face_edge_id_a)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.face_edge_id_b)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.face_edge_id_c)) > 0.01f);

        CHECK(series::sum(series::abs(series::get_x(tetrahedron_structure.edge_vertex_ids))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.edge_vertex_id_a)) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.edge_vertex_id_b)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.edge_face_id_a)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.edge_face_id_b)) > 0.01f);
        
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_structure.arrow_vertex_ids))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.arrow_vertex_id_from)) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_structure.arrow_vertex_id_to)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.arrow_face_id_a)) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_structure.arrow_face_id_b)) > 0.01f);

    }
}

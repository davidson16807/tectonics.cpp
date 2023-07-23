

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <meshes/mesh.hpp>  

#include <rasters/components/Structure/Structure.hpp>

#include "Metrics.hpp"

#include <rasters/components/Structure/Structure_test_utils.hpp>
#include "Metrics_test_utils.hpp"


using namespace glm;
using namespace series;
using namespace rasters;

TEST_CASE( "Metrics correctness", "[Metrics]" ) {
    SECTION("Metrics must have the appropriate counts for vertex, edge, arrow, and face attributes"){

        CHECK(diamond_metrics.vertex_positions.size() == 5);
        CHECK(diamond_metrics.vertex_normals.size() == 5);
        CHECK(diamond_metrics.vertex_areas.size() == 5);
        // CHECK(diamond_metrics.vertex_average_area.size() == );

        CHECK(diamond_metrics.face_endpoint_a.size() == 4);
        CHECK(diamond_metrics.face_endpoint_b.size() == 4);
        CHECK(diamond_metrics.face_endpoint_c.size() == 4);
        CHECK(diamond_metrics.face_midpoints.size() == 4);
        CHECK(diamond_metrics.face_normals.size() == 4);
        CHECK(diamond_metrics.face_areas.size() == 4);
        // CHECK(0.49f < diamond_metrics.face_average_area && diamond_metrics.face_average_area < 0.51f);

        CHECK(diamond_metrics.edge_endpoint_a.size() == 8);
        CHECK(diamond_metrics.edge_endpoint_b.size() == 8);
        CHECK(diamond_metrics.edge_midpoints.size() == 8);
        CHECK(diamond_metrics.edge_lengths.size() == 8);
        CHECK(diamond_metrics.edge_normals.size() == 8);
        // CHECK(diamond_metrics.edge_areas.size() == );
        // CHECK(1.f < diamond_metrics.edge_average_distance && diamond_metrics.edge_average_distance < sqrt(2.f));
        
        CHECK(diamond_metrics.arrow_endpoint_from.size() == 16);
        CHECK(diamond_metrics.arrow_endpoint_to.size() == 16);
        CHECK(diamond_metrics.arrow_midpoints.size() == 16);
        CHECK(diamond_metrics.arrow_offsets.size() == 16);
        CHECK(diamond_metrics.arrow_lengths.size() == 16);
        CHECK(diamond_metrics.arrow_normals.size() == 16);
        // CHECK(diamond_metrics.arrow_areas.size() == );
        // CHECK(1.f < diamond_metrics.arrow_average_distance && diamond_metrics.arrow_average_distance < sqrt(2.f));
    }
}

TEST_CASE( "Metrics nontriviality", "[Metrics]" ) {
    SECTION("Metrics attributes must contain nonzero elements"){

        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.vertex_positions))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.vertex_normals))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_metrics.vertex_areas)) > 0.01f);

        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.face_endpoint_a))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.face_endpoint_b))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_y(tetrahedron_metrics.face_endpoint_c))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.face_midpoints))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.face_normals))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_metrics.face_areas)) > 0.01f);

        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.edge_endpoint_a))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.edge_endpoint_b))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.edge_midpoints))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_metrics.edge_lengths)) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.edge_normals))) > 0.01f);
        // CHECK(series::sum(series::abs(tetrahedron_metrics.edge_areas)) > 0.01f);
        
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.arrow_endpoint_from))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.arrow_endpoint_to))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.arrow_midpoints))) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.arrow_offsets))) > 0.01f);
        CHECK(series::sum(series::abs(tetrahedron_metrics.arrow_lengths)) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_metrics.arrow_normals))) > 0.01f);
        // CHECK(tetrahedron_metrics.arrow_areas[2] > 0.01f);

    }
}

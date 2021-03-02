
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <series/types.hpp>
#include <series/common.hpp>
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/convenience.hpp> //  operators, etc.

#include "OctosphereGrid.hpp"

rasters::OctosphereGrid octosphere_grid(0.5f * 3.14159f / 5.0f);




TEST_CASE( "OctosphereGrid.sphere_position() regularity", "[rasters]" ) {
    SECTION("OctosphereGrid.sphere_position() must be called repeatedly without changing the output"){
        for(float x = -octosphere_grid.side_leg_cell_count; x < octosphere_grid.side_leg_cell_count; x+=1.0){
        for(float y = -octosphere_grid.side_leg_cell_count; y < octosphere_grid.side_leg_cell_count; y+=1.0){
            CHECK(rasters::OctosphereGrid(0.5f * 3.14159f / 5.0f).sphere_position(glm::vec2(x,y)) == 
                  rasters::OctosphereGrid(0.5f * 3.14159f / 5.0f).sphere_position(glm::vec2(x,y)));
        }}
    }
}
TEST_CASE( "OctosphereGrid sphere_position() locality preservation", "[rasters]" ) {
    SECTION("changes in grid_position must not result in changes to sphere_position that exceed a reasonable multiple of min_cell_width"){
        const float threshold(2.0*octosphere_grid.min_cell_width);
        const glm::vec2 dx(1.0, 0.0);
        const glm::vec2 dy(0.0, 1.0);
        for(float x = -2.0*octosphere_grid.tesselation_leg_cell_count; x < 2.0*octosphere_grid.tesselation_leg_cell_count; x+=1.0){
        for(float y = -2.0*octosphere_grid.tesselation_leg_cell_count; y < 2.0*octosphere_grid.tesselation_leg_cell_count; y+=1.0){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v+dx)) < threshold );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v-dx)) < threshold );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v+dy)) < threshold );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v-dy)) < threshold );
        }}
    }
}
TEST_CASE( "OctosphereGrid sphere_position() congruence", "[rasters]" ) {
    SECTION("an modulo can be applied to input which results in the same output"){
        const float epsilon(1e-4f);
        const glm::vec2 nx(2*octosphere_grid.tesselation_leg_cell_count, 0);
        const glm::vec2 ny(0, 2*octosphere_grid.tesselation_leg_cell_count);
        for(float x = -2.0*octosphere_grid.tesselation_leg_cell_count; x < 2.0*octosphere_grid.tesselation_leg_cell_count; x+=1.0){
        for(float y = -2.0*octosphere_grid.tesselation_leg_cell_count; y < 2.0*octosphere_grid.tesselation_leg_cell_count; y+=1.0){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v+nx)) < epsilon );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v-nx)) < epsilon );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v+ny)) < epsilon );
            CHECK( glm::length(octosphere_grid.sphere_position(v) - octosphere_grid.sphere_position(v-ny)) < epsilon );
        }}
    }
}




TEST_CASE( "OctosphereGrid.grid_position() regularity", "[rasters]" ) {
    SECTION("OctosphereGrid.grid_position() must be called repeatedly without changing the output"){
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            glm::vec3 v(x,y,z);
            if(glm::length(v) > epsilon){
                CHECK(rasters::OctosphereGrid(0.5f * 3.14159f / 5.0f).grid_position(glm::vec3(x,y,z)) == 
                      rasters::OctosphereGrid(0.5f * 3.14159f / 5.0f).grid_position(glm::vec3(x,y,z)));
            }
        }}}
    }
}
TEST_CASE( "OctosphereGrid grid_position() / sphere_position() invertibility", "[rasters]" ) {
    SECTION("OctosphereGrid.sphere_position() must reconstruct input passed to grid_position() for any unit vector"){
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec3 v2 = octosphere_grid.sphere_position( octosphere_grid.grid_position(v) );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}



TEST_CASE( "OctosphereGrid memory_id() / grid_position() invertibility", "[rasters]" ) {
    SECTION("OctosphereGrid.grid_position() must reconstruct input passed to memory_id() for any unit vector"){
        for(int i = 0; i < octosphere_grid.tesselation_cell_count; i++){
            CHECK( i == octosphere_grid.memory_id(octosphere_grid.grid_position(i)) );
        }
    }
}
TEST_CASE( "OctosphereGrid memory_id() range restrictions", "[rasters]" ) {
    SECTION("OctosphereGrid.memory_id() must not produce results outside valid range"){
        for(float x = -2.0*octosphere_grid.tesselation_leg_cell_count; x < 2.0*octosphere_grid.tesselation_leg_cell_count; x+=1.0){
        for(float y = -2.0*octosphere_grid.tesselation_leg_cell_count; y < 2.0*octosphere_grid.tesselation_leg_cell_count; y+=1.0){
            int i = octosphere_grid.memory_id(glm::vec2(x,y));
            CHECK( 0 <= i );
            CHECK( i < octosphere_grid.tesselation_cell_count );
        }}
    }
}
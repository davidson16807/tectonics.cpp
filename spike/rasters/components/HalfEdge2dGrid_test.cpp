#include "HalfEdge2dGrid.hpp"

TEST_CASE( "HalfEdge2dGrid.clockwise() purity", "[rasters]" ) {
    SECTION("HalfEdge2dGrid.clockwise() must be called repeatedly without changing the output"){
        for(int x1 = 0; x1 <= 1; x1+=1){
        for(int y1 = 0; y1 <= 1; y1+=1){
        for(int x2 = 0; x2 <= 1; x2+=1){
        for(int y2 = 0; y2 <= 1; y2+=1){
            CHECK(rasters::HalfEdge2dGrid().clockwise(glm::ivec2(x1,y1), glm::ivec2(x2,y2)) == 
                  rasters::HalfEdge2dGrid().clockwise(glm::ivec2(x1,y1), glm::ivec2(x2,y2)));
        }}}}
    }
}

/*
TEST_CASE( "HalfEdge2dGrid clockwise() orbit", "[rasters]" ) {
    SECTION("after a number of successive invocations the function will repeat its output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        const float epsilon(1e-4f);
        const glm::vec2 nx(4, 0);
        const glm::vec2 ny(0, 4);
        for(float x = -2.0f; x < 2.0f; x+=0.1f){
        for(float y = -2.0f; y < 2.0f; y+=0.1f){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-ny)) < epsilon );
        }}
    }
}
TEST_CASE( "HalfEdge2dGrid clockwise() invertibility", "[rasters]" ) {
    SECTION("after a number of successive invocations the function will repeat its output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        const float epsilon(1e-4f);
        const glm::vec2 nx(4, 0);
        const glm::vec2 ny(0, 4);
        for(float x = -2.0f; x < 2.0f; x+=0.1f){
        for(float y = -2.0f; y < 2.0f; y+=0.1f){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-ny)) < epsilon );
        }}
    }
}
TEST_CASE( "HalfEdge2dGrid clockwise() antisymmetric", "[rasters]" ) {
    SECTION("after a number of successive invocations the function will repeat its output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        const float epsilon(1e-4f);
        const glm::vec2 nx(4, 0);
        const glm::vec2 ny(0, 4);
        for(float x = -2.0f; x < 2.0f; x+=0.1f){
        for(float y = -2.0f; y < 2.0f; y+=0.1f){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-ny)) < epsilon );
        }}
    }
}
/*
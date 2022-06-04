
TEST_CASE( "SpheroidLayering.layer_position() purity", "[rasters]" ) {
    SECTION("SpheroidLayering.layer_position() must be called repeatedly without changing the output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            CHECK(rasters::SpheroidLayering(tesselation).layer_position(glm::vec2(x,y)) == 
                  rasters::SpheroidLayering(tesselation).layer_position(glm::vec2(x,y)));
        }}
    }
}

TEST_CASE( "SpheroidLayering.layer_id() purity", "[rasters]" ) {
    SECTION("SpheroidLayering.layer_id() must be called repeatedly without changing the output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            CHECK(rasters::SpheroidLayering(tesselation).layer_id(glm::vec2(x,y)) == 
                  rasters::SpheroidLayering(tesselation).layer_id(glm::vec2(x,y)));
        }}
    }
}

TEST_CASE( "SpheroidLayering layer_id() / layer_position() invertibility", "[rasters]" ) {
    SECTION("SpheroidLayering.layer_position() must reconstruct input passed to layer_id() for any unit vector"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = mesh.layer_id(v);
                glm::vec3 v2 = mesh.layer_position( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}

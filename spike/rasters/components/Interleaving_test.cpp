
TEST_CASE( "CollignonMesh.tesselation_to_sphere() purity", "[rasters]" ) {
    SECTION("CollignonMesh.tesselation_to_sphere() must be called repeatedly without changing the output"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            CHECK(rasters::CollignonMesh(tesselation).tesselation_to_sphere(glm::vec2(x,y)) == 
                  rasters::CollignonMesh(tesselation).tesselation_to_sphere(glm::vec2(x,y)));
        }}
    }
}

TEST_CASE( "CollignonMesh sphere_to_tesselation() / tesselation_to_sphere() invertibility", "[rasters]" ) {
    SECTION("CollignonMesh.tesselation_to_sphere() must reconstruct input passed to sphere_to_tesselation() for any unit vector"){
        rasters::CollignonProjection projection;
        rasters::CollignonTesselation tesselation(projection);
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = mesh.sphere_to_tesselation(v);
                glm::vec3 v2 = mesh.tesselation_to_sphere( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}

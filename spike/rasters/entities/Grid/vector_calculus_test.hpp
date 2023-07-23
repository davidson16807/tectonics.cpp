
// std libraries
#include <sstream>  //std::stringstream
#include <iostream> //std::cout

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE     // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/mat3x2.hpp>           // *mat3x2

// in-house libraries
#include <series/types.hpp>  
#include <series/relational.hpp>  
#include <series/arithmetic.hpp>  
#include <series/string_cast.hpp>     // to_string
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/random.hpp>      // get_elias_noise
#include <series/glm/convenience.hpp> // get_elias_noise
#include <series/glm/string_cast.hpp> // to_string

#include <meshes/mesh.hpp>

#include "vector_calculus.hpp"

#include "../Grid/convenience.hpp"
#include "../Grid/operators.hpp"

#include "../Grid/Grid_test_utils.hpp" // nonspheroid_icosahedron_grid

using namespace rasters;

TEST_CASE( "Raster gradient determinism", "[rasters]" ) {
    auto a= make_Raster<float>(nonspheroid_icosahedron_grid, {1,2,3,4,5,6,7,8,9,10,11,12});
    SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
        CHECK(series::equal(gradient(a), gradient(a)));
    }
}

// TEST_CASE( "gradient translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);


//     auto a           = make_Raster<float>(icosphere_grid);
//     auto A_ids       = make_Raster<uint>(icosphere_grid);
//     auto Ai_ids      = make_Raster<uint>(icosphere_grid);
//     auto A_pos       = make_Raster<glm::vec3>(icosphere_grid);
//     auto Ai_pos      = make_Raster<glm::vec3>(icosphere_grid);
//     auto A_a         = make_Raster<float>(icosphere_grid);
//     auto grad_A_a    = make_Raster<glm::vec3>(icosphere_grid);
//     auto Ai_grad_A_a = make_Raster<glm::vec3>(icosphere_grid);
//     auto grad_a      = make_Raster<glm::vec3>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(series::equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }

// TEST_CASE( "gradient resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); series::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     MeshCache icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); series::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     MeshCache icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     auto a           = make_Raster<float>(icosphere1.vertex_count);
//     auto A_ids       = make_Raster<uint>(icosphere2.vertex_count);
//     auto Ai_ids      = make_Raster<uint>(icosphere1.vertex_count);
//     auto A_a         = make_Raster<float>(icosphere2.vertex_count);
//     auto grad_A_a    = make_Raster<glm::vec3>(icosphere2.vertex_count);
//     auto Ai_grad_A_a = make_Raster<glm::vec3>(icosphere1.vertex_count);
//     auto grad_a      = make_Raster<glm::vec3>(icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     series::get_elias_noise(icosphere1.vertex_positions, generator, a);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( icosphere1,a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( icosphere2,A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(series::equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }


TEST_CASE( "Raster gradient distributive over addition", "[rasters]" ) {

    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

    auto a           = make_Raster<float>(icosphere_grid);
    auto b           = make_Raster<float>(icosphere_grid);

    std::mt19937 generator(2);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, b);

    SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
        CHECK(series::equal( gradient(a+b),  gradient(a) + gradient(b), 0.01f ));
    }
}

TEST_CASE( "Raster gradient multiplication relation", "[rasters]" ) {

    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

    auto a           = make_Raster<float>(icosphere_grid);
    auto b           = make_Raster<float>(icosphere_grid);

    std::mt19937 generator(2);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
    a /= std::max(-series::min(a), series::max(a));
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, b);
    b /= std::max(-series::min(b), series::max(b));

    // glm::mat3x2 basis = glm::mat3x2(1,0,0,
    //                                 0,1,0);
    // std::cout << to_string(gradient(a*b), basis)  << std::endl;
    // std::cout << to_string(gradient(b)*a + gradient(a)*b, basis) << std::endl;

    SECTION("gradient(a*b) must generate the same output as a*gradient(b) + b*gradient(a)"){
        CHECK(series::equal( gradient(a*b),  gradient(b)*a + gradient(a)*b, 0.1f ));
    }
}

TEST_CASE( "Raster divergence determinism", "[rasters]" ) {
    auto a   = make_Raster<glm::vec3>(nonspheroid_icosahedron_grid, {
        glm::vec3(1, 2, 3 ),
        glm::vec3(4, 5, 6 ),
        glm::vec3(7, 8, 9 ),
        glm::vec3(10,11,12),
        glm::vec3(13,14,15),
        glm::vec3(16,17,18),
        glm::vec3(19,20,21),
        glm::vec3(22,23,24),
        glm::vec3(25,26,27),
        glm::vec3(28,29,30),
        glm::vec3(31,32,33),
        glm::vec3(34,35,36)
    });
    auto out1 = make_Raster<float>(nonspheroid_icosahedron_grid);
    auto out2 = make_Raster<float>(nonspheroid_icosahedron_grid);

    SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
        CHECK(series::equal(divergence(a),divergence(a)));
    }
}
// TEST_CASE( "divergence translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);


//     auto scalar      = make_Raster<float>(icosphere_grid);
//     auto a           = make_Raster<glm::vec3>(icosphere_grid);
//     auto A_ids       = make_Raster<uint>(icosphere_grid);
//     auto Ai_ids      = make_Raster<uint>(icosphere_grid);
//     auto A_pos       = make_Raster<glm::vec3>(icosphere_grid);
//     auto Ai_pos      = make_Raster<glm::vec3>(icosphere_grid);
//     auto A_a         = make_Raster<glm::vec3>(icosphere_grid);
//     auto div_A_a     = make_Raster<float>(icosphere_grid);
//     auto Ai_div_A_a  = make_Raster<float>(icosphere_grid);
//     auto div_a       = make_Raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise  ( generator, scalar   );
//     a = gradient( scalar);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( a,      div_a      );
//         get        ( a,         A_ids,  A_a        );
//         divergence ( A_a,    div_A_a    );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(series::equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }

// TEST_CASE( "divergence resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); series::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     MeshCache icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); series::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     MeshCache icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     auto scalar     = make_Raster<float>(icosphere1.vertex_count);
//     auto a          = make_Raster<glm::vec3>(icosphere1.vertex_count);
//     auto A_ids      = make_Raster<uint>(icosphere2.vertex_count);
//     auto Ai_ids     = make_Raster<uint>(icosphere1.vertex_count);
//     auto A_a        = make_Raster<glm::vec3>(icosphere2.vertex_count);
//     auto div_A_a    = make_Raster<float>(icosphere2.vertex_count);
//     auto Ai_div_A_a = make_Raster<float>(icosphere1.vertex_count);
//     auto div_a      = make_Raster<float>(icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     series::get_elias_noise  ( icosphere1, generator, scalar );
//     a = gradient( icosphere1, scalar);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere1,a,     div_a       );
//         get        ( a,         A_ids, A_a         );
//         divergence ( icosphere2,A_a,   div_A_a     );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(series::equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }


TEST_CASE( "Raster divergence distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

    auto scalar     = make_Raster<float>(icosphere_grid);
    auto a          = make_Raster<glm::vec3>(icosphere_grid);
    auto b          = make_Raster<glm::vec3>(icosphere_grid);
    auto div_a      = make_Raster<float>(icosphere_grid);
    auto div_b      = make_Raster<float>(icosphere_grid);
    auto div_a_b    = make_Raster<float>(icosphere_grid);

    std::mt19937 generator(2);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
    a = gradient(scalar);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
    b = gradient(scalar);

    SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
        CHECK(series::equal(divergence(a) + divergence( b), divergence(a+b), 0.1f));
    }
}

// TEST_CASE( "divergence multiplication relation", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto scalar     = make_Raster<float>(icosphere_grid);
//     auto a          = make_Raster<float>(icosphere_grid);
//     auto b          = make_Raster<glm::vec3>(icosphere_grid);
//     auto grad_a     = make_Raster<glm::vec3>(icosphere_grid);
//     auto div_b      = make_Raster<float>(icosphere_grid);
//     auto div_a_b    = make_Raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     b = gradient( scalar);
//     grad_a = gradient( a     );

//     SECTION("divergence(a+b) must generate the same output as a*divergence(b) + b*divergence(a)"){
//         divergence ( b,      div_b                  );
//         divergence ( b*a,    div_a_b                );
//         CHECK(series::equal( div_a_b,  div_b*a + dot(b, grad_a), 1e-0f));
//     }
// }
TEST_CASE( "Raster curl determinism", "[rasters]" ) {
    auto a   = make_Raster<glm::vec3>(nonspheroid_icosahedron_grid, {
        glm::vec3(1, 2, 3 ),
        glm::vec3(4, 5, 6 ),
        glm::vec3(7, 8, 9 ),
        glm::vec3(10,11,12),
        glm::vec3(13,14,15),
        glm::vec3(16,17,18),
        glm::vec3(19,20,21),
        glm::vec3(22,23,24),
        glm::vec3(25,26,27),
        glm::vec3(28,29,30),
        glm::vec3(31,32,33),
        glm::vec3(34,35,36)
    });
    // auto b = make_Raster<float>(    {1,1,2,3,5,8,13,21,34,55,89,144});
    auto out1= make_Raster<glm::vec3>(nonspheroid_icosahedron_grid);
    auto out2= make_Raster<glm::vec3>(nonspheroid_icosahedron_grid);

    SECTION("curl(grid, a) must generate the same output when called repeatedly"){
        CHECK(series::equal(curl(a),curl(a), 0.1f));
    }
}

TEST_CASE( "Raster curl distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

    auto scalar      = make_Raster<float>(icosphere_grid);
    auto a           = make_Raster<glm::vec3>(icosphere_grid);
    auto b           = make_Raster<glm::vec3>(icosphere_grid);
    auto curl_a      = make_Raster<glm::vec3>(icosphere_grid);
    auto curl_b      = make_Raster<glm::vec3>(icosphere_grid);
    auto curl_a_b    = make_Raster<glm::vec3>(icosphere_grid);

    std::mt19937 generator(2);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
    a = gradient(scalar);
    series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
    b = gradient(scalar);

    SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
        CHECK(series::equal(curl(a) + curl(b), curl(a+b), 0.1f));
    }
}

// TEST_CASE( "curl of gradient is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto a          = make_Raster<float>(icosphere_grid);
//     auto grad_a     = make_Raster<glm::vec3>(icosphere_grid);
//     auto curl_grad_a= make_Raster<glm::vec3>(icosphere_grid);
//     auto zeros      = make_Raster<glm::vec3>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     fill  (zeros, glm::vec3(0));

//     grad_a = gradient  (a);
//     curl      (grad_a, curl_grad_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         // CHECK(curl_grad_a == zeros);
//         CHECK(series::equal(curl_grad_a, zeros));
//     }
// }
// TEST_CASE( "divergence of curl is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto scalar     = make_Raster<float>(icosphere_grid);
//     auto a          = make_Raster<glm::vec3>(icosphere_grid);
//     auto curl_a     = make_Raster<glm::vec3>(icosphere_grid);
//     auto div_curl_a = make_Raster<float>(icosphere_grid);
//     auto zeros      = make_Raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     a = gradient(scalar);
//     fill  (zeros, 0.f);

//     curl       (a,      curl_a     );
//     divergence (curl_a, div_curl_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         CHECK(div_curl_a == zeros);
//         // CHECK(series::equal(div_curl_a, zeros));
//     }
// }
TEST_CASE( "Raster laplacian determinism", "[rasters]" ) {
    auto a    = make_Raster<float>(nonspheroid_icosahedron_grid, {1,2,3,4,5,6,7,8,9,10,11,12});
    // auto b = make_Raster<float>(nonspheroid_icosahedron_grid, {1,1,2,3,5,8,13,21,34,55,89,144});
    auto out1 = make_Raster<float>(nonspheroid_icosahedron_grid);
    auto out2 = make_Raster<float>(nonspheroid_icosahedron_grid);
    SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
        CHECK(series::equal(laplacian(a), laplacian(a), 0.1f));
    }
}


// TEST_CASE( "laplacian is divergence of gradient", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); series::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto a          = make_Raster<float>(icosphere_grid);
//     auto grad_a     = make_Raster<glm::vec3>(icosphere_grid);
//     auto div_grad_a = make_Raster<float>(icosphere_grid);
//     auto laplacian_a= make_Raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     series::get_elias_noise(generator, a);

//     laplacian (a,      laplacian_a);
//     grad_a = gradient(a);
//     divergence(grad_a, div_grad_a );
//     SECTION("laplacian(a) must generate the same output as div(grad(a))"){
//         CHECK(series::equal(laplacian_a, div_grad_a));
//     }
// }
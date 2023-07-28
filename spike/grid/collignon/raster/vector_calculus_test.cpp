
// std libraries
#include <sstream>  //std::stringstream
#include <iostream> //std::cout
#include <vector>   //std::vector

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE     // disable anonymous structs so we can build with ISO C++
#include "glm/glm.hpp"

// in-house libraries
#include <store/whole.hpp>
// #include <store/glm/whole.hpp>
#include <store/series/Noise.hpp>

#include "../Grid.hpp"
#include "vector_calculus.hpp"

TEST_CASE( "Raster gradient determinism", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::Noise<float>();
    auto out1 = std::vector<glm::vec3>(grid.vertex_count());
    auto out2 = std::vector<glm::vec3>(grid.vertex_count());
    SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
        gradient(grid, a, out1);
        gradient(grid, a, out2);
        CHECK(whole::equal(out1, out2));
    }
}

// TEST_CASE( "gradient translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);


//     auto a           = raster<float>(icosphere_grid);
//     auto A_ids       = raster<uint>(icosphere_grid);
//     auto Ai_ids      = raster<uint>(icosphere_grid);
//     auto A_pos       = raster<glm::vec3>(icosphere_grid);
//     auto Ai_pos      = raster<glm::vec3>(icosphere_grid);
//     auto A_a         = raster<float>(icosphere_grid);
//     auto grad_A_a    = raster<glm::vec3>(icosphere_grid);
//     auto Ai_grad_A_a = raster<glm::vec3>(icosphere_grid);
//     auto grad_a      = raster<glm::vec3>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         grid.gradient ( a,      grad_a         );
//         grid.get      ( a,         A_ids,  A_a            );
//         grid.gradient ( A_a,    grad_A_a       );
//         grid.get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(whole::equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }

// TEST_CASE( "gradient resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); each::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     MeshCache icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); each::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     MeshCache icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     auto a           = raster<float>(icosphere1.vertex_count);
//     auto A_ids       = raster<uint>(icosphere2.vertex_count);
//     auto Ai_ids      = raster<uint>(icosphere1.vertex_count);
//     auto A_a         = raster<float>(icosphere2.vertex_count);
//     auto grad_A_a    = raster<glm::vec3>(icosphere2.vertex_count);
//     auto Ai_grad_A_a = raster<glm::vec3>(icosphere1.vertex_count);
//     auto grad_a      = raster<glm::vec3>(icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere1.vertex_positions, generator, a);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         grid.gradient ( icosphere1,a,      grad_a         );
//         grid.get      ( a,         A_ids,  A_a            );
//         grid.gradient ( icosphere2,A_a,    grad_A_a       );
//         grid.get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(whole::equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }


// TEST_CASE( "Raster gradient distributive over addition", "[rasters]" ) {

//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

//     auto a           = raster<float>(icosphere_grid);
//     auto b           = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, b);

//     SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
//         CHECK(whole::equal( grid.gradient(a+b),  grid.gradient(a) + grid.gradient(b), 0.01f ));
//     }
// }

// TEST_CASE( "Raster gradient multiplication relation", "[rasters]" ) {

//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

//     auto a           = raster<float>(icosphere_grid);
//     auto b           = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     a /= std::max(-each::min(a), each::max(a));
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, b);
//     b /= std::max(-each::min(b), each::max(b));

//     // glm::mat3x2 basis = glm::mat3x2(1,0,0,
//     //                                 0,1,0);
//     // std::cout << to_string(grid.gradient(a*b), basis)  << std::endl;
//     // std::cout << to_string(grid.gradient(b)*a + grid.gradient(a)*b, basis) << std::endl;

//     SECTION("gradient(a*b) must generate the same output as a*gradient(b) + b*gradient(a)"){
//         CHECK(whole::equal( grid.gradient(a*b),  grid.gradient(b)*a + grid.gradient(a)*b, 0.1f ));
//     }
// }

// TEST_CASE( "Raster divergence determinism", "[rasters]" ) {
//     auto a   = raster<glm::vec3>(nonspheroid_icosahedron_grid, {
//         glm::vec3(1, 2, 3 ),
//         glm::vec3(4, 5, 6 ),
//         glm::vec3(7, 8, 9 ),
//         glm::vec3(10,11,12),
//         glm::vec3(13,14,15),
//         glm::vec3(16,17,18),
//         glm::vec3(19,20,21),
//         glm::vec3(22,23,24),
//         glm::vec3(25,26,27),
//         glm::vec3(28,29,30),
//         glm::vec3(31,32,33),
//         glm::vec3(34,35,36)
//     });
//     auto out1 = raster<float>(nonspheroid_icosahedron_grid);
//     auto out2 = raster<float>(nonspheroid_icosahedron_grid);

//     SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
//         CHECK(whole::equal(divergence(a),divergence(a)));
//     }
// }
// TEST_CASE( "divergence translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);


//     auto scalar      = raster<float>(icosphere_grid);
//     auto a           = raster<glm::vec3>(icosphere_grid);
//     auto A_ids       = raster<uint>(icosphere_grid);
//     auto Ai_ids      = raster<uint>(icosphere_grid);
//     auto A_pos       = raster<glm::vec3>(icosphere_grid);
//     auto Ai_pos      = raster<glm::vec3>(icosphere_grid);
//     auto A_a         = raster<glm::vec3>(icosphere_grid);
//     auto div_A_a     = raster<float>(icosphere_grid);
//     auto Ai_div_A_a  = raster<float>(icosphere_grid);
//     auto div_a       = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise  ( generator, scalar   );
//     a = grid.gradient( scalar);

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
//         CHECK(whole::equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }

// TEST_CASE( "divergence resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); each::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     MeshCache icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); each::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     MeshCache icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     auto scalar     = raster<float>(icosphere1.vertex_count);
//     auto a          = raster<glm::vec3>(icosphere1.vertex_count);
//     auto A_ids      = raster<uint>(icosphere2.vertex_count);
//     auto Ai_ids     = raster<uint>(icosphere1.vertex_count);
//     auto A_a        = raster<glm::vec3>(icosphere2.vertex_count);
//     auto div_A_a    = raster<float>(icosphere2.vertex_count);
//     auto Ai_div_A_a = raster<float>(icosphere1.vertex_count);
//     auto div_a      = raster<float>(icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     each::get_elias_noise  ( icosphere1, generator, scalar );
//     a = grid.gradient( icosphere1, scalar);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere1,a,     div_a       );
//         get        ( a,         A_ids, A_a         );
//         divergence ( icosphere2,A_a,   div_A_a     );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(whole::equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }


// TEST_CASE( "Raster divergence distributive over addition", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

//     auto scalar     = raster<float>(icosphere_grid);
//     auto a          = raster<glm::vec3>(icosphere_grid);
//     auto b          = raster<glm::vec3>(icosphere_grid);
//     auto div_a      = raster<float>(icosphere_grid);
//     auto div_b      = raster<float>(icosphere_grid);
//     auto div_a_b    = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     a = grid.gradient(scalar);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     b = grid.gradient(scalar);

//     SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
//         CHECK(whole::equal(divergence(a) + divergence( b), divergence(a+b), 0.1f));
//     }
// }

// TEST_CASE( "divergence multiplication relation", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto scalar     = raster<float>(icosphere_grid);
//     auto a          = raster<float>(icosphere_grid);
//     auto b          = raster<glm::vec3>(icosphere_grid);
//     auto grad_a     = raster<glm::vec3>(icosphere_grid);
//     auto div_b      = raster<float>(icosphere_grid);
//     auto div_a_b    = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     b = grid.gradient( scalar);
//     grad_a = grid.gradient( a     );

//     SECTION("divergence(a+b) must generate the same output as a*divergence(b) + b*divergence(a)"){
//         divergence ( b,      div_b                  );
//         divergence ( b*a,    div_a_b                );
//         CHECK(whole::equal( div_a_b,  div_b*a + dot(b, grad_a), 1e-0f));
//     }
// }
// TEST_CASE( "Raster curl determinism", "[rasters]" ) {
//     auto a   = raster<glm::vec3>(nonspheroid_icosahedron_grid, {
//         glm::vec3(1, 2, 3 ),
//         glm::vec3(4, 5, 6 ),
//         glm::vec3(7, 8, 9 ),
//         glm::vec3(10,11,12),
//         glm::vec3(13,14,15),
//         glm::vec3(16,17,18),
//         glm::vec3(19,20,21),
//         glm::vec3(22,23,24),
//         glm::vec3(25,26,27),
//         glm::vec3(28,29,30),
//         glm::vec3(31,32,33),
//         glm::vec3(34,35,36)
//     });
//     // auto b = raster<float>(    {1,1,2,3,5,8,13,21,34,55,89,144});
//     auto out1= raster<glm::vec3>(nonspheroid_icosahedron_grid);
//     auto out2= raster<glm::vec3>(nonspheroid_icosahedron_grid);

//     SECTION("curl(grid, a) must generate the same output when called repeatedly"){
//         CHECK(whole::equal(curl(a),curl(a), 0.1f));
//     }
// }

// TEST_CASE( "Raster curl distributive over addition", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     rasters::Grid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);

//     auto scalar      = raster<float>(icosphere_grid);
//     auto a           = raster<glm::vec3>(icosphere_grid);
//     auto b           = raster<glm::vec3>(icosphere_grid);
//     auto curl_a      = raster<glm::vec3>(icosphere_grid);
//     auto curl_b      = raster<glm::vec3>(icosphere_grid);
//     auto curl_a_b    = raster<glm::vec3>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     a = grid.gradient(scalar);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     b = grid.gradient(scalar);

//     SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
//         CHECK(whole::equal(curl(a) + curl(b), curl(a+b), 0.1f));
//     }
// }

// TEST_CASE( "curl of gradient is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto a          = raster<float>(icosphere_grid);
//     auto grad_a     = raster<glm::vec3>(icosphere_grid);
//     auto curl_grad_a= raster<glm::vec3>(icosphere_grid);
//     auto zeros      = raster<glm::vec3>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, a);
//     fill  (zeros, glm::vec3(0));

//     grad_a = grid.gradient  (a);
//     curl      (grad_a, curl_grad_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         // CHECK(curl_grad_a == zeros);
//         CHECK(whole::equal(curl_grad_a, zeros));
//     }
// }
// TEST_CASE( "divergence of curl is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto scalar     = raster<float>(icosphere_grid);
//     auto a          = raster<glm::vec3>(icosphere_grid);
//     auto curl_a     = raster<glm::vec3>(icosphere_grid);
//     auto div_curl_a = raster<float>(icosphere_grid);
//     auto zeros      = raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(icosphere_grid.metrics->vertex_positions, generator, scalar);
//     a = grid.gradient(scalar);
//     fill  (zeros, 0.f);

//     curl       (a,      curl_a     );
//     divergence (curl_a, div_curl_a );
//     SECTION("curl(grid.gradient(a)) must generate the zero vector"){
//         CHECK(div_curl_a == zeros);
//         // CHECK(whole::equal(div_curl_a, zeros));
//     }
// }
// TEST_CASE( "Raster laplacian determinism", "[rasters]" ) {
//     auto a    = raster<float>(nonspheroid_icosahedron_grid, {1,2,3,4,5,6,7,8,9,10,11,12});
//     // auto b = raster<float>(nonspheroid_icosahedron_grid, {1,1,2,3,5,8,13,21,34,55,89,144});
//     auto out1 = raster<float>(nonspheroid_icosahedron_grid);
//     auto out2 = raster<float>(nonspheroid_icosahedron_grid);
//     SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
//         CHECK(whole::equal(laplacian(a), laplacian(a), 0.1f));
//     }
// }


// TEST_CASE( "laplacian is divergence of gradient", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); each::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     auto a          = raster<float>(icosphere_grid);
//     auto grad_a     = raster<glm::vec3>(icosphere_grid);
//     auto div_grad_a = raster<float>(icosphere_grid);
//     auto laplacian_a= raster<float>(icosphere_grid);

//     std::mt19937 generator(2);
//     each::get_elias_noise(generator, a);

//     laplacian (a,      laplacian_a);
//     grad_a = grid.gradient(a);
//     divergence(grad_a, div_grad_a );
//     SECTION("laplacian(a) must generate the same output as div(grad(a))"){
//         CHECK(whole::equal(laplacian_a, div_grad_a));
//     }
// }

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
#include <store/glm/each.hpp>
#include <store/each.hpp>
#include <store/whole.hpp>
// #include <store/glm/whole.hpp>
#include <store/series/Noise.hpp>

#include "../Grid.hpp"
#include "vector_calculus.hpp"

TEST_CASE( "Raster gradient determinism", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<float>();
    auto out1 = std::vector<glm::vec3>(grid.vertex_count());
    auto out2 = std::vector<glm::vec3>(grid.vertex_count());
    SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
        collignon::gradient(grid, a, out1);
        collignon::gradient(grid, a, out2);
        CHECK(whole::equal(out1, out2));
    }
}

TEST_CASE( "Raster gradient distributive over addition", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<float>(11.0f, 11000.0f);
    auto b = series::UnitIntervalNoise<float>(12.0f, 12000.0f);
    auto ab = std::vector<float>(grid.vertex_count());
    auto grad_a = std::vector<glm::vec3>(grid.vertex_count());
    auto grad_b = std::vector<glm::vec3>(grid.vertex_count());
    auto grad_ab = std::vector<glm::vec3>(grid.vertex_count());
    auto grad_a_grad_b = std::vector<glm::vec3>(grid.vertex_count());

    SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
        each::add(a, b, ab);
        collignon::gradient(grid, ab, grad_ab);
        collignon::gradient(grid, a, grad_a);
        collignon::gradient(grid, b, grad_b);
        each::add(grad_a, grad_b, grad_a_grad_b);
        CHECK(whole::equal( grad_ab,  grad_a_grad_b, 0.01f ));
    }
}

// TEST_CASE( "Raster gradient multiplication relation", "[rasters]" ) {

//     float radius(2.0f);
//     int vertex_count_per_half_meridian(10);
//     collignon::Grid grid(radius, vertex_count_per_half_meridian);
//     auto a = series::UnitIntervalNoise<float>(11.0f, 11000.0f);
//     auto b = series::UnitIntervalNoise<float>(12.0f, 12000.0f);
//     auto ab = std::vector<float>(grid.vertex_count());
//     auto grad_ab = std::vector<glm::vec3>(grid.vertex_count());
//     auto grad_a = std::vector<glm::vec3>(grid.vertex_count());
//     auto grad_b = std::vector<glm::vec3>(grid.vertex_count());
//     auto b_grad_a = std::vector<glm::vec3>(grid.vertex_count());
//     auto a_grad_b = std::vector<glm::vec3>(grid.vertex_count());
//     auto a_grad_b_b_grad_a = std::vector<glm::vec3>(grid.vertex_count());

//     SECTION("gradient(a*b) must generate the same output as a*gradient(b) + b*gradient(a)"){
//         each::mult(a, b, ab);
//         collignon::gradient(grid, ab, grad_ab);
//         collignon::gradient(grid, a, grad_a);
//         collignon::gradient(grid, b, grad_b);
//         each::mult(a, grad_b, a_grad_b);
//         each::mult(b, grad_a, b_grad_a);
//         each::add(a_grad_b, b_grad_a, a_grad_b_b_grad_a);
//         CHECK(whole::equal( grad_ab,  a_grad_b_b_grad_a, 0.1f ));
//     }
// }


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


TEST_CASE( "Raster divergence determinism", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<glm::vec3>(glm::vec3(10,11,12), glm::vec3(10000,11000,12000));
    auto div_a1 = std::vector<float>(grid.vertex_count());
    auto div_a2 = std::vector<float>(grid.vertex_count());
    SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
        collignon::divergence(grid, a, div_a1);
        collignon::divergence(grid, a, div_a2);
        CHECK(whole::equal(div_a1, div_a2));
    }
}


TEST_CASE( "Raster divergence distributive over addition", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<glm::vec3>(glm::vec3(10,11,12), glm::vec3(10000,11000,12000));
    auto b = series::UnitIntervalNoise<glm::vec3>(glm::vec3(13,14,15), glm::vec3(13000,14000,15000));
    auto ab = std::vector<glm::vec3>(grid.vertex_count());
    auto div_a = std::vector<float>(grid.vertex_count());
    auto div_b = std::vector<float>(grid.vertex_count());
    auto div_ab = std::vector<float>(grid.vertex_count());
    auto div_a_div_b = std::vector<float>(grid.vertex_count());

    SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
        each::add(a, b, ab);
        collignon::divergence(grid, ab, div_ab);
        collignon::divergence(grid, a, div_a);
        collignon::divergence(grid, b, div_b);
        each::add(div_a, div_b, div_a_div_b);
        CHECK(whole::equal( div_ab,  div_a_div_b, 0.01f ));
    }
}


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

TEST_CASE( "Raster curl determinism", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<glm::vec3>(glm::vec3(10,11,12), glm::vec3(10000,11000,12000));
    auto curl_a1 = std::vector<glm::vec3>(grid.vertex_count());
    auto curl_a2 = std::vector<glm::vec3>(grid.vertex_count());
    SECTION("curl(grid, a) must generate the same output when called repeatedly"){
        collignon::curl(grid, a, curl_a1);
        collignon::curl(grid, a, curl_a2);
        CHECK(whole::equal(curl_a1, curl_a2));
    }
}

TEST_CASE( "Raster curl distributive over addition", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<glm::vec3>(glm::vec3(10,11,12), glm::vec3(10000,11000,12000));
    auto b = series::UnitIntervalNoise<glm::vec3>(glm::vec3(13,14,15), glm::vec3(13000,14000,15000));
    auto ab = std::vector<glm::vec3>(grid.vertex_count());
    auto curl_a = std::vector<glm::vec3>(grid.vertex_count());
    auto curl_b = std::vector<glm::vec3>(grid.vertex_count());
    auto curl_ab = std::vector<glm::vec3>(grid.vertex_count());
    auto curl_a_curl_b = std::vector<glm::vec3>(grid.vertex_count());

    SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
        each::add(a, b, ab);
        collignon::curl(grid, ab, curl_ab);
        collignon::curl(grid, a, curl_a);
        collignon::curl(grid, b, curl_b);
        each::add(curl_a, curl_b, curl_a_curl_b);
        CHECK(whole::equal( curl_ab,  curl_a_curl_b, 0.01f ));
    }
}



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


TEST_CASE( "Raster laplacian determinism", "[rasters]" ) {
    float radius(2.0f);
    int vertex_count_per_half_meridian(10);
    collignon::Grid grid(radius, vertex_count_per_half_meridian);
    auto a = series::UnitIntervalNoise<float>();
    auto laplacian_a1 = std::vector<float>(grid.vertex_count());
    auto laplacian_a2 = std::vector<float>(grid.vertex_count());
    SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
        collignon::laplacian(grid, a, laplacian_a1);
        collignon::laplacian(grid, a, laplacian_a2);
        CHECK(whole::equal(laplacian_a1, laplacian_a2));
    }
}


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

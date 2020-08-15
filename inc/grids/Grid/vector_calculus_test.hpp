
// std libraries
#include <sstream>  //std::stringstream
#include <iostream>  //std::cout

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/random.hpp>      // get_elias_noise
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

#include "vector_calculus.hpp"

#include "Grid_test_utils.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

/*
"icosahedron" is a simple 3d grid for testing rasters with a relatively large number of vertices
*/
Grid icosahedron = Grid(meshes::icosahedron.vertices, meshes::icosahedron.faces);


TEST_CASE( "gradient determinism", "[rasters]" ) {
    floats a   = floats({1,2,3,4,5,6,7,8,9,10,11,12});
    SECTION("gradient(grid, a) must generate the same output when called repeatedly"){
        CHECK(gradient(icosahedron, a)==gradient(icosahedron, a));
    }
}

// TEST_CASE( "gradient translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a           (icosphere.vertex_count);
//     uints  A_ids       (icosphere.vertex_count);
//     uints  Ai_ids      (icosphere.vertex_count);
//     vec3s  A_pos       (icosphere.vertex_count);
//     vec3s  Ai_pos      (icosphere.vertex_count);
//     floats A_a         (icosphere.vertex_count);
//     vec3s  grad_A_a    (icosphere.vertex_count);
//     vec3s  Ai_grad_A_a (icosphere.vertex_count);
//     vec3s  grad_a      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere.vertex_positions, generator, a);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( icosphere, a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( icosphere, A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }

// TEST_CASE( "gradient resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); many::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     Grid icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); many::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     Grid icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     floats a           (icosphere1.vertex_count);
//     uints  A_ids       (icosphere2.vertex_count);
//     uints  Ai_ids      (icosphere1.vertex_count);
//     floats A_a         (icosphere2.vertex_count);
//     vec3s  grad_A_a    (icosphere2.vertex_count);
//     vec3s  Ai_grad_A_a (icosphere1.vertex_count);
//     vec3s  grad_a      (icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere1.vertex_positions, generator, a);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("gradient(a) must generate the same output as unshift(gradient(shift(a)))"){
//         gradient ( icosphere1,a,      grad_a         );
//         get      ( a,         A_ids,  A_a            );
//         gradient ( icosphere2,A_a,    grad_A_a       );
//         get      ( grad_A_a,  Ai_ids, Ai_grad_A_a    );
//         CHECK(equal(grad_a, Ai_grad_A_a, 0.7f, 0.1f));
//     }
// }


TEST_CASE( "gradient distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats a           (icosphere.vertex_count);
    floats b           (icosphere.vertex_count);

    std::mt19937 generator(2);
    many::get_elias_noise(icosphere.vertex_positions, generator, a);
    many::get_elias_noise(icosphere.vertex_positions, generator, b);

    SECTION("gradient(a+b) must generate the same output as gradient(a)+gradient(b)"){
        CHECK(equal( gradient(icosphere, a+b),  gradient(icosphere, a) + gradient(icosphere, b), 0.01f, 0.01f ));
    }
}

TEST_CASE( "gradient multiplication relation", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats a           (icosphere.vertex_count);
    floats b           (icosphere.vertex_count);

    std::mt19937 generator(2);
    many::get_elias_noise(icosphere.vertex_positions, generator, a);
    many::get_elias_noise(icosphere.vertex_positions, generator, b);

    SECTION("gradient(a*b) must generate the same output as a*gradient(b) + b*gradient(a)"){
        CHECK(equal( gradient(icosphere, a*b),  gradient(icosphere, b)*a + gradient(icosphere, a)*b, 0.7f, 0.1f ));
    }
}

TEST_CASE( "divergence determinism", "[rasters]" ) {
    vec3s a    = vec3s ({
        vec3(1, 2, 3 ),
        vec3(4, 5, 6 ),
        vec3(7, 8, 9 ),
        vec3(10,11,12),
        vec3(13,14,15),
        vec3(16,17,18),
        vec3(19,20,21),
        vec3(22,23,24),
        vec3(25,26,27),
        vec3(28,29,30),
        vec3(31,32,33),
        vec3(34,35,36)
    });
    floats out1 = floats(icosahedron.vertex_count);
    floats out2 = floats(icosahedron.vertex_count);

    SECTION("divergence(grid, a) must generate the same output when called repeatedly"){
        CHECK(divergence(icosahedron, a)==divergence(icosahedron, a));
    }
}
// TEST_CASE( "divergence translation invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar      (icosphere.vertex_count);
//     vec3s  a           (icosphere.vertex_count);
//     uints  A_ids       (icosphere.vertex_count);
//     uints  Ai_ids      (icosphere.vertex_count);
//     vec3s  A_pos       (icosphere.vertex_count);
//     vec3s  Ai_pos      (icosphere.vertex_count);
//     vec3s  A_a         (icosphere.vertex_count);
//     floats div_A_a     (icosphere.vertex_count);
//     floats Ai_div_A_a  (icosphere.vertex_count);
//     floats div_a       (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise  ( icosphere, generator, scalar   );
//     a = gradient( icosphere, scalar);

//     mat4   A      = glm::rotate(mat4(1.f), 3.14f/3.f, glm::vec3(1,1,1));
//     mult(A,  icosphere.vertex_positions, A_pos);
//     icosphere.get_ids(A_pos,  A_ids);

//     mat4   Ai     = glm::inverse(A);
//     mult(Ai, icosphere.vertex_positions, Ai_pos);
//     icosphere.get_ids(Ai_pos, Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere, a,      div_a      );
//         get        ( a,         A_ids,  A_a        );
//         divergence ( icosphere, A_a,    div_A_a    );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }

// TEST_CASE( "divergence resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); many::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     Grid icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); many::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     Grid icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     floats scalar     (icosphere1.vertex_count);
//     vec3s  a          (icosphere1.vertex_count);
//     uints  A_ids      (icosphere2.vertex_count);
//     uints  Ai_ids     (icosphere1.vertex_count);
//     vec3s  A_a        (icosphere2.vertex_count);
//     floats div_A_a    (icosphere2.vertex_count);
//     floats Ai_div_A_a (icosphere1.vertex_count);
//     floats div_a      (icosphere1.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise  ( icosphere1, generator, scalar );
//     a = gradient( icosphere1, scalar);

//     icosphere1.get_ids(icosphere2.vertex_positions,  A_ids);
//     icosphere2.get_ids(icosphere1.vertex_positions,  Ai_ids);

//     SECTION("divergence(a) must generate the same output as unshift(divergence(shift(a)))"){
//         divergence ( icosphere1,a,     div_a       );
//         get        ( a,         A_ids, A_a         );
//         divergence ( icosphere2,A_a,   div_A_a     );
//         get        ( div_A_a,   Ai_ids, Ai_div_A_a );
//         CHECK(equal(div_a, Ai_div_A_a, 1e-0f));
//     }
// }


TEST_CASE( "divergence distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats scalar     (icosphere.vertex_count);
    vec3s  a          (icosphere.vertex_count);
    vec3s  b          (icosphere.vertex_count);
    floats div_a      (icosphere.vertex_count);
    floats div_b      (icosphere.vertex_count);
    floats div_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
    a = gradient( icosphere, scalar);
    many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
    b = gradient( icosphere, scalar);

    SECTION("divergence(a+b) must generate the same output as divergence(a)+divergence(b)"){
        CHECK(divergence( icosphere, a) + divergence( icosphere, b) == divergence(icosphere, a+b));
    }
}

// TEST_CASE( "divergence multiplication relation", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     floats a          (icosphere.vertex_count);
//     vec3s  b          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     floats div_b      (icosphere.vertex_count);
//     floats div_a_b    (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere.vertex_positions, generator, a);
//     many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     b = gradient( icosphere, scalar);
//     grad_a = gradient( icosphere, a     );

//     SECTION("divergence(a+b) must generate the same output as a*divergence(b) + b*divergence(a)"){
//         divergence ( icosphere, b,      div_b                  );
//         divergence ( icosphere, b*a,    div_a_b                );
//         CHECK(equal( div_a_b,  div_b*a + dot(b, grad_a), 1e-0f));
//     }
// }
TEST_CASE( "curl determinism", "[rasters]" ) {
    vec3s a    = vec3s ({
        vec3(1, 2, 3 ),
        vec3(4, 5, 6 ),
        vec3(7, 8, 9 ),
        vec3(10,11,12),
        vec3(13,14,15),
        vec3(16,17,18),
        vec3(19,20,21),
        vec3(22,23,24),
        vec3(25,26,27),
        vec3(28,29,30),
        vec3(31,32,33),
        vec3(34,35,36)
    });
    // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
    vec3s  out1 = vec3s (icosahedron.vertex_count);
    vec3s  out2 = vec3s (icosahedron.vertex_count);

    SECTION("curl(grid, a) must generate the same output when called repeatedly"){
        CHECK(curl(icosahedron, a)==curl(icosahedron, a));
    }
}

TEST_CASE( "curl distributive over addition", "[rasters]" ) {
    meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
    // icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);

    Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

    floats scalar     (icosphere.vertex_count);
    vec3s  a          (icosphere.vertex_count);
    vec3s  b          (icosphere.vertex_count);
    vec3s  curl_a      (icosphere.vertex_count);
    vec3s  curl_b      (icosphere.vertex_count);
    vec3s  curl_a_b    (icosphere.vertex_count);

    std::mt19937 generator(2);
    many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
    a = gradient( icosphere, scalar);
    many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
    b = gradient( icosphere, scalar);

    SECTION("curl(a+b) must generate the same output as curl(a)+curl(b)"){
        CHECK(curl(icosphere, a) + curl(icosphere, b) == curl(icosphere, a+b));
    }
}

// TEST_CASE( "curl of gradient is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     vec3s  curl_grad_a(icosphere.vertex_count);
//     vec3s  zeros      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere.vertex_positions, generator, a);
//     fill  (zeros, vec3(0));

//     grad_a = gradient  (icosphere, a);
//     curl      (icosphere, grad_a, curl_grad_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         // CHECK(curl_grad_a == zeros);
//         CHECK(equal(curl_grad_a, zeros));
//     }
// }
// TEST_CASE( "divergence of curl is zero", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats scalar     (icosphere.vertex_count);
//     vec3s  a          (icosphere.vertex_count);
//     vec3s  curl_a     (icosphere.vertex_count);
//     floats div_curl_a (icosphere.vertex_count);
//     floats zeros      (icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere.vertex_positions, generator, scalar);
//     a = gradient(icosphere, scalar);
//     fill  (zeros, 0.f);

//     curl       (icosphere, a,      curl_a     );
//     divergence (icosphere, curl_a, div_curl_a );
//     SECTION("curl(gradient(a)) must generate the zero vector"){
//         CHECK(div_curl_a == zeros);
//         // CHECK(equal(div_curl_a, zeros));
//     }
// }
TEST_CASE( "laplacian determinism", "[rasters]" ) {
    floats a    = floats({1,2,3,4,5,6,7,8,9,10,11,12});
    // floats b = floats({1,1,2,3,5,8,13,21,34,55,89,144});
    floats out1 = floats (icosahedron.vertex_count);
    floats out2 = floats (icosahedron.vertex_count);
    SECTION("laplacian(grid, a) must generate the same output when called repeatedly"){
        CHECK(laplacian(icosahedron, a)==laplacian(icosahedron, a));
    }
}


// TEST_CASE( "laplacian is divergence of gradient", "[rasters]" ) {
//     meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
//     Grid icosphere(icosphere_mesh.vertices, icosphere_mesh.faces);

//     floats a          (icosphere.vertex_count);
//     vec3s  grad_a     (icosphere.vertex_count);
//     floats div_grad_a (icosphere.vertex_count);
//     floats laplacian_a(icosphere.vertex_count);

//     std::mt19937 generator(2);
//     many::get_elias_noise(icosphere, generator, a);

//     laplacian (icosphere, a,      laplacian_a);
//     grad_a = gradient(icosphere, a);
//     divergence(icosphere, grad_a, div_grad_a );
//     SECTION("laplacian(a) must generate the same output as div(grad(a))"){
//         CHECK(equal(laplacian_a, div_grad_a));
//     }
// }
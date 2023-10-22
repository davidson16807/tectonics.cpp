
// std libraries
#include <sstream>  //std::stringstream
#include <iostream> //std::cout
#include <vector>   //std::vector

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3
#include "glm/glm.hpp"

// in-house libraries
#include <store/glm/each_specialization.hpp>
#include <store/glm/whole_specialization.hpp>
#include <store/glm/each.hpp>
#include <store/each.hpp>  
#include <store/whole.hpp>  
#include <store/known.hpp>  
#include <store/series/Get.hpp>
#include <store/series/Map.hpp>
#include <store/series/Range.hpp>
#include <store/series/glm/VectorInterleave.hpp>
#include <store/series/noise/GaussianNoise.hpp>
#include <store/series/noise/glm/UnitVectorNoise.hpp>

#include <field/noise/EliasNoise.hpp>
#include <field/noise/ValueNoise.hpp>
#include <field/noise/SquareNoise.hpp>
#include <field/VectorZip.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/series.hpp>

#include <grid/spheroidal/string_cast.hpp>

#include "VectorCalculusByFundamentalTheorem.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

namespace dymaxion {

    template<typename id, typename scalar>
    struct Adapter{
        Grid<id,scalar> grid;
        scalar threshold;
        std::size_t test_size;

        Adapter(const Grid<id, scalar>& grid, const scalar threshold, const std::size_t test_size):
            grid(grid),
            threshold(threshold),
            test_size(test_size)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            return whole::distance(a,b) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            // return dymaxion::to_string(grid, a, 200);
            return spheroidal::to_string(grid, a);
        }

    };

}

#define DYMAXION_TEST_GRIDDED_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(GRID,x,out)); return out; }

#define DYMAXION_TEST_UNARY_OUT_PARAMETER(TYPE,GRID,F)   \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,out)); return out; }
#define DYMAXION_TEST_BINARY_OUT_PARAMETER(TYPE,GRID,F)  \
    [=](auto x, auto y){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,out)); return out; }
#define DYMAXION_TEST_TRINARY_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x, auto y, auto z){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,z,out)); return out; }


dymaxion::Grid grid(0.1, 50);

std::vector elias_scalar_rasters{
    known::store(
        grid.vertex_count(),
        series::map(
            field::elias_noise<double>(
                series::unit_vector_noise<3>(10.0, 1.0e4), 
                series::gaussian(11.0, 1.1e4), 
                1000),
            dymaxion::vertex_positions(grid)
        )
    ),
    known::store(
        grid.vertex_count(),
        series::map(
            field::elias_noise<double>(
                series::unit_vector_noise<3>(11.0, 1.1e4), 
                series::gaussian(11.0, 1.1e4), 
                1000),
            dymaxion::vertex_positions(grid)
        )
    ),
    known::store(
        grid.vertex_count(),
        series::map(
            field::elias_noise<double>(
                series::unit_vector_noise<3>(12.0, 1.2e4), 
                series::gaussian(11.0, 1.1e4), 
                1000),
            dymaxion::vertex_positions(grid)
        )
    )
};


std::vector scalar_rasters{
    series::map(
        field::value_noise3(
            field::square_noise(
                series::unit_interval_noise(11.0, 1.1e4))),
        dymaxion::vertex_positions(grid)
    ),
    series::map(
        field::value_noise3(
            field::square_noise(
                series::unit_interval_noise(12.0, 1.2e4))),
        dymaxion::vertex_positions(grid)
    ),
    series::map(
        field::value_noise3(
            field::square_noise(
                series::unit_interval_noise(13.0, 1.3e4))),
        dymaxion::vertex_positions(grid)
    )
};

std::vector vector_rasters{
    known::store(
        grid.vertex_count(),
        series::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(10.0, 1.0e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(11.0, 1.1e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(12.0, 1.2e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(grid)
        )
    ),
    known::store(
        grid.vertex_count(),
        series::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(13.0, 1.3e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(14.0, 1.4e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(15.0, 1.5e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(grid)
        )
    ),
    known::store(
        grid.vertex_count(),
        series::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(16.0, 1.6e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(17.0, 1.7e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        series::unit_vector_noise<3>(18.0, 1.8e4), 
                        series::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(grid)
        )
    ),
};


dymaxion::Adapter adapter(grid, 1e-5, grid.vertex_count());


TEST_CASE( "Raster gradient", "[dymaxion]" ) {

    // REQUIRE(test::determinism(adapter, 
    //     "dymaxion::gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::gradient),
    //     scalar_rasters
    // ));

    // REQUIRE(test::additivity(adapter, 
    //     "dymaxion::gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::gradient),
    //     "each::add          ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     grid, each::add),
    //     "each::add          ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     scalar_rasters, scalar_rasters
    // ));

}



TEST_CASE( "Raster divergence", "[dymaxion]" ) {


    // REQUIRE(test::determinism(adapter, 
    //     "dymaxion::divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, dymaxion::divergence),
    //     vector_rasters
    // ));

    // REQUIRE(test::additivity(adapter, 
    //     "dymaxion::divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, dymaxion::divergence),
    //     "each::add          ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     "each::add          ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     grid, each::add),
    //     vector_rasters, vector_rasters
    // ));

}



TEST_CASE( "Raster curl", "[dymaxion]" ) {


    // REQUIRE(test::determinism(adapter, 
    //     "dymaxion::curl",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::curl),
    //     vector_rasters
    // ));

    // REQUIRE(test::additivity(adapter, 
    //     "dymaxion::curl",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::curl),
    //     "each::add      ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     "each::add      ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     vector_rasters, vector_rasters
    // ));

}

TEST_CASE( "Scalar Raster laplacian", "[dymaxion]" ) {
    procedural::VectorCalculusByFundamentalTheorem operators;
    for (int i = 0; i < 1; ++i)
    {
        std::cout << adapter.print(scalar_rasters[i]);
        std::cout << "arrow_dual_length0" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_dual_length0)(scalar_rasters[i]));
        std::cout << "arrow_dual_length1" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_dual_length1)(scalar_rasters[i]));
        std::cout << "arrow_dual_length2" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_dual_length2)(scalar_rasters[i]));
        std::cout << "arrow_dual_length3" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_dual_length3)(scalar_rasters[i]));
        // std::cout << "arrow_length0" << std::endl;
        // std::cout << adapter.print(
        //     DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_length0)(scalar_rasters[i]));
        // std::cout << "arrow_length1" << std::endl;
        // std::cout << adapter.print(
        //     DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_length1)(scalar_rasters[i]));
        // std::cout << "arrow_length2" << std::endl;
        // std::cout << adapter.print(
        //     DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_length2)(scalar_rasters[i]));
        // std::cout << "arrow_length3" << std::endl;
        // std::cout << adapter.print(
        //     DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.arrow_length3)(scalar_rasters[i]));
        std::cout << "differential0" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.differential0)(scalar_rasters[i]));
        std::cout << "differential1" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.differential1)(scalar_rasters[i]));
        std::cout << "differential2" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.differential2)(scalar_rasters[i]));
        std::cout << "differential3" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.differential3)(scalar_rasters[i]));
        std::cout << "arrow_normal0" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.arrow_normal0)(scalar_rasters[i]));
        std::cout << "arrow_normal1" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.arrow_normal1)(scalar_rasters[i]));
        std::cout << "arrow_normal2" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.arrow_normal2)(scalar_rasters[i]));
        std::cout << "arrow_normal3" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.arrow_normal3)(scalar_rasters[i]));
        std::cout << "vertex_dual_area" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.vertex_dual_area)(scalar_rasters[i]));
        std::cout << "square_id" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.square_id)(scalar_rasters[i]));
        std::cout << "∇⋅∇:" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence)
                (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient)(scalar_rasters[i])));
        std::cout << "∇²:" << std::endl;
        std::cout << adapter.print(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.laplacian)(scalar_rasters[i]));
        std::cout << "error:" << std::endl;
        std::cout << adapter.print(DYMAXION_TEST_BINARY_OUT_PARAMETER(double,grid,each::distance)(
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence)
                (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient)(scalar_rasters[i])),
            DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, grid, operators.laplacian)(scalar_rasters[i])));
    }

    // REQUIRE(test::determinism(adapter, 
    //     "dymaxion::laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::laplacian),
    //     scalar_rasters
    // ));

    // REQUIRE(test::additivity(adapter, 
    //     "dymaxion::laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, grid, dymaxion::laplacian),
    //     "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, grid, each::add),
    //     "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, grid, each::add),
    //     scalar_rasters, scalar_rasters
    // ));

    REQUIRE(test::composition(adapter, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        "operators.gradient",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),  
        "operators.laplacian",  DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.laplacian), 
        scalar_rasters
    ));

}

TEST_CASE( "Vector Raster laplacian", "[dymaxion]" ) {


    // REQUIRE(test::determinism(adapter, 
    //     "dymaxion::laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::laplacian),
    //     vector_rasters
    // ));

    // REQUIRE(test::additivity(adapter, 
    //     "dymaxion::laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::laplacian),
    //     "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
    //     vector_rasters, vector_rasters
    // ));

    // REQUIRE(test::composition(adapter, 
    //     "dymaxion::gradient",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::gradient),  
    //     "dymaxion::divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, dymaxion::divergence),
    //     "dymaxion::laplacian",  DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, dymaxion::laplacian), 
    //     vector_rasters
    // ));

}



// TEST_CASE( "Raster gradient multiplication relation", "[rasters]" ) {

//     float radius(2.0f);
//     int vertex_count_per_half_meridian(10);
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
//         dymaxion::gradient(grid, ab, grad_ab);
//         dymaxion::gradient(grid, a, grad_a);
//         dymaxion::gradient(grid, b, grad_b);
//         each::mult(a, grad_b, a_grad_b);
//         each::mult(b, grad_a, b_grad_a);
//         each::add(a_grad_b, b_grad_a, a_grad_b_b_grad_a);
//         CHECK(whole::equal( grad_ab,  a_grad_b_b_grad_a, 0.1f ));
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
//     auto A_ids       = raster<unsigned int>(icosphere2.vertex_count);
//     auto Ai_ids      = raster<unsigned int>(icosphere1.vertex_count);
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



// TEST_CASE( "divergence resolution invariance", "[rasters]" ) {
//     meshes::mesh icosphere_mesh1(meshes::icosahedron.vertices, meshes::icosahedron.faces);
//     icosphere_mesh1 = meshes::subdivide(icosphere_mesh1); each::normalize(icosphere_mesh1.vertices, icosphere_mesh1.vertices);
//     MeshCache icosphere1(icosphere_mesh1.vertices, icosphere_mesh1.faces);

//     meshes::mesh icosphere_mesh2(icosphere_mesh1);
//     icosphere_mesh2 = meshes::subdivide(icosphere_mesh2); each::normalize(icosphere_mesh2.vertices, icosphere_mesh2.vertices);
//     MeshCache icosphere2(icosphere_mesh2.vertices, icosphere_mesh2.faces);

//     auto scalar     = raster<float>(icosphere1.vertex_count);
//     auto a          = raster<glm::vec3>(icosphere1.vertex_count);
//     auto A_ids      = raster<unsigned int>(icosphere2.vertex_count);
//     auto Ai_ids     = raster<unsigned int>(icosphere1.vertex_count);
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

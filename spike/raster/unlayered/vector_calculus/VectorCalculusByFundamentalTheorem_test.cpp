
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
#include <math/glm/special.hpp>

#include <index/glm/each_specialization.hpp>
#include <index/glm/whole_specialization.hpp>
#include <index/glm/each.hpp>
#include <index/each.hpp>  
#include <index/whole.hpp>  
#include <index/known.hpp>  
#include <index/series/Get.hpp>
#include <index/series/Map.hpp>
#include <index/series/Range.hpp>
#include <index/series/Uniform.hpp>
#include <index/series/glm/VectorInterleave.hpp>
#include <index/series/noise/GaussianNoise.hpp>
#include <index/series/noise/glm/UnitVectorNoise.hpp>

#include <field/noise/EliasNoise.hpp>
#include <field/noise/ValueNoise.hpp>
#include <field/noise/SquareNoise.hpp>
#include <field/VectorZip.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/series.hpp>

#include <raster/spheroidal/string_cast.hpp>

#include "VectorCalculusByFundamentalTheorem.hpp"
#include "VectorCalculusByFundamentalTheorem_test.hpp"

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
            return whole::max_distance(a,b) <= threshold;
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


dymaxion::Grid grid(0.1, 30);

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


dymaxion::Adapter strict(grid, 1e-5, grid.vertex_count());


TEST_CASE( "Raster gradient", "[raster]" ) {
    unlayered::VectorCalculusByFundamentalTheorem operators;

    REQUIRE(test::determinism(strict, 
        "operators.gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),
        scalar_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.gradient ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),
        "each::add          ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     grid, each::add),
        "each::add          ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        scalar_rasters, scalar_rasters
    ));

    // results here are promising
    REQUIRE(test::equality(dymaxion::Adapter(grid, 0.3, grid.vertex_count()), 
        "The gradient of a scalar must statsify a well known relationship",
        "∇(ab)     ", 
        [=](auto a, auto b){
            std::vector<double> ab(grid.vertex_count());
            std::vector<glm::dvec3> grad_ab(grid.vertex_count());
            each::mult(a, b, ab);
            operators.gradient(grid,grad_ab,grad_ab); 
            return grad_ab;
        },
        "a∇b + b∇a ", 
        [=](auto a, auto b){
            std::vector<glm::dvec3> b_grad_a(grid.vertex_count());
            std::vector<glm::dvec3> a_grad_b(grid.vertex_count());
            std::vector<glm::dvec3> a_grad_b_b_grad_a(grid.vertex_count());
            operators.gradient(grid, a, b_grad_a);
            operators.gradient(grid, b, a_grad_b);
            each::mult(a, a_grad_b, a_grad_b);
            each::mult(b, b_grad_a, b_grad_a);
            each::add(a_grad_b, b_grad_a, a_grad_b_b_grad_a);
            return a_grad_b_b_grad_a;
        },
        scalar_rasters,
        scalar_rasters
    ));

}



TEST_CASE( "Raster divergence", "[raster]" ) {
    unlayered::VectorCalculusByFundamentalTheorem operators;

    REQUIRE(test::determinism(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     grid, each::add),
        vector_rasters, vector_rasters
    ));

    // results are horrible, the test here is only meant to track known error until our methods improve
    REQUIRE(test::equality(dymaxion::Adapter(grid, 1e4, grid.vertex_count()), 
        "The divergence of a vector must statsify a well known relationship",
        "∇⋅(aV)          ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> aV(grid.vertex_count());
            std::vector<double> div_aV(grid.vertex_count());
            each::add(a, V, aV);
            operators.divergence(grid, aV, div_aV); 
            return div_aV;
        },
        "a(∇⋅V) + (∇a)⋅V ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> grad_a(grid.vertex_count());
            std::vector<double> div_V (grid.vertex_count());
            std::vector<double> a_div_V(grid.vertex_count());
            std::vector<double> V_grad_a(grid.vertex_count());
            std::vector<double> a_div_V_V_grad_a(grid.vertex_count());
            operators.gradient(grid, a, grad_a);
            operators.divergence(grid, V, div_V);
            each::mult(a, div_V, a_div_V);
            each::dot(V, grad_a, V_grad_a);
            each::add(a_div_V, V_grad_a, a_div_V_V_grad_a);
            return a_div_V_V_grad_a;
        },
        scalar_rasters,
        vector_rasters
    ));


}


TEST_CASE( "Raster curl", "[raster]" ) {
    unlayered::VectorCalculusByFundamentalTheorem operators;

    REQUIRE(test::determinism(strict, 
        "operators.curl",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.curl),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.curl ",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.curl),
        "each::add      ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        "each::add      ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        vector_rasters, vector_rasters
    ));

    // results are horrible, the test here is only meant to track known error until our methods improve
    REQUIRE(test::equality(dymaxion::Adapter(grid, 3e3, grid.vertex_count()), 
        "The curl of a vector must statsify a well known relationship",
        "∇⋅(aV)          ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> aV(grid.vertex_count());
            std::vector<glm::dvec3> curl_aV(grid.vertex_count());
            each::add(a, V, aV);
            operators.curl(grid, aV, curl_aV); 
            return curl_aV;
        },
        "a(∇⋅V) + (∇a)⋅V ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> grad_a(grid.vertex_count());
            std::vector<glm::dvec3> curl_V (grid.vertex_count());
            std::vector<glm::dvec3> a_curl_V(grid.vertex_count());
            std::vector<glm::dvec3> V_grad_a(grid.vertex_count());
            std::vector<glm::dvec3> a_curl_V_V_grad_a(grid.vertex_count());
            operators.gradient(grid, a, grad_a);
            operators.curl(grid, V, curl_V);
            each::mult(a, curl_V, a_curl_V);
            each::cross(V, grad_a, V_grad_a);
            each::add(a_curl_V, V_grad_a, a_curl_V_V_grad_a);
            return a_curl_V_V_grad_a;
        },
        scalar_rasters,
        vector_rasters
    ));

    // results here are promising
    REQUIRE(test::composition(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        "operators.curl      ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.curl),  
        "0                   ", [](auto a){ return series::uniform(0.0); }, 
        vector_rasters
    ));

    // results are horrible, the test here is only meant to track known error until our methods improve
    REQUIRE(test::composition(dymaxion::Adapter(grid, 1e5, grid.vertex_count()), 
        "operators.curl    ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.curl),
        "operators.gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),
        "the zero vector   ", [](auto a){ return series::uniform(glm::dvec3(0.0)); }, 
        vector_rasters
    ));

}

TEST_CASE( "Scalar Raster laplacian", "[raster]" ) {
    unlayered::VectorCalculusByFundamentalTheorem operators;
    unlayered::VectorCalculusByFundamentalTheoremDebug debug;
    bool is_verbose1(false);
    bool is_verbose2(false);
    for (int i = 0; i < 1; ++i)
    {
        if (is_verbose2)
        {
            std::cout << strict.print(scalar_rasters[i]);
            std::cout << "arrow_dual_length0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_dual_length0)(scalar_rasters[i]));
            std::cout << "arrow_dual_length1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_dual_length1)(scalar_rasters[i]));
            std::cout << "arrow_dual_length2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_dual_length2)(scalar_rasters[i]));
            std::cout << "arrow_dual_length3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_dual_length3)(scalar_rasters[i]));
            std::cout << "arrow_length0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_length0)(scalar_rasters[i]));
            std::cout << "arrow_length1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_length1)(scalar_rasters[i]));
            std::cout << "arrow_length2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_length2)(scalar_rasters[i]));
            std::cout << "arrow_length3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.arrow_length3)(scalar_rasters[i]));
            std::cout << "differential0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.differential0)(scalar_rasters[i]));
            std::cout << "differential1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.differential1)(scalar_rasters[i]));
            std::cout << "differential2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.differential2)(scalar_rasters[i]));
            std::cout << "differential3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.differential3)(scalar_rasters[i]));
            std::cout << "arrow_normal0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, debug.arrow_normal0)(scalar_rasters[i]));
            std::cout << "arrow_normal1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, debug.arrow_normal1)(scalar_rasters[i]));
            std::cout << "arrow_normal2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, debug.arrow_normal2)(scalar_rasters[i]));
            std::cout << "arrow_normal3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, debug.arrow_normal3)(scalar_rasters[i]));
            std::cout << "vertex_dual_area" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.vertex_dual_area)(scalar_rasters[i]));
            std::cout << "square_id" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, debug.square_id)(scalar_rasters[i]));
            std::cout << "∇⋅∇:" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence)
                    (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient)(scalar_rasters[i])));
            std::cout << "∇²:" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.laplacian)(scalar_rasters[i]));
        }

        if (is_verbose1)
        {
            std::cout << "error:" << std::endl;
            std::cout << strict.print(DYMAXION_TEST_BINARY_OUT_PARAMETER(double,grid,each::distance)(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence)
                    (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient)(scalar_rasters[i])),
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, grid, operators.laplacian)(scalar_rasters[i])));
        }

    }

    REQUIRE(test::determinism(strict, 
        "operators.laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, grid, operators.laplacian),
        scalar_rasters
    ));

    REQUIRE(test::additivity(strict,
        "operators.laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, grid, operators.laplacian),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, grid, each::add),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, grid, each::add),
        scalar_rasters, scalar_rasters
    ));

    // results are bad, the test here is only meant to track known error until our methods improve
    REQUIRE(test::composition(dymaxion::Adapter(grid, 100.0, grid.vertex_count()), 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        "operators.gradient",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),  
        "operators.laplacian",  DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.laplacian), 
        scalar_rasters
    ));

}

TEST_CASE( "Vector Raster laplacian", "[raster]" ) {

    unlayered::VectorCalculusByFundamentalTheorem operators;

    REQUIRE(test::determinism(strict, 
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.laplacian),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.laplacian),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        "each::add           ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, grid, each::add),
        vector_rasters, vector_rasters
    ));

    // results here are horrible, the test here is only meant to track known error until our methods improve
    REQUIRE(test::composition(dymaxion::Adapter(grid, 5e5, grid.vertex_count()), 
        "operators.gradient  ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.gradient),  
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     grid, operators.divergence),
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, grid, operators.laplacian), 
        vector_rasters
    ));

}


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
//     auto grad_A_a    = raster<glm::dvec3>(icosphere2.vertex_count);
//     auto Ai_grad_A_a = raster<glm::dvec3>(icosphere1.vertex_count);
//     auto grad_a      = raster<glm::dvec3>(icosphere1.vertex_count);

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
//     auto a          = raster<glm::dvec3>(icosphere1.vertex_count);
//     auto A_ids      = raster<unsigned int>(icosphere2.vertex_count);
//     auto Ai_ids     = raster<unsigned int>(icosphere1.vertex_count);
//     auto A_a        = raster<glm::dvec3>(icosphere2.vertex_count);
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


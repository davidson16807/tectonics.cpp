
// std libraries
#include <sstream>  //std::stringstream
#include <iostream> //std::cout
#include <vector>   //std::vector

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3
#include "glm/glm.hpp"

// in-house libraries
#include <math/glm/special_specialization.hpp>
#include <math/glm/special.hpp>

#include <index/glm/whole_specialization.hpp>
#include <index/whole.hpp>  
#include <index/known.hpp>  
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>

#include <index/iterated/Metric.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Geometric.hpp>

#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/glm/GlmGeometric.hpp>

#include <field/noise/EliasNoise.hpp>
#include <field/noise/ValueNoise.hpp>
#include <field/noise/MosaicNoise.hpp>
#include <field/noise/MosaicOps.hpp>
#include <field/VectorZip.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/GridSeries.hpp>

#include "VectorCalculusByFundamentalTheorem.hpp"
#include "VectorCalculusByFundamentalTheorem_test.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

#include <grid/dymaxion/test/Adapter.hpp>

// metric, geometric, arithmetic, vector

#define DYMAXION_TEST_GRIDDED_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(GRID,x,out)); return out; }

#define DYMAXION_TEST_UNARY_OUT_PARAMETER(TYPE,GRID,F)   \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,out)); return out; }
#define DYMAXION_TEST_BINARY_OUT_PARAMETER(TYPE,GRID,F)  \
    [=](auto x, auto y){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,out)); return out; }
#define DYMAXION_TEST_TRINARY_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x, auto y, auto z){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,z,out)); return out; }

dymaxion::Grid<int,int,double> calculus_fine  (2.0, 64);
dymaxion::Grid<int,int,double> calculus_coarse(2.0, 16);

// std::vector elias_scalar_rasters{
//     known::store(
//         calculus_fine.vertex_count(),
//         procedural::map(
//             field::elias_noise<double>(
//                 procedural::unit_vector_noise<3>(10.0, 1.0e4), 
//                 procedural::gaussian(11.0, 1.1e4), 
//                 1000),
//             dymaxion::vertex_positions(calculus_fine)
//         )
//     ),
//     known::store(
//         calculus_fine.vertex_count(),
//         procedural::map(
//             field::elias_noise<double>(
//                 procedural::unit_vector_noise<3>(11.0, 1.1e4), 
//                 procedural::gaussian(11.0, 1.1e4), 
//                 1000),
//             dymaxion::vertex_positions(calculus_fine)
//         )
//     ),
//     known::store(
//         calculus_fine.vertex_count(),
//         procedural::map(
//             field::elias_noise<double>(
//                 procedural::unit_vector_noise<3>(12.0, 1.2e4), 
//                 procedural::gaussian(11.0, 1.1e4), 
//                 1000),
//             dymaxion::vertex_positions(calculus_fine)
//         )
//     )
// };


std::vector scalar_rasters{
    procedural::map(
        field::value_noise<3,double>(
            field::mosaic_noise(
                procedural::unit_interval_noise(11.0, 1.1e4)),
            field::vector_mosaic_ops<3,int,double>()
        ),
        dymaxion::vertex_positions(calculus_fine)
    ),
    procedural::map(
        field::value_noise<3,double>(
            field::mosaic_noise(
                procedural::unit_interval_noise(12.0, 1.2e4)),
            field::vector_mosaic_ops<3,int,double>()
        ),
        dymaxion::vertex_positions(calculus_fine)
    ),
    procedural::map(
        field::value_noise<3,double>(
            field::mosaic_noise(
                procedural::unit_interval_noise(13.0, 1.3e4)),
            field::vector_mosaic_ops<3,int,double>()
        ),
        dymaxion::vertex_positions(calculus_fine)
    )
};

std::vector vector_rasters{
    known::store(
        calculus_fine.vertex_count(),
        procedural::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(10.0, 1.0e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(11.0, 1.1e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(12.0, 1.2e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(calculus_fine)
        )
    ),
    known::store(
        calculus_fine.vertex_count(),
        procedural::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(13.0, 1.3e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(14.0, 1.4e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(15.0, 1.5e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(calculus_fine)
        )
    ),
    known::store(
        calculus_fine.vertex_count(),
        procedural::map(
            field::vector3_zip(
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(16.0, 1.6e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(17.0, 1.7e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000),
                field::elias_noise<double>(
                        procedural::unit_vector_noise<3>(18.0, 1.8e4), 
                        procedural::gaussian(11.0, 1.1e4), 
                        1000)
            ),
            dymaxion::vertex_positions(calculus_fine)
        )
    ),
};



TEST_CASE( "Raster gradient", "[unlayered]" ) {
    dymaxion::Adapter strict(calculus_fine, 1e-5, calculus_fine.vertex_count());
    dymaxion::VertexNormals vertex_normals(calculus_fine);
    unlayered::VectorCalculusByFundamentalTheorem operators;
    iterated::Metric metric{adapted::GlmMetric{}};
    iterated::Geometric geometric{adapted::GlmGeometric{}};
    iterated::Arithmetic arithmetic{adapted::TypedSymbolicArithmetic{0.0, 1.0}};

    REQUIRE(test::determinism(strict, 
        "operators.gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),
        scalar_rasters
    ));

    REQUIRE(test::composition(dymaxion::Adapter(calculus_fine, 0.1, calculus_fine.vertex_count()), 
        "dot with surface normal ", [=](auto gradient){ 
            std::vector<double> out(calculus_fine.vertex_count());
            geometric.dot(gradient, vertex_normals, out);
            return out;
        },
        "operators.gradient      ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),
        "the zero vector         ", [](auto a){ return procedural::uniform(0.0); }, 
        scalar_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.gradient ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     calculus_fine, arithmetic.add),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        scalar_rasters, scalar_rasters
    ));

    // results here are promising
    REQUIRE(test::equality(dymaxion::Adapter(calculus_fine, 10.0, calculus_fine.vertex_count()), 
        "The gradient of a scalar must statsify a well known relationship",
        "∇(ab)     ", 
        [=](auto a, auto b){
            std::vector<double> ab(calculus_fine.vertex_count());
            std::vector<glm::dvec3> grad_ab(calculus_fine.vertex_count());
            arithmetic.multiply(a, b, ab);
            operators.gradient(calculus_fine,grad_ab,grad_ab); 
            return grad_ab;
        },
        "a∇b + b∇a ", 
        [=](auto a, auto b){
            std::vector<glm::dvec3> b_grad_a(calculus_fine.vertex_count());
            std::vector<glm::dvec3> a_grad_b(calculus_fine.vertex_count());
            std::vector<glm::dvec3> a_grad_b_b_grad_a(calculus_fine.vertex_count());
            operators.gradient(calculus_fine, a, b_grad_a);
            operators.gradient(calculus_fine, b, a_grad_b);
            arithmetic.multiply(a, a_grad_b, a_grad_b);
            arithmetic.multiply(b, b_grad_a, b_grad_a);
            arithmetic.add(a_grad_b, b_grad_a, a_grad_b_b_grad_a);
            return a_grad_b_b_grad_a;
        },
        scalar_rasters,
        scalar_rasters
    ));

}



TEST_CASE( "Raster divergence", "[unlayered]" ) {
    dymaxion::Adapter strict(calculus_fine, 1e-5, calculus_fine.vertex_count());
    unlayered::VectorCalculusByFundamentalTheorem operators;
    iterated::Metric metric{adapted::GlmMetric{}};
    iterated::Geometric geometric{adapted::GlmGeometric{}};
    iterated::Arithmetic arithmetic{adapted::TypedSymbolicArithmetic{0.0, 1.0}};

    REQUIRE(test::determinism(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence),
        "arithmetic.add      ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        "arithmetic.add      ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double,     calculus_fine, arithmetic.add),
        vector_rasters, vector_rasters
    ));

    // results are bad but have gotten better, the test here is only meant to track known error until our methods improve
    REQUIRE(test::equality(dymaxion::Adapter(calculus_fine, 100.0, calculus_fine.vertex_count()), 
        "The divergence of a vector must statsify a well known relationship",
        "∇⋅(aV)          ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> aV(calculus_fine.vertex_count());
            std::vector<double> div_aV(calculus_fine.vertex_count());
            arithmetic.add(a, V, aV);
            operators.divergence(calculus_fine, aV, div_aV); 
            return div_aV;
        },
        "a(∇⋅V) + (∇a)⋅V ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> grad_a(calculus_fine.vertex_count());
            std::vector<double> div_V (calculus_fine.vertex_count());
            std::vector<double> a_div_V(calculus_fine.vertex_count());
            std::vector<double> V_grad_a(calculus_fine.vertex_count());
            std::vector<double> a_div_V_V_grad_a(calculus_fine.vertex_count());
            operators.gradient(calculus_fine, a, grad_a);
            operators.divergence(calculus_fine, V, div_V);
            arithmetic.multiply(a, div_V, a_div_V);
            geometric.dot(V, grad_a, V_grad_a);
            arithmetic.add(a_div_V, V_grad_a, a_div_V_V_grad_a);
            return a_div_V_V_grad_a;
        },
        scalar_rasters,
        vector_rasters
    ));

}


TEST_CASE( "Raster curl", "[unlayered]" ) {
    dymaxion::Adapter strict(calculus_fine, 1e-5, calculus_fine.vertex_count());
    unlayered::VectorCalculusByFundamentalTheorem operators;
    iterated::Metric metric{adapted::GlmMetric{}};
    iterated::Geometric geometric{adapted::GlmGeometric{}};
    iterated::Arithmetic arithmetic{adapted::TypedSymbolicArithmetic{0.0, 1.0}};

    REQUIRE(test::determinism(strict, 
        "operators.curl",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.curl),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.curl ",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.curl),
        "arithmetic.add ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        "arithmetic.add ",   DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        vector_rasters, vector_rasters
    ));

    // results are bad but getting better, the test here is only meant to track known error until our methods improve
    REQUIRE(test::equality(dymaxion::Adapter(calculus_fine, 300.0, calculus_fine.vertex_count()), 
        "The curl of a vector must statsify a well known relationship",
        "∇⋅(aV)          ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> aV(calculus_fine.vertex_count());
            std::vector<glm::dvec3> curl_aV(calculus_fine.vertex_count());
            arithmetic.add(a, V, aV);
            operators.curl(calculus_fine, aV, curl_aV); 
            return curl_aV;
        },
        "a(∇⋅V) + (∇a)⋅V ", 
        [=](auto a, auto V){
            std::vector<glm::dvec3> grad_a(calculus_fine.vertex_count());
            std::vector<glm::dvec3> curl_V (calculus_fine.vertex_count());
            std::vector<glm::dvec3> a_curl_V(calculus_fine.vertex_count());
            std::vector<glm::dvec3> V_grad_a(calculus_fine.vertex_count());
            std::vector<glm::dvec3> a_curl_V_V_grad_a(calculus_fine.vertex_count());
            operators.gradient(calculus_fine, a, grad_a);
            operators.curl(calculus_fine, V, curl_V);
            arithmetic.multiply(a, curl_V, a_curl_V);
            geometric.cross(V, grad_a, V_grad_a);
            arithmetic.add(a_curl_V, V_grad_a, a_curl_V_V_grad_a);
            return a_curl_V_V_grad_a;
        },
        scalar_rasters,
        vector_rasters
    ));

    // results here are promising
    REQUIRE(test::composition(strict, 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence),
        "operators.curl",       DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.curl),  
        "0",                    [](auto a){ return procedural::uniform(0.0); }, 
        vector_rasters
    ));

    // results are promising
    REQUIRE(test::composition(dymaxion::Adapter(calculus_fine, 300.0, calculus_fine.vertex_count()), 
        "operators.curl    ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.curl),
        "operators.gradient", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),
        "the zero vector   ", [](auto a){ return procedural::uniform(glm::dvec3(0.0)); }, 
        vector_rasters
    ));

}


TEST_CASE( "Scalar Raster laplacian", "[unlayered]" ) {
    dymaxion::Adapter strict(calculus_fine, 1e-5, calculus_fine.vertex_count());
    unlayered::VectorCalculusByFundamentalTheorem operators;
    unlayered::VectorCalculusByFundamentalTheoremDebug debug;
    iterated::Metric metric{adapted::GlmMetric{}};
    iterated::Geometric geometric{adapted::GlmGeometric{}};
    iterated::Arithmetic arithmetic{adapted::TypedSymbolicArithmetic{0.0, 1.0}};
    bool is_verbose1(false);
    bool is_verbose2(false);
    for (int i = 0; i < 1; ++i)
    {
        if (is_verbose2)
        {
            std::cout << strict.print(scalar_rasters[i]);
            std::cout << "arrow_dual_length0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_dual_length0)(scalar_rasters[i]));
            std::cout << "arrow_dual_length1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_dual_length1)(scalar_rasters[i]));
            std::cout << "arrow_dual_length2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_dual_length2)(scalar_rasters[i]));
            std::cout << "arrow_dual_length3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_dual_length3)(scalar_rasters[i]));
            std::cout << "arrow_length0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_length0)(scalar_rasters[i]));
            std::cout << "arrow_length1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_length1)(scalar_rasters[i]));
            std::cout << "arrow_length2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_length2)(scalar_rasters[i]));
            std::cout << "arrow_length3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.arrow_length3)(scalar_rasters[i]));
            std::cout << "differential0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.differential0)(scalar_rasters[i]));
            std::cout << "differential1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.differential1)(scalar_rasters[i]));
            std::cout << "differential2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.differential2)(scalar_rasters[i]));
            std::cout << "differential3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.differential3)(scalar_rasters[i]));
            std::cout << "arrow_normal0" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, debug.arrow_normal0)(scalar_rasters[i]));
            std::cout << "arrow_normal1" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, debug.arrow_normal1)(scalar_rasters[i]));
            std::cout << "arrow_normal2" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, debug.arrow_normal2)(scalar_rasters[i]));
            std::cout << "arrow_normal3" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, debug.arrow_normal3)(scalar_rasters[i]));
            std::cout << "vertex_dual_area" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.vertex_dual_area)(scalar_rasters[i]));
            std::cout << "square_id" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, debug.square_id)(scalar_rasters[i]));
            std::cout << "∇⋅∇:" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence)
                    (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient)(scalar_rasters[i])));
            std::cout << "∇²:" << std::endl;
            std::cout << strict.print(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.laplacian)(scalar_rasters[i]));
        }

        if (is_verbose1)
        {
            std::cout << "error:" << std::endl;
            std::cout << strict.print(DYMAXION_TEST_BINARY_OUT_PARAMETER(double,calculus_fine,metric.distance)(
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence)
                    (DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient)(scalar_rasters[i])),
                DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, calculus_fine, operators.laplacian)(scalar_rasters[i])));
        }

    }

    REQUIRE(test::determinism(strict, 
        "operators.laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, calculus_fine, operators.laplacian),
        scalar_rasters
    ));

    REQUIRE(test::additivity(strict,
        "operators.laplacian", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double, calculus_fine, operators.laplacian),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, calculus_fine, arithmetic.add),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (double, calculus_fine, arithmetic.add),
        scalar_rasters, scalar_rasters
    ));

    // results here are fairly good
    REQUIRE(test::composition(dymaxion::Adapter(calculus_fine, 100.0, calculus_fine.vertex_count()), 
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence),
        "operators.gradient",   DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),  
        "operators.laplacian",  DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.laplacian), 
        scalar_rasters
    ));

}

TEST_CASE( "Vector Raster laplacian", "[unlayered]" ) {
    dymaxion::Adapter strict(calculus_fine, 1e-5, calculus_fine.vertex_count());
    unlayered::VectorCalculusByFundamentalTheorem operators;
    iterated::Metric metric{adapted::GlmMetric{}};
    iterated::Geometric geometric{adapted::GlmGeometric{}};
    iterated::Arithmetic arithmetic{adapted::TypedSymbolicArithmetic{0.0, 1.0}};

    REQUIRE(test::determinism(strict, 
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.laplacian),
        vector_rasters
    ));

    REQUIRE(test::additivity(strict, 
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.laplacian),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        "arithmetic.add     ", DYMAXION_TEST_BINARY_OUT_PARAMETER (glm::dvec3, calculus_fine, arithmetic.add),
        vector_rasters, vector_rasters
    ));

    // results are fairly poor but have gotten better, the test here is only meant to track known error until our methods improve
    REQUIRE(test::composition(dymaxion::Adapter(calculus_fine, 1000.0, calculus_fine.vertex_count()), 
        "operators.gradient  ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.gradient),  
        "operators.divergence", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(double,     calculus_fine, operators.divergence),
        "operators.laplacian ", DYMAXION_TEST_GRIDDED_OUT_PARAMETER(glm::dvec3, calculus_fine, operators.laplacian), 
        vector_rasters
    ));

}


/*
TODO:
* resolution invariance
* translation invariance
* rotation invariance

example:

TEST_CASE( "gradient resolution invariance", "[rasters]" ) {
    // gradient(a) must generate the same output as unshift(gradient(shift(a)))
    operators.gradient(calculus_fine, 
        procedural::get(a, 
            procedural::map(dymaxion::nearest_vertex_id(calculus_fine), dymaxion::vertex_positions(calculus_coarse))),
        grad_fine_a
    );
}
*/


#undef DYMAXION_TEST_GRIDDED_OUT_PARAMETER
#undef DYMAXION_TEST_UNARY_OUT_PARAMETER
#undef DYMAXION_TEST_BINARY_OUT_PARAMETER
#undef DYMAXION_TEST_TRINARY_OUT_PARAMETER
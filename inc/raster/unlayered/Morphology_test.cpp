
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
#include <math/glm/special_specialization.hpp>
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
#include <field/noise/MosaicNoise.hpp>
#include <field/noise/MosaicOps.hpp>
#include <field/VectorZip.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/series.hpp>

#include <raster/spheroidal/string_cast.hpp>

#include "Morphology.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

#include <grid/dymaxion/test/Adapter.hpp>

#define MORPHOLOGY_TEST_BINARY_NONGRID(TYPE,GRID,F) \
    [=](auto x, auto y){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        (F(x,y,out)); return out; }

#define MORPHOLOGY_TEST_UNARY(TYPE,GRID,F) \
    [=](auto x){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        (F(GRID,x,out)); return out; }

#define MORPHOLOGY_TEST_UNARY_1_SCRATCH(TYPE,GRID,F)   \
    [=](auto x){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        std::vector<TYPE> scratch1(GRID.vertex_count()); \
        (F(GRID,x,out,scratch1)); return out; }

#define MORPHOLOGY_TEST_UNARY_2_SCRATCH(TYPE,GRID,F) \
    [=](auto x){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        std::vector<TYPE> scratch1(GRID.vertex_count()); \
        std::vector<TYPE> scratch2(GRID.vertex_count()); \
        (F(GRID,x,out,scratch1,scratch2)); return out; }

#define MORPHOLOGY_TEST_UNARY_RADIUS_1_SCRATCH(TYPE,GRID,R,F)   \
    [=](auto x){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        std::vector<TYPE> scratch1(GRID.vertex_count()); \
        (F(GRID,x,out,R,scratch1)); return out; }

#define MORPHOLOGY_TEST_UNARY_RADIUS_2_SCRATCH(TYPE,GRID,R,F) \
    [=](auto x){ \
        std::vector<TYPE> out(GRID.vertex_count()); \
        std::vector<TYPE> scratch1(GRID.vertex_count()); \
        std::vector<TYPE> scratch2(GRID.vertex_count()); \
        (F(GRID,x,out,R,scratch1,scratch2)); return out; }

dymaxion::Grid morphology_fine  (2.0, 32);
dymaxion::Grid morphology_coarse(2.0, 16);

std::vector boolean_rasters{
    known::greaterThan(series::uniform(0.5), 
        series::map(
            field::value_noise<3,double>(
                field::mosaic_noise(
                    series::unit_interval_noise(11.0, 1.1e4)),
                field::vector_mosaic_ops<3,int,double>()
            ),
            dymaxion::vertex_positions(morphology_fine)
        )
    ),
    known::greaterThan(series::uniform(0.5), 
        series::map(
            field::value_noise<3,double>(
                field::mosaic_noise(
                    series::unit_interval_noise(12.0, 1.2e4)),
                field::vector_mosaic_ops<3,int,double>()
            ),
            dymaxion::vertex_positions(morphology_fine)
        )
    ),
    known::greaterThan(series::uniform(0.5), 
        series::map(
            field::value_noise<3,double>(
                field::mosaic_noise(
                    series::unit_interval_noise(13.0, 1.3e4)),
                field::vector_mosaic_ops<3,int,double>()
            ),
            dymaxion::vertex_positions(morphology_fine)
        )
    )
};


TEST_CASE( "Boolean Raster erode", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.erode", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.erode),
        boolean_rasters
    ));

    REQUIRE(test::nonincreasing(strict, 
        "morphology.erode", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.erode),
        "the sum",           [](auto a){return whole::sum<int>(a);},
        boolean_rasters
    ));
    
    REQUIRE(test::unary_identity(strict, 
        "the \"empty\" raster", std::vector<bool>(morphology_fine.vertex_count(), false),
        "morphology.erode",     MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.erode)
    ));
    
    REQUIRE(test::unary_distributivity(strict, 
        "morphology.erode", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.erode),
        "intersect",        MORPHOLOGY_TEST_BINARY_NONGRID(bool, morphology_fine, each::intersect),
        boolean_rasters, boolean_rasters
    ));
    
}

TEST_CASE( "Boolean Raster dilate", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.dilate", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.dilate),
        boolean_rasters
    ));

    REQUIRE(test::nondecreasing(strict, 
        "morphology.dilate", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.dilate),
        "the sum",           [](auto a){return whole::sum<int>(a);},
        boolean_rasters
    ));

    REQUIRE(test::unary_identity(strict, 
        "the \"full\" raster", std::vector<bool>(morphology_fine.vertex_count(), true),
        "morphology.erode",    MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.erode)
    ));

    REQUIRE(test::unary_distributivity(strict, 
        "morphology.dilate", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.dilate),
        "unite",             MORPHOLOGY_TEST_BINARY_NONGRID(bool, morphology_fine, each::unite),
        boolean_rasters, boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster padding", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.padding", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.padding),
        boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster margin", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.margin", MORPHOLOGY_TEST_UNARY(bool, morphology_fine, morphology.margin),
        boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster opening", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.opening", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.opening),
        boolean_rasters
    ));

    REQUIRE(test::nonincreasing(strict, 
        "morphology.opening", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.opening),
        "the sum",           [](auto a){return whole::sum<int>(a);},
        boolean_rasters
    ));

    REQUIRE(test::unary_idempotence(strict, 
        "morphology.opening", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.opening),
        boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster closing", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.closing", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.closing),
        boolean_rasters
    ));

    REQUIRE(test::nondecreasing(strict, 
        "morphology.closing", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.closing),
        "the sum",           [](auto a){return whole::sum<int>(a);},
        boolean_rasters
    ));

    REQUIRE(test::unary_idempotence(strict, 
        "morphology.closing", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.closing),
        boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster black_top_hat", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.black_top_hat", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.black_top_hat),
        boolean_rasters
    ));

}

TEST_CASE( "Boolean Raster white_top_hat", "[unlayered]" ) {
    dymaxion::Adapter strict(morphology_fine, 1e-5, morphology_fine.vertex_count());
    unlayered::Morphology morphology;

    REQUIRE(test::determinism(strict, 
        "morphology.white_top_hat", MORPHOLOGY_TEST_UNARY_1_SCRATCH(bool, morphology_fine, morphology.white_top_hat),
        boolean_rasters
    ));

}

#undef MORPHOLOGY_TEST_UNARY
#undef MORPHOLOGY_TEST_UNARY_1_SCRATCH
#undef MORPHOLOGY_TEST_UNARY_2_SCRATCH
#undef MORPHOLOGY_TEST_UNARY_RADIUS_1_SCRATCH
#undef MORPHOLOGY_TEST_UNARY_RADIUS_2_SCRATCH

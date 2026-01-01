
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

#include <index/known.hpp>  
#include <index/procedural/Get.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/glm/VectorInterleave.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/adapted/boolean/BooleanStrings.hpp>
#include <index/adapted/symbolic/SymbolicMetric.hpp>
#include <index/aggregated/Order.hpp>

#include <field/noise/EliasNoise.hpp>
#include <field/noise/ValueNoise.hpp>
#include <field/noise/MosaicNoise.hpp>
#include <field/VectorZip.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/GridSeries.hpp>

#include "Morphology.hpp"

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>

#include <grid/dymaxion/test/Adapter.hpp>

#define MORPHOLOGY_TEST_GRIDDED_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(GRID,x,out)); return out; }

#define MORPHOLOGY_TEST_UNARY_OUT_PARAMETER(TYPE,GRID,F)   \
    [=](auto x){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,out)); return out; }
#define MORPHOLOGY_TEST_BINARY_OUT_PARAMETER(TYPE,GRID,F)  \
    [=](auto x, auto y){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,out)); return out; }
#define MORPHOLOGY_TEST_TRINARY_OUT_PARAMETER(TYPE,GRID,F) \
    [=](auto x, auto y, auto z){ std::vector<TYPE> out(GRID.vertex_count()); (F(x,y,z,out)); return out; }

dymaxion::Grid fine  (2.0, 32);
dymaxion::Grid coarse(2.0, 16);

auto mask = known::greaterThan(procedural::uniform(0.5), 
        procedural::map(
            field::value_noise<3,double>(
                field::mosaic_noise(
                    procedural::unit_interval_noise(11.0, 1.1e4))),
            dymaxion::vertex_positions(fine)
        )
    )

dymaxion::Adapter strict(fine, 1e-5, fine.vertex_count());

TEST_CASE( "Scalar Raster erode", "[unlayered]" ) {
    spheroidal::Strings strings{
        adapted::BooleanStrings{}, 
        aggregated::Order{
            adapted::SymbolicOrder{}
        }
    };

    unlayered::Morphology morphology;
    std::vector<bool> out(fine.vertex_count());
    std::vector<bool> scratch1(fine.vertex_count());
    std::vector<bool> scratch2(fine.vertex_count());

    std::cout << "original:" << std::endl;
    std::cout << strings.format(fine, mask) << std::endl;

    morphology.erode(fine, mask, out);
    std::cout << "erode:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.dilate(fine, mask, out);
    std::cout << "dilate:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.margin(fine, mask, out);
    std::cout << "margin:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.padding(fine, mask, out);
    std::cout << "padding:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.opening(fine, mask, out, scratch1);
    std::cout << "opening:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.closing(fine, mask, out, scratch1);
    std::cout << "closing:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.white_top_hat(fine, mask, out, scratch1);
    std::cout << "white_top_hat:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    morphology.black_top_hat(fine, mask, out, scratch1);
    std::cout << "black_top_hat:" << std::endl;
    std::cout << strings.format(fine, out) << std::endl;

    // REQUIRE(test::determinism(strict, 
    //     "morphology.erode", MORPHOLOGY_TEST_GRIDDED_OUT_PARAMETER(bool, fine, morphology.erode),
    //     boolean_rasters
    // ));

}

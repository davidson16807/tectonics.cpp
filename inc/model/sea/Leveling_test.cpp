
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL

#include <math/analytic/Sum.hpp>
#include <math/analytic/Gaussian.hpp>
#include <math/inspected/InverseByNewtonsMethod.hpp>

#include <field/noise/RankedFractalBrownianNoise.hpp>
#include <field/Compose.hpp>

#include <unit/si.hpp>

#include <relation/PolynomialRailyardRelation.hpp>

#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Nary.hpp>
#include <index/procedural/Map.hpp>
#include <index/procedural/Uniform.hpp>

#include <grid/dymaxion/Grid.hpp>
#include <grid/dymaxion/GridSeries.hpp>

#include "Leveling.hpp"

#include <unit/_test_tools.hpp>
#include <test/properties.hpp>

TEST_CASE( "sea::Leveling depth/volume invertibility", "[rock]" ) {

    using length = si::length<float>;
    using area = si::area<float>;
    using volume = si::volume<float>;

    using lengths = std::vector<length>;
    using areas = std::vector<area>;

    length meter(si::meter);
    area meter2(si::meter2);
    volume megameter3(si::megameter3);

    si::AbsoluteUnitAdapter<length> length_adapter(meter);
    si::AbsoluteUnitAdapter<volume> volume_adapter(0.1*megameter3);

    iterated::Identity copy;
    iterated::Arithmetic<adapted::SymbolicArithmetic> arithmetic;

    float min_elevation(-16000.0f);
    float max_elevation( 16000.0f);

    length world_radius(6.371e6 * si::meter);
    int vertices_per_square_side(32);
    dymaxion::Grid<int,int,float> grid(world_radius/meter, vertices_per_square_side);
    dymaxion::VertexPositions vertex_positions(grid);
    dymaxion::VertexDualAreas vertex_areas_in_meters(grid);
    areas vertex_areas(grid.vertex_count());
    arithmetic.multiply(vertex_areas_in_meters, procedural::uniform(meter2), vertex_areas);

    aggregated::Order<adapted::SymbolicOrder> order;
    analytic::Sum<float,analytic::Gaussian<float>> hypsometry_pdf_unscaled {
        analytic::Gaussian(-4019.0f, 1113.0f, 0.232f),
        analytic::Gaussian(  797.0f, 1169.0f, 0.209f)
    };
    auto hypsometry_cdf_unscaled = analytic::integral(hypsometry_pdf_unscaled);
    // auto hypsometry_pdf_ddx = analytic::derivative(hypsometry_pdf_unscaled);
    auto hypsometry_cdf_unscaled_range = hypsometry_cdf_unscaled(max_elevation) - hypsometry_cdf_unscaled(min_elevation);
    auto hypsometry_cdf = hypsometry_cdf_unscaled / hypsometry_cdf_unscaled_range;
    auto hypsometry_pdf = hypsometry_pdf_unscaled / hypsometry_cdf_unscaled_range;
    auto hypsometry_cdfi = inspected::inverse_by_newtons_method(hypsometry_cdf, hypsometry_pdf, 0.5f, 30);

    auto rfbm = field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/world_radius, 12.0f, 1.1e4f);

    auto elevation_meters_for_position = field::compose(hypsometry_cdfi, rfbm);
    auto elevation_in_meters = procedural::map(elevation_meters_for_position, vertex_positions);
    lengths displacement(grid.vertex_count());
    arithmetic.multiply(elevation_in_meters, procedural::uniform(meter), displacement);
    arithmetic.subtract(displacement, procedural::uniform(order.min(displacement)), displacement);

    sea::Leveling<length, float> sea_leveling(meter, 0.01);

    std::vector<volume> sea_volumes{
        100 * megameter3,
        1.332 * megameter3,
        0.1 * megameter3,
        0.0 * megameter3
    };

    std::vector<length> sea_depths{
        1000.0 * meter,
        0.0 * meter
    };

    REQUIRE(test::codomain(length_adapter,
        "positive", [=](auto depth){ return depth >= 0.0*meter; },
        "depth",  [&](auto volume){ return sea_leveling.depth(displacement, vertex_areas, volume); }, 
        sea_volumes));

    REQUIRE(test::codomain(volume_adapter,
        "positive", [=](auto volume){ return volume >= 0.0*megameter3; },
        "volume", [&](auto depth) { return sea_leveling.volume(displacement, vertex_areas, depth); }, 
        sea_depths));

    REQUIRE(test::left_invertibility(length_adapter,
        "depth",  [&](auto volume){ return sea_leveling.depth(displacement, vertex_areas, volume); }, 
        "volume", [&](auto depth) { return sea_leveling.volume(displacement, vertex_areas, depth); }, 
        sea_depths));

    REQUIRE(test::left_invertibility(volume_adapter,
        "volume", [&](auto depth) { return sea_leveling.volume(displacement, vertex_areas, depth); }, 
        "depth",  [&](auto volume){ return sea_leveling.depth(displacement, vertex_areas, volume); }, 
        sea_volumes));

}


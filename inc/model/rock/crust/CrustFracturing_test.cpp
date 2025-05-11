
// standard libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// glm libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // needed for GlmMetric
#define GLM_FORCE_SWIZZLE   // allow swizzle methods like .xy()

// in-house libraries
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicEquivalence.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/scalar/IdStrings.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Nary.hpp>
#include <index/aggregated/Equivalence.hpp>
#include <index/grouped/Statistics.hpp>

#include <unit/si.hpp>

#include <grid/dymaxion/Grid.hpp>                   
#include <grid/dymaxion/VertexDownsamplingIds.hpp>  // dymaxion::VertexDownsamplingIds

#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings
#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp>

#include <model/rock/stratum/StratumProperties.hpp>  // AgedStratumDensity
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryBuoyancyPressure
#include <model/rock/stratum/StratumSummarization.hpp>
#include <model/rock/formation/FormationSummarization.hpp>
#include <model/rock/formation/estimated/EarthlikeIgneousFormationGeneration.hpp>

#include "CrustFracturing.hpp"

TEST_CASE( "CrustFracturing", "[rock]" ) {

    using vec3 = glm::vec3;

    using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;
    using pressure = si::pressure<float>;
    using acceleration = si::acceleration<float>;

    length meter(si::meter);
    length world_radius(6.371e6 * si::meter);
    density mantle_density(3000.0*si::kilogram/si::meter3);

    int vertices_per_fine_square_side(30);
    int vertices_per_coarse_square_side(vertices_per_fine_square_side/2);

    dymaxion::Grid<int,int,float> fine(world_radius/meter, vertices_per_fine_square_side);
    dymaxion::Grid<int,int,float> coarse(world_radius/meter, vertices_per_coarse_square_side);

    const int M(2); // number of mass pools

    // INITIALIZE
    rock::EarthlikeIgneousFormationGeneration earthlike(fine, world_radius/2.0f, 0.5f, 10);
    auto generation = earthlike(12.0f, 1.1e4f);
    rock::Formation<M> igneous_formation(fine.vertex_count());
    iterated::Identity copy;
    copy(generation, igneous_formation);

    // SUMMARIZE
    auto age_of_world = 0.0f*si::megayear;
    std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
    };
    auto formation_summarize = rock::formation_summarization<2>(
        rock::stratum_summarization<2>(
          rock::AgedStratumDensity{densities_for_age, age_of_world},
          mass(si::tonne)
        ), 
        meter
    );
    int plate_id(1);
    rock::FormationSummary formation_summary(fine.vertex_count());
    formation_summarize(fine, plate_id, igneous_formation, formation_summary);

    // CALCULATE BUOYANCY
    iterated::Unary buoyancy_pressure_for_formation_summary(
        rock::StratumSummaryBuoyancyPressure{
            acceleration(si::standard_gravity), 
            mantle_density, 
        }
    );
    std::vector<pressure> fine_buoyancy_pressure(fine.vertex_count());
    buoyancy_pressure_for_formation_summary(formation_summary, fine_buoyancy_pressure);

    // DOWNSAMPLE
    std::vector<pressure> coarse_buoyancy_pressure(coarse.vertex_count());
    iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic{}};
    aggregated::Order order{adapted::SymbolicOrder{}};
    grouped::Statistics stats{adapted::TypedSymbolicArithmetic<float>(0.0f, 1.0f)};
    dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.memory, coarse.memory);
    stats.sum(vertex_downsampling_ids, fine_buoyancy_pressure, coarse_buoyancy_pressure);
    arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_buoyancy_pressure);

    // STRIP UNITS TO CALCULATE GRADIENT
    std::vector<float> vertex_scalars1(coarse.vertex_count());
    arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(pressure(1)), vertex_scalars1);

    // CALCULATE STRESS
    unlayered::VectorCalculusByFundamentalTheorem calculus;
    std::vector<vec3> vertex_gradient(coarse.vertex_count());
    calculus.gradient(coarse, vertex_scalars1, vertex_gradient);

    // FRACTURE
    const int plate_count = 8;
    rock::CrustFracturing<int,float> fracturing;
    std::vector<unlayered::FloodFillState<int,float>> fractures(plate_count, 
        unlayered::FloodFillState<int,float>(0, coarse.vertex_count()));
    fracturing.fracture(coarse, vertex_gradient, fractures);

    std::vector<int> plate_counts(coarse.vertex_count());
    fracturing.counts(fractures, plate_counts);

    std::vector<int> plate_map(coarse.vertex_count());
    fracturing.map(fractures, plate_map);

    adapted::SymbolicOrder suborder;
    adapted::IdStrings<int> substrings;
    aggregated::Order ordered(suborder);
    auto ascii_art = spheroidal::Strings(substrings, ordered);
    // auto strings = aggregated::Strings(substrings, ordered, vertices_per_fine_square_side);
    std::cout << ascii_art.format(coarse, plate_counts) << std::endl << std::endl;
    std::cout << ascii_art.format(coarse, plate_map) << std::endl << std::endl;

    // TEST THAT PLATES ARE DISJOINT
    REQUIRE(order.less_than_equal(plate_counts, procedural::uniform(1)));

    // TEST THAT PLATES ARE NONEMPTY
    std::vector<int> plate_sizes(plate_count);
    fracturing.sizes(fractures, plate_sizes);
    REQUIRE(order.not_equal(procedural::uniform(0), plate_sizes));

}


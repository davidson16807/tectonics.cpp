
/*
this file is primarily meant to test StratumGenerator[], 
but incidentally tests other functionality within rock::, to summarize:
* FormationGeneration[]
* Formation
* FormationSummary
* FormationSummarization()
* StratumSummaryIsostaticDisplacement()

It does so by testing that this diagram commutes:

      seed ⟶ formation
   desired ↘     ↓ achieved
             displacement
*/

// std libraries
#include <iostream>
#include <string>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_FORCE_SWIZZLE   // allow swizzle methods like .xy()

// in house libraries
#include <math/glm/special_specialization.hpp>
#include <math/glm/special.hpp>
#include <math/analytic/Sum.hpp>
#include <math/analytic/Gaussian.hpp>
#include <math/analytic/Error.hpp>
#include <math/inspected/InverseByNewtonsMethod.hpp>
#include <math/inspected/Compose.hpp>

#include <relation/PolynomialRailyardRelation.hpp>
#include <relation/ScalarRelation.hpp>

#include <index/series/Map.hpp>
#include <index/series/Uniform.hpp>
#include <index/series/Range.hpp>                   // Range
#include <index/series/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/series/noise/glm/UnitVectorNoise.hpp>
#include <index/series/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/si/SiMetric.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/aggregated/Metric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Nary.hpp>

#include <field/noise/ValueNoise.hpp>               // ValueNoise
#include <field/noise/PerlinNoise.hpp>              // PerlinNoise
#include <field/noise/MosaicNoise.hpp>              // MosaicNoise
#include <field/Compose.hpp>                        // Compose
#include <field/VectorZip.hpp>                      // VectorZip
#include <field/noise/MosaicOps.hpp>                // field::VectorMosaicOps
#include <field/noise/RankedFractalBrownianNoise.hpp>// field::ranked_fractal_brownian_noise

#include <grid/cartesian/UnboundedIndexing.hpp>     // field::UnboundedIndexing
#include <grid/dymaxion/Indexing.hpp>               // dymaxion::Indexing
#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::VertexPositions

#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings

#include <model/rock/stratum/Stratum.hpp>  // Stratum
#include <model/rock/formation/Formation.hpp>  // Formation
#include <model/rock/stratum/StratumSummary.hpp>  // StratumSummary
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/formation/FormationSummary.hpp>  // FormationSummary

#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization

TEST_CASE( "FormationGeneration must be able to achieve desired displacements as indicated by elevation_for_position", "[rock]" ) {

    // using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;

    length meter(si::meter);

    length radius(6.371e6 * si::meter);
    int vertices_per_square_side(16);
    dymaxion::Grid grid(radius/meter, vertices_per_square_side);
    dymaxion::VertexPositions vertex_positions(grid);
    dymaxion::VertexNormals vertex_normals(grid);

    // auto max_earth_elevation =   8848.0 * si::meter;
    // auto min_earth_elevation = -10924.0 * si::meter;

    float min_elevation(-16000.0f);
    float max_elevation( 16000.0f);

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

    auto elevation_for_position = 
        field::compose(
            relation::ScalarRelation(1.0f, length(si::meter), hypsometry_cdfi),
            field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 12.0f, 1.1e4f)
        );

    std::vector<length> intended_displacements(grid.vertex_count());
    iterated::Unary{elevation_for_position}(vertex_positions, intended_displacements);

    adapted::SymbolicOrder suborder;
    aggregated::Order order_aggregation(suborder);
    length min_observed_elevation(order_aggregation.min(intended_displacements));

    adapted::SymbolicArithmetic subarithmetic {length(0),length(1)};
    iterated::Arithmetic arithmetic(subarithmetic);
    arithmetic.subtract(intended_displacements, series::uniform(min_observed_elevation), intended_displacements);

    auto strings = spheroidal::Strings(
      adapted::SiStrings{}, 
      aggregated::Order{suborder}
    );

    std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
    };

    auto age_of_world = 0.0f*si::megayear;
    int plate_id = 1;
    auto formation_summarization = rock::formation_summarization<2>(
      rock::stratum_summarization<2>(
        rock::AgedStratumDensity{densities_for_age, age_of_world}
      ), 
      grid
    );

    iterated::Unary displacements_for_formation_summary(
      rock::StratumSummaryIsostaticDisplacement{
        density(3250.0*si::kilogram/si::meter3)
      });

    rock::FormationGeneration strata(
      grid,
      elevation_for_position,
      // displacements are from Charette & Smith 2010 (shallow ocean), enclopedia britannica (shelf bottom"continental slope"), 
      // wikipedia (shelf top), and Sverdrup & Fleming 1942 (land)
      // Funck et al. (2003) suggests a sudden transition from felsic to mafic occuring at ~4km depth or 8km thickness
      relation::get_linear_interpolation_function(si::meter, si::megayear, 
        {-11000.0, -5000.0, -4000.0, -2000.0, -900.0},
        {250.0,    100.0,   0.0,       100.0, 1000.0}),
      relation::get_linear_interpolation_function(si::meter, 2890.0 * si::kilogram/si::meter2,
        {-4500.0, -4000.0},
        {7100.0,      0.0}),
      relation::get_linear_interpolation_function(si::meter, 2700.0 * si::kilogram/si::meter2,
        {-4500.0, -4000.0,  -950.0,  840.0,    8848.0},
        {0.0,      7100.0, 28300.0, 36900.0, 70000.0}),
      relation::get_linear_interpolation_function(si::meter, 1.0, 
        {-1500.0, 8848.0},
        {0.25,      0.25}), // from Gillis (2013)
      relation::get_linear_interpolation_function(si::meter, 1.0, 
        {-1500.0, 8848.0},
        {0.15,      0.15}) // based on estimate from Wikipedia
    );

    rock::FormationSummary summary(grid.vertex_count());
    std::vector<length> actual_displacements(grid.vertex_count());
    std::vector<length> probe(grid.vertex_count());
    formation_summarization(plate_id, strata, summary);
    displacements_for_formation_summary(summary, actual_displacements);
    // for (std::size_t i = 0; i < strata.size(); ++i)
    // {
    //   probe[i] = stratum_tools.isostatic_displacement(summary[i]);
    // }

    std::cout << strings.format(grid, intended_displacements) << std::endl << std::endl;
    std::cout << strings.format(grid, actual_displacements) << std::endl << std::endl;
    // std::cout << strings.format(grid, probe) << std::endl << std::endl;

    aggregated::Metric metric(adapted::SiMetric{});
    REQUIRE(metric.distance(intended_displacements, actual_displacements) < 2.0f*si::kilometer);
}


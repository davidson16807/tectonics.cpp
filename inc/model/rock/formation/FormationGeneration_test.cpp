
/*
this file is primarily meant to test StratumGenerator[], 
but incidentally tests other functionality within rock::, to summarize:
* FormationGeneration[]
* Formation
* FormationSummary
* FormationSummarization()
* FormationSummaryAiryIsostaticDisplacement()

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

#include <index/procedural/Map.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/Range.hpp>                   // Range
#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/si/SiMetric.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/adapted/si/SiClosedForm.hpp>
#include <index/aggregated/Metric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Statistics.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/ClosedForm.hpp>
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
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::VertexPositions

#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings

#include <model/rock/stratum/Stratum.hpp>  
#include <model/rock/stratum/StratumSummary.hpp>  
#include <model/rock/formation/Formation.hpp>  
#include <model/rock/formation/FormationSummary.hpp>  
#include <model/rock/formation/FormationSummaryAiryIsostaticDisplacement.hpp>  
#include <model/rock/formation/FormationSummaryArchimedianDisplacement.hpp>  

#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/stratum/StratumForElevation.hpp>  // StratumForElevation
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization

TEST_CASE( "FormationGeneration must be able to achieve desired displacements as indicated by elevation_for_position", "[rock]" ) {

    using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;

    length meter(si::meter);

    length radius(6.371e6 * si::meter);
    int vertices_per_square_side(16);
    dymaxion::Grid<int,int,float> grid(radius/meter, vertices_per_square_side);
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

    adapted::TypedSymbolicArithmetic subarithmetic {length(0),length(1)};
    adapted::SiClosedForm si_closed_form;

    adapted::SymbolicOrder suborder;
    aggregated::Order order(suborder);
    aggregated::Statistics stats(subarithmetic, si_closed_form);
    iterated::ClosedForm closed_form(si_closed_form);

    iterated::Arithmetic arithmetic(subarithmetic);
    arithmetic.subtract(intended_displacements, procedural::uniform(order.min(intended_displacements)), intended_displacements);

    auto strings = spheroidal::Strings(adapted::SiStrings{}, order);

    const double mafic_rho(2890.0); // average density of mafic crust in kg/m³, from Carlson & Rakin 1984
    const double felsic_rho(2700.0); // average density of felsic crust in kg/m³, fit to data, often quoted at 2700, Christensen & Mooney 1995 estimate 2830
    // const double average_mafic_age(65.0); // in megayears

    std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
      // relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 30.0, 200.0}, {1970.0, 2450.0, 3380.0}), // from Carlson & Herrick (1990) up to 30My, density of oldest crust is the density of pure (nonporous) augite
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 65.0, 200.0}, {2500.0, mafic_rho, 3600.0}), // started with Carlson & Herrick (1990), then tried to fit the data
      // relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 200.0}, {mafic_rho, 3600.0}), // fit to data, often quoted from 2800 to 3300 but foundering crust should be denser than the mantle
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 1000.0}, {felsic_rho, felsic_rho})
    };

    auto age_of_world = 0.0f*si::megayear;
    int plate_id = 1;
    auto formation_summarization = rock::formation_summarization<2>(
      rock::stratum_summarization<2>(
        rock::AgedStratumDensity{densities_for_age, age_of_world},
        mass(si::tonne)
      ), 
      meter
    );

    rock::FormationSummaryArchimedianDisplacement displacements_for_formation_summary(
      density(3300.0*si::kilogram/si::meter3)); // fit to data, often quoted as 3300, should be somewhere around 3380 (solid augite) and 3810 (forsterite)

    // elevations of mafic-felsic transition in meters, based on data from Funck 2003
    const double mafic_lo (-4000.0); 
    const double mafic_hi (-3000.0);
    const double felsic_lo (-4500.0);
    const double felsic_hi (-4000.0);
    // const double atlantic_ridge(-2500.0); // depth in meters
    // const double average_ocean_depth(-3682.0); // in meters
    const double average_mafic_thickness(5000.0); // in meters, data from Funck 2003 shows ~5000 by Newfoundland

    rock::StratumForElevation stratum_per_area_for_elevation {
        /*
        See EarthlikeIgneousFormationGeneration-source-data.svg for maps containing source data for mafic thickness
        See Funck 2003 for data used for continent-ocean transition.
        Other data are from Encyclopedia Britannica (shelf bottoms), wikipedia (shelf tops) and Sverdrup & Fleming 1942 (land)
        */
        relation::get_linear_interpolation_function(si::meter, si::megayear, 
          {-6500.0, -6000.0, -5000.0, mafic_lo, mafic_hi, -500.0},
          {  200.0,   180.0,    50.0,      0.0,    70.0, 1000.0}),
        relation::get_linear_interpolation_function(si::meter, mafic_rho * si::kilogram, // mass per the unit area defined by the grid
          {mafic_lo,                mafic_hi},
          {average_mafic_thickness,      0.0}),
        relation::get_linear_interpolation_function(si::meter, felsic_rho * si::kilogram, // mass per the unit area defined by the grid
          {felsic_lo, felsic_hi,  -500.0,   840.0,  8848.0},
          {0.0,          3000.0, 28300.0, 36900.0, 70000.0}),
        relation::get_linear_interpolation_function(si::meter, 1.0, 
          {-1500.0, 8848.0},
          {0.25,      0.25}), // from Gillis (2013)
        relation::get_linear_interpolation_function(si::meter, 1.0, 
          {-1500.0, 8848.0},
          {0.15,      0.15}) // based on estimate from Wikipedia
    };

    rock::FormationGeneration igneous(grid, field::compose(stratum_per_area_for_elevation, elevation_for_position));

    rock::FormationSummary summary(grid.vertex_count());

    std::vector<length> actual_displacements(grid.vertex_count());
    formation_summarization(grid, plate_id, igneous, summary);
    displacements_for_formation_summary(summary, actual_displacements);
    arithmetic.subtract(actual_displacements, procedural::uniform(order.min(actual_displacements)), actual_displacements);

    std::vector<length> displacement_error(grid.vertex_count());
    arithmetic.subtract(actual_displacements, intended_displacements, displacement_error);
    arithmetic.subtract(displacement_error, procedural::uniform(stats.mean(displacement_error)), displacement_error);
    std::vector<length> abs_displacement_error(grid.vertex_count());
    closed_form.abs(displacement_error, abs_displacement_error);

    // std::vector<density> original_densities(grid.vertex_count());
    // iterated::Unary formation_density{rock::AgedStratumDensity{densities_for_age, age_of_world}};
    // formation_density(igneous, original_densities);

    // std::vector<density> summarized_densities(grid.vertex_count());
    // iterated::Unary formation_summary_density{rock::StratumSummaryDensity{}};
    // formation_summary_density(summary, summarized_densities);

    std::cout << strings.format(grid, intended_displacements) << std::endl << std::endl;
    std::cout << strings.format(grid, actual_displacements) << std::endl << std::endl;
    std::cout << strings.format(grid, displacement_error) << std::endl << std::endl;
    // std::cout << strings.format(grid, original_densities) << std::endl << std::endl;
    // std::cout << strings.format(grid, summarized_densities) << std::endl << std::endl;

    aggregated::Metric metric(adapted::SiMetric{});
    REQUIRE(order.max(abs_displacement_error) < 0.3f*si::kilometer);

}



// std libraries
#include <iostream>
#include <string>

// glm libraries
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
#include <index/glm/each.hpp>                       // get
#include <index/each.hpp>                           // get
#include <index/glm/known.hpp>                      // greaterThan
#include <index/known.hpp>                          // greaterThan
#include <index/whole.hpp>                          // max, mean
#include <index/series/Range.hpp>                   // Range
#include <index/series/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/series/noise/glm/UnitVectorNoise.hpp>
#include <index/series/noise/GaussianNoise.hpp>
#include <index/adapted/si/SiMetric.hpp>
#include <index/aggregated/Metric.hpp>

#include <field/noise/ValueNoise.hpp>               // ValueNoise
#include <field/noise/PerlinNoise.hpp>              // PerlinNoise
#include <field/noise/MosaicNoise.hpp>              // MosaicNoise
#include <field/Compose.hpp>                        // Compose
#include <field/VectorZip.hpp>                      // VectorZip
#include <field/noise/MosaicOps.hpp>                // field::VectorMosaicOps
#include <field/noise/FractalBrownianNoise.hpp>     // dymaxion::FractalBrownianNoise

#include <grid/cartesian/UnboundedIndexing.hpp>     // field::UnboundedIndexing
#include <grid/dymaxion/Indexing.hpp>               // dymaxion::Indexing
#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::VertexPositions
#include <grid/dymaxion/noise/MosaicOps.hpp>        // dymaxion::MosaicOps

#include <raster/spheroidal/string_cast.hpp>        // spheroidal::to_string

#include <model/rock/stratum/Stratum.hpp>  // Stratum
#include <model/rock/formation/Formation.hpp>  // Formation
#include <model/rock/stratum/StratumSummary.hpp>  // StratumSummary
#include <model/rock/formation/FormationSummary.hpp>  // FormationSummary

#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumGeneration.hpp>  // StratumGeneration
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/stratum/StratumSummaryTools.hpp>  // StratumSummaryTools
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/formation/FormationSummaryTools.hpp>  // FormationSummaryTools

  TEST_CASE( "StratumGeneration must be able to achieve desired displacements as indicated by elevation_for_position", "[rock]" ) {

      using length = si::length<float>;
      using density = si::density<float>;

      auto radius = 6.371e6 * si::meter;
      int vertices_per_square_side(16);
      dymaxion::Grid grid(radius/si::meter, vertices_per_square_side);

      // auto max_earth_elevation =   8848.0 * si::meter;
      auto min_earth_elevation = -10924.0 * si::meter;

      auto min_elevation(-16000.0f * si::meter);
      auto max_elevation( 16000.0f * si::meter);

      analytic::Sum<float,analytic::Gaussian<float>> hypsometry_pdf_unscaled {
        analytic::Gaussian(-4019.0f, 1113.0f, 0.232f),
        analytic::Gaussian(  797.0f, 1169.0f, 0.209f)
      };
      auto hypsometry_cdf_unscaled = analytic::integral(hypsometry_pdf_unscaled);
      // auto hypsometry_pdf_ddx = analytic::derivative(hypsometry_pdf_unscaled);
      auto hypsometry_cdf_unscaled_range = 
        hypsometry_cdf_unscaled(max_elevation/si::meter) - 
        hypsometry_cdf_unscaled(min_elevation/si::meter);
      auto hypsometry_cdf = hypsometry_cdf_unscaled / hypsometry_cdf_unscaled_range;
      auto hypsometry_pdf = hypsometry_pdf_unscaled / hypsometry_cdf_unscaled_range;
      auto hypsometry_cdfi = inspected::inverse_by_newtons_method(hypsometry_cdf, hypsometry_pdf, 0.5f, 30);

      auto fbm = field::fractal_brownian_noise(
        field::value_noise<3,float>(
            field::mosaic_noise(
              series::gaussian(series::unit_interval_noise(12.0f, 1.1e4f)), 
              cartesian::UnboundedIndexing<int>()),
            field::vector_mosaic_ops<3,int,float>()
        ), 10, 0.5);
      auto fbm_cdf = analytic::Error(0.0f, 1.0f, (1.0f/(std::sqrt(2.0f*3.1415926f))));

      auto elevation_for_position = 
          field::compose(
              relation::ScalarRelation(1.0f, length(si::meter), 
                  inspected::compose(hypsometry_cdfi, fbm_cdf)),
              fbm);

      dymaxion::VertexPositions vertex_positions(grid);
      std::vector<length> intended_displacements(grid.vertex_count());
      each::map(elevation_for_position, vertex_positions, intended_displacements);
      each::sub(intended_displacements, series::uniform(length(min_earth_elevation)), intended_displacements);

      std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
      };

      auto age_of_world = 0.0f*si::megayear;
      int plate_id = 1;
      rock::AgedStratumDensity stratum_density(densities_for_age, age_of_world);
      auto stratum_summarization = rock::stratum_summarization<2>(stratum_density, plate_id);
      auto formation_summarization = rock::formation_summarization<2>(stratum_summarization, grid);

      rock::StratumSummaryTools stratum_tools(density(3075.0*si::kilogram/si::meter3));
      rock::FormationSummaryTools formation_tools(stratum_tools);

      rock::StratumGeneration strata(
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
      formation_summarization(strata, summary);
      formation_tools.isostatic_displacement(summary, actual_displacements);


      adapted::SiMetric submetric;
      aggregated::Metric metric(submetric);
      REQUIRE(metric.distance(intended_displacements, actual_displacements) < 30.0f*si::kilometer);
  }


    /*
    TESTS:
    `StratumGeneration`:
      seed ⟶ formation
   desired ↘     ↓ achieved
             displacement

    `*Summary`s:
      AreaDensity/mass/density is the limit over this diagram:
      Crust ⟶ Formation
        ↓        ↓ 
        CS  ⟶   FS
    */


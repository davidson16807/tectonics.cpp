
/*
this file tests an assortment of functionality within rock::, namely:
* Crust
* CrustSummary
* Formation
* FormationSummary
* CrustOps.flatten()
* FormationOps.flatten()
* CrustSummarization()
* FormationSummarization()
* FormationGeneration[]

It does so by testing that mass properties are commutative as the limit of this diagram:

                flatten
             Crust ⟶ Formation
    summarize  ↓         ↓    summarize
              CS   ⟶    FS
                flatten
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

// objects
#include <model/rock/stratum/Stratum.hpp>  // Stratum
#include <model/rock/stratum/StratumSummary.hpp>  // StratumSummary
#include <model/rock/formation/Formation.hpp>  // Formation
#include <model/rock/formation/FormationSummary.hpp>  // FormationSummary
#include <model/rock/crust/Crust.hpp>  // Crust
#include <model/rock/crust/CrustSummary.hpp>  // CrustSummary

// arrows
#include <model/rock/mineral/MineralOps.hpp>  // StratumProperties
#include <model/rock/stratum/StratumOps.hpp>  // StratumProperties
#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/stratum/StratumForAreaAndElevation.hpp>  // StratumForAreaAndElevation
#include <model/rock/formation/FormationOps.hpp>  // CrustSummaryOps
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/formation/FormationSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustSummarization.hpp>  // CrustSummarization
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps

  TEST_CASE( "FormationGeneration must be able to achieve desired displacements as indicated by elevation_for_position", "[rock]" ) {

      // using mass = si::mass<float>;
      using length = si::length<float>;
      using density = si::density<float>;

      length meter(si::meter);

      length radius(6.371e6 * si::meter);
      int vertices_per_square_side(16);
      dymaxion::Grid grid(radius/meter, vertices_per_square_side);

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
      auto hypsometry_cdfi_meters = relation::ScalarRelation(1.0f, length(si::meter), hypsometry_cdfi);

      rock::StratumForAreaAndElevation stratum_for_area_elevation {
          // displacements are from Charette & Smith 2010 (shallow ocean), enclopedia britannica (shelf bottom"continental slope"), 
          // wikipedia (shelf top), and Sverdrup & Fleming 1942 (land)
          // Funck et al. (2003) suggests a sudden transition from felsic to mafic occuring at ~4km depth or 8km thickness
          relation::get_linear_interpolation_function(si::meter, si::megayear, 
            {-11000.0, -5000.0, -4500.0, -2000.0, -900.0},
            {250.0,    100.0,   0.0,       100.0, 1000.0}),
          relation::get_linear_interpolation_function(si::meter, si::kilogram/si::meter2,
            {-5000.0,              -4500.0},
            {3300.0 * 7100.0, 2890.0 * 0.0}),
          relation::get_linear_interpolation_function(si::meter, 2600.0 * si::kilogram/si::meter2,
            {-5000.0, -4500.0,  -950.0,  840.0,    8848.0},
            {0.0,      7100.0, 28300.0, 36900.0, 70000.0}),
          relation::get_linear_interpolation_function(si::meter, 1.0, 
            {-1500.0, 8848.0},
            {0.25,      0.25}), // from Gillis (2013)
          relation::get_linear_interpolation_function(si::meter, 1.0, 
            {-1500.0, 8848.0},
            {0.15,      0.15}) // based on estimate from Wikipedia
      };

      rock::FormationGeneration igneous(grid, 
        field::compose(
              hypsometry_cdfi_meters,
              field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 12.0f, 1.1e4f)
        ), 
        stratum_for_area_elevation
      );

      const int M = 2;
      const int F = 2;

      iterated::Copy copy{};
      rock::Formation<M> formation1;
      rock::Formation<M> formation2;
      copy(igneous(field::compose(
              hypsometry_cdfi_meters,
              field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 12.0f, 1.1e4f)
          )), formation1);
      copy(igneous(field::compose(
              hypsometry_cdfi_meters,
              field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 20.0f, 1.2e4f)
          )), formation2);
      rock::Crust<M,F> crust{formation1, formation2};

      // "flatten" ops
      auto crust_ops = 
        rock::CrustOps{
          rock::FormationOps{
            rock::StratumOps<M>{
              rock::MineralOps{}
          }}};
      auto stratum_summary_ops = 
        rock::StratumSummaryOps{
          density(3075.0*si::kilogram/si::meter3)
        };
      auto crust_summary_ops = 
        rock::CrustSummaryOps{
          rock::ColumnSummaryOps{
            stratum_summary_ops,
            length(si::centimeter)
          }
        };

      // "summarize" ops
      auto age_of_world = 0.0f*si::megayear;
      std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, M> densities_for_age {
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
      };
      auto stratum_summarization = 
        rock::stratum_summarization<M>(
          rock::AgedStratumDensity{densities_for_age, age_of_world}
        );
      auto formation_summarization = rock::formation_summarization<M>(stratum_summarization, grid);
      auto crust_summarization = rock::CrustSummarization{
        formation_summarization, rock::FormationSummaryOps{stratum_summary_ops}
      };


      int plate_id = 1;

      rock::Formation<M> formation(grid.vertex_count());
      crust_ops.flatten(crust, formation);

      rock::FormationSummary formation_summary1(grid.vertex_count());
      formation_summarization(plate_id, formation, formation_summary1);

      rock::FormationSummary scratch(grid.vertex_count());
      rock::CrustSummary crust_summary(grid.vertex_count());
      crust_summarization(plate_id, crust, crust_summary, scratch);

      rock::FormationSummary formation_summary2(grid.vertex_count());
      crust_summary_ops.flatten(crust_summary, formation_summary2);

      auto strings = spheroidal::Strings(
        adapted::SiStrings{}, 
        aggregated::Order{suborder}
      );

      std::vector<length> probe(grid.vertex_count());
      // for (std::size_t i = 0; i < formation.size(); ++i)
      // {
      //   probe[i] = stratum_tools.isostatic_displacement(summary[i]);
      // }

      // std::cout << strings.format(grid, intended_displacements) << std::endl << std::endl;
      // std::cout << strings.format(grid, actual_displacements) << std::endl << std::endl;
      // std::cout << strings.format(grid, probe) << std::endl << std::endl;

      aggregated::Metric metric(adapted::SiMetric{});
      REQUIRE(metric.distance(formation_summary1, formation_summary2) < 1.0f*si::kilometer);

  }


    /*
      area_density/mass/density is the limit over this diagram:
                flatten
             crust ⟶ formation
    summarize  ↓         ↓    summarize
              CS   ⟶    FS
                flatten
    */


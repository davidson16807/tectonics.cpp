
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

#include <index/adapted/si/SiMetric.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/aggregated/Metric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Nary.hpp>

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::VertexPositions

#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings

// objects
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
#include <model/rock/formation/FormationOps.hpp>  // CrustSummaryOps
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/formation/FormationSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustSummarization.hpp>  // CrustSummarization
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps

#include <model/rock/estimated/EarthlikeIgneousFormationGeneration.hpp>

// #include "_test_tools.hpp"
#include <model/rock/crust/_test_tools.hpp>

#include <test/structures/grouplike.hpp>

TEST_CASE( "CrustOps::absorb() monoid", "[rock]" ) {

    const int M = 2; // mineral count
    const int F = 5; // formation count

    // using mass = si::mass<float>;
    using length = si::length<float>;
    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(2);
    dymaxion::Grid grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, radius/2.0f, 0.5f, 10);

    iterated::Copy copy{};
    rock::Formation<M> formation1(grid.vertex_count());
    copy(generation(12.0f, 1.1e4f), formation1);

    rock::Formation<M> formation2(grid.vertex_count());
    copy(generation(22.0f, 1.2e4f), formation2);

    rock::Formation<M> formation3(grid.vertex_count());
    copy(generation(33.0f, 1.3e4f), formation3);

    rock::Formation<M> formation4(grid.vertex_count());
    copy(generation(44.0f, 1.4e4f), formation3);

    rock::Formation<M> formation5(grid.vertex_count());
    copy(generation(55.0f, 1.5e4f), formation5);

    rock::Crust<M,F> crust1{formation1, formation2, formation3, formation4, formation5};
    rock::Crust<M,F> crust2{formation5, formation4, formation3, formation2, formation1};

    rock::StratumStore<M> empty_stratum;
    rock::Formation<M> empty_formation(grid.vertex_count(), empty_stratum);
    rock::Crust<M,F> empty_crust;
    empty_crust.fill(empty_formation);

    std::vector<rock::Crust<M,F>> crusts{empty_crust, crust1, crust2};

    rock::CrustOps<M> ops;
    rock::CrustAdapter<M,F> testing;

    test::Monoid monoid1(
        "an empty Crust", empty_crust,
        "absorb",        [=](auto& a, auto& b){
            auto out = empty_crust;
            rock::Formation<M> scratch(grid.vertex_count(), empty_stratum);
            // for (int i = 0; i < F; ++i)
            // {
            //   std::cout << std::to_string(empty_crust[i].size()) << std::endl;
            //   std::cout << std::to_string(crust1[i].size()) << std::endl;
            //   std::cout << std::to_string(crust2[i].size()) << std::endl;
            // }
            // std::cout << std::endl;

            ops.absorb(a,b,out, scratch); 
            return out;
        }
    );

    REQUIRE(monoid1.valid(testing, crusts));

}

/*
TEST_CASE( "FormationGeneration must be able to achieve desired displacements as indicated by elevation_for_position", "[rock]" ) {

    // using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;

    const int M = 2;
    const int F = 2;

    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(4);
    dymaxion::Grid grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, 2.0f*meter/radius, 0.5f, 10);

    iterated::Copy copy{};
    rock::Formation<M> formation1(grid.vertex_count());
    copy(generation(12.0f, 1.1e4f), formation1);

    rock::Formation<M> formation2(grid.vertex_count());
    copy(generation(22.0f, 1.2e4f), formation2);

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
*/

/*
  area_density/mass/density is the limit over this diagram:
            flatten
         crust ⟶ formation
summarize  ↓         ↓    summarize
          CS   ⟶    FS
            flatten
*/


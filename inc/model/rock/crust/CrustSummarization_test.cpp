
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
* EarthlikeIgneousFormationGeneration[]

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
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::VertexPositions

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
#include <model/rock/stratum/StratumSummaryOps.hpp>  // StratumSummaryOps
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/formation/FormationOps.hpp>  // FormationOps
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/formation/FormationProperties.hpp>  // FormationMass
#include <model/rock/formation/FormationSummaryOps.hpp>  // FormationSummaryOps
#include <model/rock/formation/FormationSummaryProperties.hpp>  // FormationSummaryMass
#include <model/rock/crust/CrustOps.hpp>  // CrustOps
#include <model/rock/crust/CrustProperties.hpp>  // CrustMass
#include <model/rock/crust/CrustSummarization.hpp>  // CrustSummarization
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustSummaryProperties.hpp>  // CrustSummaryMass

#include <model/rock/formation/EarthlikeIgneousFormationGeneration.hpp>

#include <unit/_test_tools.hpp> // UnitAdapter
#include <model/rock/crust/_test_tools.hpp> // CrustAdapter

TEST_CASE( "CrustOps::flatten()/CrustSummarization() mass conservation", "[rock]" ) {

    const int M = 2; // mineral count
    const int F = 5; // formation count

    using mass = si::mass<float>;
    using length = si::length<float>;
    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(2);
    dymaxion::Grid<int,int,float> grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, radius/2.0f, 0.5f, 10);

    iterated::Identity copy{};
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

    rock::StratumStore<M> empty_stratum;
    rock::Formation<M> empty_formation(grid.vertex_count(), empty_stratum);
    rock::Crust<M,F> empty_crust; empty_crust.fill(empty_formation);

    rock::Crust<M,F> full{formation1, formation2, formation3, formation4, formation5};
    rock::Crust<M,F> empty_metaigneous{formation5, formation1, formation2, formation3, empty_formation};
    rock::Crust<M,F> empty_igneous{formation4, formation5, formation1, empty_formation, empty_formation};
    rock::Crust<M,F> empty_metasedimentary{formation3, formation4, empty_formation, empty_formation, empty_formation};
    rock::Crust<M,F> empty_sedimentary{formation2, empty_formation, empty_formation, empty_formation, empty_formation};
    rock::Crust<M,F> empty_sediment{empty_formation, formation3, formation4, formation5, formation2};
    rock::Crust<M,F> empty_middle{formation3, formation4, empty_formation, formation5, formation2};

    std::vector<rock::Crust<M,F>> crusts{
      full, empty_metaigneous, empty_igneous, empty_metasedimentary, empty_sedimentary, empty_sediment, empty_middle, empty_crust
    };

    int plate_id = 1;
    auto age_of_world = 0.0f*si::megayear;
    std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
      relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
    };

    rock::CrustOps<M> crust_ops;
    rock::CrustSummaryOps crust_summary_ops{
      rock::ColumnSummaryOps{
        length(si::centimeter)
      }
    };
    auto formation_summarize = rock::formation_summarization<2>(
      rock::stratum_summarization<2>(
        rock::AgedStratumDensity{densities_for_age, age_of_world},
        mass(si::tonne)
      ), 
      meter
    );
    auto crust_summarize = rock::crust_summarization<M,F>(
      formation_summarize, 
      crust_summary_ops
    );
    rock::CrustMass<M,F> crust_mass;
    rock::FormationMass<M> formation_mass;
    rock::CrustSummaryMass crust_summary_mass;
    rock::FormationSummaryMass formation_summary_mass;
    si::UnitAdapter testing(1e-3f);
    // rock::CrustSummaryAdapter testing2;

    rock::Formation<M> formation(grid.vertex_count());
    rock::FormationSummary formation_summary(grid.vertex_count());
    rock::CrustSummary crust_summary(grid.vertex_count());

    for (std::size_t i = 0; i < crusts.size(); ++i)
    {
        mass starting_mass = crust_mass(crusts[i]);
        crust_ops.flatten(crusts[i], formation);
        REQUIRE(testing.equal(starting_mass, formation_mass(formation)));
        formation_summarize(grid, plate_id, formation, formation_summary);
        REQUIRE(testing.equal(starting_mass, formation_summary_mass(grid, formation_summary)));
        crust_summarize(grid, plate_id, crusts[i], crust_summary, formation_summary);
        REQUIRE(testing.equal(starting_mass, crust_summary_mass(grid, crust_summary)));
        crust_summary_ops.flatten(crust_summary, formation_summary);
        REQUIRE(testing.equal(starting_mass, formation_summary_mass(grid, formation_summary)));
    }

    /*
      area_density/mass/density is the limit over this diagram:
                flatten
             crust ⟶ formation
    summarize  ↓         ↓    summarize
              CS   ⟶    FS
                flatten
    */

}


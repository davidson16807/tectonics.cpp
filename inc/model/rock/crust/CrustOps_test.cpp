
/*
this file tests an assortment of functionality within rock::, namely:
* Crust
* CrustOps.absorb()
* CrustMass()
* FormationGeneration[]
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
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/formation/FormationOps.hpp>  // CrustSummaryOps
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration
#include <model/rock/formation/FormationSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustOps.hpp>  // CrustOps
#include <model/rock/crust/CrustProperties.hpp>  // CrustMass
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps

#include <model/rock/formation/estimated/EarthlikeIgneousFormationGeneration.hpp>

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
    dymaxion::Grid<int,int,float> grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, radius/2.0f, 0.5f, 10, radius);

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
      empty_crust, full, empty_metaigneous, empty_igneous, empty_metasedimentary, empty_sedimentary, empty_sediment, empty_middle
    };

    rock::CrustOps<M> ops;
    rock::CrustAdapter<M,F> testing(0.01);

    test::Monoid monoid1(
        "an empty Crust", empty_crust,
        "absorb", [=](auto& a, auto& b){
            auto out = empty_crust;
            auto scratch = empty_formation;
            ops.absorb(a,b,out, scratch); 
            return out;
        }
    );

    REQUIRE(monoid1.valid(testing, crusts));

}

TEST_CASE( "CrustOps::absorb() mass conservation", "[rock]" ) {

    const int M = 2; // mineral count
    const int F = 5; // formation count

    using length = si::length<float>;
    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(2);
    dymaxion::Grid<int,int,float> grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, radius/2.0f, 0.5f, 10, radius);

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
      empty_crust, full, empty_metaigneous, empty_igneous, empty_metasedimentary, empty_sedimentary, empty_sediment, empty_middle
    };

    rock::CrustOps<M> ops;
    rock::CrustMass<M,F> crust_mass;
    rock::CrustAdapter<M,F> testing(1e-4);

    REQUIRE(test::binary_conservation(testing,
            "absorb", [=](auto& a, auto& b){
                auto out = empty_crust;
                auto scratch = empty_formation;
                ops.absorb(a,b,out, scratch); 
                return out;
            },
            "mass",   [=](auto& a){
                return crust_mass(a);
            },
            crusts, crusts
        ));


}


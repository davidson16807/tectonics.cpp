
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/formation/estimated/EarthlikeIgneousFormationGeneration.hpp>

#include "Formation.hpp"
#include "FormationOps.hpp"
#include "_test_tools.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>
#include <test/structures/grouplike.hpp>
#include <test/macros.hpp>

TEST_CASE( "FormationOps::absorb() commutative monoid", "[rock]" ) {

    const int M = 2;

    // using mass = si::mass<float>;
    using length = si::length<float>;
    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(4);
    dymaxion::Grid<int,int,float> grid(radius/meter, vertices_per_square_side);
    rock::EarthlikeIgneousFormationGeneration generation(grid, radius/2.0f, 0.5f, 10, radius);

    iterated::Identity copy{};
    rock::Formation<M> formation1(grid.vertex_count());
    copy(generation(12.0f, 1.1e4f), formation1);

    rock::Formation<M> formation2(grid.vertex_count());
    copy(generation(20.0f, 1.2e4f), formation2);

    rock::StratumStore<M> empty;
    rock::Formation<M> e(grid.vertex_count(), empty);
    std::vector<rock::Formation<M>> formations{e, formation1, formation2};

    rock::FormationOps<M> ops;
    rock::FormationAdapter<M> testing;

    test::CommutativeMonoid commutative_monoid1(
        "an empty Formation", e,
        "combine",            [=](auto& a, auto& b){
            rock::Formation<M> out(grid.vertex_count()); 
            ops.combine(a,b,out); 
            return out;
        }
    );

    REQUIRE(commutative_monoid1.valid(testing, formations));

}

/*
TEST_CASE( "Formation combine() mass conservation", "[rock]" ) {

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

    rock::FormationGenerationByElevation igneous(grid, 
        field::compose(
            hypsometry_cdfi_meters,
            field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 12.0f, 1.1e4f)
        ), 
        stratum_for_area_elevation,
        si::meter
    );

    const int M = 2;
    const int F = 2;

    iterated::Identity copy{};
    rock::Formation<M> a;
    rock::Formation<M> b;
    copy(igneous(field::compose(
          hypsometry_cdfi_meters,
          field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 12.0f, 1.1e4f)
      )), a);
    copy(igneous(field::compose(
          hypsometry_cdfi_meters,
          field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f*meter/radius, 20.0f, 1.2e4f)
      )), b);

    rock::FormationOps<M> ops;
    rock::FormationAdapter<M> testing;

    SECTION("the result of passing two valid Formation objects to combine() must produce a valid Formation with equivalent area_density"){
        CHECK(si::distance(ops.combine(a, b).area_density(), a.area_density() + b.area_density()) <=
            0.001f * si::max(ops.combine(a, b).area_density(), a.area_density() + b.area_density()));
    }
}
*/

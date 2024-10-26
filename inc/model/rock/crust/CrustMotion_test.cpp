
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // allow component-wise math
#include <glm/vec3.hpp> // *vec3

// in house libraries
#include <index/iterated/Nary.hpp>

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/estimated/EarthlikeIgneousFormationGeneration.hpp>

#include <model/rock/crust/CrustMotion.hpp>

#include <unit/_test_tools.hpp>

#include <test/predicate.hpp>
#include <test/properties.hpp>
// #include <test/structures/grouplike.hpp>
// #include <test/macros.hpp>

TEST_CASE( "drag_per_angular_velocity", "[rock]" ) {

    using length = si::length<double>;
    using density = si::density<double>;
    using viscosity = si::dynamic_viscosity<double>;
    using acceleration = si::acceleration<double>;
    using angular_momentum = si::angular_momentum<double>;

    const int M = 2; // mineral count

    int vertices_per_square_side(4);
    length world_radius(6.371e6 * si::meter);
    density mantle_density(3300.0*si::kilogram/si::meter3);
    viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
    dymaxion::Grid grid(world_radius/length(si::meter), vertices_per_square_side);

    unlayered::VectorCalculusByFundamentalTheorem calculus;
    auto motion = rock::crust_motion<M>(
        calculus, grid, 
        world_radius, 
        acceleration(si::standard_gravity), 
        mantle_density, 
        mantle_viscosity
    );

    std::vector<length> dimensions{
        length(0),
        length(si::meter),
        length(si::kilometer)
        // length(10*si::kilometer)
    };

    // rock::CrustAdapter<M,F> testing(0.01);
    si::UnitAdapter<double> adapter(1e-2);

    REQUIRE(test::codomain(adapter, 
        "nonnegative", [=](auto y){ return y >= angular_momentum(0); },
        "drag_per_angular_velocity", [=](auto a, auto b, auto c){ return motion.drag_per_angular_velocity(a,b,c);}, 
        dimensions, dimensions, dimensions
    ));

    for(const auto& length_ : dimensions){
        REQUIRE(test::commutativity(adapter, 
            "drag_per_angular_velocity(thickness,width)", [=](auto thickness, auto width){ 
                return motion.drag_per_angular_velocity(length_, thickness, width); 
            }, dimensions, dimensions
        ));
    }

    for(const auto& dimension1 : dimensions){
        for(const auto& dimension2 : dimensions){
            REQUIRE(test::nondecreasing(adapter, 
                "drag_per_angular_velocity(length)", [=](auto length){ 
                    return motion.drag_per_angular_velocity(length, dimension1, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nondecreasing(adapter, 
                "drag_per_angular_velocity(thickness)", [=](auto thickness){ 
                    return motion.drag_per_angular_velocity(dimension1, thickness, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nondecreasing(adapter, 
                "drag_per_angular_velocity(width)", [=](auto width){ 
                    return motion.drag_per_angular_velocity(dimension1, dimension2, width);
                },
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nonaccelerating(adapter, 
                "drag_per_angular_velocity(thickness)", [=](auto thickness){ 
                    return motion.drag_per_angular_velocity(dimension1, thickness, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                "distance",  [](auto x, auto y){return si::distance(x,y);},
                dimensions
            ));
            REQUIRE(test::nonaccelerating(adapter, 
                "drag_per_angular_velocity(width)", [=](auto width){ 
                    return motion.drag_per_angular_velocity(dimension1, dimension2, width);
                },
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                "distance",  [](auto x, auto y){return si::distance(x,y);},
                dimensions
            ));
        }
    }

    // NOTE: `drag_per_angular_velocity` is provably *not* scale invariant
    // REQUIRE(test::invariance(adapter, 
    //     "drag_per_angular_velocity", [=](auto a, auto b, auto c){ 
    //         return motion.drag_per_angular_velocity(a, b, c);
    //     },
    //     "scaling", [](auto x){return x*2.0;},
    //     lengths, lengths, lengths
    // ));

}

TEST_CASE( "slab properties that use singleton data structures", "[rock]" ) {

    using length = si::length<double>;
    using area = si::area<double>;
    using volume = si::volume<double>;
    using density = si::density<double>;
    using viscosity = si::dynamic_viscosity<double>;
    using acceleration = si::acceleration<double>;

    const int M = 2; // mineral count

    int vertices_per_square_side(4);
    length world_radius(6.371e6 * si::meter);
    density mantle_density(3300.0*si::kilogram/si::meter3);
    viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
    dymaxion::Grid grid(world_radius/length(si::meter), vertices_per_square_side);

    unlayered::VectorCalculusByFundamentalTheorem calculus;
    auto motion = rock::crust_motion<M>(
        calculus, grid, 
        world_radius, 
        acceleration(si::standard_gravity), 
        mantle_density, 
        mantle_viscosity
    );

    std::vector<length> positive_lengths{
        // length(0),
        length(si::meter),
        length(si::kilometer),
        length(10*si::kilometer)
    };

    std::vector<area> areas{
        area(0),
        area(si::meter2),
        area(si::kilometer2),
        area(world_radius*world_radius)
    };

    std::vector<volume> positive_volumes{
        // volume(0),
        volume(si::meter3),
        volume(si::kilometer3),
        volume(world_radius*world_radius*world_radius)
    };

    std::vector<int> positive_cell_counts{1, 10, 100, 1000, 10000, 100000};

    // rock::CrustAdapter<M,F> testing(0.01);
    si::UnitAdapter<double> adapter(1e-2);

    REQUIRE(test::codomain(adapter, 
        "nonnegative", [=](auto y){ return y >= length(0); },
        "slab_width", [=](auto area_, auto length_){ return motion.slab_width(area_,length_);}, 
        areas, positive_lengths
    ));

    REQUIRE(test::codomain(adapter, 
        "nonnegative", [=](auto y){ return y >= length(0); },
        "slab_length", [=](auto area_, auto cell_count){ return motion.slab_length(area_,cell_count);}, 
        areas, positive_cell_counts
    ));

    REQUIRE(test::codomain(adapter, 
        "nonnegative", [=](auto y){ return y >= length(0); },
        "slab_thickness", [=](auto area_, auto volume_){ return motion.slab_thickness(volume_, area_);}, 
        areas, positive_volumes
    ));

    for(const auto& area_ : areas){
        REQUIRE(test::nonincreasing(adapter, 
            "slab_width", [=](auto length_){ return motion.slab_width(area_, length_); },
            "identity",  [](auto x){return x;},
            "increment", [](auto x){return x+length(10);},
            positive_lengths
        ));
        REQUIRE(test::nonincreasing(adapter, 
            "slab_length", [=](auto cell_count){ return motion.slab_length(area_, cell_count); },
            "identity",  [](auto x){return x;},
            "increment", [](auto x){return x+1;},
            positive_cell_counts
        ));
    }

    for(const auto& length_ : positive_lengths){
        REQUIRE(test::increasing(adapter, 
            "slab_width", [=](auto area_){ return motion.slab_width(area_, length_); },
            "identity",  [](auto x){return x;},
            "increment", [](auto x){return x+area(10);},
            areas
        ));
    }

    for(const auto& cell_count : positive_cell_counts){
        REQUIRE(test::increasing(adapter, 
            "slab_length", [=](auto area_){ return motion.slab_length(area_, cell_count); },
            "identity",  [](auto x){return x;},
            "increment", [](auto x){return x+area(10);},
            areas
        ));
    }

    for(const auto& volume_ : positive_volumes){
        REQUIRE(test::nonincreasing(adapter, 
            "slab_thickness", [=](auto area_){ return motion.slab_thickness(volume_, area_); },
            "identity",  [](auto x){return x;},
            "increment", [](auto x){return x+area(10);},
            areas
        ));
    }

}


TEST_CASE( "slab properties that use `FormationSummary`s", "[rock]" ) {

    const int M = 2; // mineral count
    const int F = 5; // formation count

    using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;
    length meter(si::meter);
    length radius(6.371e6f * meter);

    int vertices_per_square_side(2);
    dymaxion::Grid grid(radius/meter, vertices_per_square_side);
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

    rock::Crust<M,F> igneous_only1{empty_formation, empty_formation, empty_formation, formation1, empty_formation};
    rock::Crust<M,F> igneous_only2{empty_formation, empty_formation, empty_formation, formation2, empty_formation};
    rock::Crust<M,F> igneous_only3{empty_formation, empty_formation, empty_formation, formation3, empty_formation};
    rock::Crust<M,F> igneous_only4{empty_formation, empty_formation, empty_formation, formation4, empty_formation};
    rock::Crust<M,F> igneous_only5{empty_formation, empty_formation, empty_formation, formation5, empty_formation};

    std::vector<rock::Crust<M,F>> crusts{
        igneous_only1, igneous_only2, igneous_only3, igneous_only4, igneous_only5
    };

}

/*
GOTCHAS:
* `drag_per_angular_velocity` is *not* scale invariant
TESTS:
DONE:
* `drag_per_angular_velocity` is commutative wrt thickness and width
* `drag_per_angular_velocity` is decelerating wrt thickness, length, and width
* domains:
    * `drag_per_angular_velocity > 0`
    * `slab_length > 0`
    * `slab_width > 0`
    * `slab_thickness > 0`
* monotonic: 
    * `drag_per_angular_velocity` increases wrt length, width, and thickness
    * `slab_length` increases wrt area
    * `slab_width` increases wrt area
    * `slab_thickness` increases wrt volume

TODO:
* `rigid_body_torque` is linear with respect to force magnitudes
* `buoyancy_forces × surface normal == 0` 
* `slab_thickness * slab_width * slab_length` must reproduce `slab_volume`
* `slab_thickness * slab_area` must reproduce `slab_volume`
* `slab_width * slab_length` must reproduce `slab_area`
* `slab_width ≥ slab_length` for procedurally generated plates
* domains:
    * `slab_volume > 0`
    * `slab_cell_count > 0`
    * `slab_area > 0`
* rotationally invariant:
    * `is_slab`
    * `slab_cell_count`
    * `slab_volume`
    * `slab_area`
    * `rigid_body_torque`
    * `buoyancy_forces`

* `buoyancy_forces` in combination with `drag_per_angular_velocity` and `CrustFracture`
    must produce velocities on the same order as velocities seen on earth
    when given earthlike `FormationSummary`s
* nontrivial output in combination with `CrustFracture` when given earthlike `FormationSummary`s:
    * 0 ≤ slab_cell_count < N
    * 0 ≤ slab_volume < total_volume
    * 0 ≤ slab_area < total_area
* `drag_per_angular_velocity` reproduces results from Schellart 2010 when combined with appropriate torque
*/


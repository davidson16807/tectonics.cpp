
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // allow component-wise math
#include <glm/vec3.hpp> // *vec3

// in house libraries
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

TEST_CASE( "drag_per_angular_velocity commutativity", "[rock]" ) {

    using length = si::length<double>;
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

    std::vector<length> lengths{
        // length(0*si::meter),
        length(1*si::meter),
        // length(1*si::kilometer)
        // length(10*si::kilometer)
    };

    // rock::CrustAdapter<M,F> testing(0.01);
    si::UnitAdapter<double> adapter(1e-2);

    for(const auto& length_ : lengths){
        REQUIRE(test::commutativity(adapter, 
            "drag_per_angular_velocity(thickness,width)", [=](auto thickness, auto width){ 
                return motion.drag_per_angular_velocity(length_, thickness, width); 
            }, lengths, lengths
        ));
    }

    for(const auto& dimension1 : lengths){
        for(const auto& dimension2 : lengths){
            // REQUIRE(test::decelerating(adapter, 
            //     "drag_per_angular_velocity(length)", [=](auto length_){ 
            //         return motion.drag_per_angular_velocity(length_, dimension1, dimension2); 
            //     }, 
            //     "identity",  [](auto x){return x;},
            //     "increment", [](auto x){return x+length(10);},
            //     "distance",  [](auto x, auto y){return si::distance(x,y);},
            //     lengths
            // ));
            REQUIRE(test::decelerating(adapter, 
                "drag_per_angular_velocity(thickness)", [=](auto thickness){ 
                    return motion.drag_per_angular_velocity(dimension1, thickness, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                "distance",  [](auto x, auto y){return si::distance(x,y);},
                lengths
            ));
            REQUIRE(test::decelerating(adapter, 
                "drag_per_angular_velocity(width)", [=](auto width){ 
                    return motion.drag_per_angular_velocity(dimension1, dimension2, width);
                },
                "identity",  [](auto x){return x;},
                "increment", [](auto x){return x+length(10);},
                "distance",  [](auto x, auto y){return si::distance(x,y);},
                lengths
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

    // codomain(const Adapter& adapter, 
    //     const std::string validity_name, const Valid& valid, 
    //     const std::string f_name, const F& f, 
    //     const A& as
    // )

    // increasing(const Adapter& adapter, 
    //     const std::string f_name, const F& f, 
    //     const std::string g_name, const G& g, 
    //     const A& as
    // )

    /*
    TESTS:
    DONE:
    * `drag_per_angular_velocity` is commutative wrt thickness and width
    * `drag_per_angular_velocity` is decelerating wrt thickness, length, and width
    GOTCHAS:
    * `drag_per_angular_velocity` is *not* scale invariant
    TODO:
    * `drag_per_angular_velocity` reproduces results from Schellart 2010 when combined with appropriate torque
    * `rigid_body_torque` is linear with respect to force magnitudes
    * `buoyancy_forces ⋅ surface normal == 0` 
    * `slab_thickness * slab_width * slab_length` must reproduce `slab_volume`
    * `slab_thickness * slab_area` must reproduce `slab_volume`
    * `slab_width * slab_length` must reproduce `slab_area`
    * `slab_width ≥ slab_length`
    * domains:
        * `drag_per_angular_velocity > 0`
        * `slab_volume > 0`
        * `slab_cell_count > 0`
        * `slab_area > 0`
        * `slab_thickness > 0`
        * `slab_length > 0`
        * `slab_width > 0`
    * monotonic: 
        * `drag_per_angular_velocity` increases wrt length, width, and thickness
        * `slab_thickness` increases wrt volume
        * `slab_length` increases wrt area
        * `slab_width` increases wrt area
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
    */

}


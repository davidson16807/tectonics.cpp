
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
        length(0*si::meter),
        length(1*si::meter),
        length(1*si::kilometer),
        length(10*si::kilometer)
    };

    // rock::CrustAdapter<M,F> testing(0.01);
    si::UnitAdapter<double> adapter(1e-2);

    for(const auto& length_ : lengths){
        test::commutativity(adapter, 
            "drag_per_angular_velocity", [=](auto thickness, auto width){ 
                return motion.drag_per_angular_velocity(thickness, length_, width); 
            }, lengths, lengths
        );
    }

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

}


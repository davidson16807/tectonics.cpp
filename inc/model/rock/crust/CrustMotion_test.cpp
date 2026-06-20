
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <glm/vec3.hpp> // *vec3
#include <glm/gtx/string_cast.hpp> // *vec3

// in house libraries
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/metric/MetricOrder.hpp>
#include <index/adapted/metric/MetricEquivalence.hpp>
#include <index/adapted/glm/GlmStrings.hpp>
#include <index/adapted/glm/GlmGeometric.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Order.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Geometric.hpp>
#include <index/aggregated/Equivalence.hpp>
#include <index/grouped/Statistics.hpp>
#include <index/procedural/Uniform.hpp>

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/spheroidal/Strings.hpp> // spheroidal::Strings

#include <grid/dymaxion/Grid.hpp>   // dymaxion::Grid
#include <grid/dymaxion/GridSeries.hpp> // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp> // dymaxion::VertexDownsamplingIds

#include <model/rock/stratum/StratumStore.hpp>
#include <model/rock/stratum/StratumSummarization.hpp>
#include <model/rock/formation/Formation.hpp>
#include <model/rock/formation/FormationSummarization.hpp>
#include <model/rock/crust/Crust.hpp>
#include <model/rock/formation/EarthlikeIgneousFormationGeneration.hpp>

#include <model/rock/crust/CrustMotion.hpp>

#include <unit/_test_tools.hpp>

#include <test/predicate.hpp>
#include <test/properties.hpp>
// #include <test/structures/grouplike.hpp>
// #include <test/macros.hpp>

#include "CrustFracturing.hpp"

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
    dymaxion::Grid<int,int,float> grid(world_radius/length(si::meter), vertices_per_square_side);

    unlayered::VectorCalculusByFundamentalTheorem calculus;
    auto motion = rock::crust_motion<M,float,double>(
        calculus,  
        world_radius, 
        acceleration(si::standard_gravity), 
        mantle_density, 
        mantle_viscosity,
        length(si::meter)
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
                "drag_per_angular_velocity(length)", [=](length length_){ 
                    return motion.drag_per_angular_velocity(length_, dimension1, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](length x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nondecreasing(adapter, 
                "drag_per_angular_velocity(thickness)", [=](length thickness){ 
                    return motion.drag_per_angular_velocity(dimension1, thickness, dimension2); 
                }, 
                "identity",  [](auto x){return x;},
                "increment", [](length x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nondecreasing(adapter, 
                "drag_per_angular_velocity(width)", [=](length width){ 
                    return motion.drag_per_angular_velocity(dimension1, dimension2, width);
                },
                "identity",  [](auto x){return x;},
                "increment", [](length x){return x+length(10);},
                dimensions
            ));
            REQUIRE(test::nonaccelerating(adapter, 
                "drag_per_angular_velocity(thickness)", [=](length thickness){ 
                    return motion.drag_per_angular_velocity(dimension1, thickness, dimension2); 
                }, 
                "increment", [](length x){return x+length(10);},
                "distance",  [](auto x, auto y){return si::distance(x,y);},
                dimensions
            ));
            REQUIRE(test::nonaccelerating(adapter, 
                "drag_per_angular_velocity(width)", [=](length width){ 
                    return motion.drag_per_angular_velocity(dimension1, dimension2, width);
                },
                "increment", [](length x){return x+length(10);},
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
    dymaxion::Grid<int,int,float> grid(world_radius/length(si::meter), vertices_per_square_side);

    unlayered::VectorCalculusByFundamentalTheorem calculus;
    auto motion = rock::crust_motion<M,float,double>(
        calculus,  
        world_radius, 
        acceleration(si::standard_gravity), 
        mantle_density, 
        mantle_viscosity,
        length(si::meter)
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

    rock::Crust<M,F> igneous_only1{empty_formation, empty_formation, empty_formation, formation1, empty_formation};
    rock::Crust<M,F> igneous_only2{empty_formation, empty_formation, empty_formation, formation2, empty_formation};
    rock::Crust<M,F> igneous_only3{empty_formation, empty_formation, empty_formation, formation3, empty_formation};
    rock::Crust<M,F> igneous_only4{empty_formation, empty_formation, empty_formation, formation4, empty_formation};
    rock::Crust<M,F> igneous_only5{empty_formation, empty_formation, empty_formation, formation5, empty_formation};

    std::vector<rock::Crust<M,F>> crusts{
        igneous_only1, igneous_only2, igneous_only3, igneous_only4, igneous_only5
    };

}

TEST_CASE( "buoyancy_forces ⋅ surface normal == 0", "[rock]" ) {

    using vec3 = glm::vec3;

    using mass = si::mass<float>;
    using length = si::length<float>;
    using density = si::density<float>;
    using pressure = si::pressure<float>;
    using acceleration = si::acceleration<float>;
    using viscosity = si::dynamic_viscosity<float>;

    length meter(si::meter);
    length world_radius(6.371e6 * si::meter);
    density mantle_density(3000.0*si::kilogram/si::meter3);
    viscosity mantle_viscosity(1.57e20*si::pascal*si::second);

    int vertices_per_fine_square_side(30);
    int vertices_per_coarse_square_side(vertices_per_fine_square_side/2);

    dymaxion::Grid<int,int,float> fine(world_radius/meter, vertices_per_fine_square_side);
    dymaxion::Grid<int,int,float> coarse(world_radius/meter, vertices_per_coarse_square_side);

    const int M(2); // number of mass pools

    // INITIALIZE
    rock::EarthlikeIgneousFormationGeneration earthlike(fine, world_radius/2.0f, 0.5f, 10);
    auto generation = earthlike(12.0f, 1.1e4f);
    rock::Formation<M> igneous_formation(fine.vertex_count());
    iterated::Identity copy;
    copy(generation, igneous_formation);

    // SUMMARIZE
    auto age_of_world = 0.0f*si::megayear;
    std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
        relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
    };
    auto formation_summarize = rock::formation_summarization<2>(
        rock::stratum_summarization<2>(
          rock::AgedStratumDensity{densities_for_age, age_of_world},
          mass(si::tonne)
        ), 
        meter
    );
    int plate_id(1);
    rock::FormationSummary formation_summary(fine.vertex_count());
    formation_summarize(fine, plate_id, igneous_formation, formation_summary);

    // CALCULATE BUOYANCY
    iterated::Unary buoyancy_pressure_for_formation_summary(
        rock::StratumSummaryBuoyancyPressure{
            acceleration(si::standard_gravity), 
            mantle_density, 
        }
    );
    std::vector<pressure> fine_buoyancy_pressure(fine.vertex_count());
    buoyancy_pressure_for_formation_summary(formation_summary, fine_buoyancy_pressure);

    // DOWNSAMPLE
    std::vector<pressure> coarse_buoyancy_pressure(coarse.vertex_count());
    iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic{}};
    aggregated::Order order{adapted::SymbolicOrder{}};
    grouped::Statistics stats{adapted::TypedSymbolicArithmetic<float>(0.0f, 1.0f)};
    dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.memory, coarse.memory);
    stats.sum(vertex_downsampling_ids, fine_buoyancy_pressure, coarse_buoyancy_pressure);
    arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_buoyancy_pressure);

    // STRIP UNITS TO CALCULATE GRADIENT
    std::vector<float> coarse_buoyancy_pressure_in_pascals(coarse.vertex_count());
    arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(pressure(1)), coarse_buoyancy_pressure_in_pascals);

    // CALCULATE STRESS
    unlayered::VectorCalculusByFundamentalTheorem calculus;
    std::vector<vec3> vertex_gradient(coarse.vertex_count());
    calculus.gradient(coarse, coarse_buoyancy_pressure_in_pascals, vertex_gradient);

    // FRACTURE
    const int plate_count = 8;
    rock::CrustFracturing<int,float> fracturing;
    std::vector<unlayered::FloodFillState<int,float>> fractures(plate_count, 
        unlayered::FloodFillState<int,float>(0, coarse.vertex_count()));
    fracturing.fracture(coarse, vertex_gradient, fractures);

    std::vector<int> coarse_plate_map(coarse.vertex_count());
    fracturing.map(fractures, coarse_plate_map);

    iterated::Index index;
    std::vector<int> fine_plate_map(fine.vertex_count());
    index(coarse_plate_map, vertex_downsampling_ids, fine_plate_map);

    auto motion = rock::crust_motion<M,float,double>(
        calculus, 
        world_radius, 
        acceleration(si::standard_gravity), 
        mantle_density, 
        mantle_viscosity,
        meter
    );

    dymaxion::VertexNormals normals(fine);
    iterated::Geometric<adapted::GlmGeometric> geometric;
    iterated::Metric<adapted::GlmMetric> metric3;
    std::vector<vec3> cross(fine.vertex_count());
    std::vector<float> dot_lengths(fine.vertex_count());
    std::vector<float> fine_slab_pull_lengths(fine.vertex_count());
    aggregated::Equivalence<adapted::MetricEquivalence<double, adapted::GlmMetric>> equivalence(
        adapted::MetricEquivalence<double, adapted::GlmMetric>(1e-4)
    );
    std::vector<bool> fine_plate_existance(fine.vertex_count());
    std::vector<vec3> fine_slab_pull(fine.vertex_count());
    std::vector<bool> fine_slab_existance(fine.vertex_count());
    adapted::GlmStrings substrings3;
    spheroidal::Strings ascii_art(adapted::ScalarStrings<float>{}, aggregated::Order<adapted::SymbolicOrder>{});
    // spheroidal::Strings ascii_art(substrings3, aggregated::Order{adapted::MetricOrder{adapted::GlmMetric{}}});
    iterated::Order<adapted::SymbolicOrder> orders;
    for (int i = 0; i < plate_count; ++i)
    {
        fracturing.exists(fine_plate_map, i, fine_plate_existance);
        motion.slab_pull(fine, fine_buoyancy_pressure, fine_plate_existance, si::force<float>(si::newton), fine_slab_pull);

        geometric.dot(fine_slab_pull, normals, dot_lengths);
        metric3.length(fine_slab_pull, fine_slab_pull_lengths);
        orders.max(fine_slab_pull_lengths, procedural::uniform(0.001f), fine_slab_pull_lengths);
        arithmetic.divide(dot_lengths, fine_slab_pull_lengths, dot_lengths);
        std::cout << ascii_art.format(fine, dot_lengths) << std::endl;
        // REQUIRE(order.greater_than(dot_lengths, procedural::uniform(0.0f)));
        REQUIRE(order.less_than(dot_lengths, procedural::uniform(0.01f)));

        motion.is_slab(fine_slab_pull, fine_slab_existance);
        auto slab_volume = motion.slab_volume(fine, formation_summary, fine_slab_existance);
        auto slab_area = motion.slab_area(fine, formation_summary, fine_slab_existance);
        auto slab_cell_count = motion.slab_cell_count(fine_slab_existance);
        auto slab_thickness = motion.slab_thickness(slab_volume, slab_area);
        auto slab_length = motion.slab_length(slab_area, slab_cell_count);
        auto slab_width = motion.slab_width(slab_area, slab_length);
        auto slab_drag_per_angular_velocity = motion.drag_per_angular_velocity(slab_length, slab_thickness, slab_width);
        auto slab_torque = motion.rigid_body_torque(fine, fine_slab_pull, si::newton, si::newton*si::meter);
        auto slab_angular_velocity = glm::length(slab_torque)*si::newton*si::meter/slab_drag_per_angular_velocity;
        auto slab_period = 1.0/slab_angular_velocity;

        REQUIRE(slab_volume > 0.0*si::kilometer3);
        REQUIRE(slab_volume < fine.total_volume() * meter * meter * meter);
        REQUIRE(slab_area > 0.0*si::kilometer2);
        REQUIRE(slab_area < fine.total_area() * meter * meter);
        REQUIRE(slab_cell_count > 0);
        REQUIRE(slab_cell_count < fine.vertex_count());
        REQUIRE(slab_thickness > 0.0 * si::kilometer);
        REQUIRE(slab_length > 0.0 * si::kilometer);
        REQUIRE(slab_width > 0.0 * si::kilometer);
        REQUIRE(si::distance(slab_thickness * slab_width * slab_length, slab_volume) < 30.0*si::meter3);
        REQUIRE(si::distance(slab_thickness * slab_area, slab_volume) < 10.0*si::meter3);
        REQUIRE(si::distance(slab_width * slab_length, slab_area) < si::meter2);
        REQUIRE(slab_width >= slab_length);
        // REQUIRE(slab_drag_per_angular_velocity > 0.0);
        /*
        Solomon, "On the Forces Driving Plate Tectonics" (1975), lists angular velocities for tectonic plates.
        From those velocities, we can determine the shortest period of rotation is ~180 My.
        The longest period of rotation (excluding the plate they used as their reference frame) is ~600 My.
        Larson, "Global plate velocities from the Global Positioning System" (1997) estimates even longer periods, 
        from 486My to 3Gy. We note that our model tends to underpredict periods.
        In this test case, our longest period (183My) is roughly equivalent to the shortest period reported by Solomon.
        This is surprising since we expected plate motion to only occur in our model 
        if a precise combination of circumstances were met along a plate boundary
        This finding at least assures us that the model is able to regularly produce these circumstances.
        For the purpose of testing, we will only require that period assumes a valid order of magnitude.
        */
        REQUIRE(slab_period > 10*si::megayear);
        REQUIRE(slab_period < 1000*si::megayear);

        std::cout << ascii_art.format(fine, fine_slab_existance) << std::endl;
        std::cout << slab_volume << std::endl;
        std::cout << slab_area << std::endl;
        std::cout << slab_cell_count << std::endl;
        std::cout << slab_thickness << std::endl;
        std::cout << slab_length << std::endl;
        std::cout << slab_width << std::endl;
        std::cout << slab_drag_per_angular_velocity << std::endl;
        std::cout << glm::to_string(slab_torque) << std::endl;
        std::cout << glm::length(slab_torque)*si::newton*si::meter << std::endl;
        std::cout << slab_period << std::endl;

    }

}

/*
GOTCHAS:
* `drag_per_angular_velocity` is *not* scale invariant
TESTS:
DONE:
* miscellaneous:
    * `drag_per_angular_velocity` is commutative wrt thickness and width
    * `drag_per_angular_velocity` is decelerating wrt thickness, length, and width
    * `|slab_pull ⋅ surface normal| == 0` 
    * `slab_thickness * slab_width * slab_length` must reproduce `slab_volume`
    * `slab_thickness * slab_area` must reproduce `slab_volume`
    * `slab_width * slab_length` must reproduce `slab_area`
    * `slab_width ≥ slab_length` for procedurally generated plates
* domains:
    * `drag_per_angular_velocity > 0`
    * `slab_pull > 0`
    * `slab_length > 0`
    * `slab_width > 0`
    * `slab_thickness > 0`
    * `total_volume > slab_volume > 0`
    * `total_area > slab_area > 0`
    * `N > slab_cell_count > 0`
* monotonic: 
    * `drag_per_angular_velocity` increases wrt length, width, and thickness
    * `slab_length` increases wrt area
    * `slab_width` increases wrt area
    * `slab_thickness` increases wrt volume

TODO:
* `rigid_body_torque` is linear with respect to force magnitudes
* rotationally invariant:
    * `is_slab`
    * `slab_cell_count`
    * `slab_volume`
    * `slab_area`
    * `rigid_body_torque`
    * `buoyancy_forces`

* `buoyancy_forces` in combination with `drag_per_angular_velocity` and `CrustFracturing`
    must produce velocities on the same order as velocities seen on earth when given earthlike `FormationSummary`
* `drag_per_angular_velocity` reproduces results from Schellart 2010 when combined with appropriate torque
*/


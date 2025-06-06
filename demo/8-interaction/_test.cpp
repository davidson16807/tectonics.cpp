/*
disable warnings about openmp pragmas - the code can run without them, 
and gprof can't run on output using the -fopenmp flag and still provide accurate information
*/
#pragma GCC diagnostic ignored "-Wunknown-pragmas" 

// std libraries
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

#include <math/glm/special_specialization.hpp>
#include <math/glm/special.hpp>
#include <math/analytic/Sum.hpp>
#include <math/analytic/Gaussian.hpp>
#include <math/analytic/Error.hpp>
#include <math/inspected/InverseByNewtonsMethod.hpp>

// in house libraries
#include <index/procedural/Map.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/whole.hpp>                          // max, mean
#include <index/procedural/Range.hpp>                   // Range
#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/scalar/IdStrings.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Strings.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Bitset.hpp>
#include <index/grouped/Statistics.hpp>

#include <field/Uniform.hpp>                        // Uniform
#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/GridCache.hpp>              // dymaxion::GridCache
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp>  // dymaxion::VertexDownsamplingIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/unlayered/SeedBasedFloodFilling.hpp>     // unlayered::SeedBasedFloodFilling
#include <raster/unlayered/NeighborBasedFloodFilling.hpp> // unlayered::NeighborBasedFloodFilling
#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings
#include <raster/unlayered/Morphology.hpp>          // unlayered::Morphology

#include <model/rock/mineral/GrainType.hpp>
#include <model/rock/column/ColumnSummaryProperties.hpp>
#include <model/rock/stratum/StratumProperties.hpp>
#include <model/rock/stratum/StratumSummarization.hpp>
#include <model/rock/stratum/StratumSummaryProperties.hpp>
#include <model/rock/formation/Formation.hpp>
#include <model/rock/formation/FormationField.hpp>
#include <model/rock/formation/FormationSummarization.hpp>
#include <model/rock/formation/FormationSummaryArchimedianDisplacement.hpp>
#include <model/rock/formation/EarthlikeIgneousFormationGeneration.hpp>
#include <model/rock/crust/FormationType.hpp>
#include <model/rock/crust/Crust.hpp>
#include <model/rock/crust/CrustOps.hpp>
#include <model/rock/crust/CrustSummarization.hpp>
#include <model/rock/crust/CrustSummaryOps.hpp>
#include <model/rock/crust/CrustSummaryProperties.hpp>
#include <model/rock/crust/CrustMotion.hpp>
#include <model/rock/crust/CrustFracturing.hpp>
#include <model/rock/crust/CrustSummaryPredicates.hpp>
#include <model/rock/crust/EmptyCrustField.hpp>
#include <model/rock/crust/IgneousCrustField.hpp>
#include <model/rock/lithosphere/Lithosphere.hpp>
#include <model/rock/lithosphere/LithosphereSummary.hpp>
#include <model/rock/lithosphere/LithosphereSummarization.hpp>
#include <model/rock/lithosphere/LithosphereReferenceFrames.hpp>

#include <update/OrbitalControlState.hpp>           
#include <update/OrbitalControlUpdater.hpp>         

#include <view/ColorscaleSurfaceShaderProgram.hpp>  
#include <view/IndicatorSwarmShaderProgram.hpp>     
#include <view/MultichannelSurfaceShaderProgram.hpp>


namespace minerals{
  enum enumeration {
    mafic,
    felsic
  };
  const int count(2);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> arguments(argv, argv + argc); 
  float seed1(argc<=1? 12.0f : std::stoi(arguments[1]));
  float seed2(argc<=2? 1.1e4f : std::stoi(arguments[2]));

  std::cout << "seed parameters: " << std::to_string(seed1) << ", " << std::to_string(seed2) << std::endl;

  // initialize GLFW
  if (!glfwInit()) {
    std::cout << stderr << " ERROR: could not start GLFW3" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for MacOS
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // we don't want the old OpenGL

  // open a window
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Profiler", NULL, NULL);
  if (!window) {
    std::cout << stderr << " ERROR: could not open window with GLFW3" << std::endl;
    glfwTerminate();
    return 1;
  }

  // create a GLEW context
  glfwMakeContextCurrent(window);

  // initialize GLEW 
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << stderr << " ERROR: Failed to initialize GL" << std::endl;
    return -1;
  }

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  /* OUR STUFF GOES HERE NEXT */

  const int M(minerals::count);
  const int F(5);

  using mass = si::mass<float>;
  using density = si::density<float>;
  using length = si::length<float>;
  using pressure = si::pressure<float>;
  using viscosity = si::dynamic_viscosity<float>;
  using acceleration = si::acceleration<float>;

  using vec3 = glm::vec3;
  using mat3 = glm::mat3;
  using mat4 = glm::mat4;

  using bools = std::vector<bool>;

  using Grid = dymaxion::Grid<std::int8_t, int, float>;

  using Minerals = std::array<rock::Mineral,M>;

  length meter(si::meter);

  length world_radius(6.371e6 * si::meter);
  density mantle_density(3000.0*si::kilogram/si::meter3);
  viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
  int vertices_per_fine_square_side(60);
  int vertices_per_coarse_square_side(vertices_per_fine_square_side/2);
  dymaxion::GridCache fine(Grid(world_radius/meter, vertices_per_fine_square_side));
  dymaxion::GridCache coarse(Grid(world_radius/meter, vertices_per_coarse_square_side));

  iterated::Identity copy;

  // GENERATE THE CRUST
  rock::EarthlikeIgneousFormationGeneration earthlike(fine, world_radius/2.0f, 0.5f, 10);
  // earthlike is only used for an in-order traversal, below, so its faster to use GridCache
  // it only uses vertex_positions and vertex_dual_areas, of which both are cached

  auto generation = earthlike(seed1, seed2);
  rock::StratumStore<M> empty_stratum;
  rock::Formation<M> empty_formation(fine.vertex_count(), empty_stratum);
  rock::Formation<M> igneous_formation(fine.vertex_count());
  rock::EmptyCrustField empty_crust;
  copy(generation, igneous_formation);
  rock::Crust<M,F> crust{empty_formation, empty_formation, empty_formation, igneous_formation, empty_formation};

  // CALCULATE BUOYANCY
  auto age_of_world = 0.0f*si::megayear;
  std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
    // relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 65.0, 200.0}, {2500.0, 2890.0, 3600.0}), // started with Carlson & Herrick (1990), then tried to fit the data
    // relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 1000.0}, {2700.0, 2700.0})
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
  };

  rock::CrustSummaryOps crust_summary_ops{
    rock::ColumnSummaryOps{
      length(si::centimeter)
    }
  };

  rock::CrustOps<M> crust_ops;

  rock::CrustSummaryProperty existance{rock::ColumnSummaryExists()};
  rock::CrustSummaryProperty thicknesses{rock::ColumnSummaryThickness()};
  rock::CrustSummaryProperty densities{rock::ColumnSummaryDensity()};
  rock::CrustSummaryProperty place_counts{rock::ColumnSummaryPlateCount()};
  rock::CrustSummaryProperty displacements{rock::ColumnSummaryIsostaticDisplacement(density(3300.0*si::kilogram/si::meter3))};

  auto crust_summarize0 = 
    rock::crust_summarization<M,F>(
      rock::formation_summarization<2>(
        rock::stratum_summarization<2>(
          rock::AgedStratumDensity{densities_for_age, age_of_world},
          mass(0.0*si::tonne)
        ), 
        // fine,
        // world_radius
        meter
      ), crust_summary_ops);

  // SUMMARIZE THE CRUST
  rock::CrustSummary crust_summary(fine.vertex_count());
  rock::FormationSummary formation_summary(fine.vertex_count());
  int plate_id(1);
  crust_summarize0(fine, plate_id, crust, crust_summary, formation_summary);
  // crust_summarize is in-order, so its faster to use a GridCache
  crust_summary_ops.flatten(crust_summary, formation_summary);

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
  dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.grid.memory, coarse.grid.memory);
  stats.sum(vertex_downsampling_ids, fine_buoyancy_pressure, coarse_buoyancy_pressure);
  arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_buoyancy_pressure);

  // STRIP UNITS TO CALCULATE GRADIENT
  std::vector<float> coarse_buoyancy_pressure_in_pascals(coarse.vertex_count());
  arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(pressure(1)), coarse_buoyancy_pressure_in_pascals);

  // CALCULATE STRESS
  unlayered::VectorCalculusByFundamentalTheorem calculus;
  std::vector<vec3> vertex_gradient(coarse.vertex_count());
  calculus.gradient(coarse.grid, coarse_buoyancy_pressure_in_pascals, vertex_gradient);

  // FRACTURE
  const int P = 8; // plate count
  rock::CrustFracturing<int,float> fracturing;
  std::vector<unlayered::FloodFillState<int,float>> fractures(P, 
      unlayered::FloodFillState<int,float>(0, coarse.vertex_count()));
  fracturing.fracture(coarse, vertex_gradient, fractures);
  auto plates = rock::Lithosphere<M,F>(P, crust);
  auto locals = rock::LithosphereSummary(P, crust_summary);
  auto globals = rock::LithosphereSummary(P, crust_summary);
  rock::CrustSummary master(fine.vertex_count());
  std::vector<rock::CrustSummary> localized(P, crust_summary); // the global CrustSummary localized into each plate
  rock::FormationSummary scratch_formation(fine.vertex_count());
  std::vector<rock::FormationSummary> scratch(P, scratch_formation); // scratch memory for LithosphereSummarization
  bools ownable(fine.vertex_count());
  bools top(fine.vertex_count());
  bools below(fine.vertex_count());
  bools foundering(fine.vertex_count());
  bools exists(fine.vertex_count());
  bools rifting(fine.vertex_count());
  bools accompanied(fine.vertex_count());
  bools detaching(fine.vertex_count());
  bools bools_scratch(fine.vertex_count());

  // GENERATE PLATE MAP FROM FRACTURE STATES
  std::vector<int> coarse_plate_map(coarse.vertex_count());
  fracturing.map(fractures, coarse_plate_map);

  // UPSAMPLE THE PLATE MAP
  iterated::Index index;
  std::vector<int> fine_plate_map(fine.vertex_count());
  index(coarse_plate_map, vertex_downsampling_ids, fine_plate_map);

  // print plate map and buoyancy field
  auto id_strings = spheroidal::Strings(
    adapted::IdStrings<float>(adapted::dotshades), 
    aggregated::Order<adapted::SymbolicOrder>()
  );
  auto scalar_strings = spheroidal::Strings(
    adapted::ScalarStrings<float>(adapted::dotshades), 
    aggregated::Order<adapted::SymbolicOrder>()
  );
  auto si_strings = spheroidal::Strings(
    adapted::SiStrings(adapted::dotshades), 
    aggregated::Order<adapted::SymbolicOrder>()
  );
  // std::cout << id_strings.format(fine, fine_plate_map) << std::endl;
  // std::cout << si_strings.format(fine, fine_buoyancy_pressure) << std::endl;

  // CALCULATE MOTION FOR EACH PLATE
  auto motion = rock::crust_motion<M,float,double>(
      calculus, 
      world_radius, 
      acceleration(si::standard_gravity), 
      mantle_density, 
      mantle_viscosity,
      meter
  );
  std::vector<bool> fine_plate_existance(fine.vertex_count());
  std::vector<vec3> fine_slab_pull(fine.vertex_count());
  std::vector<bool> fine_slab_existance(fine.vertex_count());
  std::vector<vec3> angular_velocities_in_seconds(P);
  std::vector<mat3> orientations(P, mat3(1));
  for (int i = 0; i < P; ++i)
  {
      fracturing.exists(fine_plate_map, i, fine_plate_existance);
      motion.slab_pull(fine, fine_buoyancy_pressure, fine_plate_existance, si::force<float>(si::newton), fine_slab_pull);
      motion.is_slab(fine_slab_pull, fine_slab_existance);
      auto slab_volume = motion.slab_volume(fine, formation_summary, fine_slab_existance);
      auto slab_area = motion.slab_area(fine, formation_summary, fine_slab_existance);
      auto slab_cell_count = motion.slab_cell_count(fine_slab_existance);
      auto slab_thickness = motion.slab_thickness(slab_volume, slab_area);
      auto slab_length = motion.slab_length(slab_area, slab_cell_count);
      auto slab_width = motion.slab_width(slab_area, slab_length);
      auto slab_drag_per_angular_velocity = motion.drag_per_angular_velocity(slab_length, slab_thickness, slab_width);
      auto slab_torque_in_newton_meters = motion.rigid_body_torque(fine, fine_slab_pull, si::newton, si::newton*si::meter);
      angular_velocities_in_seconds[i] = slab_torque_in_newton_meters*(si::newton*si::meter/slab_drag_per_angular_velocity*si::second);
      crust_ops.ternary(fine, fine_plate_existance, plates[i], empty_crust, plates[i]);
  }

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<std::int8_t,int,float> grids(vertices_per_fine_square_side);
  dymaxion::VertexPositions fine_vertex_positions(fine.grid);
  std::vector<density> densities_i(fine.vertex_count());
  std::vector<length> lengths_i(fine.vertex_count());
  std::vector<std::vector<length>> lengths(P, lengths_i);
  std::vector<float> buffer_color_values(fine.vertex_count());
  std::vector<std::uint8_t> buffer_exists_i(fine.vertex_count(), std::uint8_t(1));
  std::vector<std::vector<std::uint8_t>> buffer_exists(P, buffer_exists_i);
  std::vector<float> buffer_scalars1(fine.vertex_count());
  std::vector<float> buffer_uniform(fine.vertex_count(), 1.0f);
  std::vector<float> buffer_scalars2_i(fine.vertex_count());
  std::vector<std::vector<float>> buffer_scalars2(P, buffer_scalars2_i);
  std::vector<glm::vec3> buffer_positions(fine.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangles_size(fine_vertex_positions));
  std::cout << "vertex count:        " << fine.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << fine.vertices_per_meridian() << std::endl;
  // copy(vertex_colored_scalars, buffer_color_values);
  std::vector<bool> mask1(fine.vertex_count());
  std::vector<bool> mask2(fine.vertex_count());
  std::vector<bool> mask3(fine.vertex_count());

  // copy(vertex_colored_scalars, buffer_color_values);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(fine_vertex_positions, buffer_positions);
  grids.storeTriangles(procedural::range<unsigned int>(coarse.vertex_count()), buffer_element_vertex_ids);

  length procedural_terrain_far_distance(3e3*si::kilometer);
  length planet_billboard_near_distance(1e7*si::kilometer); // ~10 * solar radius 

  // initialize control state
  update::OrbitalControlState control_state(
      glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f, // angular_position
      glm::vec2(0), // angular_direction
      (world_radius+procedural_terrain_far_distance)/meter, // min_zoom_distance
      23.0f // log2_height
  );

  // initialize view state
  view::ViewState view_state;
  view_state.projection_matrix = glm::perspective(
    3.14159f*45.0f/180.0f, 
    850.0f/640.0f, 
    procedural_terrain_far_distance/meter,      // near plane distance
    planet_billboard_near_distance/meter // far plane distance
  );
  view_state.view_matrix = control_state.get_view_matrix();
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);
  view::ColorscaleSurfacesViewState colorscale_state;
  colorscale_state.min_color_value = whole::min(coarse_buoyancy_pressure_in_pascals);
  colorscale_state.max_color_value = whole::max(coarse_buoyancy_pressure_in_pascals);
  colorscale_state.darken_threshold = 0.0;

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;
  view::IndicatorSwarmShaderProgram indicator_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  rock::FormationSummaryArchimedianDisplacement displacement_for_summary(
    density(3300.0*si::kilogram/si::meter3)
  );

  auto frames = rock::lithosphere_reference_frames<int,float,mat3>(
    dymaxion::NearestVertexId<std::int8_t,int,float>(fine.grid),
    fine.vertex_positions // vertex_positions is traversed in-order so it's faster to use a cache
  );

  iterated::Bitset<adapted::BooleanBitset> bitset;
  unlayered::Morphology morphology{bitset};
  rock::CrustSummaryPredicates predicates{morphology, bitset};

  const float pi(3.1415926535);
  const float oo(std::numeric_limits<float>::max());
  // const int frame_count(1000);

  auto start = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)){

      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      auto crust_summarize = 
        rock::crust_summarization<M,F>(
          rock::formation_summarization<2>(
            rock::stratum_summarization<2>(
              rock::AgedStratumDensity{densities_for_age, age_of_world},
              mass(0.0*si::tonne)
            ), 
            // fine,
            // world_radius
            meter
          ), crust_summary_ops);
      auto summarization = rock::lithosphere_summarization<M,F>(crust_summarize, crust_summary_ops);

      // colorscale_state.max_color_value = -10.0;
      // colorscale_state.min_color_value = 16.0;

      // motion
      for (std::size_t i(0); i < P; ++i)
      {
        orientations[i] = 
          glm::rotate(
            mat4(orientations[i]),
            float(si::kiloyear/si::second) * 2.0f*pi * glm::length(angular_velocities_in_seconds[i]), 
            glm::normalize(angular_velocities_in_seconds[i]));
      }

      // summarize
      summarization .summarize (fine, plates, locals, scratch);   // summarize each plate into a (e.g.) CrustSummary raster
      frames        .globalize (orientations, locals, globals);   // resample plate-specific rasters onto a global grid
      summarization .flatten   (globals,      master);            // condense globalized rasters into e.g. LithosphereSummary
      frames        .localize  (orientations, master, localized); // resample global raster to a plate-specific for each plate
      // summarization uses fine only for vertex_dual_areas, so it's faster if fine is a GridCache

      const si::length<double> average_crust_thickness(7.1*si::kilometer); // McKenzie and O'nions 1992
      const si::density<double> average_crust_density(2890.0*si::kilogram/si::meter3); // Carlson and Raskin 1984
      rock::IgneousCrustField fresh_crust(
        rock::FormationField(
          field::uniform(
            rock::Stratum<M>(
              age_of_world,
              age_of_world,
              Minerals{
                rock::Mineral(
                  average_crust_density * average_crust_thickness * si::meter * si::meter, 
                  0, int(rock::GrainType::unweathered_extrusive)), // mafic
                rock::Mineral(0.0*si::kilogram, 0) // felsic
              }
            ))));

      // rifting and subduction
      for (std::size_t i(0); i < P; ++i)
      {

        // find rifting and subduction zones
        // note where we use locals and localized, this is important!
        // locals preserves original cell boundaries without distortion from resampling, but only carries info on a single plate
        // localized carries info on the plates that share a cell, but is distorted from resampling
        predicates.exists(i, locals[i], exists); 
        predicates.foundering(mantle_density, locals[i], foundering);
        predicates.ownable(i, localized[i], ownable);
        predicates.rifting(fine, ownable, exists, rifting, bools_scratch);
        predicates.top(i, localized[i], top);
        predicates.below(i, localized[i], below);
        predicates.accompanied(i, mantle_density, localized[i], accompanied);
        predicates.detaching(fine, accompanied, below, foundering, exists, detaching, bools_scratch);

        // // apply rifting and subduction
        crust_ops.ternary(fine, rifting, fresh_crust, plates[i], plates[i]);
        crust_ops.ternary(fine, detaching, empty_crust, plates[i], plates[i]);

        /*
        Q: Why don't we determine rifting on master, then localize the result to each plate?
        A: We are trying to minimize the number of out-of-order traversals through memory,
           since that is the limiting factor to the performance of the application.
           Each localization is an out-of-order operation. Calculating rifting is in-order.
           We're building an application that doesn't just calculate rifting, but many things, almost always in-order.
           If all those things were localized to each plate, it would significantly affect performance.
           However since those things are almost always calculated strictly from density and thickness,
           we can localize density and thickness and then calculate things on the localization for a performance gain.
        */

        // // PLATE COUNT:
        // place_counts(localized[i], buffer_scalars2_i);
        // colorscale_state.max_color_value = 8.0;
        // colorscale_state.min_color_value = 0.0;

        // // PLATE ID:
        // copy(procedural::uniform(i), buffer_scalars2_i);
        // colorscale_state.max_color_value = 8.0;
        // colorscale_state.min_color_value = 0.0;

        copy(exists, buffer_exists_i);

        // // DETACHING:
        // copy(below, buffer_scalars2_i);
        // colorscale_state.min_color_value = 0.0f;
        // colorscale_state.max_color_value = 1.0f;

        // // THICKNESS:
        // thicknesses(locals[i], lengths_i);
        // arithmetic.divide(lengths_i, procedural::uniform(length(si::kilometer)), buffer_scalars2_i);
        // colorscale_state.min_color_value = order.min(buffer_scalars2_i, oo);
        // colorscale_state.max_color_value = order.max(buffer_scalars2_i,-oo);

        // // DENSITY:
        // densities(locals[i], densities_i);
        // arithmetic.divide(densities_i, procedural::uniform(density(si::kilogram/si::meter3)), buffer_scalars2_i);
        // colorscale_state.min_color_value = order.min(buffer_scalars2_i, oo);
        // colorscale_state.max_color_value = order.max(buffer_scalars2_i,-oo);

        // DISPLACEMENT:
        displacements(locals[i], lengths_i);
        arithmetic.divide(lengths_i, procedural::uniform(length(si::kilometer)), buffer_scalars2_i);
        colorscale_state.min_color_value = order.min(buffer_scalars2_i, oo);
        colorscale_state.max_color_value = order.max(buffer_scalars2_i,-oo);

        /*
        This demo shows the buoyancy field for each plate 
        as plates rotate according to angular velocities
        that were calculated from the buoyancy field.
        If all works as planned, the plates will move 
        so that overlaps form in the areas of low buoyancy (blue).
        */

        colorscale_program.draw(
          buffer_positions,   // position
          buffer_scalars2_i,  // color value
          buffer_uniform,     // displacement
          buffer_uniform,     // darken
          buffer_exists_i,    // culling
          buffer_element_vertex_ids,
          colorscale_state,
          mat4(orientations[i]),
          view_state,
          GL_TRIANGLES
        );
      }

      // put stuff we've been drawing onto the display
      glfwSwapBuffers(window);

      std::queue<messages::Message> message_poll = message_queue.poll();
      while (!message_poll.empty())
      {
        update::OrbitalControlUpdater::update(control_state, message_poll.front(), control_state);
        message_poll.pop();
      }
      // control_state.angular_position.x += 1.0f * 3.1415926f/180.0f;
      view_state.view_matrix = control_state.get_view_matrix();
  }
  auto stop = std::chrono::high_resolution_clock::now();
  std::cout << std::to_string(duration_cast<std::chrono::milliseconds>(stop-start).count()) << std::endl;

  // close GL context and any other GLFW resources
  message_queue.deactivate(window);
  glfwTerminate();
  return 0;
}


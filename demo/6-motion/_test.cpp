
// std libraries
#include <iostream>
#include <string>
#include <vector>
#include <functional>

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

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp>  // dymaxion::VertexDownsamplingIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/unlayered/SeedBasedFloodFilling.hpp>     // unlayered::SeedBasedFloodFilling
#include <raster/unlayered/NeighborBasedFloodFilling.hpp> // unlayered::NeighborBasedFloodFilling
#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings
#include <raster/unlayered/Morphology.hpp>          // unlayered::Morphology

#include <model/rock/formation/estimated/EarthlikeIgneousFormationGeneration.hpp>
#include <model/rock/column/ColumnSummaryProperties.hpp>  // ColumnProperties
#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/formation/Formation.hpp>       // Formation
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/crust/Crust.hpp>  // Crust
#include <model/rock/crust/CrustSummarization.hpp>  // CrustSummarization
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustSummaryProperties.hpp>  // CrustProperties
#include <model/rock/crust/CrustMotion.hpp>         // CrustMotion
#include <model/rock/crust/CrustFracturing.hpp>     // CrustFracturing

#include <update/OrbitalControlState.hpp>           // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // update::OrbitalControlUpdater

#include <view/ColorscaleSurfaceShaderProgram.hpp>  // view::ColorscaleSurfaceShaderProgram
#include <view/IndicatorSwarmShaderProgram.hpp>     // view::IndicatorSwarmShaderProgram
#include <view/MultichannelSurfaceShaderProgram.hpp>// view::MultichannelSurfaceShaderProgram

int main() {
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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Plate Motion", NULL, NULL);
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

  using mass = si::mass<float>;
  using density = si::density<float>;
  using length = si::length<float>;
  using pressure = si::pressure<float>;
  using viscosity = si::dynamic_viscosity<float>;
  using acceleration = si::acceleration<float>;

  using vec3 = glm::vec3;
  using mat3 = glm::mat3;
  using mat4 = glm::mat4;

  length meter(si::meter);

  length world_radius(6.371e6 * si::meter);
  density mantle_density(3000.0*si::kilogram/si::meter3);
  viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
  int vertices_per_fine_square_side(30);
  int vertices_per_coarse_square_side(vertices_per_fine_square_side/2);
  dymaxion::Grid<int,int,float> fine(world_radius/meter, vertices_per_fine_square_side);
  dymaxion::Grid<int,int,float> coarse(world_radius/meter, vertices_per_coarse_square_side);

  iterated::Identity copy;

  const int M(2);
  const int F(5);

  // GENERATE THE CRUST
  rock::EarthlikeIgneousFormationGeneration earthlike(fine, world_radius/2.0f, 0.5f, 10);
  auto generation = earthlike(12.0f, 1.1e4f);
  rock::StratumStore<M> empty_stratum;
  rock::Formation<M> empty_formation(fine.vertex_count(), empty_stratum);
  rock::Formation<M> igneous_formation(fine.vertex_count());
  copy(generation, igneous_formation);
  rock::Crust<M,F> crust{empty_formation, empty_formation, empty_formation, igneous_formation, empty_formation};

  // CALCULATE BUOYANCY
  auto age_of_world = 0.0f*si::megayear;
  std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
  };

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

  // SUMMARIZE THE CRUST
  auto crust_summarize = rock::crust_summarization<M,F>(
    formation_summarize, 
    crust_summary_ops
  );
  rock::CrustSummary crust_summary(fine.vertex_count());
  rock::FormationSummary formation_summary(fine.vertex_count());
  int plate_id(1);
  crust_summarize(fine, plate_id, crust, crust_summary, formation_summary);
  crust_summary_ops.flatten(crust_summary, formation_summary);
  // formation_summarize(fine, plate_id, igneous_formation, formation_summary);

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

  // GENERATE PLATE MAP FROM FRACTURE STATES
  std::vector<int> coarse_plate_map(coarse.vertex_count());
  fracturing.map(fractures, coarse_plate_map);

  // UPSAMPLE THE PLATE MAP
  iterated::Index index;
  std::vector<int> fine_plate_map(fine.vertex_count());
  index(coarse_plate_map, vertex_downsampling_ids, fine_plate_map);

  auto id_strings = spheroidal::Strings(
    adapted::IdStrings<float>(adapted::dotshades), 
    aggregated::Order<adapted::SymbolicOrder>()
  );
  auto si_strings = spheroidal::Strings(
    adapted::SiStrings(adapted::dotshades), 
    aggregated::Order<adapted::SymbolicOrder>()
  );
  std::cout << id_strings.format(fine, fine_plate_map) << std::endl;
  std::cout << si_strings.format(fine, fine_buoyancy_pressure) << std::endl;

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
  std::vector<vec3> angular_velocities_in_seconds(plate_count);
  std::vector<mat3> orientations(plate_count, mat3(1));
  for (int i = 0; i < plate_count; ++i)
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
  }

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,int,float> grids(vertices_per_fine_square_side);
  dymaxion::VertexPositions fine_vertex_positions(fine);
  std::vector<length> displacements(fine.vertex_count());
  std::vector<float> buffer_color_values(fine.vertex_count());
  std::vector<std::uint8_t> buffer_exists(fine.vertex_count(), std::uint8_t(1));
  std::vector<float> buffer_scalars1(fine.vertex_count());
  std::vector<float> buffer_scalars2(fine.vertex_count());
  std::vector<float> buffer_uniform(fine.vertex_count(), 1.0f);
  std::vector<glm::vec3> buffer_positions(fine.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(fine_vertex_positions));
  std::cout << "vertex count:        " << fine.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << fine.vertices_per_meridian() << std::endl;
  // copy(vertex_colored_scalars, buffer_color_values);
  std::vector<float> scratch(fine.vertex_count());
  std::vector<bool> mask1(fine.vertex_count());
  std::vector<bool> mask2(fine.vertex_count());
  std::vector<bool> mask3(fine.vertex_count());

  // copy(vertex_colored_scalars, buffer_color_values);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(fine_vertex_positions, buffer_positions);
  grids.storeTriangleStrips(procedural::range<unsigned int>(coarse.vertex_count()), buffer_element_vertex_ids);

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

  iterated::Unary displacement_for_formation_summary(
    rock::StratumSummaryIsostaticDisplacement{
      density(3300.0*si::kilogram/si::meter3)
    }
  );

  const float pi(3.1415926535);
  while(!glfwWindowShouldClose(window)) {

      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // colorscale_state.max_color_value = -10.0;
      // colorscale_state.min_color_value = 16.0;

      // colorscale_state.max_color_value = 0.0;
      // colorscale_state.min_color_value = 8.0;

      colorscale_state.max_color_value = order.min(fine_buoyancy_pressure)/pressure(si::pascal);
      colorscale_state.min_color_value = order.max(fine_buoyancy_pressure)/pressure(si::pascal);

      for (std::size_t i(0); i < plate_count; ++i)
      {
        orientations[i] = 
          glm::rotate(
            mat4(orientations[i]),
            float(si::kiloyear/si::second) * 2.0f*pi * glm::length(angular_velocities_in_seconds[i]), 
            glm::normalize(angular_velocities_in_seconds[i]));
      }

      for (std::size_t i(0); i < plate_count; ++i)
      {
        fracturing.exists(fine_plate_map, i, buffer_exists);
        displacement_for_formation_summary(formation_summary, displacements);
        // arithmetic.divide(displacements, procedural::uniform(length(si::kilometer)), buffer_scalars2);
        arithmetic.divide(fine_buoyancy_pressure, procedural::uniform(pressure(si::pascal)), buffer_scalars2);
        // copy(fine_plate_map, buffer_scalars2);

        /*
        This demo shows the buoyancy field for each plate 
        as plates rotate according to angular velocities
        that were calculated from the buoyancy field.
        If all works as planned, the plates will move 
        so that overlaps form in the areas of low buoyancy (blue).
        */

        colorscale_program.draw(
          buffer_positions, // position
          buffer_scalars2,  // color value
          buffer_uniform,   // displacement
          buffer_uniform,   // darken
          buffer_exists,   // culling
          buffer_element_vertex_ids,
          colorscale_state,
          mat4(orientations[i]),
          view_state,
          GL_TRIANGLE_STRIP
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

  // close GL context and any other GLFW resources
  message_queue.deactivate(window);
  glfwTerminate();
  return 0;
}


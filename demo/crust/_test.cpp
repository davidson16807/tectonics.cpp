
// std libraries
#include <iostream>
#include <string>

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
#include <index/glm/known.hpp>                      // greaterThan
#include <index/known.hpp>                          // greaterThan
#include <index/whole.hpp>                          // max, mean
#include <index/procedural/Range.hpp>                   // Range
#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings

#include <model/rock/estimated/EarthlikeIgneousFormationGeneration.hpp>
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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Terrain", NULL, NULL);
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
  using force = si::force<float>;
  using viscosity = si::dynamic_viscosity<float>;
  using acceleration = si::acceleration<float>;

  length meter(si::meter);

  length world_radius(6.371e6 * si::meter);
  density mantle_density(3000.0*si::kilogram/si::meter3);
  viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
  int vertices_per_square_side(32);
  dymaxion::Grid grid(world_radius/meter, vertices_per_square_side);
  dymaxion::VertexPositions vertex_positions(grid);
  dymaxion::VertexNormals vertex_normals(grid);

  iterated::Identity copy;

  const int M(2);
  const int F(5);

  rock::EarthlikeIgneousFormationGeneration earthlike(grid, world_radius/2.0f, 0.5f, 10, world_radius);
  auto generation = earthlike(12.0f, 1.1e4f);

  rock::StratumStore<M> empty_stratum;
  rock::Formation<M> empty_formation(grid.vertex_count(), empty_stratum);
  rock::Formation<M> igneous_formation(grid.vertex_count());
  // copy(generation(12.0f, 1.1e4f), igneous_formation);
  copy(generation, igneous_formation);
  rock::Crust<M,F> crust{empty_formation, empty_formation, empty_formation, igneous_formation, empty_formation};

  auto age_of_world = 0.0f*si::megayear;
  std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
  };


  rock::CrustSummaryOps crust_summary_ops{
    rock::ColumnSummaryOps{
      rock::StratumSummaryOps{density(3300.0*si::kilogram/si::meter3)}, 
      length(si::centimeter)
    }
  };

  auto formation_summarize = rock::formation_summarization<2>(
    rock::stratum_summarization<2>(
      rock::AgedStratumDensity{densities_for_age, age_of_world},
      mass(si::tonne)
    ), 
    grid,
    world_radius
  );

  auto crust_summarize = rock::crust_summarization<M,F>(
    formation_summarize, 
    crust_summary_ops
  );
  unlayered::VectorCalculusByFundamentalTheorem calculus;
  auto motion = rock::crust_motion<M>(
      calculus, grid, 
      world_radius, 
      acceleration(si::standard_gravity), 
      mantle_density, 
      mantle_viscosity
  );
  iterated::Unary displacements_for_formation_summary(
    // rock::StratumSummaryThickness{}
    rock::StratumSummaryIsostaticDisplacement{
      density(3300.0*si::kilogram/si::meter3)
    }
  );

  rock::CrustSummary crust_summary(grid.vertex_count());
  rock::FormationSummary formation_summary(grid.vertex_count());
  std::vector<force> buoyancy(grid.vertex_count());
  std::vector<length> actual_displacements(grid.vertex_count());
  std::vector<float> vertex_scalars1(grid.vertex_count());

  int plate_id(1);
  // crust_summarize(plate_id, crust, crust_summary, formation_summary);
  // crust_summary_ops.flatten(crust_summary, formation_summary);
  formation_summarize(plate_id, igneous_formation, formation_summary);
  motion.buoyancy(formation_summary, buoyancy);
  displacements_for_formation_summary(formation_summary, actual_displacements);

  iterated::Arithmetic arithmetic(adapted::SymbolicArithmetic(length(0),length(1)));
  arithmetic.divide(actual_displacements, procedural::uniform(length(1)), vertex_scalars1);

  adapted::SymbolicOrder suborder;
  adapted::SiStrings substrings;
  aggregated::Order ordered(suborder);
  auto strings = spheroidal::Strings(substrings, ordered);
  std::cout << strings.format(grid, actual_displacements) << std::endl << std::endl;

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(vertices_per_square_side);
  std::vector<float> buffer_color_values(grid.vertex_count());
  std::vector<float> buffer_scalars2(grid.vertex_count());
  std::vector<float> buffer_uniform(grid.vertex_count(), 1.0f);
  std::vector<glm::vec3> buffer_positions(grid.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(vertex_positions));
  std::cout << "vertex count:        " << grid.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << grid.vertices_per_meridian() << std::endl;
  // copy(vertex_colored_scalars, buffer_color_values);
  std::vector<float> scratch(grid.vertex_count());
  std::vector<bool> mask1(grid.vertex_count());
  std::vector<bool> mask2(grid.vertex_count());
  std::vector<bool> mask3(grid.vertex_count());

  // copy(vertex_colored_scalars, buffer_color_values);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(vertex_positions, buffer_positions);
  grids.storeTriangleStrips(procedural::range<unsigned int>(grid.vertex_count()), buffer_element_vertex_ids);

  // initialize control state
  update::OrbitalControlState control_state;
  control_state.min_zoom_distance = 1.0f;
  control_state.log2_height = 2.5f;
  control_state.angular_position = glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f;
  
  // initialize view state
  view::ViewState view_state;
  view_state.projection_matrix = glm::perspective(
    3.14159f*45.0f/180.0f, 
    850.0f/640.0f, 
    1e-3f, 1e16f
  );
  view_state.view_matrix = control_state.get_view_matrix();
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);
  view::ColorscaleSurfacesViewState colorscale_state;
  colorscale_state.max_color_value = whole::max(buffer_color_values);
  colorscale_state.darken_threshold = whole::mean(buffer_scalars2);

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(vertex_scalars1) << std::endl;
  std::cout << whole::max(vertex_scalars1) << std::endl;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      colorscale_program.draw(
        buffer_positions,    // position
        vertex_scalars1, // color value
        buffer_uniform,      // displacement
        buffer_uniform,      // darken
        buffer_uniform,      // culling
        buffer_element_vertex_ids,
        colorscale_state,
        view_state,
        GL_TRIANGLE_STRIP
      );

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

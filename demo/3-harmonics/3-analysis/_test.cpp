
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
#include <index/whole.hpp>                          // max, mean
#include <index/procedural/Range.hpp>                   // Range
#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/scalar/ScalarMetric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Statistics.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Metric.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings
#include <raster/spheroidal/HarmonicAnalysis.hpp>   // spheroidal::HarmonicAnalysis

// #include <model/rock/stratum/StratumGenerator.hpp>  // StratumGenerator

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Harmonic Analysis", NULL, NULL);
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
  float radius(2.0f);
  int vertices_per_square_side(32);
  dymaxion::Grid<int,int,float> grid(radius, vertices_per_square_side);
  dymaxion::VertexPositions vertex_positions(grid);
  dymaxion::VertexNormals vertex_normals(grid);
  dymaxion::VertexDualAreas vertex_dual_areas(grid);

  auto vertex_square_ids = dymaxion::square_ids(grid);

  // auto vertex_colored_scalars = procedural::range();

  std::vector<float> vertex_colored_scalars(grid.vertex_count());
  for (int i = 0; i < grid.vertex_count(); ++i)
  {
    vertex_colored_scalars[i] = grid.memory.memory_id(grid.memory.grid_id(i));
    // vertex_colored_scalars[i] = grid.memory.memory_id(grid.memory.grid_id(i)+glm::ivec2(10,10));
    // vertex_colored_scalars[i] = (grid.vertex_position(i).z);
  }

  iterated::Identity copy;


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

  auto rfbm = field::ranked_fractal_brownian_noise<3>(10, 0.5f, 2.0f/radius, 12.0f, 1.1e4f);

  auto elevation_meters_for_position = field::compose(hypsometry_cdfi, rfbm);

  auto elevation_in_meters = procedural::map(elevation_meters_for_position, vertex_positions);

  auto analysis = spheroidal::harmonic_analysis<double,5>(
    vertex_positions,
    vertex_dual_areas
  );

  auto vertex_scalars1 = analysis.compose(
    analysis.decompose(elevation_in_meters)
  );

  // auto vertex_scalars1 = elevation_in_meters;

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,int,float> grids(vertices_per_square_side);
  std::vector<float> buffer_color_values(grid.vertex_count());
  std::vector<float> buffer_square_ids(grid.vertex_count());
  std::vector<float> buffer_scalars2(grid.vertex_count());
  std::vector<float> buffer_scalars1(grid.vertex_count());
  std::vector<float> buffer_uniform(grid.vertex_count(), 1.0f);
  std::vector<std::uint8_t>  buffer_culling(grid.vertex_count(), std::uint8_t(1));
  std::vector<glm::vec3> buffer_positions(grid.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(vertex_positions));
  std::cout << "vertex count:        " << grid.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << grid.vertices_per_meridian() << std::endl;
  // copy(vertex_colored_scalars, buffer_color_values);
  std::vector<float> scratch(grid.vertex_count());
  std::vector<bool> mask1(grid.vertex_count());
  std::vector<bool> mask2(grid.vertex_count());
  std::vector<bool> mask3(grid.vertex_count());

  // auto metric = iterated::Metric{adapted::ScalarMetric<float>{}};

  // copy(vertex_colored_scalars, buffer_color_values);
  copy(vertex_square_ids, buffer_square_ids);
  copy(vertex_scalars1, buffer_scalars1);
  // metric.distance(elevation_in_meters, vertex_scalars1, buffer_scalars1);
  copy(elevation_in_meters, buffer_scalars2);
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
  colorscale_state.max_color_value = whole::max(vertex_scalars1);
  colorscale_state.min_color_value = whole::min(vertex_scalars1);

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  adapted::SymbolicOrder suborder;
  adapted::ScalarStrings<float> substrings;
  aggregated::Order ordered(suborder);
  spheroidal::Strings strings(substrings, ordered);
  std::cout << strings.format(grid, vertex_scalars1) << std::endl << std::endl;
  std::cout << strings.format(grid, elevation_in_meters) << std::endl << std::endl;

  std::cout << whole::min(buffer_scalars1) << std::endl;
  std::cout << whole::max(buffer_scalars1) << std::endl;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // colorscale_program.draw(
      //   buffer_positions,    // position
      //   buffer_scalars1, // color value
      //   buffer_uniform,      // displacement
      //   buffer_scalars2,      // darken
      //   buffer_culling,      // culling
      //   buffer_element_vertex_ids,
      //   colorscale_state,
      //   view_state,
      //   GL_TRIANGLE_STRIP
      // );

      debug_program.draw(
        buffer_positions,
        // buffer_color_values, // red
        buffer_scalars2, // red
        buffer_scalars1, // green
        std::vector<float>(grid.vertex_count(), 0.0f), // blue
        std::vector<float>(grid.vertex_count(), 1.0f), // opacity
        std::vector<float>(grid.vertex_count(), 0.0f), // displacement
        buffer_element_vertex_ids,
        glm::vec4(-10000.0f, -10000.0f, 0.0f, 0.0f),
        glm::vec4( 10000.0f,  10000.0f, 1.0f, 1.0f),
        glm::mat4(1),
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

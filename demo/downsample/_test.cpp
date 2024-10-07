
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
#include <index/series/Map.hpp>
#include <index/series/Uniform.hpp>
#include <index/glm/each.hpp>                       // get
#include <index/each.hpp>                           // get
#include <index/glm/known.hpp>                      // greaterThan
#include <index/known.hpp>                          // greaterThan
#include <index/whole.hpp>                          // max, mean
#include <index/series/Range.hpp>                   // Range
#include <index/series/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/series/noise/glm/UnitVectorNoise.hpp>
#include <index/series/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/scalar/ScalarClosedForm.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/aggregated/Order.hpp>
#include <index/grouped/Statistics.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp>    // dymaxion::VertexDownsamplingIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Downsample", NULL, NULL);
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
  int fine_vertices_per_square_side(32);
  dymaxion::Grid fine(radius, fine_vertices_per_square_side);
  dymaxion::Grid coarse(radius, fine_vertices_per_square_side/8);
  dymaxion::VertexPositions fine_vertex_positions(fine);
  dymaxion::VertexPositions coarse_vertex_positions(coarse);
  dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.memory, coarse.memory);

  float min_elevation(-16000.0f);
  float max_elevation( 16000.0f);

  iterated::Identity copy;
  iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic<float>(0.0f, 1.0f)};
  grouped::Statistics statistics{adapted::SymbolicArithmetic<float>(0.0f, 1.0f)};

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
  auto fine_elevation_meters = series::map(elevation_meters_for_position, fine_vertex_positions);

  std::vector<float> coarse_elevation_meters(coarse.vertex_count());
  statistics.sum(vertex_downsampling_ids, fine_elevation_meters, coarse_elevation_meters);
  arithmetic.divide(coarse_elevation_meters, series::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_elevation_meters);

  adapted::ScalarStrings<float> substrings;
  aggregated::Order ordered(adapted::SymbolicOrder{});
  auto strings = spheroidal::Strings(substrings, ordered);
  std::cout << strings.format(fine, fine_elevation_meters) << std::endl << std::endl;
  std::cout << strings.format(coarse, coarse_elevation_meters) << std::endl << std::endl;

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(coarse.vertices_per_square_side());
  std::vector<float> buffer_color_values(coarse.vertex_count());
  std::vector<float> buffer_square_ids(coarse.vertex_count());
  std::vector<float> buffer_scalars2(coarse.vertex_count());
  std::vector<float> buffer_scalars1(coarse.vertex_count());
  std::vector<float> buffer_uniform(coarse.vertex_count(), 1.0f);
  std::vector<glm::vec3> buffer_positions(coarse.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(coarse_vertex_positions));
  std::cout << "vertex count:        " << coarse.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << coarse.vertices_per_meridian() << std::endl;
  std::vector<float> scratch(coarse.vertex_count());
  std::vector<bool> mask1(coarse.vertex_count());
  std::vector<bool> mask2(coarse.vertex_count());
  std::vector<bool> mask3(coarse.vertex_count());

  copy(coarse_elevation_meters, buffer_scalars1);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(coarse_vertex_positions, buffer_positions);
  grids.storeTriangleStrips(series::range<unsigned int>(coarse.vertex_count()), buffer_element_vertex_ids);

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
  colorscale_state.max_color_value = whole::max(buffer_scalars1);
  colorscale_state.min_color_value = whole::min(buffer_scalars1);
  colorscale_state.darken_threshold = whole::mean(buffer_scalars2);

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(buffer_scalars1) << std::endl;
  std::cout << whole::max(buffer_scalars1) << std::endl;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      colorscale_program.draw(
        buffer_positions, // position
        buffer_scalars1,  // color value
        buffer_uniform,   // displacement
        buffer_uniform,   // darken
        buffer_uniform,   // culling
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


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
#include <index/adapted/si/SiStrings.hpp>
#include <index/adapted/glm/GlmStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/metric/MetricOrder.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/known.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/GridSeries.hpp>                 // dymaxion::BufferVertexIds
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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Gradient", NULL, NULL);
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
  float radius(3.0f);
  int vertices_per_square_side(16);
  dymaxion::Grid grid(radius, vertices_per_square_side);
  dymaxion::VertexPositions vertex_positions(grid);
  dymaxion::VertexNormals vertex_normals(grid);

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

  using length = si::length<float>;
  auto min_earth_elevation = -10924.0 * si::meter;

  auto elevation_for_position = 
      field::compose(
          relation::ScalarRelation(1.0f, length(si::meter), hypsometry_cdfi),
          rfbm);

  auto elevation_in_meters = procedural::map(elevation_meters_for_position, vertex_positions);

  iterated::Unary elevations_for_positions(elevation_for_position);
  std::vector<length> elevation(grid.vertex_count());
  elevations_for_positions(vertex_positions, elevation);

  iterated::Arithmetic lengths(adapted::TypedSymbolicArithmetic(length(0),length(1)));
  lengths.subtract(elevation, procedural::uniform(length(min_earth_elevation)), elevation);

  auto vertex_scalars1 = elevation_in_meters;

  // auto vertex_directions = known::store(
  //     grid.vertex_count(),
  //     procedural::map(
  //         field::vector3_zip(
  //             field::elias_noise<float>(
  //                     procedural::unit_vector_noise<3>(10.0f, 1.0e4f), 
  //                     procedural::gaussian(11.0f, 1.1e4f), 
  //                     1000),
  //             field::elias_noise<float>(
  //                     procedural::unit_vector_noise<3>(11.0f, 1.1e4f), 
  //                     procedural::gaussian(12.0f, 1.2e4f), 
  //                     1000),
  //             field::elias_noise<float>(
  //                     procedural::unit_vector_noise<3>(12.0f, 1.2e4f), 
  //                     procedural::gaussian(13.0f, 1.3e4f), 
  //                     1000)
  //         ),
  //         dymaxion::vertex_positions(grid)
  //     )
  // );

  iterated::Identity copy;
  iterated::Metric metric{adapted::GlmMetric{}};

  unlayered::VectorCalculusByFundamentalTheorem spatial;
  std::vector<glm::vec3> vertex_gradient(grid.vertex_count());
  spatial.gradient(grid, vertex_scalars1, vertex_gradient);
  // metric.normalize(vertex_gradient, vertex_gradient);

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(vertices_per_square_side);
  std::vector<float> buffer_color_values(grid.vertex_count());
  std::vector<float> buffer_scalars2(grid.vertex_count());
  std::vector<float> buffer_scalars1(grid.vertex_count());
  std::vector<float> buffer_uniform(grid.vertex_count(), 1.0f);
  // std::vector<std::byte>  buffer_culling(grid.vertex_count(), std::byte(0));
  std::vector<glm::vec3> buffer_positions(grid.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(vertex_positions));
  std::cout << "vertex count:        " << grid.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << grid.vertices_per_meridian() << std::endl;
  std::vector<float> scratch(grid.vertex_count());
  std::vector<bool> mask1(grid.vertex_count());
  std::vector<bool> mask2(grid.vertex_count());
  std::vector<bool> mask3(grid.vertex_count());

  copy(vertex_scalars1, buffer_scalars1);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(vertex_positions, buffer_positions);
  grids.storeTriangleStrips(procedural::range<unsigned int>(grid.vertex_count()), buffer_element_vertex_ids);

  // flatten vector raster for OpenGL
  iterated::Arithmetic scalars(adapted::TypedSymbolicArithmetic(0.0f, 1.0f));
  buffer::PyramidBuffers<int, float> pyramids;
  std::vector<glm::vec3> vectors_element_position(pyramids.triangles_size<3>(3));
  std::vector<glm::vec3> vectors_instance_position(grid.vertex_count());
  std::vector<glm::vec3> vectors_instance_heading(grid.vertex_count());
  std::vector<glm::vec3> vectors_instance_up(grid.vertex_count());
  std::vector<glm::vec4> vectors_instance_color(grid.vertex_count(), glm::vec4(1.0f));
  std::vector<float> vectors_instance_scale(grid.vertex_count());
  float pyramid_radius(grid.total_circumference()/(8.0*grid.vertices_per_meridian()));
  float pyramid_halflength(2.5f*pyramid_radius);
  pyramids.storeTriangles(
      glm::vec3(0,0,-1) * pyramid_halflength, 
      glm::vec3(0,0, 1) * pyramid_halflength, 
      glm::vec3(1,0, 0),  pyramid_radius, 3, 
      vectors_element_position);
  copy   (known::mult(vertex_positions, procedural::uniform(1+pyramid_halflength/grid.total_radius())),  vectors_instance_position);
  copy   (vertex_gradient,   vectors_instance_heading);
  // copy   (procedural::uniform(glm::vec3(0,0,1)),   vectors_instance_heading);
  copy   (vertex_normals,    vectors_instance_up);
  metric.length (vertex_gradient,   vectors_instance_scale);
  scalars.divide(vectors_instance_scale, procedural::uniform(whole::max(vectors_instance_scale)), vectors_instance_scale);

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

  adapted::SymbolicOrder suborder;
  adapted::SiStrings substrings;
  aggregated::Order ordered(suborder);
  auto strings = spheroidal::Strings(substrings, ordered);
  std::cout << strings.format(grid, elevation) << std::endl << std::endl;

  adapted::GlmStrings substrings3;
  spheroidal::Strings strings3(substrings3, aggregated::Order{adapted::MetricOrder{adapted::GlmMetric{}}});
  std::cout << strings3.format(grid, vertex_gradient) << std::endl << std::endl;

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::IndicatorSwarmShaderProgram indicator_program;  
  view::MultichannelSurfaceShaderProgram debug_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(buffer_scalars1) << std::endl;
  std::cout << whole::max(buffer_scalars1) << std::endl;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      debug_program.draw(
        buffer_positions,
        // buffer_color_values, // red
        std::vector<float>(grid.vertex_count(), 0.0f), // red
        buffer_scalars1, // green
        std::vector<float>(grid.vertex_count(), 0.0f), // blue
        std::vector<float>(grid.vertex_count(), 1.0f), // opacity
        std::vector<float>(grid.vertex_count(), 0.0f), // displacement
        buffer_element_vertex_ids,
        glm::vec4(0.0f, -10000.0f, 0.0f, 0.0f),
        glm::vec4(0.0f,  10000.0f, 1.0f, 1.0f),
        glm::mat4(1),
        view_state,
        GL_TRIANGLE_STRIP
      );

      // colorscale_program.draw(
      //   buffer_positions,    // position
      //   buffer_color_values,   // color value
      //   buffer_uniform,      // displacement
      //   buffer_uniform,      // darken
      //   buffer_culling,      // culling
      //   buffer_element_vertex_ids,
      //   colorscale_state,
      //   view_state,
      //   GL_TRIANGLE_STRIP
      // );

      indicator_program.draw(
        vectors_element_position,
        vectors_instance_position,
        vectors_instance_heading,
        vectors_instance_up,
        vectors_instance_scale,
        vectors_instance_color,
        glm::mat4(1),
        view_state,
        GL_TRIANGLES
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

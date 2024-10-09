
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
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/aggregated/Order.hpp>
#include <index/grouped/Statistics.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Bitset.hpp>
#include <index/iterated/Order.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp>    // dymaxion::VertexDownsamplingIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/unlayered/Morphology.hpp>          // unlayered::Morphology
#include <raster/unlayered/FloodFilling.hpp>        // unlayered::FloodFilling
#include <raster/unlayered/Voronoi.hpp>             // unlayered::Voronoi
#include <raster/unlayered/ImageSegmentation.hpp>   // unlayered::ImageSegmentation
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
  using vec3 = glm::vec3;
  float radius(2.0f);
  int fine_vertices_per_square_side(30);
  dymaxion::Grid fine(radius, fine_vertices_per_square_side);
  dymaxion::Grid coarse(radius, fine_vertices_per_square_side/3);
  dymaxion::VertexPositions fine_vertex_positions(fine);
  dymaxion::VertexPositions coarse_vertex_positions(coarse);
  dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.memory, coarse.memory);

  float min_elevation(-16000.0f);
  float max_elevation( 16000.0f);

  iterated::Identity copy;
  iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic<float>(0.0f, 1.0f)};
  grouped::Statistics stats{adapted::SymbolicArithmetic<float>(0.0f, 1.0f)};

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
  std::vector<vec3> vertex_gradient(coarse.vertex_count());
  std::vector<float> scratch(coarse.vertex_count());
  std::vector<bool> mask1(coarse.vertex_count());
  std::vector<bool> mask2(coarse.vertex_count());
  std::vector<bool> mask3(coarse.vertex_count());
  std::vector<std::uint8_t> similar_plate_id(coarse.vertex_count());
  std::vector<bool> is_undecided(coarse.vertex_count());
  std::vector<bool> is_there(coarse.vertex_count());

  std::vector<float> coarse_elevation_meters(coarse.vertex_count());
  stats.sum(vertex_downsampling_ids, fine_elevation_meters, coarse_elevation_meters);
  arithmetic.divide(coarse_elevation_meters, series::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_elevation_meters);

  iterated::Order order{adapted::SymbolicOrder{}};
  aggregated::Order ordered(adapted::SymbolicOrder{});

  unlayered::VectorCalculusByFundamentalTheorem calculus;
  auto fill = unlayered::flood_filling<int,float>(
    [](auto U, auto V){ return math::similarity (U,V) > std::cos(M_PI * 60.0f/180.0f); }
  );
  auto segment = unlayered::image_segmentation<int,float>(fill, adapted::GlmMetric{});

  std::uint8_t plate_count(8);

  calculus.gradient(coarse, coarse_elevation_meters, vertex_gradient);

  if (true)
  {
    copy(series::uniform(0), similar_plate_id);
    similar_plate_id[0] = 1;
  }
  else
  {
    segment(
      coarse, vertex_gradient, plate_count-1, 10, 
      similar_plate_id, scratch, mask1, mask2, mask3
    );
  }

  auto ternary = iterated::Ternary{};
  auto bitset = iterated::Bitset{adapted::BooleanBitset{}};
  auto morphology = unlayered::Morphology{bitset};

  if(true){
    for(std::uint8_t j(0); j < 2; ++j)
    {
      for (std::uint8_t i(0); i < plate_count; ++i)
      {
          order.equal(similar_plate_id, series::uniform(i), is_there);
          order.equal(similar_plate_id, series::uniform(0), is_undecided);
          morphology.dilate(coarse, is_there, mask1);
          morphology.dilate(coarse, mask1, is_there);
          bitset.intersect(is_undecided, is_there, is_there);
          ternary(is_there, series::uniform(i), similar_plate_id, similar_plate_id);
      }
    }
  }

  if(false){
    grouped::Statistics stats3{adapted::SymbolicArithmetic(vec3(0),vec3(1))};
    iterated::Metric metric{adapted::GlmMetric{}};
    auto voronoi = unlayered::Voronoi{adapted::GlmMetric{}};

    std::vector<std::uint8_t> nearest_plate_id(coarse.vertex_count());
    std::vector<vec3>plate_seeds(8,vec3(0,0,0));
    stats3.sum(similar_plate_id, coarse_vertex_positions, plate_seeds);
    metric.normalize(plate_seeds, plate_seeds);
    voronoi(coarse_vertex_positions, plate_seeds, nearest_plate_id);
    order.equal(similar_plate_id, series::uniform(0), is_undecided);
    ternary(is_undecided, nearest_plate_id, similar_plate_id, similar_plate_id);
  }

  adapted::ScalarStrings<float> substrings;
  auto strings = spheroidal::Strings(substrings, ordered);
  std::cout << strings.format(coarse, similar_plate_id) << std::endl << std::endl;
  morphology.dilate  (coarse, is_there, mask1, 5, mask2);

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

  copy(similar_plate_id, buffer_scalars1);
  copy(coarse_elevation_meters, buffer_scalars2);
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
        buffer_scalars2,   // darken
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

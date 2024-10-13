
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
#include <index/adapted/scalar/ScalarClosedForm.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/scalar/IdStrings.hpp>
#include <index/adapted/glm/GlmStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/adapted/metric/MetricOrder.hpp>
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
  GLFWwindow* window = glfwCreateWindow(1133, 850, "Hello Downsample", NULL, NULL);
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
  dymaxion::Grid coarse(radius, fine_vertices_per_square_side/2);
  dymaxion::VertexPositions fine_vertex_positions(fine);
  dymaxion::VertexPositions coarse_vertex_positions(coarse);
  dymaxion::VertexNormals coarse_vertex_normals(coarse);
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
  auto fine_elevation_meters = procedural::map(elevation_meters_for_position, fine_vertex_positions);
  std::vector<vec3> vertex_gradient(coarse.vertex_count());
  std::vector<float> scratch(coarse.vertex_count());
  std::vector<bool> mask1(coarse.vertex_count());
  std::vector<bool> mask2(coarse.vertex_count());
  std::vector<bool> mask3(coarse.vertex_count());
  std::vector<std::uint8_t> similar_plate_id(coarse.vertex_count());
  std::vector<std::uint8_t> dilated_plate_id(coarse.vertex_count());
  std::vector<std::uint8_t> nearest_plate_id(coarse.vertex_count());
  std::vector<std::uint8_t> visible_plate_id(coarse.vertex_count());
  std::vector<bool> is_undecided(coarse.vertex_count());
  std::vector<bool> is_there(coarse.vertex_count());

  std::vector<float> coarse_elevation_meters(coarse.vertex_count());
  stats.sum(vertex_downsampling_ids, fine_elevation_meters, coarse_elevation_meters);
  arithmetic.divide(coarse_elevation_meters, procedural::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_elevation_meters);

  iterated::Order orders{adapted::SymbolicOrder{}};
  aggregated::Order order(adapted::SymbolicOrder{});

  unlayered::VectorCalculusByFundamentalTheorem calculus;
  auto fill = unlayered::flood_filling<int,float>(
    [](auto U, auto V){ return math::similarity (U,V) > std::cos(M_PI * 45.0f/180.0f); }
  );
  auto segment = unlayered::image_segmentation<int,float>(fill, adapted::GlmMetric{});

  std::uint8_t plate_count(8);

  calculus.gradient(coarse, coarse_elevation_meters, vertex_gradient);

  if (false)
  {
    copy(procedural::uniform(0), similar_plate_id);
    similar_plate_id[0] = 1;
  }
  else
  {
    segment(
      coarse, vertex_gradient, plate_count-1, 10, 
      similar_plate_id, scratch, mask1, mask2, mask3
    );
  }


  iterated::Ternary ternary{};
  iterated::Bitset bitset{adapted::BooleanBitset{}};
  unlayered::Morphology morphology{bitset};
  iterated::Metric metric{adapted::GlmMetric{}};

  copy(similar_plate_id, dilated_plate_id);
  if(true){
    for(std::uint8_t j(0); j < 2; ++j)
    {
      for (std::uint8_t i(0); i < plate_count; ++i)
      {
          orders.equal(similar_plate_id, procedural::uniform(i), is_there);
          orders.equal(similar_plate_id, procedural::uniform(0), is_undecided);
          morphology.dilate(coarse, is_there, mask1);
          morphology.dilate(coarse, mask1, is_there);
          bitset.intersect(is_undecided, is_there, is_there);
          ternary(is_there, procedural::uniform(i), dilated_plate_id, dilated_plate_id);
      }
    }
  }

  copy(dilated_plate_id, nearest_plate_id);
  if(false){
    grouped::Statistics stats3{adapted::SymbolicArithmetic(vec3(0),vec3(1))};
    unlayered::Voronoi voronoi{adapted::GlmMetric{}};

    std::vector<vec3>plate_seeds(8,vec3(0,0,0));
    stats3.sum(nearest_plate_id, coarse_vertex_positions, plate_seeds);
    metric.normalize(plate_seeds, plate_seeds);
    orders.equal(similar_plate_id, procedural::uniform(0), is_undecided);
    voronoi(coarse_vertex_positions, plate_seeds, nearest_plate_id);
    ternary(is_undecided, nearest_plate_id, similar_plate_id, nearest_plate_id);
  }

  adapted::ScalarStrings<float> substrings(adapted::dotshades);
  spheroidal::Strings strings(substrings, order);
  adapted::GlmStrings substrings3;
  spheroidal::Strings strings3(substrings3, aggregated::Order{adapted::MetricOrder{adapted::GlmMetric{}}});
  std::cout << strings3.format(coarse, vertex_gradient) << std::endl << std::endl;
  std::cout << strings.format(coarse, similar_plate_id) << std::endl << std::endl;
  std::cout << strings.format(coarse, dilated_plate_id) << std::endl << std::endl;
  std::cout << strings.format(coarse, nearest_plate_id) << std::endl << std::endl;

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(coarse.vertices_per_square_side());
  std::vector<float> buffer_color_values(coarse.vertex_count());
  std::vector<float> buffer_scalars2(coarse.vertex_count());
  std::vector<float> buffer_scalars1(coarse.vertex_count());
  std::vector<float> buffer_uniform(coarse.vertex_count(), 1.0f);
  std::vector<glm::vec3> buffer_positions(coarse.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(coarse_vertex_positions));
  std::cout << "vertex count:        " << coarse.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << coarse.vertices_per_meridian() << std::endl;

  copy(coarse_elevation_meters, buffer_scalars2);
  // copy(similar_plate_id, buffer_scalars2);
  copy(coarse_vertex_positions, buffer_positions);
  grids.storeTriangleStrips(procedural::range<unsigned int>(coarse.vertex_count()), buffer_element_vertex_ids);

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
  colorscale_state.darken_threshold = 0.5f;// whole::mean(buffer_scalars2);

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;
  view::IndicatorSwarmShaderProgram indicator_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(buffer_scalars1) << std::endl;
  std::cout << whole::max(buffer_scalars1) << std::endl;

  // flatten vector raster for OpenGL
  buffer::PyramidBuffers<int, float> pyramids;
  std::vector<glm::vec3> vectors_element_position(pyramids.triangles_size<3>(3));
  std::vector<glm::vec3> vectors_instance_position(coarse.vertex_count());
  std::vector<glm::vec4> vectors_instance_color(coarse.vertex_count(), glm::vec4(1.0f));
  std::vector<glm::vec3> vectors_instance_up(coarse.vertex_count());
  std::vector<float> vectors_instance_scale(coarse.vertex_count());
  float pyramid_radius(coarse.total_circumference()/(8.0*coarse.vertices_per_meridian()));
  float pyramid_halflength(2.5f*pyramid_radius);
  pyramids.storeTriangles(
      glm::vec3(0,0,-1) * pyramid_halflength, 
      glm::vec3(0,0, 1) * pyramid_halflength, 
      glm::vec3(1,0, 0),  pyramid_radius, 3, 
      vectors_element_position);
  copy          (known::mult(coarse_vertex_positions, procedural::uniform(1+pyramid_halflength/coarse.total_radius())),  vectors_instance_position);
  copy          (coarse_vertex_normals, vectors_instance_up);
  metric.length (vertex_gradient,   vectors_instance_scale);
  arithmetic.divide(vectors_instance_scale, procedural::uniform(whole::max(vectors_instance_scale)), vectors_instance_scale);

  int frame_id(0);
  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (frame_id == 0)
      {
        copy(similar_plate_id, buffer_scalars1);
      }
      else if (frame_id == 100)
      {
        copy(dilated_plate_id, buffer_scalars1);
      }
      else if (frame_id == 200)
      {
        copy(nearest_plate_id, buffer_scalars1);
      }
      frame_id = (frame_id+1)%300;

      colorscale_state.max_color_value = whole::max(buffer_scalars1);
      colorscale_state.min_color_value = whole::min(buffer_scalars1);
      colorscale_program.draw(
        buffer_positions, // position
        buffer_scalars1,  // color value
        buffer_uniform,   // displacement
        buffer_scalars2,  // darken
        buffer_uniform,   // culling
        buffer_element_vertex_ids,
        colorscale_state,
        view_state,
        GL_TRIANGLE_STRIP
      );

      indicator_program.draw(
        vectors_element_position,
        vectors_instance_position,
        vertex_gradient,
        vectors_instance_up,
        vectors_instance_scale,
        vectors_instance_color,
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


// std libraries
#include <iostream>
#include <string>
#include <random>       // rngs

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

#include <math/glm/special_specialization.hpp>
#include <math/glm/special.hpp>

// in house libraries
#include <index/series/Map.hpp>
#include <index/series/Uniform.hpp>
#include <index/series/glm/VectorInterleave.hpp>
#include <index/series/glm/VectorDeinterleave.hpp>
#include <index/glm/each.hpp>                           // get
#include <index/each.hpp>                           // get
#include <index/glm/known.hpp>                          // greaterThan
#include <index/known.hpp>                          // greaterThan
#include <index/whole.hpp>                          // max, mean
#include <index/series/Range.hpp>                   // Range

#include <index/series/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/series/noise/glm/UnitVectorNoise.hpp>
#include <index/series/noise/GaussianNoise.hpp>

#include <field/noise/ValueNoise.hpp>               // ValueNoise
#include <field/noise/PerlinNoise.hpp>              // PerlinNoise
#include <field/noise/MosaicNoise.hpp>              // MosaicNoise
#include <field/noise/EliasNoise.hpp>               // EliasNoise
#include <field/VectorZip.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/cartesian/UnboundedIndexing.hpp>     // field::UnboundedIndexing
#include <field/noise/MosaicOps.hpp>                // field::VectorMosaicOps
#include <field/noise/FractalBrownianNoise.hpp>     // dymaxion::FractalBrownianNoise

#include <grid/dymaxion/Indexing.hpp>               // dymaxion::Indexing
#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/noise/MosaicOps.hpp>        // dymaxion::MosaicOps
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/Morphology.hpp>          // unlayered::Morphology
#include <raster/unlayered/FloodFilling.hpp>        // unlayered::FloodFilling
#include <raster/unlayered/ImageSegmentation.hpp>   // unlayered::ImageSegmentation
#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/spheroidal/string_cast.hpp>        // spheroidal::to_string

#include <update/OrbitalControlState.hpp>           // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // update::OrbitalControlUpdater

#include <view/ColorscaleSurfaceShaderProgram.hpp>   // view::ColorscaleSurfaceShaderProgram
#include <view/IndicatorSwarmShaderProgram.hpp>      // view::IndicatorSwarmShaderProgram
#include <view/MultichannelSurfaceShaderProgram.hpp> // view::MultichannelSurfaceShaderProgram

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello World", NULL, NULL);
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
  int vertices_per_square_side(64);
  dymaxion::Grid grid(radius, vertices_per_square_side);
  dymaxion::VertexPositions vertex_positions(grid);
  dymaxion::VertexNormals vertex_normals(grid);

  auto vertex_square_ids = dymaxion::square_ids(grid);

  // auto vertex_colored_scalars = series::range();

  std::vector<float> vertex_colored_scalars(grid.vertex_count());
  for (int i = 0; i < grid.vertex_count(); ++i)
  {
    vertex_colored_scalars[i] = grid.memory.memory_id(grid.vertex_grid_id(i));
    // vertex_colored_scalars[i] = grid.memory.memory_id(grid.vertex_grid_id(i)+glm::ivec2(10,10));
    // vertex_colored_scalars[i] = (grid.vertex_position(i).z);
  }

  // auto vertex_colored_scalars = series::map(
  //     field::value_noise<3,float>(
  //         field::mosaic_noise(series::unit_interval_noise(11.0f, 1.1e4f)),
  //         dymaxion::mosaic_ops<int,float>()
  //     ),
  //     vertex_positions
  // );

  // auto mask = 
  //   known::greaterThan(series::uniform(0.5), 
  //         series::map(
  //             field::value_noise3<float>(
  //                 field::mosaic_noise(series::unit_interval_noise(11.0, 1.1e4)),
  //                 dymaxion::mosaic_ops<int,float>()
  //             ),
  //             dymaxion::vertex_positions(grid)
  //         )
  //     );
  // std::vector<bool> out(grid.vertex_count());
  // std::vector<bool> scratch(grid.vertex_count());
  // unlayered::Morphology morphology;
  // morphology.closing(grid, mask, out, scratch);
  // std::cout << "closing:" << std::endl;
  // std::cout << spheroidal::to_string(grid, out) << std::endl;
  // std::vector<float> vertex_scalars2(grid.vertex_count());
  // each::copy(out, vertex_scalars2);

  auto vertex_scalars1 = series::map(
      field::fractal_brownian_noise<int,float>(
          field::value_noise<3,float>(
              field::mosaic_noise(series::unit_interval_noise(11.0f, 1.1e4f), cartesian::UnboundedIndexing<int>()),
              field::vector_mosaic_ops<3,int,float>()
          ), 10, 0.5f),
      dymaxion::VertexPositions(grid)
  );

  auto vertex_scalars2 = series::map(
      field::fractal_brownian_noise<int,float>(
          field::value_noise<3,float>(
              field::mosaic_noise(series::unit_interval_noise(11.0f, 1.1e4f), cartesian::UnboundedIndexing<int>()),
              field::vector_mosaic_ops<3,int,float>()
          ), 10, 0.5f),
      dymaxion::VertexPositions(grid)
  );

  // auto vertex_directions = known::store(
  //     grid.vertex_count(),
  //     series::map(
  //         field::vector3_zip(
  //             field::elias_noise<float>(
  //                     series::unit_vector_noise<3>(10.0f, 1.0e4f), 
  //                     series::gaussian(11.0f, 1.1e4f), 
  //                     1000),
  //             field::elias_noise<float>(
  //                     series::unit_vector_noise<3>(11.0f, 1.1e4f), 
  //                     series::gaussian(12.0f, 1.2e4f), 
  //                     1000),
  //             field::elias_noise<float>(
  //                     series::unit_vector_noise<3>(12.0f, 1.2e4f), 
  //                     series::gaussian(13.0f, 1.3e4f), 
  //                     1000)
  //         ),
  //         dymaxion::vertex_positions(grid)
  //     )
  // );

  std::vector<glm::vec3> vertex_gradient(grid.vertex_count());
  unlayered::VectorCalculusByFundamentalTheorem spatial;
  spatial.gradient(grid, vertex_scalars1, vertex_gradient);

  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(vertices_per_square_side);
  std::vector<float> buffer_color_values(grid.vertex_count());
  std::vector<float> buffer_square_ids(grid.vertex_count());
  std::vector<float> buffer_scalars2(grid.vertex_count());
  std::vector<float> buffer_scalars1(grid.vertex_count());
  std::vector<float> buffer_uniform(grid.vertex_count(), 1.0f);
  std::vector<glm::vec3> buffer_positions(grid.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(vertex_positions));
  std::cout << "vertex count:        " << grid.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << grid.vertices_per_meridian() << std::endl;
  // each::copy(vertex_colored_scalars, buffer_color_values);
  std::vector<float> scratch(grid.vertex_count());
  std::vector<bool> mask1(grid.vertex_count());
  std::vector<bool> mask2(grid.vertex_count());
  std::vector<bool> mask3(grid.vertex_count());
  // auto filling = unlayered::flood_filling<int,float>(
  //   [](auto U, auto V){ return math::similarity (U,V) > std::cos(M_PI * 60.0f/180.0f); }
  // );
  // filling.fill(
  //   grid, vertex_gradient, 
  //   series::uniform(true),
  //   whole::max_id(known::length<float>(vertex_gradient)), 
  //   vertex_colored_scalars, 
  //   mask1
  // );
  // auto segmentation = unlayered::image_segmentation<int,float>(filling);
  // segmentation.segment(
  //   grid, vertex_gradient, 7, 10, 
  //   vertex_colored_scalars, scratch, mask1, mask2, mask3
  // );
  each::copy(vertex_colored_scalars, buffer_color_values);
  each::copy(vertex_square_ids, buffer_square_ids);
  each::copy(vertex_scalars1, buffer_scalars1);
  each::copy(vertex_scalars2, buffer_scalars2);
  each::copy(vertex_positions, buffer_positions);
  grids.storeTriangleStrips(series::range<unsigned int>(grid.vertex_count()), buffer_element_vertex_ids);

  // flatten vector raster for OpenGL
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
      glm::vec3(-1,0,0) * pyramid_halflength, 
      glm::vec3(1,0,0)  * pyramid_halflength, 
      glm::vec3(0,0,1), pyramid_radius, 3,
      vectors_element_position);
  each::copy   (known::mult(vertex_positions, series::uniform(1+pyramid_halflength/grid.total_radius())),  vectors_instance_position);
  each::copy   (vertex_gradient,   vectors_instance_heading);
  each::copy   (vertex_normals,    vectors_instance_up);
  each::length (vertex_gradient,   vectors_instance_scale);
  each::div    (vectors_instance_scale, series::uniform(whole::max(vectors_instance_scale)), vectors_instance_scale);

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
  view::IndicatorSwarmShaderProgram indicator_program;  
  view::MultichannelSurfaceShaderProgram debug_program;

  // initialize data for shader program
  std::vector<float> points = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
  };

  std::vector<float> colors = {
   1.0f,  
   0.0f,  
   0.0f
  };

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(buffer_scalars1) << std::endl;
  std::cout << whole::max(buffer_scalars1) << std::endl;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // colorscale_program.draw(
      //   buffer_positions,    // position
      //   buffer_color_values, // color value
      //   buffer_uniform,      // displacement
      //   buffer_scalars1,     // darken
      //   buffer_scalars2,     // culling
      //   buffer_element_vertex_ids,
      //   colorscale_state,
      //   view_state,
      //   GL_TRIANGLE_STRIP
      // );

      debug_program.draw(
        buffer_positions,
        // buffer_color_values, // red
        std::vector<float>(grid.vertex_count(), 0.0f), // red
        buffer_scalars1, // green
        std::vector<float>(grid.vertex_count(), 0.0f), // blue
        std::vector<float>(grid.vertex_count(), 1.0f), // opacity
        std::vector<float>(grid.vertex_count(), 0.0f), // displacement
        buffer_element_vertex_ids,
        glm::vec4(0.0f, whole::min(buffer_scalars1), 0.0f, 0.0f),
        glm::vec4(0.0f, whole::max(buffer_scalars1), 1.0f, 1.0f),
        view_state,
        GL_TRIANGLE_STRIP
      );

      // colorscale_program.draw(
      //   buffer_positions,    // position
      //   buffer_color_values,   // color value
      //   buffer_uniform,      // displacement
      //   buffer_uniform,      // darken
      //   buffer_uniform,      // culling
      //   buffer_element_vertex_ids,
      //   colorscale_state,
      //   view_state,
      //   GL_TRIANGLE_STRIP
      // );

      // indicator_program.draw(
      //   vectors_element_position,
      //   vectors_instance_position,
      //   vectors_instance_heading,
      //   vectors_instance_up,
      //   vectors_instance_scale,
      //   vectors_instance_color,
      //   view_state,
      //   GL_TRIANGLES
      // );

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

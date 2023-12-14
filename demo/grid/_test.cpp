
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
#include <index/series/glm/VectorDeinterleave.hpp>
#include <index/each.hpp>                           // get
#include <index/whole.hpp>                          // max, mean
#include <index/known.hpp>                          // greaterThan
#include <index/series/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/series/Range.hpp>                   // Range

#include <field/noise/ValueNoise.hpp>               // ValueNoise
#include <field/noise/SquareNoise.hpp>              // SquareNoise

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/Morphology.hpp>          // Morphology
#include <raster/spheroidal/string_cast.hpp>        // to_string

#include <update/OrbitalControlState.hpp>           // OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // OrbitalControlUpdater

#include <view/ColorscaleSurfacesShaderProgram.hpp> // ColorscaleSurfacesShaderProgram

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
  double radius(2.0);
  int vertices_per_square_side(2);
  dymaxion::Grid grid(radius, vertices_per_square_side);
  dymaxion::VertexPositions vertex_positions(grid);

  // auto vertex_colored_scalars = dymaxion::square_ids(grid);

  // auto vertex_colored_scalars = series::range();

  // std::vector<double> vertex_colored_scalars(grid.vertex_count());
  // for (int i = 0; i < grid.vertex_count(); ++i)
  // {
  //   vertex_colored_scalars[i] = grid.vertex_position(i).z;
  // }

  auto vertex_colored_scalars = series::map(
      field::value_noise3(
          field::square_noise(
              series::unit_interval_noise(11.0f, 1.1e4f))),
      vertex_positions
  );

  // auto mask = 
  //   known::greaterThan(series::uniform(0.5), 
  //         series::map(
  //             field::value_noise3(
  //                 field::square_noise(
  //                     series::unit_interval_noise(11.0, 1.1e4))),
  //             dymaxion::vertex_positions(grid)
  //         )
  //     );
  // std::vector<bool> out(grid.vertex_count());
  // std::vector<bool> scratch(grid.vertex_count());
  // unlayered::Morphology morphology;
  // morphology.closing(grid, mask, out, scratch);
  // std::cout << "closing:" << std::endl;
  // std::cout << spheroidal::to_string(grid, out) << std::endl;
  // std::vector<double> vertex_displacements(grid.vertex_count());
  // each::copy(out, vertex_displacements);

  auto vertex_displacements = series::uniform(0.0f);
  // auto vertex_displacements = series::map(
  //     field::value_noise3(
  //         field::square_noise(
  //             series::unit_interval_noise(12.0f, 1.2e4f))),
  //     vertex_positions
  // );

  // flatten raster for WebGL
  dymaxion::WholeGridBuffers grids(vertices_per_square_side);
  std::vector<float> buffer_color_values(grids.triangle_strips_size(vertex_colored_scalars));
  std::vector<float> buffer_displacements(grids.triangle_strips_size(vertex_displacements));
  std::vector<float> buffer_positions(grids.triangle_strips_size(vertex_positions));
  std::cout << "vertex count:        " << grid.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << grid.vertices_per_meridian() << std::endl;
  std::cout << "scalar buffer size:  " << buffer_displacements.size() << std::endl;
  grids.storeTriangleStrips(vertex_colored_scalars, buffer_color_values);
  grids.storeTriangleStrips(vertex_displacements, buffer_displacements);
  grids.storeTriangleStrips(vertex_positions, buffer_positions);

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
  view::ColorscaleSurfacesViewState<float> colorscale_state;
  colorscale_state.max_value = whole::max(buffer_color_values);
  colorscale_state.sealevel = whole::mean(buffer_displacements);

  // initialize shader program
  view::ColorscaleSurfacesShaderProgram colorscale_program;  

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

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      colorscale_program.draw(
        buffer_positions, 
        buffer_color_values, 
        buffer_displacements,
        // points,
        // colors,
        // colors,
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

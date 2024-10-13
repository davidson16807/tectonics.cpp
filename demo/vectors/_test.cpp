
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
#include <index/adapted/glm/GlmStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/metric/MetricOrder.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>
#include <index/iterated/Arithmetic.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

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

  iterated::Identity copy;
  iterated::Metric metric{adapted::GlmMetric{}};

  using vec3 = glm::vec3;
  using vec4 = glm::vec4;

  // flatten vector raster for OpenGL
  iterated::Arithmetic scalars(adapted::SymbolicArithmetic(0.0f, 1.0f));
  buffer::PyramidBuffers<int, float> pyramids;
  std::vector<vec3> vectors_element_position(pyramids.triangles_size<3>(3));

  std::vector<vec3> vectors_instance_position{ 
    vec3( 0, 0, 1),
    vec3( 0, 0,-1),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 1, 0, 0),
    vec3(-1, 0, 0)
  };

  std::vector<vec3> vectors_instance_heading{
    vec3( 0, 0, 1),
    vec3( 0, 0,-1),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 1, 0, 0),
    vec3(-1, 0, 0)
  };

  std::vector<vec3> vectors_instance_up{
    vec3( 1, 0, 0),
    vec3(-1, 0, 0),
    vec3( 0, 0, 1),
    vec3( 0, 0,-1),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0)
  };

  std::vector<glm::vec4> vectors_instance_color{
    vec4( 0, 0, 1, 1),
    vec4( 1, 1, 0, 1),
    vec4( 0, 1, 0, 1),
    vec4( 1, 0, 1, 1),
    vec4( 1, 0, 0, 1),
    vec4( 0, 1, 1, 1)
  };

  std::vector<float> vectors_instance_scale{
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
  };

  float pyramid_halflength(0.1);
  float pyramid_radius(pyramid_halflength/3.0f);
  pyramids.storeTriangles(
      vec3(0,0,-1) * pyramid_halflength, 
      vec3(0,0, 1) * pyramid_halflength, 
      vec3(1,0, 0),  pyramid_radius, 3, 
      vectors_element_position);

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

  // initialize shader program
  view::IndicatorSwarmShaderProgram indicator_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      indicator_program.draw(
        vectors_element_position,
        vectors_instance_position,
        vectors_instance_heading,
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


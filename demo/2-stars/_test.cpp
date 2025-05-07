
// std libraries
#include <iostream>
#include <string>

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

// in house libraries
#include <update/OrbitalControlState.hpp>           // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // update::OrbitalControlUpdater

#include <view/RealisticPointSourceSwarmShaderProgram.hpp>     // view::RealisticPointSourceSwarmShaderProgram

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Stars", NULL, NULL);
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
  // using vec4 = glm::vec4;

  std::vector<vec3> instance_origins{ 
    vec3( 0, 0, 1),
    vec3( 0, 0, 0),
    vec3( 0, 0,-1),
    vec3( 0,-1,-1),
    vec3( 0,-1, 0),
    vec3( 0,-1, 1),
    vec3( 0, 1,-1),
    vec3( 0, 1, 0),
    vec3( 0, 1, 1),

    vec3(-1, 0, 1),
    vec3(-1, 0, 0),
    vec3(-1, 0,-1),
    vec3(-1,-1,-1),
    vec3(-1,-1, 0),
    vec3(-1,-1, 1),
    vec3(-1, 1,-1),
    vec3(-1, 1, 0),
    vec3(-1, 1, 1),

    vec3( 1, 0, 1),
    vec3( 1, 0, 0),
    vec3( 1, 0,-1),
    vec3( 1,-1,-1),
    vec3( 1,-1, 0),
    vec3( 1,-1, 1),
    vec3( 1, 1,-1),
    vec3( 1, 1, 0),
    vec3( 1, 1, 1)
  };

  std::vector<vec3> instance_surface_emissions{ 
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 1),//
    vec3( 0, 1, 0),//
    vec3( 0, 1, 1),//
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),

    vec3( 0, 0, 0),
    vec3( 1, 0, 0),//
    vec3( 1, 0, 1),//
    vec3( 1, 1, 0),//
    vec3( 1, 1, 1),//
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),

    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0),
    vec3( 0, 0, 0)
  };

  std::vector<float> instance_radii(instance_origins.size(),0.1);
  std::vector<vec3> instance_light_luminosity(instance_origins.size(),vec3(3));
  std::vector<vec3> instance_light_source(instance_origins.size(),vec3(0));
  glm::mat4 model_matrix(1);

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
  view_state.resolution = glm::vec2(850, 640);
  view_state.point_spread_function_pixel_standard_deviation = 1.0f;
  view_state.point_spread_function_starburst_pixel_amplitude = 0.2f;
  view_state.exposure_intensity = 0.0001f;
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);

  // initialize shader program
  view::RealisticPointSourceSwarmShaderProgram sphere_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      sphere_program.draw(
        instance_origins,
        instance_radii,
        instance_light_source,
        instance_origins,
        instance_surface_emissions,
        model_matrix,
        view_state
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


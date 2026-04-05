// std libraries
#include <iostream>
#include <string>

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// in house libraries
#include <update/OrbitalControlState.hpp>           // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // update::OrbitalControlUpdater

#include <view/IndicatorLineShaderProgram.hpp>      // view::IndicatorLineShaderProgram

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Lines", NULL, NULL);
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
  using vec4 = glm::vec4;

  // positions of nodes that bound abutted line segments
  std::vector<vec3> node_positions{
    vec3(-1,-1,-1),
    vec3(-1,-1, 1),
    vec3(-1, 1, 1),
    vec3( 1, 1, 1),
    vec3( 1, 1,-1),
    vec3( 1,-1,-1),
    vec3(-1,-1,-1)
  };

  // one color per segment between consecutive node positions
  std::vector<vec4> instance_colors{
    vec4(1,0,0,1),
    vec4(0,1,0,1),
    vec4(0,0,1,1),
    vec4(1,1,0,1),
    vec4(1,0,1,1),
    vec4(0,1,1,1)
  };

  std::size_t segment_count = instance_colors.size();

  std::vector<glm::vec3> instance_start;
  std::vector<glm::vec3> instance_stop;

  instance_start.reserve(segment_count);
  instance_stop.reserve(segment_count);

  for (std::size_t i = 0; i < segment_count; ++i)
  {
    instance_start.push_back(node_positions[i]);
    instance_stop.push_back(node_positions[i+1]);
  }

  glm::mat4 model_matrix(1);

  // initialize control state
  update::OrbitalControlState control_state;
  update::OrbitalControlUpdater orbit_updater;
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
  view_state.render_pass = view::RenderPassType::overlays;

  // initialize shader program
  view::IndicatorLineShaderProgram line_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      line_program.draw(
        instance_start,
        instance_stop,
        instance_colors,
        4, // pixel_width
        2, // scale_height_count
        model_matrix,
        view_state
      );

      // put stuff we've been drawing onto the display
      glfwSwapBuffers(window);

      std::queue<messages::Message> message_poll = message_queue.poll();
      while (!message_poll.empty())
      {
        orbit_updater.update(control_state, message_poll.front(), control_state);
        message_poll.pop();
      }

      view_state.view_matrix = control_state.get_view_matrix();
  }

  // close GL context and any other GLFW resources
  message_queue.deactivate(window);
  glfwTerminate();
  return 0;
}
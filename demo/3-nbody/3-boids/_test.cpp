
// std libraries
#include <iostream>
#include <string>
#include <format>   // std::format
#include <algorithm>// std::clamp
#include <chrono>   // std::chrono::high_resolution_clock
// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries

// in house libraries

#include <math/special.hpp>                  // math::floormod

#include <field/poles/MonopoleVector.hpp>             // field::MonopoleVector
#include <field/poles/MonopoleScalar.hpp>             // field::MonopoleScalar
#include <field/poles/ShallowBarnesHutMultipole.hpp>  // field::ShallowBarnesHutMultipole
#include <field/poles/DeepBarnesHutMultipole.hpp>     // field::DeepBarnesHutMultipole

#include <unit/si.hpp>                       // si::unit

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <model/orbit/Elements.hpp>          // orbit::Elements
#include <model/orbit/ElementsAndState.hpp>  // 
#include <model/orrery/OrbitSystem.hpp>      // orrery:OrbitSystem
#include <model/orrery/SceneTrees.hpp>       // orrery:SceneTrees
#include <model/orrery/DenseContiguousComponents.hpp> // orrery:UnsortedEphemeralComponents

#include <update/OrbitalNavigationState.hpp>    // update::OrbitalNavigationState
#include <update/OrbitalNavigationUpdater.hpp>  // update::OrbitalNavigationUpdater

#include <view/IndicatorMeshSwarmShaderProgram.hpp>     // view::IndicatorMeshSwarmShaderProgram

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Orrery", NULL, NULL);
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

  using clock = std::chrono::high_resolution_clock;

  using vec3 = glm::vec3;
  using vec4 = glm::vec4;
  // using vec4 = glm::vec4;

  using time = si::time<float>;

  glm::mat4 model_matrix(1);

  // initialize control state
  update::OrbitalNavigationState control_state;
  update::OrbitalNavigationUpdater orbit_updater;
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
  view_state.render_pass = view::RenderPassType::overlays;

  // flatten vector raster for OpenGL
  buffer::PyramidBuffers<int, float> pyramids;
  std::vector<vec3> element_position(pyramids.triangles_size(3));

  float pyramid_halflength(0.1);
  float pyramid_radius(pyramid_halflength/3.0f);
  pyramids.storeTriangles(
      vec3(0,0,-1) * pyramid_halflength, 
      vec3(0,0, 1) * pyramid_halflength, 
      vec3(1,0, 0),  pyramid_radius, 3, 
      element_position);

  std::vector<vec3> instance_position{ 
    vec3( 0, 0, 0),
    vec3( 0, 0, 1),
    vec3( 0, 0,-1),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 0, 1, 1),
    vec3( 0,-1,-1),

    vec3( 1, 0, 0),
    vec3( 1, 0, 1),
    vec3( 1, 0,-1),
    vec3( 1, 1, 0),
    vec3( 1,-1, 0),
    vec3( 1, 1, 1),
    vec3( 1,-1,-1),

    vec3(-1, 0, 0),
    vec3(-1, 0, 1),
    vec3(-1, 0,-1),
    vec3(-1, 1, 0),
    vec3(-1,-1, 0),
    vec3(-1, 1, 1),
    vec3(-1,-1,-1),
  };

  std::vector<vec3> instance_velocity{
    vec3( 0, 0, 0),
    vec3( 0, 0, 1),
    vec3( 0, 0,-1),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 0, 1, 1),
    vec3( 0,-1,-1),

    vec3( 1, 0, 0),
    vec3( 1, 0, 1),
    vec3( 1, 0,-1),
    vec3( 1, 1, 0),
    vec3( 1,-1, 0),
    vec3( 1, 1, 1),
    vec3( 1,-1,-1),

    vec3(-1, 0, 0),
    vec3(-1, 0, 1),
    vec3(-1, 0,-1),
    vec3(-1, 1, 0),
    vec3(-1,-1, 0),
    vec3(-1, 1, 1),
    vec3(-1,-1,-1),
  };

  std::vector<vec3> instance_up(27, vec3( 1, 0, 0));
  std::vector<vec4> instance_color(27, vec4(1));
  std::vector<float> instance_scale(27, 1.0f);

  // initialize shader program
  view::IndicatorMeshSwarmShaderProgram indicator_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  using Separation = field::MonopoleVector< 2,float,vec3>;
  using Cohesion   = field::MonopoleVector<-2,float,vec3>;
  using Alignment  = field::MonopoleScalar< 2,vec3,vec3>;
  using Weight     = field::MonopoleScalar< 2,float,vec3>;

  using Separations  = field::DeepBarnesHutMultipole<3,std::size_t,float,Separation>;
  using Cohesions    = field::DeepBarnesHutMultipole<3,std::size_t,float,Cohesion>;
  using Alignments   = field::DeepBarnesHutMultipole<3,std::size_t,float,Alignment>;
  using Weights      = field::DeepBarnesHutMultipole<3,std::size_t,float,Weight>;

  // now introduce the Multipole field
  Separations  separations  (vec3(0), 1000.0, 0.001);
  Cohesions    cohesions    (vec3(0), 1000.0, 0.001);
  Alignments   alignments   (vec3(0), 1000.0, 0.001);
  Weights      weights      (vec3(0), 1000.0, 0.001);

  const float separation_weight = 0.1f;
  const float cohesion_weight = 0.1f;
  const float alignment_weight = 0.1f;
  const float drag_coefficient = 0.3f;

  time t(0);
  auto real_time_now = clock::now();
  auto real_start_time = real_time_now;
  std::chrono::duration<float> real_frame_time = (real_time_now - real_start_time);
  while(!glfwWindowShouldClose(window)) {

      real_start_time = real_time_now;
      real_time_now = clock::now();
      real_frame_time = (real_time_now - real_start_time);

      auto dt = si::second * real_frame_time.count();

      t+=dt;

      separations.clear();
      cohesions.clear();
      alignments.clear();
      weights.clear();

      for (std::size_t i = 0; i < instance_position.size(); ++i)
      {
        separations.add(instance_position[i], separation_weight);
        cohesions  .add(instance_position[i], cohesion_weight);
        alignments .add(instance_position[i], instance_velocity[i]);
        weights    .add(instance_position[i], 1.0f);
      }

      for (std::size_t i = 0; i < instance_position.size(); ++i)
      {
        vec3 average_velocity = alignments(instance_position[i]) / weights(instance_position[i]);
        vec3 alignment = (average_velocity - instance_velocity[i]) * alignment_weight;
        vec3 cohesion = cohesions  (instance_position[i]);
        vec3 separation = -separations(instance_position[i]);
        vec3 drag = -drag_coefficient * glm::length2(instance_velocity[i]) * instance_velocity[i];
        vec3 acceleration = cohesion + separation + alignment + drag;
        instance_velocity[i] += acceleration         * float(dt/si::second);
        instance_position[i] += instance_velocity[i] * float(dt/si::second);
        instance_position[i]  = glm::mod(instance_position[i] + vec3(5), vec3(10)) - vec3(5);
      }

      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      indicator_program.draw(
        element_position,
        instance_position,
        instance_velocity,
        instance_up,
        instance_scale,
        instance_color,
        glm::mat4(1), // model matrix
        view_state,
        GL_TRIANGLES
      );

      // put stuff we've been drawing onto the display
      glfwSwapBuffers(window);

      // now the model updates based on human feedback
      std::queue<messages::Message> message_poll = message_queue.poll();
      while (!message_poll.empty())
      {
        auto message = message_poll.front();
        orbit_updater.update(control_state, message, control_state);
        message_poll.pop();
      }

      // update the state of the view matrix to reflect the control state
      view_state.view_matrix = control_state.get_view_matrix();
  }

  // close GL context and any other GLFW resources
  message_queue.deactivate(window);
  glfwTerminate();
  return 0;
}


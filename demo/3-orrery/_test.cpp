
// std libraries
#include <iostream>
#include <string>
#include <format>   // std::format

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries

// in house libraries

#include <unit/si.hpp>                       // si::unit
#include <model/orbit/Elements.hpp>          // orbit::Elements
#include <model/orbit/ElementsAndState.hpp>  // 
#include <model/orrery/OrbitSystem.hpp>      // orrery:OrbitSystem
#include <model/orrery/Components.hpp>       // orrery:Components

#include <update/OrbitalControlState.hpp>    // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>  // update::OrbitalControlUpdater

#include <view/IndicatorSphereSwarmShaderProgram.hpp>     // view::IndicatorSphereSwarmShaderProgram

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Points", NULL, NULL);
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

  using dvec3 = glm::vec3;
  using vec3 = glm::vec3;
  using vec4 = glm::vec4;
  // using vec4 = glm::vec4;

  using mass = si::mass<double>;
  using length = si::length<double>;

  constexpr double pi = 3.141592653589793238462643383271;

  constexpr auto m = si::meter;
  constexpr auto m3 = si::meter3;
  constexpr auto kg = si::kilogram;
  constexpr auto s = si::second;

  float Rs(si::solar_radius / si::meter);

  constexpr auto au = si::astronomical_unit/m;
  constexpr auto G = si::gravitational_constant / (m3/(kg*s*s));
  const auto mass_of_sun = si::solar_mass; 

  using Elements = orbit::Elements<double>;
  using ElementsAndState = orbit::ElementsAndState<double>;
  using Propagator = orbit::UniversalPropagator<double>;
  using Properties = orbit::Properties<double>;
  using Orbit = orbit::Universals<double>;
  using Orbits = orrery::Components<int,Orbit>;
  using TrackPositions = std::vector<orrery::TrackPosition<int,double>>;
  Properties properties(dvec3(1,0,0), dvec3(0,0,1), G, pi);
  Propagator propagator(G);
  ElementsAndState converter(properties);

  orrery::OrbitSystem<int,double> orbit_system(propagator, properties);

  std::vector<std::pair<mass, Elements>> planets = {
      { mass_of_sun, Elements(5.790905e10,  0.2056, 7.005  * si::degree, 0.0, 0.0, 0.0) }, // Mercury
      { mass_of_sun, Elements(1.082080e11,  0.0068, 3.3947 * si::degree, 0.0, 0.0, 0.0) }, // Venus
      { mass_of_sun, Elements(1.49598023e11,0.0167, 0.0    * si::degree, 0.0, 0.0, 0.0) }, // Earth
      { mass_of_sun, Elements(2.279392e11,  0.0934, 1.851  * si::degree, 0.0, 0.0, 0.0) }, // Mars
      { mass_of_sun, Elements(7.78570e11,   0.0489, 1.305  * si::degree, 0.0, 0.0, 0.0) }, // Jupiter
      { mass_of_sun, Elements(1.43353e12,   0.0565, 2.484  * si::degree, 0.0, 0.0, 0.0) }, // Saturn
      { mass_of_sun, Elements(2.87246e12,   0.046,  0.770  * si::degree, 0.0, 0.0, 0.0) }, // Uranus
      { mass_of_sun, Elements(4.49506e12,   0.009,  1.769  * si::degree, 0.0, 0.0, 0.0) }, // Neptune
      { mass_of_sun, Elements(39.482*au,    0.2488, 17.14  * si::degree, 0.0, 0.0, 0.0) }, // Pluto
  };

  // (mass, Universals)
  Orbits orbits;
  // Elliptics
  int entity_id_counter = 0;
  for (const auto& mass_elements : planets) {
    const auto body_kg = mass_elements.first/kg;
    const Elements& elements = mass_elements.second;
    const auto state = converter.get_state_from_elements(elements, body_kg);
    orbits.add(entity_id_counter++, Orbit(body_kg, state));
  }

  TrackPositions positions;
  std::vector<int> filtered {0,1,2,3,4,5,6,7,8};

  orbit_system.positions(
    orbits, 
    filtered, 
    1.0, // time_offset
    positions);

  std::vector<vec3> instance_origins{};
  for (std::size_t i = 0; i < positions.size(); ++i)
  {
    instance_origins.push_back(positions[i].position);
    std::cout << std::format("{}\t{}\t{}\n",positions[i].position.x, positions[i].position.y, positions[i].position.z) << std::endl;
  }

  std::vector<glm::vec4> instance_color{
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(1.0, 0.5, 0.0, 1.0),
    vec4(1.0, 1.0, 0.5, 1.0),
    vec4(0.5, 1.0, 0.5, 1.0),
    vec4(0.5, 0.5, 1.0, 1.0),
    vec4(1.0, 0.5, 0.5, 1.0),
  };

  glm::mat4 model_matrix(1);

  // initialize control state
  update::OrbitalControlState control_state;
  control_state.min_zoom_distance = 1.0f;
  control_state.log2_height = std::log2(40.0*Rs);
  // control_state.log2_height = 20.0f;
  control_state.angular_position = glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f;

  length procedural_terrain_far_distance(3e3*si::kilometer);
  length planet_billboard_near_distance(1e7*si::kilometer); // ~10 * solar radius 
  length point_source_near_distance(30.0*si::astronomical_unit); // semi-major axis of pluto

  // initialize view state
  view::ViewState view_state;
  view_state.projection_matrix = glm::perspective(
    3.14159f*45.0f/180.0f, 
    850.0f/640.0f, 
    // 1e3f,
    // 1e16f
    float(planet_billboard_near_distance/m), 
    float(point_source_near_distance/m)
  );
  view_state.view_matrix = control_state.get_view_matrix();
  view_state.resolution = glm::vec2(850, 640);
  view_state.wavelength = glm::vec3(650e-9, 550e-9, 450e-9);
  view_state.exposure_intensity = 1e3*si::global_solar_constant/(si::watt/si::meter2);
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);


  // initialize shader program
  view::IndicatorSphereSwarmShaderProgram sphere_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      sphere_program.draw(
        instance_origins,
        instance_color,
        10, //instance_pixel_radius
        vec3(0,0,1), // light direction
        // vec3(0.5,0.5,1), // light direction
        vec3(1.0), // ambient light
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



// std libraries
#include <iostream>
#include <string>
#include <format>   // std::format
#include <algorithm>// std::clamp

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries

// in house libraries

#include <index/iterated/Nary.hpp>           // iterated::Cast

#include <math/special.hpp>                  // math::floormod

#include <unit/si.hpp>                       // si::unit

#include <model/orbit/Elements.hpp>          // orbit::Elements
#include <model/orbit/ElementsAndState.hpp>  // 
#include <model/orrery/OrbitSystem.hpp>      // orrery:OrbitSystem
#include <model/orrery/SceneTrees.hpp>       // orrery:SceneTrees
#include <model/orrery/CommonComponents.hpp> // orrery:UnsortedEphemeralComponents

#include <update/OrbitalNavigationState.hpp>    // update::OrbitalNavigationState
#include <update/OrbitalNavigationUpdater.hpp>  // update::OrbitalNavigationUpdater
#include <update/TreeNavigationUpdater.hpp>     // update::TreeNavigationUpdater

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

  using dvec3 = glm::dvec3;
  using vec3 = glm::vec3;
  using vec4 = glm::vec4;
  // using vec4 = glm::vec4;

  using mass = si::mass<double>;
  using time = si::time<double>;
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
  const auto mass_of_earth_moon = 6.0457e24 * si::kilogram;    
  const auto mass_of_jupiter = si::jupiter_mass;     
  const auto mass_of_saturn = 5.683e26 * si::kilogram;     
  const auto mass_of_uranus = 8.681e25 * si::kilogram;     
  const auto mass_of_neptune = 1.024e26 * si::kilogram;     
  const auto mass_of_pluto             = 1.3e22 * si::kilogram; 
  const auto mass_of_charon            = 1.5e21 * si::kilogram; 
  const auto mass_of_pluto_charon      = mass_of_pluto + mass_of_charon;

  using Elements = orbit::Elements<double>;
  using ElementsAndState = orbit::ElementsAndState<double>;
  using Propagator = orbit::UniversalPropagator<double>;
  using Properties = orbit::Properties<double>;
  using Orbit = orbit::Universals<double>;
  using Orbits = orrery::CommonComponents<int,Orbit>;
  using TrackPositions = std::vector<orrery::TrackPosition<int,double>>;
  Properties properties(dvec3(1,0,0), dvec3(0,0,1), G, pi);
  Propagator propagator(G);
  ElementsAndState converter(properties);
  iterated::Cast cast;

  /*
  TODO:
  * spins
  * sampling for imperceptibles
  * insolation
  */

  orrery::OrbitSystem<int,double> orbit_system(propagator, properties);
  orrery::SceneTrees<int,double> scene_trees;

  // within the confines of this ECS implementation, parent_ids are one-to-one with entities, so we store them using a std::vector
  std::vector<int> parent_ids {0,0,0,0,0,0,0,0,0,0,0,10,10,3,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,8,9,9,9,9,9,9,}; 
  // entities that have perceptible orbits, we use 32-bit ints for entity ids and more than 1 out of 32 are represented, so a bool mask is more sparse, but checking the mask introduces branching logic
  std::vector<bool> perceptible(parent_ids.size(), true);
  perceptible[0] = false;// 0 is sun
  // entities that have orbits, this is just a temporary construct used for initialization 
  // so we don't care if we use std::vector or std::pair
  std::vector<std::pair<mass, Elements>> elliptics = {
    //  combined mass         sma           ecc     inclination          lan  aop  ma
      { mass_of_sun, Elements(5.790905e10,  0.2056, 7.005  * si::degree, 0.0, 0.0, 0.0) },   // 1   Mercury
      { mass_of_sun, Elements(1.082080e11,  0.0068, 3.3947 * si::degree, 0.0, 0.0, 0.0) },   // 2   Venus
      { mass_of_sun, Elements(1.49598023e11,0.0167, 0.0    * si::degree, 0.0, 0.0, 0.0) },   // 3   Earth
      { mass_of_sun, Elements(2.279392e11,  0.0934, 1.851  * si::degree, 0.0, 0.0, 0.0) },   // 4   Mars
      { mass_of_sun, Elements(7.78570e11,   0.0489, 1.305  * si::degree, 0.0, 0.0, 0.0) },   // 5   Jupiter
      { mass_of_sun, Elements(1.43353e12,   0.0565, 2.484  * si::degree, 0.0, 0.0, 0.0) },   // 6   Saturn
      { mass_of_sun, Elements(2.87246e12,   0.046,  0.770  * si::degree, 0.0, 0.0, 0.0) },   // 7   Uranus
      { mass_of_sun, Elements(4.49506e12,   0.009,  1.769  * si::degree, 0.0, 0.0, 0.0) },   // 8   Neptune
      { mass_of_sun, Elements(39.482*au,    0.2488, 17.14  * si::degree, 0.0, 0.0, 0.0) },   // 9   Pluto-Charon
      { mass_of_sun, Elements(1.6442*au, 0.38385,3.4079*si::degree, 73.196*si::degree, 319.32*si::degree, 232.01) }, // 10  Didymos
      { mass_of_sun, Elements(1.206e3,   0.0,  169.3*si::degree, 0.0, 0.0, 0.0) },   // 11  Dimorphos, "Didymoon", pre-impact
      { mass_of_sun, Elements(1.144e3,   0.02, 169.3*si::degree, 0.0, 0.0, 0.0) },   // 12  Dimorphos, "Didymoon", post-impact
      { mass_of_earth_moon, Elements(384e6, 0.0549, 0.0   * si::degree, 0.0, 0.0, 0.0) },    // 13  Moon
      { mass_of_jupiter, Elements(421.8e6,  0.0041, 0.050 * si::degree, 0.0, 0.0, 0.0) },    // 14  Io
      { mass_of_jupiter, Elements(671.1e6,  0.0090, 0.470 * si::degree, 0.0, 0.0, 0.0) },    // 15  Europa
      { mass_of_jupiter, Elements(1070.4e6, 0.0013, 0.200 * si::degree, 0.0, 0.0, 0.0) },    // 16  Ganymede
      { mass_of_jupiter, Elements(1882.7e6, 0.0074, 0.192 * si::degree, 0.0, 0.0, 0.0) },    // 17  Callisto
      { mass_of_saturn, Elements(186e6,     0.0196, 1.53  * si::degree, 0.0, 0.0, 0.0) },    // 18  Mimas
      { mass_of_saturn, Elements(238.4e6,   0.0047, 0.02  * si::degree, 0.0, 0.0, 0.0) },    // 19  Enceladus
      { mass_of_saturn, Elements(295e6,     0.0001, 1.09  * si::degree, 0.0, 0.0, 0.0) },    // 20  Tethys
      { mass_of_saturn, Elements(377.7e6,   0.0022, 0.02  * si::degree, 0.0, 0.0, 0.0) },    // 21  Dione
      { mass_of_saturn, Elements(527.2e6,   0.001,  0.35  * si::degree, 0.0, 0.0, 0.0) },    // 22  Rhea
      { mass_of_saturn, Elements(1221.9e6,  0.0288, 0.33  * si::degree, 0.0, 0.0, 0.0) },    // 23  Titan
      { mass_of_saturn, Elements(1481.5e6,  0.0274, 0.43  * si::degree, 0.0, 0.0, 0.0) },    // 24  Hyperion
      { mass_of_saturn, Elements(3561.7e6,  0.0283, 157.0 * si::degree, 0.0, 0.0, 0.0) },    // 25  Iapetus
      { mass_of_uranus, Elements(129846e3,  0.0014, 4.421 * si::degree, 0.0, 0.0, 0.0) },    // 26  Miranda
      { mass_of_uranus, Elements(190929e3,  0.0014, 0.026 * si::degree, 0.0, 0.0, 0.0) },    // 27  Ariel
      { mass_of_uranus, Elements(265986e3,  0.0039, 0.083 * si::degree, 0.0, 0.0, 0.0) },    // 28  Umbriel
      { mass_of_uranus, Elements(265986e3,  0.0016, 0.114 * si::degree, 0.0, 0.0, 0.0) },    // 29  Titania
      { mass_of_uranus, Elements(583511e3,  0.0016, 0.125 * si::degree, 0.0, 0.0, 0.0) },    // 30  Oberon
      { mass_of_neptune, Elements(354759e3, 0.0000, 156.865*si::degree, 0.0, 0.0, 0.0) },    // 31  Triton
      { mass_of_pluto_charon, Elements(2126e3,  0.0000, 0.000*si::degree, 0.0, 0.0, 0.0) },  // 32  Pluto
      { mass_of_pluto_charon, Elements(17470e3, 0.0000, 0.000*si::degree, 0.0, 0.0, 0.0) },  // 33  Charon
      { mass_of_pluto_charon, Elements(42656e3, 5.787,  0.809*si::degree, 0.0, 0.0, 0.0) },  // 34  Styx
      { mass_of_pluto_charon, Elements(48694e3, 2.036,  0.133*si::degree, 0.0, 0.0, 0.0) },  // 35  Nix
      { mass_of_pluto_charon, Elements(57783e3, 3.280,  0.389*si::degree, 0.0, 0.0, 0.0) },  // 36  Kerberos
      { mass_of_pluto_charon, Elements(64738e3, 5.862,  0.242*si::degree, 0.0, 0.0, 0.0) },  // 37  Hydra
  };

  // TODO: store this using `Components<vec4>`
  std::vector<glm::vec4> instance_color(parent_ids.size(), vec4(1));
  instance_color[0 ] = vec4(1.0, 1.0, 0.0, 1.0); // Sun
  instance_color[1 ] = vec4(0.5, 0.5, 0.5, 1.0); // Mercury
  instance_color[2 ] = vec4(0.5, 0.5, 0.0, 1.0); // Venus
  instance_color[3 ] = vec4(0.0, 0.0, 1.0, 1.0); // Earth
  instance_color[4 ] = vec4(1.0, 0.0, 0.0, 1.0); // Mars
  instance_color[5 ] = vec4(1.0, 0.5, 0.0, 1.0); // Jupiter
  instance_color[6 ] = vec4(1.0, 1.0, 0.5, 1.0); // Saturn
  instance_color[7 ] = vec4(0.5, 1.0, 0.5, 1.0); // Uranus
  instance_color[8 ] = vec4(0.5, 0.5, 1.0, 1.0); // Neptune
  instance_color[9 ] = vec4(1.0, 0.5, 0.5, 0.0); // Pluto-Charon
  instance_color[14] = vec4(0.5, 0.5, 0.0, 1.0); // Io
  instance_color[15] = vec4(0.5, 0.5, 1.0, 1.0); // Europa
  instance_color[23] = vec4(1.0, 1.0, 0.0, 1.0); // Titan
  instance_color[31] = vec4(1.0, 0.5, 0.5, 1.0); // Triton

  // (mass, Universals)
  Orbits orbits;
  // Elliptics
  int entity_id_counter = 1;
  for (const auto& mass_elements : elliptics) {
    const auto body_kg = mass_elements.first/kg;
    const Elements& elements = mass_elements.second;
    const auto state = converter.get_state_from_elements(elements, body_kg);
    orbits.add(entity_id_counter++, Orbit(body_kg, state));
  }

  TrackPositions orbital_parent_offsets;
  std::vector<dvec3> parent_offsets(parent_ids.size());
  std::vector<bool> is_origin_ancestor(parent_ids.size());

  std::vector<dvec3> body_origins(parent_ids.size());    // stores positions at double precision
  std::vector<vec3> instance_origins(parent_ids.size()); // stores positions for the shader

  glm::mat4 model_matrix(1);

  // initialize control state
  update::OrbitalNavigationState control_state;
  update::OrbitalNavigationUpdater orbit_updater;
  update::TreeNavigationUpdater<int> tree_updater("9","0","[","]");
  control_state.min_zoom_distance = 1.0f;
  control_state.log2_height = std::log2(60.0*Rs);
  // control_state.log2_height = 20.0f;
  control_state.angular_position = glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f;

  length procedural_terrain_far_distance(3e3*si::kilometer);
  length planet_billboard_near_distance(1e8*si::kilometer); // ~10 * solar radius 
  length point_source_near_distance(1e3*si::astronomical_unit);

  // initialize view state
  view::ViewState view_state;
  view_state.projection_matrix = glm::perspective(
    3.14159f*45.0f/180.0f, 
    850.0f/640.0f, 
    // 1e3f,
    // 1e16f
    float(planet_billboard_near_distance/1e5/m), 
    float(point_source_near_distance/m)
  );
  view_state.view_matrix = control_state.get_view_matrix();
  view_state.resolution = glm::vec2(850, 640);
  view_state.wavelength = glm::vec3(650e-9, 550e-9, 450e-9);
  view_state.exposure_intensity = 1e2*si::global_solar_constant/(si::watt/si::meter2);
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);
  view_state.render_pass = view::RenderPassType::overlays;

  // initialize shader program
  view::IndicatorSphereSwarmShaderProgram sphere_program;

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::vector<time> timesteps{
    si::second,
    si::minute,
    si::hour,
    si::day,
    si::week,
    si::month,
    si::year,
    si::decayear,
    si::hectoyear,
    si::kiloyear,
    10.0*si::kiloyear,
    100.0*si::kiloyear,
    si::megayear,
    10.0*si::megayear,
    100.0*si::megayear,
  };

  int timestep_id(0);
  std::size_t origin_id(0);
  time t(0);
  while(!glfwWindowShouldClose(window)) {

      t+=timesteps[timestep_id]/60.0;

      orbit_system.offsets(
        orbits, 
        perceptible, 
        t/si::second, // time_offset
        orbital_parent_offsets
      );

      /*
      TODO: `update` should be unecessary when using components,
      `.offsets` could instead set results to a `Components` objects,
      and remove the need for `TrackPositions` and its `node` attribute.
      The major downside is the loss of conceptual purity.
      */ 
      scene_trees.update(
        orbital_parent_offsets,
        parent_offsets,
        parent_offsets
      );

      scene_trees.is_ancestor(
        parent_ids,
        origin_id, //earth
        is_origin_ancestor
      );

      scene_trees.positions(
        parent_offsets,
        parent_ids,
        is_origin_ancestor,
        origin_id,
        body_origins
      );

      cast(body_origins, instance_origins);

      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      sphere_program.draw(
        instance_origins,
        instance_color,
        5, //instance_pixel_radius
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
        auto message = message_poll.front();
        orbit_updater.update(control_state, message, control_state);
        origin_id = tree_updater.update(origin_id, parent_ids, message);
        if (std::holds_alternative<messages::KeyboardMessage>(message))
        {
          auto key_message = std::get<messages::KeyboardMessage>(message);
          if (key_message.action != messages::release)
          {
            auto c = key_message.character;
            /* < slower   */     if (c == ",") { timestep_id = std::clamp(timestep_id-1, 0, int(timesteps.size()-1)); } 
            /* > faster   */else if (c == ".") { timestep_id = std::clamp(timestep_id+1, 0, int(timesteps.size()-1)); } 
            /* || pause   */else if (c == "/") { timestep_id = 0; } 
            std::cout << timestep_id << std::endl;
          }
        }
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


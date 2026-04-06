
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

#include <unit/si.hpp>                       // si::unit

#include <model/orbit/Elements.hpp>          // orbit::Elements
#include <model/orbit/ElementsAndState.hpp>  // 
#include <model/orrery/OrbitSystem.hpp>      // orrery:OrbitSystem
#include <model/orrery/SceneTrees.hpp>       // orrery:SceneTrees
#include <model/orrery/CommonComponents.hpp> // orrery:UnsortedEphemeralComponents

#include <update/OrbitalNavigationState.hpp>    // update::OrbitalNavigationState
#include <update/OrbitalNavigationUpdater.hpp>  // update::OrbitalNavigationUpdater
#include <update/TreeNavigationUpdater.hpp>     // update::TreeNavigationUpdater
#include <update/ValueHotkeyPresetUpdater.hpp>  // update::ValueHotkeyPresetUpdater
#include <update/BoundedOptionNavigationUpdater.hpp> // update::BoundedOptionNavigationUpdater
#include <update/CyclingOptionNavigationUpdater.hpp> // update::CyclingOptionNavigationUpdater

#include <view/IndicatorSphereSwarmShaderProgram.hpp>     // view::IndicatorSphereSwarmShaderProgram

#include <files/VectorTableFiles.hpp>            // view::VectorTableFiles
#include <codecs/orbit/ElementsVectorCodec.hpp>  // view::ElementsVectorCodec
#include <codecs/orbit/BodyVectorCodec.hpp>      // view::BodyVectorCodec
#include <codecs/DelimitedTableLineCodec.hpp>  // view::DelimitedTableLineCodec

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

  using dvec3 = glm::dvec3;
  using vec3 = glm::vec3;
  using vec4 = glm::vec4;

  using mass = si::mass<double>;
  using time = si::time<double>;
  using length = si::length<double>;

  constexpr double pi = 3.141592653589793238462643383271;

  constexpr auto m = si::meter;
  constexpr auto m3 = si::meter3;
  constexpr auto kg = si::kilogram;
  constexpr auto s = si::second;

  float Rs(si::solar_radius / si::meter);

  // const auto mass_of_sun = si::solar_mass; 
  // const auto mass_of_earth_moon = 6.0457e24 * kg;    
  // const auto mass_of_jupiter    = si::jupiter_mass;     
  // const auto mass_of_saturn     = 5.683e26 * kg;     
  // const auto mass_of_uranus     = 8.681e25 * kg;     
  // const auto mass_of_neptune    = 1.024e26 * kg;     
  // const auto mass_of_pluto         = 1.3e22 * kg; 
  // const auto mass_of_charon        = 1.5e21 * kg; 
  // const auto mass_of_pluto_charon  = mass_of_pluto + mass_of_charon;

  using Elements = orbit::Elements<double>;
  using ElementsAndState = orbit::ElementsAndState<double>;
  using Propagator = orbit::UniversalPropagator<double>;
  using Properties = orbit::Properties<double>;
  using Orbit = orbit::Universals<double>;
  using Orbits = orrery::CommonComponents<int,Orbit>;
  using TrackPositions = std::vector<orrery::TrackPosition<int,double>>;

  Properties properties(
    dvec3(1,0,0), 
    dvec3(0,0,1), 
    si::gravitational_constant / (m3/(kg*s*s)), 
    pi
  );

  Propagator propagator(
    si::gravitational_constant / (m3/(kg*s*s)), 
    35,    // max_refinement_count
    1e-15, // max_precision
    5,     // laguerre_method_n
    true   // force_congruence
  );

  ElementsAndState converter(properties);

  /*
  TODO:
  * spins
  * sampling for imperceptibles
  * insolation
  */

  orrery::OrbitSystem<int,double> orbit_system(propagator, properties);
  orrery::SceneTrees<int,double> scene_trees;
  files::VectorTableFiles<codecs::DelimitedTableLineCodec> tsvs(codecs::DelimitedTableLineCodec("\t"));
  codecs::ElementsVectorCodec<double> element_codec;
  codecs::BodyVectorCodec<int,double,codecs::ElementsVectorCodec<double>> body_rows(element_codec);

  // within the confines of this ECS implementation, parent_ids are one-to-one with entities, so we store them using a std::vector
  std::vector<Elements> elliptics;
  std::vector<int> parent_ids; 
  std::vector<mass> masses; 
  std::vector<std::string> labels; 
  std::vector<std::vector<std::string>> scratch_table;
  std::vector<vec4> instance_color;

  tsvs.read("elliptics.tsv", scratch_table);
  body_rows.decode(scratch_table,  elliptics, parent_ids, masses, labels, instance_color);

  // entities that have perceptible orbits, we use 32-bit ints for entity ids and more than 1 out of 32 are represented, so a bool mask is more sparse, but checking the mask introduces branching logic
  std::vector<bool> perceptible(parent_ids.size(), true);
  perceptible[0] = false;// 0 is sun

  // (mass, Universals)
  Orbits orbits;
  // Elliptics
  int entity_id_counter = 1;
  for(std::size_t i=0; i<elliptics.size(); i++)
  {
    const auto body_kg = masses[i]/kg;
    const Elements& elements = elliptics[i];
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
  update::TreeNavigationUpdater<int> scene_tree_updater("9","0",false, "[","]",true);
  update::CyclingOptionNavigationUpdater<std::size_t> scene_node_updater("m","n", true);
  update::BoundedOptionNavigationUpdater<std::size_t> timewarp_updater(",", ".", false);
  update::ValueHotkeyPresetUpdater<std::size_t> pause_updater("/",0);
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
  auto real_time_now = clock::now();
  auto real_start_time = real_time_now;
  std::chrono::duration<double> real_frame_time = (real_time_now - real_start_time);
  while(!glfwWindowShouldClose(window)) {

      real_start_time = real_time_now;
      real_time_now = clock::now();
      real_frame_time = (real_time_now - real_start_time);

      t+=timesteps[timestep_id] * real_frame_time.count();

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
        origin_id, //first ancestor
        is_origin_ancestor
      );

      scene_trees.positions(
        parent_offsets,
        parent_ids,
        is_origin_ancestor,
        origin_id,
        instance_origins
      );

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

      // now the model updates based on human feedback
      std::queue<messages::Message> message_poll = message_queue.poll();
      while (!message_poll.empty())
      {
        auto message = message_poll.front();
        orbit_updater.update(control_state, message, control_state);
        origin_id = scene_tree_updater.update(origin_id, parent_ids, message);
        origin_id = scene_node_updater.update(origin_id, parent_ids, message);
        timestep_id = timewarp_updater.update(timestep_id, timesteps, message);
        timestep_id = pause_updater.update(timestep_id, message);
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



// std libraries
#include <iostream>
#include <string>

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

// in house libraries
#include <unit/si.hpp>                              // si::units
#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <update/OrbitalControlState.hpp>           // update::OrbitalControlState
#include <update/OrbitalControlUpdater.hpp>         // update::OrbitalControlUpdater

#include <view/RealisticSphereSwarmShaderProgram.hpp>     // view::RealisticSphereSwarmShaderProgram


template<typename energy_per_temperature>
struct Atmospheres
{
  const energy_per_temperature boltzmann_constant;
  Atmospheres(const energy_per_temperature boltzmann_constant):
    boltzmann_constant(boltzmann_constant)
  {}
  template<typename temperature, typename mass, typename acceleration>
  auto scale_height(const temperature atmosphere_temperature, const mass molecular_mass, const acceleration gravity) const
  {
    return boltzmann_constant * atmosphere_temperature / (molecular_mass * gravity);
  }
};
template<typename energy_per_temperature>
Atmospheres<energy_per_temperature> atmospheres(const energy_per_temperature boltzmann_constant)
{
  return Atmospheres<energy_per_temperature>(boltzmann_constant);
}

template<typename acceleration_per_area_density>
struct PointMasses
{
  const acceleration_per_area_density gravitational_constant;
  PointMasses(const acceleration_per_area_density gravitational_constant):
    gravitational_constant(gravitational_constant)
  {}
  template<typename mass, typename length>
  auto gravity(const mass point_mass, const length radius) const
  {
    return gravitational_constant * point_mass / (radius * radius);
  }
};
template<typename acceleration_per_area_density>
PointMasses<acceleration_per_area_density> point_masses(const acceleration_per_area_density gravitational_constant)
{
  return PointMasses<acceleration_per_area_density>(gravitational_constant);
}

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

  /*
  temperatures for stars from wikipedia
  temperatures for planets and brown dwarves are for earth, luhman 16, and an arbitrary hot jupiter
  */
  std::vector<float> instance_surface_temperature{ 
    0,0,300,600,1210,0,0,0,0,
    0,5771,2100,9700,30000,0,0,0,0,
    0,0,0,0,0,0,0,0,0
  };

  /*
  temperatures for stars calculated with approximation from Carl Hansen, "Stellar Interiors"
  temperatures for brown dwarves from Garani (2021)
  */
  std::vector<float> instance_core_temperature{ 
    0,0,0,1e5,3e6,0,0,0,0, // earth, large hot jupiter, Luhman 16A
    0,15e6,94e3,53e6,2e9,0,0,0,0, // sunlike, red dwarf, blue giant, ultramassive, 
    0,0,0,0,0,0,0,0,0
  };

  std::vector<float> instance_atmosphere_molecular_mass{ 
    0,0,28.97,2.22,2.22,0,0,0,0, // earth, large hot jupiter, Luhman 16A
    0,0.6,0.6,0.6,0.6,0,0,0,0, // hydrogen plasma, estimated by Carl Hansen, "Stellar Interiors"
    0,0,0,0,0,0,0,0,0
  };

  std::vector<double> instance_radii{ 
    0.0,0.0,
    si::earth_radius/si::meter, // earth
    si::jupiter_radius/si::meter, // large hot jupiter
    0.85*si::jupiter_radius/si::meter, // Luhman 16A
    0.0,0.0,0.0,0.0, 
    0.0,
    si::solar_radius/si::meter, // sunlike
    0.09*si::solar_radius/si::meter, // red dwarf
    1.7*si::jupiter_radius/si::meter, // blue giant
    7.4*si::jupiter_radius/si::meter, // ultramassive
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0
  };

  std::vector<float> instance_masses{ 
    0.0,0.0,0.0,
    si::earth_mass/si::kilogram, // earth
    si::jupiter_mass/si::kilogram, // large hot jupiter
    0.034*si::solar_mass/si::kilogram, // Luhman 16A
    0.0,0.0,0.0, 
    0.0,
    1.0 * si::solar_mass/si::kilogram, // sunlike
    0.07 * si::solar_mass/si::kilogram, // red dwarf
    2.1 * si::solar_mass/si::kilogram, // blue giant
    18.0 * si::solar_mass/si::kilogram, // ultramassive
    0.0,0.0,0.0,0.0, 
    0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0
  };

  /*
  scale heights calculated from the standard equation, h=kᵦT/(mg)
  */
  auto atmosphere = atmospheres(si::boltzmann_constant);
  auto point = point_masses(si::gravitational_constant);
  std::vector<float> instance_atmosphere_scale_height;
  std::vector<float> instance_view_radii;
  for(std::size_t i=0; i<instance_radii.size(); i++)
  {
    instance_atmosphere_scale_height.push_back(
      atmosphere.scale_height(instance_surface_temperature[i] * si::kelvin, instance_atmosphere_molecular_mass[i] * si::dalton, 
        point.gravity(double(instance_masses[i]) * si::kilogram, instance_radii[i] * si::meter)
      ) / si::meter
    );
    instance_view_radii.push_back(float(std::max(0.1, instance_radii[i]*si::meter/si::solar_radius)));
  };

  // "beta_*" is the rest of the fractional loss.
  // it is dependant on wavelength, and the density ratio
  // So all together, the fraction of sunlight that scatters to a given angle is: beta(wavelength) * gamma(angle) * density_ratio
  std::vector<vec3> instance_beta_ray_sun(instance_origins.size(), vec3(5.20e-6, 1.21e-5, 2.96e-5));
  std::vector<vec3> instance_beta_mie_sun(instance_origins.size(), vec3(1e-6));
  std::vector<vec3> instance_beta_abs_sun(instance_origins.size(), vec3(1e-6));
  std::vector<vec3> instance_light_source(instance_origins.size(), vec3(0));

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
  view_state.exposure_intensity = 0.0003f;
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);

  // initialize shader program
  view::RealisticSphereSwarmShaderProgram sphere_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      sphere_program.draw(
        instance_origins,
        instance_view_radii,
        instance_surface_temperature,
        instance_core_temperature,
        instance_atmosphere_scale_height,
        instance_light_source,
        instance_origins,
        instance_beta_ray_sun,
        instance_beta_mie_sun,
        instance_beta_abs_sun,
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


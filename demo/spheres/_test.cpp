
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

template<typename scalar>
struct Spheres
{
  const scalar pi;
  Spheres(const scalar pi):
    pi(pi)
  {}
  template<typename length>
  auto surface_area(const length radius) const {
      return scalar(4)*pi*radius*radius;
  }
  template<typename length>
  auto volume(const length radius) const {
      return scalar(4)/scalar(3)*pi*radius*radius*radius;
  }
  template<glm::qualifier quality=glm::defaultp>
  bool is_point_inside(
    const glm::vec<3,scalar,quality> point, 
    const glm::vec<3,scalar,quality> origin, 
    const scalar radius
  ) const {
    return glm::length(point-origin) < radius;
  }
  template<glm::qualifier quality=glm::defaultp>
  bool distance_from_surface_to_point(
    const glm::vec<3,scalar,quality> point, 
    const glm::vec<3,scalar,quality> origin, 
    const scalar radius
  ) const {
    return glm::length(point-origin) - radius;
  }
  /*
  bool is_sphere_inside(
  ){
    return glm::length(origin1-origin2) < radius1+radius2
  }
  bool is_sphere_overlapped(
  ){
    return glm::length(origin1-origin2) < radius1+radius2;
  }
  template<typename scalar, glm::qualifier quality=glm::defaultp>
  maybe_range distance_to_surface_along_line(
    const glm::vec<3,scalar,quality> reference, 
    const glm::vec<3,scalar,quality> direction, 
    const glm::vec<3,scalar,quality> origin, 
    const scalar radius
  ) const {

    float xz = glm::dot(origin - reference, direction);
    float z = glm::length(reference + direction * xz - origin);
    float y2 = r * r - z * z;
    float dxr = std::sqrt(std::max(y2, 1e-10));
    return maybe_range(
        xz - dxr,
        xz + dxr, 
        y2 > 0.
    );
  }
  maybe_circle intersection_with_sphere(
  ){
    return maybe_circle(O,N,r,exists);
  }
  */
};
template<typename scalar>
Spheres<scalar> spheres(const scalar pi)
{
  return Spheres<scalar>(pi);
}

template<
  typename Spheres,
  typename mass,
  typename length,
  typename luminosity,
  typename temperature,
  typename intensity_per_temperature4
>
struct Stars
{
  const Spheres sphere;
  const intensity_per_temperature4 stephan_boltzmann_constant;
  const mass solar_mean_molecular_mass;
  const mass solar_mass;
  const length solar_radius;
  const luminosity solar_luminosity;
  const temperature solar_core_temperature;
  Stars(
    const Spheres sphere,
    const intensity_per_temperature4 stephan_boltzmann_constant,
    const mass solar_mean_molecular_mass,
    const mass solar_mass,
    const length solar_radius,
    const luminosity solar_luminosity,
    const temperature solar_core_temperature
  ):
    sphere(sphere),
    stephan_boltzmann_constant(stephan_boltzmann_constant),
    solar_mean_molecular_mass(solar_mean_molecular_mass),
    solar_mass(solar_mass),
    solar_radius(solar_radius),
    solar_luminosity(solar_luminosity),
    solar_core_temperature(solar_core_temperature)
  {}
  // the approximations below are ripped from Artifexian's video on stars:
  // https://www.youtube.com/watch?v=x55nxxaWXAM
  // we only need rough guesses for these properties, so they should be good enough
  luminosity luminosity_estimate(const mass star_mass) const
  {
      return solar_luminosity * pow(star_mass/solar_mass, 3.5);
  }
  length radius_estimate(const mass star_mass) const
  {
      return solar_radius * (star_mass < solar_mass? pow(star_mass/solar_mass, 0.8) : pow(star_mass/solar_mass, 0.5));
  }
  temperature surface_temperature_estimate(const mass star_mass) const
  {
      return sqrt(sqrt(intensity_estimate(star_mass) / stephan_boltzmann_constant));
  }
  temperature core_temperature_estimate(const mass star_mass, const mass star_mean_molecular_mass)  const
  {
      return (
          solar_core_temperature 
          * (star_mean_molecular_mass / solar_mean_molecular_mass)
          * (star_mass / solar_mass)
          / (radius_estimate(star_mass) / solar_radius)
      );
  }
  auto intensity_estimate(const mass star_mass) const
  {
      return luminosity_estimate(star_mass) / sphere.surface_area(radius_estimate(star_mass));
  }
  // from Carl Hansen et al., "Stellar Interiors"
};
template<
  typename Spheres,
  typename mass,
  typename length,
  typename luminosity,
  typename temperature,
  typename intensity_per_temperature4
>
auto stars(
    const Spheres sphere,
    const intensity_per_temperature4 stephan_boltzmann_constant,
    const mass solar_mass,
    const mass solar_mean_molecular_mass,
    const length solar_radius,
    const luminosity solar_luminosity,
    const temperature solar_core_temperature
){
  return Stars<Spheres,mass,length,luminosity,temperature,intensity_per_temperature4>
  (
    sphere,
    stephan_boltzmann_constant,
    solar_mass,
    solar_mean_molecular_mass,
    solar_radius,
    solar_luminosity,
    solar_core_temperature
  );
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


  std::vector<vec3> instance_grid_ids{ 
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
    1,1,301,601,1211,1,1,1,1,
    1,5771,2101,9701,30001,1,1,1,1,
    1,1,1,1,1,1,1,1,1
  };

  /*
  temperatures for stars calculated with approximation from Carl Hansen, "Stellar Interiors"
  temperatures for brown dwarves from Garani (2021)
  */
  std::vector<float> instance_core_temperature{ 
    1,1,1,1e5,3e6,1,1,1,1, // earth, large hot jupiter, Luhman 16A
    1,15e6,94e3,53e6,2e9,1,1,1,1, // sunlike, red dwarf, blue giant, ultramassive, 
    1,1,1,1,1,1,1,1,1
  };

  float Hp(0.6); // molecular mass of a hydrogen plasma, estimated by Carl Hansen, "Stellar Interiors"
  std::vector<float> instance_atmosphere_molecular_mass{ 
    Hp,Hp,28.97,2.22,2.22,Hp,Hp,Hp,Hp, // earth, large hot jupiter, Luhman 16A
    Hp,Hp,Hp,Hp,Hp,Hp,Hp,Hp,Hp,
    Hp,Hp,Hp,Hp,Hp,Hp,Hp,Hp,Hp
  };

  float Re(si::earth_radius / si::meter);
  float Rj(si::jupiter_radius / si::meter);
  float Rs(si::solar_radius / si::meter);

  std::vector<float> instance_radii{ 
    Rj,Rj,
    Re, // earth
    Rj, // large hot jupiter
    0.85f*Rj, // Luhman 16A
    Rj,Rj,Rj,Rj, 
    Rj,
    Rs, // sunlike
    0.09f*Rs, // red dwarf
    1.7f*Rs, // blue giant
    7.4f*Rs, // ultramassive
    Rj,Rj,Rj,Rj,
    Rj,Rj,Rj,Rj,Rj,Rj,Rj,Rj,Rj
  };

  float Me(si::earth_mass / si::kilogram);
  float Mj(si::jupiter_mass / si::kilogram);
  float Ms(si::solar_mass / si::kilogram);
  si::mass minimum_stellar_mass(85.2*si::jupiter_mass);

  std::vector<float> instance_masses{
    Mj,Mj,Mj,
    Me, // earth
    Mj, // large hot jupiter
    0.034f*Ms, // Luhman 16A
    Mj,Mj,Mj, 
    Mj,
    1.0f * Ms, // sunlike
    0.1f * Ms, // red dwarf
    2.1f * Ms, // blue giant
    18.0f * Ms, // ultramassive
    Mj,Mj,Mj,Mj, 
    Mj,Mj,Mj,Mj,Mj,Mj,Mj,Mj,Mj
  };

  /*
  scale heights calculated from the standard equation, h=kᵦT/(mg)
  */
  auto atmosphere = atmospheres(si::boltzmann_constant);
  auto point = point_masses(si::gravitational_constant);
  auto star = stars(
    spheres(3.14159265358979),
    si::stephan_boltzmann_constant,
    0.6*si::dalton,
    si::solar_mass,
    si::solar_radius,
    si::solar_luminosity,
    15e6*si::kelvin
  );
  std::vector<float> instance_atmosphere_scale_height;
  std::vector<vec3> instance_origins;
  std::vector<vec3> instance_illumination_luminosity;
  for(std::size_t i=0; i<instance_radii.size(); i++)
  {    instance_origins.push_back(instance_grid_ids[i]*Rs*10.0f);
    instance_illumination_luminosity.push_back(vec3(si::solar_luminosity/si::watt));
    auto mass = instance_masses[i]*si::kilogram;
    instance_radii[i] = mass < minimum_stellar_mass? instance_radii[i] : star.radius_estimate(mass)/si::meter;
    instance_core_temperature[i] = mass < minimum_stellar_mass? 1.0 : star.core_temperature_estimate(mass, 0.6*si::dalton)/si::kelvin;
    instance_surface_temperature[i] = mass < minimum_stellar_mass? 1.0 : star.surface_temperature_estimate(mass)/si::kelvin;
    instance_atmosphere_scale_height.push_back(
      float(
        atmosphere.scale_height(
          instance_surface_temperature[i] * si::kelvin, 
          instance_atmosphere_molecular_mass[i] * si::dalton, 
          point.gravity(instance_masses[i] * si::kilogram, instance_radii[i] * si::meter)
        ) / si::meter
      )
    );
    std::cout << instance_atmosphere_scale_height[i] << " " << instance_surface_temperature[i] << std::endl;
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
  control_state.log2_height = std::log2(40.0*Rs);
  // control_state.log2_height = 20.0f;
  control_state.angular_position = glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f;

  using length = si::length<float>;
  length meter(si::meter);
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
    planet_billboard_near_distance/meter, 
    point_source_near_distance/meter
  );
  view_state.view_matrix = control_state.get_view_matrix();
  view_state.resolution = glm::vec2(850, 640);
  view_state.wavelength = glm::vec3(650e-9, 550e-9, 450e-9);
  view_state.exposure_intensity = 1e9*si::global_solar_constant/(si::watt/si::meter2);
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
        instance_radii,
        instance_surface_temperature,
        instance_core_temperature,
        instance_atmosphere_scale_height,
        instance_light_source,
        instance_illumination_luminosity,
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


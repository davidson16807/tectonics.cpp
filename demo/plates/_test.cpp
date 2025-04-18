
// std libraries
#include <iostream>
#include <string>
#include <vector>
#include <functional>

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
#include <index/known.hpp>                          // max, mean
#include <index/whole.hpp>                          // max, mean
#include <index/procedural/Range.hpp>                   // Range
#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/glm/UnitVectorNoise.hpp>
#include <index/procedural/noise/GaussianNoise.hpp>
#include <index/adapted/symbolic/TypedSymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/adapted/si/SiStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Strings.hpp>
#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Bitset.hpp>
#include <index/grouped/Statistics.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp> // dymaxion::RankedFractalBrownianNoise

#include <relation/ScalarRelation.hpp>

#include <buffer/PyramidBuffers.hpp>                // buffer::PyramidBuffers

#include <grid/dymaxion/Grid.hpp>                   // dymaxion::Grid
#include <grid/dymaxion/series.hpp>                 // dymaxion::BufferVertexIds
#include <grid/dymaxion/VertexDownsamplingIds.hpp>  // dymaxion::VertexDownsamplingIds
#include <grid/dymaxion/buffer/WholeGridBuffers.hpp>// dymaxion::WholeGridBuffers

#include <raster/unlayered/VectorCalculusByFundamentalTheorem.hpp> // unlayered::VectorCalculusByFundamentalTheorem
#include <raster/unlayered/SeedBasedFloodFilling.hpp>     // unlayered::SeedBasedFloodFilling
#include <raster/unlayered/NeighborBasedFloodFilling.hpp> // unlayered::NeighborBasedFloodFilling
#include <raster/spheroidal/Strings.hpp>            // spheroidal::Strings
#include <raster/unlayered/Morphology.hpp>          // unlayered::Morphology

#include <model/rock/estimated/EarthlikeIgneousFormationGeneration.hpp>
#include <model/rock/column/ColumnSummaryProperties.hpp>  // ColumnProperties
#include <model/rock/stratum/StratumProperties.hpp>  // StratumProperties
#include <model/rock/stratum/StratumSummarization.hpp>  // StratumSummarization
#include <model/rock/stratum/StratumSummaryProperties.hpp>  // StratumSummaryIsostaticDisplacement
#include <model/rock/formation/Formation.hpp>       // Formation
#include <model/rock/formation/FormationSummarization.hpp>  // FormationSummarization
#include <model/rock/crust/Crust.hpp>  // Crust
#include <model/rock/crust/CrustSummarization.hpp>  // CrustSummarization
#include <model/rock/crust/CrustSummaryOps.hpp>  // CrustSummaryOps
#include <model/rock/crust/CrustSummaryProperties.hpp>  // CrustProperties
#include <model/rock/crust/CrustMotion.hpp>         // CrustMotion

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
  GLFWwindow* window = glfwCreateWindow(850, 640, "Hello Terrain", NULL, NULL);
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

  using mass = si::mass<float>;
  using density = si::density<float>;
  using length = si::length<float>;
  using pressure = si::pressure<float>;
  using viscosity = si::dynamic_viscosity<float>;
  using acceleration = si::acceleration<float>;

  using vec3 = glm::vec3;

  length meter(si::meter);

  length world_radius(6.371e6 * si::meter);
  density mantle_density(3000.0*si::kilogram/si::meter3);
  viscosity mantle_viscosity(1.57e20*si::pascal*si::second);
  int vertices_per_fine_square_side(30);
  int vertices_per_coarse_square_side(vertices_per_fine_square_side/2);
  dymaxion::Grid fine(world_radius/meter, vertices_per_fine_square_side);
  dymaxion::Grid coarse(world_radius/meter, vertices_per_coarse_square_side);
  dymaxion::VertexPositions fine_vertex_positions(fine);
  dymaxion::VertexPositions coarse_vertex_positions(coarse);
  dymaxion::VertexNormals coarse_vertex_normals(coarse);
  dymaxion::VertexDownsamplingIds vertex_downsampling_ids(fine.memory, coarse.memory);

  iterated::Identity copy;

  const int M(2);
  const int F(5);

  rock::EarthlikeIgneousFormationGeneration earthlike(fine, world_radius/2.0f, 0.5f, 10, world_radius);
  auto generation = earthlike(12.0f, 1.1e4f);

  rock::StratumStore<M> empty_stratum;
  rock::Formation<M> empty_formation(fine.vertex_count(), empty_stratum);
  rock::Formation<M> igneous_formation(fine.vertex_count());
  copy(generation, igneous_formation);
  rock::Crust<M,F> crust{empty_formation, empty_formation, empty_formation, igneous_formation, empty_formation};

  auto age_of_world = 0.0f*si::megayear;
  std::array<relation::PolynomialRailyardRelation<si::time<double>,si::density<double>,0,1>, 2> densities_for_age {
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2890.0, 3300.0}), // Carlson & Raskin 1984
    relation::get_linear_interpolation_function(si::megayear, si::kilogram/si::meter3, {0.0, 250.0}, {2600.0, 2600.0})
  };

  rock::CrustSummaryOps crust_summary_ops{
    rock::ColumnSummaryOps{
      rock::StratumSummaryOps{density(3300.0*si::kilogram/si::meter3)}, 
      length(si::centimeter)
    }
  };

  auto formation_summarize = rock::formation_summarization<2>(
    rock::stratum_summarization<2>(
      rock::AgedStratumDensity{densities_for_age, age_of_world},
      mass(si::tonne)
    ), 
    fine,
    world_radius
  );

  auto crust_summarize = rock::crust_summarization<M,F>(
    formation_summarize, 
    crust_summary_ops
  );
  unlayered::VectorCalculusByFundamentalTheorem calculus;
  // auto motion = rock::crust_motion<M>(
  //     calculus, fine, 
  //     world_radius, 
  //     acceleration(si::standard_gravity), 
  //     mantle_density, 
  //     mantle_viscosity
  // );

  iterated::Unary buoyancy_pressure_for_formation_summary(
    rock::StratumSummaryBuoyancyPressure{
      acceleration(si::standard_gravity), 
      mantle_density, 
    }
    // rock::StratumSummaryThickness{}
    // rock::StratumSummaryIsostaticDisplacement{
    //   density(3300.0*si::kilogram/si::meter3)
    // }
  );

  grouped::Statistics stats{adapted::TypedSymbolicArithmetic<float>(0.0f, 1.0f)};
  iterated::Arithmetic arithmetic{adapted::SymbolicArithmetic{}};

  rock::CrustSummary crust_summary(fine.vertex_count());
  rock::FormationSummary formation_summary(fine.vertex_count());
  std::vector<pressure> fine_buoyancy_pressure(fine.vertex_count());

  std::vector<pressure> coarse_buoyancy_pressure(coarse.vertex_count());
  std::vector<float> vertex_scalars1(coarse.vertex_count());

  int plate_id(1);
  crust_summarize(plate_id, crust, crust_summary, formation_summary);
  crust_summary_ops.flatten(crust_summary, formation_summary);
  formation_summarize(plate_id, igneous_formation, formation_summary);
  // motion.buoyancy(formation_summary, coarse_buoyancy_pressure);
  buoyancy_pressure_for_formation_summary(formation_summary, fine_buoyancy_pressure);

  std::vector<float> scalar_buoyancy_pressure(coarse.vertex_count());
  stats.sum(vertex_downsampling_ids, fine_buoyancy_pressure, coarse_buoyancy_pressure);
  arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(std::pow(float(vertex_downsampling_ids.factor), 2.0f)), coarse_buoyancy_pressure);
  arithmetic.divide(coarse_buoyancy_pressure, procedural::uniform(pressure(1)), vertex_scalars1);

  iterated::Metric metric3{adapted::GlmMetric{}};
  aggregated::Order order(adapted::SymbolicOrder{});

  std::vector<vec3> vertex_gradient(coarse.vertex_count());
  calculus.gradient(coarse, vertex_scalars1, vertex_gradient);

  // float average_separation(world_radius/meter*3.1415926535/coarse.vertices_per_meridian());
  // auto is_similar = [average_separation](auto A, auto U, auto O, auto V) { 
  //     // auto AOV = math::similarity(A-O,V) > -0.5f;
  //     // return std::isnan(AOV)? true : AOV;
  //     // return true;
  //     return math::similarity (U,V) > std::cos(M_PI * 130.0f/180.0f);
  // };
  // auto is_unfractured = [average_separation](auto A, auto U, auto O, auto V) { 
  //     // count += 1.0f;
  //     // sum += glm::distance(A+U,B+V) - glm::distance(A,B);
  //     // return math::similarity(U,B-A) <= 0.5 && (glm::distance(A+U,B+V) - glm::distance(A,B)) < 7500.0f;
  //     // return true;
  //     // if (glm::distance(B,A) > 0.0001)
  //     // {
  //     //   count += 1.0f;
  //     //   sum += glm::distance(A+U,B+V) / average_separation;
  //     //   // std::cout << glm::distance(A+U,B+V) << std::endl;
  //     // }
  //     auto B = A + average_separation*glm::normalize(O-A);
  //     auto displacement = (std::abs(glm::distance(A+U,B+V)-average_separation) / average_separation);
  //     return std::isnan(displacement) || (displacement < 1.2e5f);
  // };
  auto is_unfractured = [](vec3 A, vec3 U, vec3 B, vec3 V, vec3 O) { 
      // std::cout<<std::to_string(V.x) << " " << std::to_string(V.y) << " " << std::to_string(V.z) <<std::endl;
      // vec3 A2 = A+U;
      // vec3 B2 = B+V;
      vec3 I = B-A;
      float l = glm::length(I);
      vec3 K = glm::normalize(A+B);
      vec3 J = l*glm::normalize(glm::cross(I,K)); // orthogonal to I, running along the surface, same scale as I
      // vec3 I2 = B2-A2;
      vec3 DL = V-U; // equivalent to I2-I, calculated as V-U to avoid precision issues;
      float tensile_DL = glm::dot(DL,I) / l; // effectively the scalar projection of DL onto I
      float shear_DL = glm::dot(DL,J) / l; // effectively the scalar projection of DL onto J
      float tensile_stress_factor = tensile_DL / l; // proportionate to tensile stress
      float shear_stress_factor = shear_DL / l; // proportionate to shear stress
      float compressive_strength(-1e-30);
      float tensile_strength(3e-11);
      float shear_strength(3e-10);
      // std::cout<<std::to_string(shear_stress_factor*1e12) <<std::endl;
      return 
        compressive_strength < tensile_stress_factor && tensile_stress_factor < tensile_strength
        && std::abs(shear_stress_factor) < shear_strength;
      ;
  };
  auto distance = [](vec3 A, vec3 U, vec3 B, vec3 V, vec3 O) { 
    return glm::distance(A,O);
  };
  // auto displacement = [average_separation](auto A, auto U, auto O, auto V) { 
  //     auto B = A + average_separation*glm::normalize(O-A);
  //     auto displacement = (std::abs(glm::distance(A+U,B+V)-average_separation) / average_separation);
  //     return (displacement < 1.2e5f);
  // };

  // segmentation
  auto fill = unlayered::neighbor_based_flood_filling<int,float>(is_unfractured, distance);
  iterated::Ternary ternary;
  std::vector<float> lengths(vertex_gradient.size());
  metric3.length(vertex_gradient, lengths);
  std::vector<bool> is_considered(vertex_gradient.size(), true);
  std::vector<unlayered::FloodFillState<int,float>> states;
  for(std::size_t j(0); j<8; j++)
  {
    states.emplace_back(int(order.max_id(lengths)), int(vertex_gradient.size()));
    for (int i = 0; i < 30; ++i)
    {
      fill.advance(coarse, vertex_gradient, is_considered, states[j]);
    }
    ternary(is_considered, lengths, procedural::uniform(0), lengths);
  }

  iterated::Bitset bitset{adapted::BooleanBitset{}};
  unlayered::Morphology morphology{bitset};

  adapted::SymbolicOrder suborder;
  adapted::SiStrings substrings;
  aggregated::Order ordered(suborder);
  auto ascii_art = spheroidal::Strings(substrings, ordered);
  // auto strings = aggregated::Strings(substrings, ordered, vertices_per_fine_square_side);
  std::cout << ascii_art.format(fine, fine_buoyancy_pressure) << std::endl << std::endl;
  // std::cout << strings.format(fine_buoyancy_pressure) << std::endl << std::endl;


  // flatten raster for OpenGL
  dymaxion::WholeGridBuffers<int,float> grids(vertices_per_coarse_square_side);
  std::vector<float> buffer_color_values(coarse.vertex_count());
  std::vector<float> buffer_scalars1(coarse.vertex_count());
  std::vector<float> buffer_scalars2(coarse.vertex_count());
  std::vector<float> buffer_uniform(coarse.vertex_count(), 1.0f);
  std::vector<std::byte> buffer_culling(coarse.vertex_count(), std::byte(0));
  std::vector<glm::vec3> buffer_positions(coarse.vertex_count());
  std::vector<unsigned int> buffer_element_vertex_ids(grids.triangle_strips_size(coarse_vertex_positions));
  std::cout << "vertex count:        " << coarse.vertex_count() << std::endl;
  std::cout << "vertices per meridian" << coarse.vertices_per_meridian() << std::endl;
  // copy(vertex_colored_scalars, buffer_color_values);
  std::vector<float> scratch(coarse.vertex_count());
  std::vector<bool> mask1(coarse.vertex_count());
  std::vector<bool> mask2(coarse.vertex_count());
  std::vector<bool> mask3(coarse.vertex_count());

  // copy(vertex_colored_scalars, buffer_color_values);
  // copy(vertex_scalars2, buffer_scalars2);
  copy(coarse_vertex_positions, buffer_positions);
  grids.storeTriangleStrips(procedural::range<unsigned int>(coarse.vertex_count()), buffer_element_vertex_ids);

  length procedural_terrain_far_distance(3e3*si::kilometer);
  length planet_billboard_near_distance(1e7*si::kilometer); // ~10 * solar radius 

  // initialize control state
  update::OrbitalControlState control_state(
      glm::vec2(45.0f, 30.0f) * 3.14159f/180.0f, // angular_position
      glm::vec2(0), // angular_direction
      (world_radius+procedural_terrain_far_distance)/meter, // min_zoom_distance
      23.0f // log2_height
  );
  
  // initialize view state
  view::ViewState view_state;
  view_state.projection_matrix = glm::perspective(
    3.14159f*45.0f/180.0f, 
    850.0f/640.0f, 
    procedural_terrain_far_distance/meter,      // near plane distance
    planet_billboard_near_distance/meter // far plane distance
  );
  view_state.view_matrix = control_state.get_view_matrix();
  // view_state.projection_type = view::ProjectionType::heads_up_display;
  // view_state.projection_matrix = glm::mat4(1);
  // view_state.view_matrix = glm::mat4(1);
  view::ColorscaleSurfacesViewState colorscale_state;
  colorscale_state.min_color_value = whole::min(vertex_scalars1);
  colorscale_state.max_color_value = whole::max(vertex_scalars1);
  colorscale_state.darken_threshold = whole::mean(buffer_scalars2);

  // initialize shader program
  view::ColorscaleSurfaceShaderProgram colorscale_program;  
  view::MultichannelSurfaceShaderProgram debug_program;
  view::IndicatorSwarmShaderProgram indicator_program;  

  // initialize MessageQueue for MVU architecture
  messages::MessageQueue message_queue;
  message_queue.activate(window);

  std::cout << whole::min(vertex_scalars1) << std::endl;
  std::cout << whole::max(vertex_scalars1) << std::endl;

  // flatten vector raster for OpenGL
  buffer::PyramidBuffers<int, float> pyramids;
  std::vector<glm::vec3> vectors_element_position(pyramids.triangles_size<3>(3));
  std::vector<glm::vec3> vectors_instance_position(coarse.vertex_count());
  std::vector<glm::vec4> vectors_instance_color(coarse.vertex_count(), glm::vec4(1.0f));
  std::vector<glm::vec3> vectors_instance_up(coarse.vertex_count());
  std::vector<float> vectors_instance_scale(coarse.vertex_count());
  float pyramid_radius(coarse.total_circumference()/(8.0*coarse.vertices_per_meridian()));
  float pyramid_halflength(2.5f*pyramid_radius);
  pyramids.storeTriangles(
      glm::vec3(0,0,-1) * pyramid_halflength, 
      glm::vec3(0,0, 1) * pyramid_halflength, 
      glm::vec3(1,0, 0),  pyramid_radius, 3, 
      vectors_element_position);
  copy          (known::mult(coarse_vertex_positions, procedural::uniform(1+pyramid_halflength/coarse.total_radius())),  vectors_instance_position);
  copy          (coarse_vertex_normals, vectors_instance_up);
  metric3.length (vertex_gradient,   vectors_instance_scale);
  arithmetic.divide(vectors_instance_scale, procedural::uniform(whole::max(vectors_instance_scale)), vectors_instance_scale);

  int frame_id(0);
  int iteration_id(0);
  while(!glfwWindowShouldClose(window)) {

      if (frame_id == 0)
      {
        for (std::size_t j(1); j < states.size(); ++j)
        {
          fill.advance(coarse, vertex_gradient, is_considered, states[j]);
        }
        for (std::size_t j(1); j < states.size(); ++j)
        {
          ternary(states[j].is_included, procedural::uniform(j), buffer_scalars1, buffer_scalars1);
        }
        std::size_t candidate_count = 0;
        for (std::size_t j(1); j < states.size(); ++j)
        {
          candidate_count += states[j].candidates.size();
        }
        if (candidate_count<=0)
        {
          std::cout << "DONE: " << iteration_id << std::endl;
        }
        iteration_id++;
      }
      frame_id = (frame_id+1)%10;

      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      colorscale_state.max_color_value = whole::max(buffer_scalars1);
      colorscale_state.min_color_value = whole::min(buffer_scalars1);
      colorscale_program.draw(
        buffer_positions, // position
        buffer_scalars1,  // color value
        buffer_uniform,   // displacement
        buffer_uniform,   // darken
        buffer_culling,   // culling
        buffer_element_vertex_ids,
        colorscale_state,
        glm::mat4(1),
        view_state,
        GL_TRIANGLE_STRIP
      );

      indicator_program.draw(
        vectors_element_position,
        vectors_instance_position,
        vertex_gradient,
        vectors_instance_up,
        vectors_instance_scale,
        vectors_instance_color,
        glm::mat4(1),
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

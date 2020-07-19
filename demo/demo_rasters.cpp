// C libraries
#include <stdio.h>

// std libraries
#include <string>
#include <random>       // rngs

// gl libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm libraries
#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly

// in house libraries
#include <many/types.hpp>  
#include <many/glm/random.hpp>  
#include <meshes/mesh.hpp>
#include <grids/SpheroidGrid/string_cast.hpp>  
#include <view/ColorscaleSurfacesShaderProgram.hpp>
#include <view/TriangleTestShaderProgram.hpp>

int main() {
  // initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for MacOS
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // we don't want the old OpenGL

  // open a window
  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }

  // create a GLEW context
  glfwMakeContextCurrent(window);

  // initialize GLEW 
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW");
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

  /* OTHER STUFF GOES HERE NEXT */
  std::mt19937 generator(2);
  meshes::mesh icosphere_mesh(meshes::icosahedron.vertices, meshes::icosahedron.faces);
  icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
  icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
  icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
  icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
  icosphere_mesh = meshes::subdivide(icosphere_mesh); many::normalize(icosphere_mesh.vertices, icosphere_mesh.vertices);
  rasters::SpheroidGrid icosphere_grid(icosphere_mesh.vertices, icosphere_mesh.faces);
  many::floats vertex_color_values = many::floats(icosphere_mesh.vertices.size());
  many::floats vertex_displacements = many::floats(icosphere_mesh.vertices.size());
  many::get_elias_noise(icosphere_grid.vertex_positions, generator, vertex_color_values, 10, 0.0001f);
  many::get_elias_noise(icosphere_grid.vertex_positions, generator, vertex_displacements, 10, 0.0001f);
  std::string str_raster = to_string(icosphere_grid, vertex_color_values);
  std::cout << str_raster << std::endl;
  many::floats flattened_face_vertex_color_values(icosphere_grid.flattened_face_vertex_ids.size());
  many::floats flattened_face_vertex_displacements(icosphere_grid.flattened_face_vertex_ids.size());
  many::get(vertex_color_values,  icosphere_grid.flattened_face_vertex_ids, flattened_face_vertex_color_values);
  many::get(vertex_displacements, icosphere_grid.flattened_face_vertex_ids, flattened_face_vertex_displacements);
  view::TriangleTestShaderProgram program;  
  view::ColorscaleSurfacesViewState<float> colorscale_state;
  view::ViewState view_state;

  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      program.draw();
      // update other events like input handling
      glfwPollEvents();
      // put stuff we've been drawing onto the display
      glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}


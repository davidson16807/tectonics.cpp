#include <stdio.h>

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MULTI_LINE_STRING(a) #a

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

  float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
  };

  float colors[] = {
   1.0f,  0.0f,  0.0f,
   0.0f,  1.0f,  0.0f,
   0.0f,  0.0f,  1.0f
  };
  
  int points_id = 0;
  GLuint points_vbo = 0;
  // create a new vertex array buffer, VBO
  glGenBuffers(1, &points_vbo);
  // set current buffer in OpenGL's state machine to points_vbo 
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  // store values within current buffer
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);
  
  int colors_id = 1;
  GLuint colors_vbo = 0;
  // create a new vertex array buffer, VBO
  glGenBuffers(1, &colors_vbo);
  // set current buffer in OpenGL's state machine to colors_vbo 
  glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
  // store values within current buffer
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), colors, GL_STATIC_DRAW);

  GLuint vao = 0;
  // generate new vertex array object, VAO
  glGenVertexArrays(1, &vao);
  // set current array in OpenGL's state machine to vao
  glBindVertexArray(vao);

  // enable attributes
  glEnableVertexAttribArray(points_id);
  glEnableVertexAttribArray(colors_id);
  
  // set current buffer in OpenGL's state machine to points_vbo (if not already)
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  // define layout of the first (current?) buffer
  // attr #, 3 dimensions, floats
  glVertexAttribPointer(points_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // set current buffer in OpenGL's state machine to points_vbo (if not already)
  glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
  // define layout of the first (current?) buffer
  // attr #, 3 dimensions, floats
  glVertexAttribPointer(colors_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);

const char* vertex_shader =
"#version 330\n"
"in vec3 point;"
"in vec3 color;"
"out vec3 v_color;"
"void main() {"
"  v_color = color;"
"  gl_Position = vec4(point, 1.0);"
"}";

  const char* fragment_shader =
"#version 330\n"
"in vec3 v_color;"
"out vec4 frag_color;"
"void main() {"
"  frag_color = vec4(v_color, 1.);"
"}";

  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  // const char* vertex_source = vertex_shader.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
  glCompileShader(vertex_shader_id);

  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  // const char* fragment_source = fragment_shader.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
  glCompileShader(fragment_shader_id);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, fragment_shader_id);
  glAttachShader(shader_program, vertex_shader_id);

  glBindAttribLocation(shader_program, points_id, "point");
  glBindAttribLocation(shader_program, colors_id, "color");

  glLinkProgram(shader_program);
  
  while(!glfwWindowShouldClose(window)) {
      // wipe drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(shader_program);
      glBindVertexArray(vao);
      // draw points 0-3 from the currently bound VAO with current in-use shader
      //glDrawArrays(GL_POINTS, 0, 3);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // update other events like input handling
      glfwPollEvents();
      // put stuff we've been drawing onto the display
      glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}


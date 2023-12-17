#pragma once

// std libraries
#include <regex>          // std::regex
#include <vector>         // std::vector
#include <unordered_set>  // std::unordered_set

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/gtc/type_ptr.hpp>// *vec4

namespace view 
{

	class TriangleTestShaderProgram
	{
	    // properties shared by all attributes within the program
	    static constexpr GLenum normalize = GL_FALSE; // don't normalize the data
	    static constexpr GLsizei stride = 0;          // 0: advance size*sizeof(type) each iteration to get the next position
	    static constexpr void* offset = NULL;         // NULL: start at the beginning of the buffer

		std::string vertexShaderGlsl;
		std::string fragmentShaderGlsl;

		GLuint shaderProgramId;
		GLuint attributeId;
		GLuint positionBufferId;
		GLuint colorValueBufferId;

		// attributes
	    GLuint positionLocation;
	    GLuint colorValueLocation;

		bool isDisposed;
	public:
		~TriangleTestShaderProgram()
		{
		}
		explicit TriangleTestShaderProgram() :
			vertexShaderGlsl(
				std::regex_replace(
					R"(#version 330
						in vec3 point;
						in vec3 color;
						out vec3 v_color;
						void main() {
						  v_color = color;
						  gl_Position = vec4(point, 1.0);
						})",
					std::regex("\\{\\}"), 
					get_default_clipspace_position_glsl
				)
			),
			fragmentShaderGlsl(
				  R"(#version 330
					in vec3 v_color;
					out vec4 frag_color;
					void main() {
					  frag_color = vec4(v_color, 1.);
					})"
			),
			isDisposed(false)
		{
    		int success;
		    char infoLog[512];

			/*
			NOTE: OpenGL copies shader source code strings 
			when glShaderSource is called, so an application may free
			its copy of the source code strings immediately after the function returns
			*/
			GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			const char *vertex_shader_c_str = vertexShaderGlsl.c_str();
			glShaderSource(vertexShaderId, 1, &vertex_shader_c_str, NULL);
			glCompileShader(vertexShaderId);
		    // check for shader compile errors
		    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		    if (!success)
		    {
		        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		    }

			GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
			const char *fragment_shader_c_str = fragmentShaderGlsl.c_str();
			glShaderSource(fragmentShaderId, 1, &fragment_shader_c_str, NULL);
			glCompileShader(fragmentShaderId);
		    // check for shader compile errors
		    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		    if (!success)
		    {
		        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		    }

			shaderProgramId = glCreateProgram();
			glAttachShader(shaderProgramId, fragmentShaderId);
			glAttachShader(shaderProgramId, vertexShaderId);
    		glLinkProgram(shaderProgramId);
		    // check for linking errors
		    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
		    if (!success) {
		        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
		        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		    }

	        // delete the shaders, they're linked to our program now and no longer needed
	        glDeleteShader(vertexShaderId);
	        glDeleteShader(fragmentShaderId);

	        // UNIFORMS

	        // ATTRIBUTES

			// generate new vertex array object, VAO
		    glGenVertexArrays(1, &attributeId);
		    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		    glBindVertexArray(attributeId);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &positionBufferId);
			positionLocation = glGetAttribLocation(shaderProgramId, "point");
		    glEnableVertexAttribArray(positionLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &colorValueBufferId);
			colorValueLocation = glGetAttribLocation(shaderProgramId, "color");
		    glEnableVertexAttribArray(colorValueLocation);
		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &colorValueBufferId);
		        glDeleteBuffers(1, &positionBufferId);
		        glDeleteProgram(shaderProgramId);
        	}
		}

		template <typename T>
		bool canDepict(const std::vector<T>& points, const std::vector<T>& colors){
			return !isDisposed;
		}

		template <typename T>
		void draw(const std::vector<T>& points, const std::vector<T>& colors){
			if (!canDepict(points, colors))
			{
				return;
			}
			glUseProgram(shaderProgramId);
			glBindVertexArray(attributeId);

			//ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*points.size(), &points.front(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(positionLocation, 3, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, colorValueBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*colors.size(), &colors.front(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(colorValueLocation, 3, GL_FLOAT, normalize, stride, offset);

			glDrawArrays(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ points.size()/3);
		}
	};
}
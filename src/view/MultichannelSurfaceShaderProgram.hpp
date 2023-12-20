#pragma once

// std libraries
#include <regex>          // std::regex
#include <vector>         // std::vector
#include <unordered_set>  // std::unordered_set

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/gtc/type_ptr.hpp>// *vec4

// in house libraries
#include <view/ViewState.hpp> // ViewState

namespace view 
{

	/*
	`ColorscaleSurfaceShader` renders a surface (i.e. where vertices are referenced by an element buffer) 
	in a way that tries to maximize the number of possible scalar values that can be usefully depicted at a single time.
	This is intended mainly for debugging. 

	To accomplish this goal, each color channel can be controlled separately with its own scalar buffer.
	*/
	class MultichannelSurfaceShaderProgram
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
		GLuint redBufferId;
		GLuint greenBufferId;
		GLuint blueBufferId;
		GLuint opacityBufferId;
		GLuint displacementBufferId;
		GLuint elementVertexBufferId;

		// attributes
	    GLuint positionLocation;
	    GLuint redLocation;
	    GLuint greenLocation;
	    GLuint blueLocation;
	    GLuint opacityLocation;
	    GLuint displacementLocation;

		// uniforms
	    GLint viewMatrixLocation;
		GLint modelMatrixLocation;
		GLint projectionMatrixLocation;
		GLint minValuesLocation;
		GLint maxValuesLocation;

		bool isDisposed;

	public:
		~MultichannelSurfaceShaderProgram()
		{
		}
		explicit MultichannelSurfaceShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  model_matrix;
			        uniform mat4  view_matrix;
			        uniform mat4  projection_matrix;
			        uniform vec4  min_values;
			        uniform vec4  max_values;
			        in      vec4  vertex_position;
			        in      float vertex_red;
			        in      float vertex_green;
			        in      float vertex_blue;
			        in      float vertex_opacity;
			        in      float vertex_displacement;
			        out     vec4  fragment_color_in;
			        void main(){
			        	// NOTE: for a heads up display, set all `*_matrix` parameters to identity
		            	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
			        	fragment_color_in = smoothstep(min_values, max_values, 
			        		vec4(vertex_red, vertex_green, vertex_blue, vertex_opacity));
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        in      vec4  fragment_color_in;
			        out     vec4  fragment_color;

			        void main() {
		        		fragment_color = fragment_color_in;
			        }
				)"
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
			const char* vertex_shader_glsl = vertexShaderGlsl.c_str();
			glShaderSource(vertexShaderId, 1, &vertex_shader_glsl, NULL);
			glCompileShader(vertexShaderId);
		    // check for shader compile errors
		    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		    if (!success)
		    {
		        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		    }

			GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fragment_shader_glsl = fragmentShaderGlsl.c_str();
			glShaderSource(fragmentShaderId, 1, &fragment_shader_glsl, NULL);
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

			viewMatrixLocation = glGetUniformLocation(shaderProgramId, "view_matrix");
			modelMatrixLocation = glGetUniformLocation(shaderProgramId, "model_matrix");
			projectionMatrixLocation = glGetUniformLocation(shaderProgramId, "projection_matrix");

			minValuesLocation = glGetUniformLocation(shaderProgramId, "min_values");
			maxValuesLocation = glGetUniformLocation(shaderProgramId, "max_values");

	        // ATTRIBUTES

			// generate new vertex array object, VAO
		    glGenVertexArrays(1, &attributeId);
		    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		    glBindVertexArray(attributeId);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &positionBufferId);
			positionLocation = glGetAttribLocation(shaderProgramId, "vertex_position");
		    glEnableVertexAttribArray(positionLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &redBufferId);
			redLocation = glGetAttribLocation(shaderProgramId, "vertex_red");
		    glEnableVertexAttribArray(redLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &greenBufferId);
			greenLocation = glGetAttribLocation(shaderProgramId, "vertex_green");
		    glEnableVertexAttribArray(greenLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &blueBufferId);
			blueLocation = glGetAttribLocation(shaderProgramId, "vertex_blue");
		    glEnableVertexAttribArray(blueLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &opacityBufferId);
			opacityLocation = glGetAttribLocation(shaderProgramId, "vertex_opacity");
		    glEnableVertexAttribArray(opacityLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &displacementBufferId);
			displacementLocation = glGetAttribLocation(shaderProgramId, "vertex_displacement");
		    glEnableVertexAttribArray(displacementLocation);

			glGenBuffers(1, &elementVertexBufferId);
		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &redBufferId);
		        glDeleteBuffers(1, &positionBufferId);
		        glDeleteBuffers(1, &displacementBufferId);
		        glDeleteProgram(shaderProgramId);
        	}
		}

	    /*
	    `canDepict()` returns whether this instance can fully depict a model using the given
	    view state upon calling `.draw()`.
	    If it cannot, the view state should be used to create a new instance, 
	    and the old instance should be disposed before falling out of scope.
	    This is not a test for whether the WebGL context or the view state 
	    is well formatted. It is strictly a test of the program's private state.

	    This demonstrates our approach to handling WebGL state management.
	    If changing something is trivial, like uniforms or attributes, 
	    we simply change it during the draw call without disposing resources.
	    If changing something requires managing highly intertwined private 
	    resources like shaders or programs, we simply wipe the slate clean
	    on the first sign that anything falls out of sync.
	    */
		bool canDepict(
			const ViewState& view_state
		){
			return !isDisposed;
		}

		/*
	    `draw()` adds a depiction of a scalar field to the framebuffer that is currently 
	    bound to the program's context using options from a given view state.
	    Does nothing if unable to depict the given view state, as determined by canDepict().
		The only state that is allowed to be modified is that of the framebuffer
		*/
		template <typename T, glm::qualifier Q=glm::defaultp>
		void draw(
			const std::vector<float>& vertex_positions, 
			const std::vector<T>& vertex_red,
			const std::vector<T>& vertex_green,
			const std::vector<T>& vertex_blue,
			const std::vector<T>& vertex_opacity,
			const std::vector<float>& vertex_displacements, 
			const std::vector<unsigned int>& element_vertex_ids,
			const glm::vec<4,T,Q>& min_values,
			const glm::vec<4,T,Q>& max_values,
			const ViewState& view_state,
			const unsigned int gl_mode=GL_TRIANGLES
		){
			if (!canDepict(view_state))
			{
				return;
			}
			if (view_state.render_pass != RenderPassType::solids)
			{
				return; 
			}

			assert(vertex_positions.size()/3 == vertex_red.size());
			assert(vertex_red.size() == vertex_displacements.size());
			assert(vertex_green.size() == vertex_displacements.size());
			assert(vertex_blue.size() == vertex_displacements.size());
			assert(vertex_opacity.size() == vertex_displacements.size());

			glUseProgram(shaderProgramId);
			glBindVertexArray(attributeId);
			glEnable(GL_CULL_FACE);  
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			//ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_positions.size(), &vertex_positions.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(positionLocation);
            glVertexAttribPointer(positionLocation, 3, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, redBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_red.size(), &vertex_red.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(redLocation);
            glVertexAttribPointer(redLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, greenBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_green.size(), &vertex_green.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(greenLocation);
            glVertexAttribPointer(greenLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, blueBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_blue.size(), &vertex_blue.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(blueLocation);
            glVertexAttribPointer(blueLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, opacityBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_opacity.size(), &vertex_opacity.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(opacityLocation);
            glVertexAttribPointer(opacityLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, displacementBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_displacements.size(), &vertex_displacements.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(displacementLocation);
            glVertexAttribPointer(displacementLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVertexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T)*element_vertex_ids.size(), &element_vertex_ids.front(), GL_DYNAMIC_DRAW);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));

	        glUniform4fv(minValuesLocation, 1, glm::value_ptr(min_values));
	        glUniform4fv(maxValuesLocation, 1, glm::value_ptr(max_values));

			glDrawElements(gl_mode, /*element count*/ element_vertex_ids.size(), GL_UNSIGNED_INT, 0);
		}
	};
}


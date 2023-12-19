#pragma once

// std libraries
#include <vector>         // std::vector

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/gtc/type_ptr.hpp>// *vec4

// in house libraries
#include <view/ViewState.hpp> // ViewState

namespace view 
{

	/*
	`IndicatorSwarmShaderProgram` renders a swarm of objects
	that are each copies of a single abstract concept, such as directions or positions.
	As abstract concepts, objects have no real world analog to imitate,
	so surfaces could be depicted in any style.
	To avoid inconsistent design, surfaces are depicted in a standardized style,
	which is rough, nonmetallic, and textureless.
	*/

	class IndicatorSwarmShaderProgram
	{
	    // properties shared by all attributes within the program
	    static constexpr GLenum normalize = GL_FALSE; // don't normalize the data
	    static constexpr GLsizei stride = 0;          // 0: advance size*sizeof(type) each iteration to get the next position
	    static constexpr void* offset = NULL;         // NULL: start at the beginning of the buffer

		std::string vertexShaderGlsl;
		std::string fragmentShaderGlsl;

		GLuint shaderProgramId;
		GLuint vertexArrayId;

		// element buffer ids
		GLuint elementPositionBufferId;

		// instance buffer ids
		GLuint instancePositionBufferId;
		GLuint instanceHeadingBufferId;
		GLuint instanceUpBufferId;
		GLuint instanceColorBufferId;
		GLuint instanceScaleBufferId;

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instancePositionLocation;
		GLuint instanceHeadingLocation;
		GLuint instanceUpLocation;
		GLuint instanceColorLocation;
		GLuint instanceScaleLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint projectionTypeLocation;

		bool isDisposed;

	public:
		explicit IndicatorSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  model_matrix;
			        uniform mat4  view_matrix;
			        uniform mat4  projection_matrix;
			        in      vec3  element_position;
			        in      vec3  instance_position;
			        in      vec3  instance_heading;
			        in      vec3  instance_up;
			        in      vec4  instance_color;
			        in      float instance_scale;
			        out     vec4  fragment_color_in;
			        void main(){
			            fragment_color_in = instance_color;
			            vec3 I = normalize(instance_heading);
			            vec3 J = normalize(cross(instance_up, instance_heading));
			            vec3 K = cross(I,J);
			            mat3 instance_matrix = instance_scale * transpose(mat3(I,J,K));
			        	vec3 instance_element_position = instance_matrix * element_position + instance_position;
			        	// NOTE: for a heads up display, set all `*_matrix` parameters to identity
			            gl_Position = projection_matrix * view_matrix * model_matrix * vec4(instance_element_position,1);
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

	        // ATTRIBUTES

			// generate new vertex array object, VAO
		    glGenVertexArrays(1, &vertexArrayId);
		    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		    glBindVertexArray(vertexArrayId);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &elementPositionBufferId);
			elementPositionLocation = glGetAttribLocation(shaderProgramId, "element_position");
		    glEnableVertexAttribArray(elementPositionLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instancePositionBufferId);
			instancePositionLocation = glGetAttribLocation(shaderProgramId, "instance_position");
		    glEnableVertexAttribArray(instancePositionLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceHeadingBufferId);
			instanceHeadingLocation = glGetAttribLocation(shaderProgramId, "instance_heading");
		    glEnableVertexAttribArray(instanceHeadingLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceUpBufferId);
			instanceUpLocation = glGetAttribLocation(shaderProgramId, "instance_up");
		    glEnableVertexAttribArray(instanceUpLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceColorBufferId);
			instanceColorLocation = glGetAttribLocation(shaderProgramId, "instance_color");
		    glEnableVertexAttribArray(instanceColorLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceScaleBufferId);
			instanceScaleLocation = glGetAttribLocation(shaderProgramId, "instance_scale");
		    glEnableVertexAttribArray(instanceScaleLocation);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &instanceColorBufferId);
		        glDeleteBuffers(1, &elementPositionBufferId);
		        glDeleteBuffers(1, &instancePositionBufferId);
		        glDeleteBuffers(1, &instanceHeadingBufferId);
		        glDeleteBuffers(1, &instanceUpBufferId);
		        glDeleteProgram(shaderProgramId);
        	}
		}

	    /*
	    */
		bool canDepict(const ViewState& view_state){
			return !isDisposed;
		}

		/*
		*/
		template <typename T>
		void draw(
			const std::vector<T>& element_position, 
			const std::vector<T>& instance_position, 
			const std::vector<T>& instance_heading, 
			const std::vector<T>& instance_up, 
			const std::vector<T>& instance_scale, 
			const std::vector<T>& instance_color, 
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

			glUseProgram(shaderProgramId);
			glBindVertexArray(vertexArrayId);
			glEnable(GL_CULL_FACE);  
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW); 

			//ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, elementPositionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*element_position.size(), &element_position.front(), GL_STATIC_DRAW);
		    glEnableVertexAttribArray(elementPositionLocation);
            glVertexAttribPointer(elementPositionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instancePositionBufferId,0);

			glBindBuffer(GL_ARRAY_BUFFER, instancePositionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_position.size(), &instance_position.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instancePositionLocation);
            glVertexAttribPointer(instancePositionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instancePositionLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceHeadingBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_heading.size(), &instance_heading.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceHeadingLocation);
            glVertexAttribPointer(instanceHeadingLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceHeadingLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceUpBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_up.size(), &instance_up.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceUpLocation);
            glVertexAttribPointer(instanceUpLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceUpLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceScaleBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_color.size(), &instance_color.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceScaleLocation);
            glVertexAttribPointer(instanceScaleLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceScaleLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceColorBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_color.size(), &instance_color.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceColorLocation);
            glVertexAttribPointer(instanceColorLocation, 4, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, instanceScaleBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*instance_scale.size(), &instance_scale.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceScaleLocation);
            glVertexAttribPointer(instanceScaleLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceScaleLocation,1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));

			glDrawArraysInstanced(gl_mode, /*array offset*/ 0, /*vertex count*/ element_position.size()/3, instance_position.size()/3);
		}
	};
}

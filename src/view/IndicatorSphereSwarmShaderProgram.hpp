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
	`IndicatorSphereSwarmShaderProgram` renders a swarm of spheres that represent abstract concepts.
	*/

	class IndicatorSphereSwarmShaderProgram
	{
	    // properties shared by all attributes within the program
	    static constexpr GLenum normalize = GL_FALSE; // don't normalize the data
	    static constexpr GLsizei stride = 0;          // 0: advance size*sizeof(type) each iteration to get the next position
	    static constexpr void* offset = NULL;         // NULL: start at the beginning of the buffer

		std::string vertexShaderGlsl;
		std::string fragmentShaderGlsl;

		std::vector<glm::vec3> elementPositions;

		GLuint shaderProgramId;
		GLuint vertexArrayId;

		// BUFFER IDS

		// element buffer ids
		GLuint elementPositionBufferId;

		// instance buffer ids
		GLuint instanceOriginBufferId;
		GLuint instanceColorBufferId;

		// LOCATIONS

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceOriginLocation;
		GLuint instanceColorLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint resolutionLocation;
		GLuint lightDirectionLocation;
		GLuint ambientLightLocation;
		GLuint pixelRadiusLocation;

		bool isDisposed;

	public:
		explicit IndicatorSphereSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        uniform mat4  view_for_clip;
			        uniform vec2  resolution;
			        uniform float pixel_radius;
			        in      vec3  element_position;
			        in      vec3  instance_origin;
			        in      vec4  instance_color;
			        out     vec3  fragment_element_position;
			        out     vec4  fragment_color_in;
			        void main(){
			        	/*
			        	spheres are billboards, which must always face the camera:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			        	for scaling data: the element→clip map is a scaling operation
			        	*/
			            fragment_color_in = instance_color;
			        	vec4 global_origin = global_for_local * vec4(instance_origin,1);
			        	vec4 view_origin = view_for_global * global_origin;
			        	vec4 clip_origin = clip_for_view * view_origin;
			        	vec4 clip_position = clip_origin + 
			        		vec4(pixel_radius/resolution.y * 
			        			 element_position * 
			        			 clip_origin.w / vec3(resolution.x/resolution.y,1,1), 
			        			0.0);
			        	fragment_element_position = element_position;
			            gl_Position = clip_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform vec2  resolution;
			        uniform vec3  light_direction;
			        uniform vec3  ambient_light;
			        in      vec4  fragment_color_in;
			        in      vec3  fragment_element_position;
			        out     vec4  fragment_color;

			        void main() {
			        	float z2 = 1-dot(fragment_element_position.xy, fragment_element_position.xy);
			        	if(z2<0.0) { discard; }
			        	vec3 N = vec3(fragment_element_position.xy,sqrt(z2));
			        	vec3 V = vec3(0,0,1);
			        	vec3 L = normalize(light_direction);
			        	vec3 fraction_received = (vec3(dot(N,L)) + ambient_light) / (1.0 + ambient_light);
			            fragment_color = vec4(fraction_received,1) * fragment_color_in;
			        }
				)"
			),
			isDisposed(false)
		{
			elementPositions = {
				glm::vec3(-1,-1,0),
				glm::vec3( 1,-1,0),
				glm::vec3( 1, 1,0),
				glm::vec3( 1, 1,0),
				glm::vec3(-1, 1,0),
				glm::vec3(-1,-1,0)
			};

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
		    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		    if (!success)
		    {
		        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
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

			viewMatrixLocation = glGetUniformLocation(shaderProgramId, "view_for_global");
			modelMatrixLocation = glGetUniformLocation(shaderProgramId, "global_for_local");
			projectionMatrixLocation = glGetUniformLocation(shaderProgramId, "clip_for_view");
			resolutionLocation = glGetUniformLocation(shaderProgramId, "resolution");
			lightDirectionLocation = glGetUniformLocation(shaderProgramId, "light_direction");
			ambientLightLocation = glGetUniformLocation(shaderProgramId, "ambient_light");
			pixelRadiusLocation = glGetUniformLocation(shaderProgramId, "pixel_radius");

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
			glGenBuffers(1, &instanceOriginBufferId);
			instanceOriginLocation = glGetAttribLocation(shaderProgramId, "instance_origin");
		    glEnableVertexAttribArray(instanceOriginLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceColorBufferId);
			instanceColorLocation = glGetAttribLocation(shaderProgramId, "instance_color");
		    glEnableVertexAttribArray(instanceColorLocation);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &elementPositionBufferId);
		        glDeleteBuffers(1, &instanceColorBufferId);
		        glDeleteBuffers(1, &instanceOriginBufferId);
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
		void draw(
			const std::vector<glm::vec3>& instance_origin,
			const std::vector<glm::vec4>& instance_color, 
			const float& pixel_radius,
			const glm::vec3 light_direction,
			const glm::vec3 ambient_light,
			const glm::mat4 model_matrix,
			const ViewState& view_state
		){

			if (!canDepict(view_state))
			{
				return;
			}

			if (view_state.render_pass != RenderPassType::overlays)
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
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elementPositions.size(), &elementPositions.front(), GL_STATIC_DRAW);
		    glEnableVertexAttribArray(elementPositionLocation);
            glVertexAttribPointer(elementPositionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceOriginBufferId,0);

			glBindBuffer(GL_ARRAY_BUFFER, instanceOriginBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*instance_origin.size(), &instance_origin.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceOriginLocation);
            glVertexAttribPointer(instanceOriginLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceOriginLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceColorBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*instance_color.size(), &instance_color.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceColorLocation);
            glVertexAttribPointer(instanceColorLocation, 4, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceColorLocation,1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform2fv      (resolutionLocation,       1, glm::value_ptr(view_state.resolution));
	        glUniform3fv      (lightDirectionLocation,   1, glm::value_ptr(light_direction));
	        glUniform3fv      (ambientLightLocation,     1, glm::value_ptr(ambient_light));
	        glUniform1f       (pixelRadiusLocation, pixel_radius);

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), instance_origin.size());
		}
	};
}

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
	`RealisticSphereSwarmShaderProgram` renders a swarm of physically realistic spheres.
	The spheres are held together by gravity.
	Phases of matter within the spheres are sorted by density in the usual order (solid>liquid>gas>plasma).
	Each phase has uniform material properties aside from temperature and density in gases and plasmas. 
	In gases and plasmas, density drops exponentially with radius,
	and temperature varies with radius in the same manner seen in main sequence stars
	(for planets with atmospheres, this assumption is inconsequential).
	The spheres are illuminated by no more than one point light source, 
	or one point light source overwhelms all others,
	and the spheres do not create light in any way aside from thermal emission.
	No further assumptions are made about the spheres. 

	The assumptions above are designed so that `RealisticSphereSwarmShaderProgram` 
	should be able to approximate virtually all heavenly spherical bodies when viewed from a distance.
	`RealisticSphereSwarmShaderProgram` can be combined with other shaders 
	to capture notable nonspherical effects such as black hole discs and ringed bodies.
	*/

	class RealisticSphereSwarmShaderProgram
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

		// element buffer ids
		GLuint elementPositionBufferId;

		// instance buffer ids
		GLuint instanceOriginBufferId;
		GLuint instanceRadiusBufferId;

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceOriginLocation;
		GLuint instanceRadiusLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint resolutionLocation;

		bool isDisposed;

	public:
		explicit RealisticSphereSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        uniform mat4  view_for_clip;
			        in      vec3  element_position;
			        in      vec3  instance_origin;
			        in      float instance_radius;
			        out     mat4  element_for_clip;
			        out     vec3  fragment_element_position;
			        void main(){
			        	/*
			        	spheres are billboards, which must always face the camera:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			        	global→view is assumed not to represent scaling, so
			            for scaling data:  local→view = local→global→view = local→global
			        	*/
			            mat4 scale_map = mat4(instance_radius);
			        	mat4 instance_for_element = mat4(scale_map[0], scale_map[1], scale_map[2], vec4(instance_origin,1));
			        	mat4 global_for_element = global_for_local * instance_for_element;
			        	mat4 position_map = view_for_global * global_for_element;
			        	mat4 view_for_element = mat4(global_for_element[0], global_for_element[1], global_for_element[2], position_map[3]);
			        	mat4 clip_for_element = clip_for_view * view_for_element;
			        	vec4 clip_position = clip_for_element * vec4(element_position,1);
			        	element_for_clip = inverse(clip_for_element);
			        	fragment_element_position = element_position;
			            gl_Position = clip_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform vec2  resolution;
			        in      mat4  element_for_clip;
			        in      vec3  fragment_element_position;
			        out     vec4  fragment_color;

			        void main() {
			        	/*
			        	We tried implementing this to consider projection 
			        	by setting element_position = element_for_clip * clip_position,
			        	where clip_position = 2 * gl_FragCoord/resolution - 1, 
			        	however element_for_clip assumes a single value for all fragments.
			        	Fortunately, this shader program is intended for rendering distant objects,
			        	so projection can be assumed to be roughly orthographic.
			        	Element vertices are hard coded so that x and y element coordinates
			        	approximate those of a unit sphere.
			        	*/
			        	float z = 1-dot(fragment_element_position.xy, fragment_element_position.xy);
			        	if(z<0.0) { discard; }
			            fragment_color = vec4(fragment_element_position.xy,z,1);
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

			viewMatrixLocation = glGetUniformLocation(shaderProgramId, "view_for_global");
			modelMatrixLocation = glGetUniformLocation(shaderProgramId, "global_for_local");
			projectionMatrixLocation = glGetUniformLocation(shaderProgramId, "clip_for_view");
			resolutionLocation = glGetUniformLocation(shaderProgramId, "resolution");

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
			glGenBuffers(1, &instanceRadiusBufferId);
			instanceRadiusLocation = glGetAttribLocation(shaderProgramId, "instance_radius");
		    glEnableVertexAttribArray(instanceRadiusLocation);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &elementPositionBufferId);
		        glDeleteBuffers(1, &instanceOriginBufferId);
		        glDeleteBuffers(1, &instanceRadiusBufferId);
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
			const std::vector<float>& instance_radius,
			const ViewState& view_state
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
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elementPositions.size(), &elementPositions.front(), GL_STATIC_DRAW);
		    glEnableVertexAttribArray(elementPositionLocation);
            glVertexAttribPointer(elementPositionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceOriginBufferId,0);

			glBindBuffer(GL_ARRAY_BUFFER, instanceOriginBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*instance_origin.size(), &instance_origin.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceOriginLocation);
            glVertexAttribPointer(instanceOriginLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceOriginLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceRadiusBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*instance_radius.size(), &instance_radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceRadiusLocation);
            glVertexAttribPointer(instanceRadiusLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceRadiusLocation,1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform2fv      (resolutionLocation,       1, glm::value_ptr(view_state.resolution));

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), instance_origin.size());
		}
	};
}

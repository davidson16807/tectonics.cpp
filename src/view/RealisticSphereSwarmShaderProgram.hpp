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
	The surface of spheres are approximated as a diffusely reflective material
	that has uniform material properties and emission.
	The spheres are illuminated by no more than one point source of light, 
	or one point light source overwhelms all others.
	The projection that spheres are viewed with is suitably close to orthographic.
	No further assumptions are made about the spheres. 

	The assumptions above are designed so that `RealisticSphereSwarmShaderProgram` 
	should be able to approximate virtually all heavenly spherical bodies when viewed from a distance.
	`RealisticSphereSwarmShaderProgram` is also intended to allow combination with other shaders 
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
		GLuint instanceLightSourceBufferId;
		GLuint instanceLightLuminosityBufferId;
		GLuint instanceSurfaceEmissionBufferId;

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceOriginLocation;
		GLuint instanceRadiusLocation;
		GLuint instanceLightSourceLocation;
		GLuint instanceLightLuminosityLocation;
		GLuint instanceSurfaceEmissionLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint exposureIntensityLocation;
		GLuint gammaLocation;

		bool isDisposed;

	public:
		explicit RealisticSphereSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        in      vec3  element_position;
			        in      vec3  instance_origin;
			        in      float instance_radius;
			        in      vec3  instance_surface_emission;
			        in      vec3  instance_illumination_source;
			        in      vec3  instance_illumination_luminosity;
			        out     vec3  fragment_element_position;
			        out     vec3  fragment_light_direction;
			        out     vec3  fragment_light_intensity;
			        out     vec3  fragment_surface_emission;

			        const   float pi = 3.141592653589793238462643383279;

			        void main(){
			        	/*
			        	spheres are billboards, which must always face the camera:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			        	global→view is assumed not to represent scaling, so
			            for scaling data:  local→view = local→global→view = local→global
			        	*/
			            mat4 scale_map = mat4(instance_radius);
			        	vec4 view_for_element_origin = view_for_global * global_for_local * vec4(instance_origin,1);
			        	mat4 view_for_element = mat4(scale_map[0], scale_map[1], scale_map[2], view_for_element_origin);
			        	vec4 clip_position = clip_for_view * view_for_element * vec4(element_position,1);
			            vec3 instance_illumination_offset = instance_illumination_source-instance_origin;
			        	float v = length(view_for_element_origin);
			        	float l = length(instance_illumination_offset);
			        	fragment_light_intensity = max(vec3(0),instance_illumination_luminosity) / (4.0*pi*l*l) / (4.0*pi*v*v);
			        	fragment_surface_emission = instance_surface_emission / (4.0*pi*v*v);
			        	fragment_light_direction = (
			        		clip_for_view * 
			        		view_for_global * 
			        		global_for_local * 
			        		vec4(instance_illumination_offset/l,0)
			        	).xyz;
			        	fragment_element_position = element_position;
			            gl_Position = clip_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform float exposure_intensity;
			        uniform float gamma;
			        in      vec3  fragment_element_position;
			        in      vec3  fragment_light_direction;
			        in      vec3  fragment_light_intensity;
			        in      vec3  fragment_surface_emission;
			        out     vec4  fragment_color;

					vec3 get_signal3_for_intensity3(
					    in vec3 intensity, in float gamma
					){
					    return vec3(
					        pow(intensity.x, 1./gamma),
					        pow(intensity.y, 1./gamma),
					        pow(intensity.z, 1./gamma)
					    );
					}

					vec3 get_ldrtone3_for_intensity3(
						in vec3 intensity, in float exposure_intensity
					){
						return 1.0 - exp(-intensity/exposure_intensity);
					}

			        void main() {
			        	/*
			        	We tried implementing this to consider projection 
			        	by setting element_position = element_for_clip * clip_position,
			        	where clip_position = 2 * gl_FragCoord/resolution - 1, 
			        	and element_for_clip is passed in from the vertex shader,
			        	however element_for_clip assumes a single value for all fragments.
			        	Fortunately, this shader program is motivated by the need to render distant objects,
			        	so projection can be assumed to be roughly orthographic,
			        	such that vec3(0,0,1) is the view direction.
			        	Element vertices are hard coded so that x and y element coordinates
			        	approximate those of a unit sphere.
			        	*/
			        	float z = 1-dot(fragment_element_position.xy, fragment_element_position.xy);
			        	if(z<0.0) { discard; }
			        	vec3 Nhat = vec3(fragment_element_position.xy,-z);
			        	vec3 Lhat = fragment_light_direction;
			        	vec3 fraction = vec3(dot(Nhat,Lhat));
			        	vec3 intensity = fraction * fragment_light_intensity + fragment_surface_emission;
			            fragment_color = vec4(
					    	get_signal3_for_intensity3(
					    		get_ldrtone3_for_intensity3(
					    			intensity, 
					    			exposure_intensity
					    		), 
						    	gamma
					    	),
			            1);
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
			exposureIntensityLocation = glGetUniformLocation(shaderProgramId, "exposure_intensity");
			gammaLocation = glGetUniformLocation(shaderProgramId, "gamma");

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

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceLightSourceBufferId);
			instanceLightSourceLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_source");
		    glEnableVertexAttribArray(instanceLightSourceLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceLightLuminosityBufferId);
			instanceLightLuminosityLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_luminosity");
		    glEnableVertexAttribArray(instanceLightLuminosityLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceSurfaceEmissionBufferId);
			instanceSurfaceEmissionLocation = glGetAttribLocation(shaderProgramId, "instance_surface_emission");
		    glEnableVertexAttribArray(instanceSurfaceEmissionLocation);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &elementPositionBufferId);
		        glDeleteBuffers(1, &instanceOriginBufferId);
		        glDeleteBuffers(1, &instanceRadiusBufferId);
		        glDeleteBuffers(1, &instanceLightSourceBufferId);
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
			const std::vector<glm::vec3>& origin,
			const std::vector<float>& radius,
			const std::vector<glm::vec3>& light_source,
			const std::vector<glm::vec3>& light_luminosity,
			const std::vector<glm::vec3>& surface_emission,
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
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*origin.size(), &origin.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceOriginLocation);
            glVertexAttribPointer(instanceOriginLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceOriginLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceRadiusBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*radius.size(), &radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceRadiusLocation);
            glVertexAttribPointer(instanceRadiusLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceRadiusLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceLightSourceBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*light_source.size(), &light_source.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceLightSourceLocation);
            glVertexAttribPointer(instanceLightSourceLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceLightSourceLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceLightLuminosityBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*light_luminosity.size(), &light_luminosity.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceLightLuminosityLocation);
            glVertexAttribPointer(instanceLightLuminosityLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceLightLuminosityLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceSurfaceEmissionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surface_emission.size(), &surface_emission.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceSurfaceEmissionLocation);
            glVertexAttribPointer(instanceSurfaceEmissionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceSurfaceEmissionLocation,1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform1f       (exposureIntensityLocation, view_state.exposure_intensity);
	        glUniform1f       (gammaLocation, view_state.gamma);

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), origin.size());
		}
	};
}

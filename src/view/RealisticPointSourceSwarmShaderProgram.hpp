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
	`RealisticPointSourceSwarmShaderProgram` renders a swarm of physically realistic point sources.
	The point sources are approximated as very small or distant spheres
	that are composed of a diffusely reflective material wtih uniform material properties and emissions.
	The spheres are illuminated by no more than one point source of light, 
	or one point light source overwhelms all others,
	No further assumptions are made about the spheres. 

	The assumptions above are designed so that `RealisticPointSourceSwarmShaderProgram` 
	should be able to approximate virtually all naturally forming heavenly point sources of light.
	*/

	class RealisticPointSourceSwarmShaderProgram
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
		GLuint psfPixelStandardDeviationLocation;
		GLuint psfStarburstPixelAmplitudeLocation;
		GLuint psfStarburstRayCountLocation;
		GLuint signalCutoffLocation;
		GLuint exposureIntensityLocation;
		GLuint gammaLocation;
		GLuint resolutionLocation;

		bool isDisposed;

	public:
		explicit RealisticPointSourceSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        uniform mat4  view_for_clip;
			        uniform vec2  resolution;
			        uniform float psf_pixel_standard_deviation;
			        uniform float psf_starburst_pixel_amplitude;
			        uniform float exposure_intensity;
			        uniform float gamma;
			        uniform float signal_cutoff;
			        in      vec3  element_position;
			        in      vec3  instance_origin;
			        in      float instance_radius;
			        in      vec3  instance_light_source;
			        in      vec3  instance_light_luminosity;
			        in      vec3  instance_surface_emission;
			        out     mat4  element_for_clip;
			        out     vec3  fragment_element_position;
			        out     vec3  fragment_point_intensity;
			        out     float fragment_clipspace_radius;

			        const   float pi = 3.141592653589793238462643383279;

			        float max3 (vec3 v) {
					  return max (max (v.x, v.y), v.z);
					}

			        /*
			        `approx_fraction_of_diffusely_reflected_light_of_sphere` approximates
			        the fraction of all diffusely reflected light on a sphere that is reflected off
			        towards a viewer according to a reflection_angle in radians.
			        A reflection_angle of 0 indicates the viewer sees the sphere fully lit,
			        and a reflection_angle of π indicates the viewer sees only the dark side of the sphere.
			        Light is diffusely reflected in the sense that it obeys Lambertian reflectance.
			        Error is guaranteed not to exceed 0.001 throughout the valid domain ([0,π]).
			        See approx_fraction_of_diffusely_reflected_light_of_sphere.py for design work.
			        */
			        float approx_fraction_of_diffusely_reflected_light_of_sphere (
			        	in float reflection_angle
			        ) {
			        	const float fudge = 0.149;
						return (
							cos(
								reflection_angle + 
								fudge*(1.0-cos(2.0*reflection_angle))*0.5
							) + 1.0
						)*0.25;
					}

					float get_intensity_for_signal(
					    in float intensity, in float gamma
					){
					    return pow(intensity, gamma);
					}

					float get_intensity_for_ldrtone(
						in float ldr_tone, in float exposure_intensity
					){
						return -exposure_intensity * log(1.0 - ldr_tone);
					}

			        void main(){
			        	/*
			        	point sources are billboards that must always face the camera and consume the same screen space:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			            for scaling data:  the local→view map is a scaling transform
			        	*/

			        	// L: direction from point source to light, in clip space
			        	vec4 view_for_element_origin = view_for_global * global_for_local * vec4(instance_origin,1);
			            vec3 instance_light_offset = instance_light_source-instance_origin;
			        	float l = length(instance_light_offset);
			        	float v = length(view_for_element_origin);
			        	vec3 Lhat = (
			        		clip_for_view * 
			        		view_for_global * 
			        		global_for_local * 
			        		vec4(instance_light_offset/l,0)
			        	).xyz;  
			        	// V: direction from point source to viewer, in clip space
			        	vec3 V = vec3(0,0,-1); 
			        	float reflection_angle = acos(dot(V,Lhat));
			        	float fraction = approx_fraction_of_diffusely_reflected_light_of_sphere(reflection_angle);
			        	fragment_point_intensity = (
			        		fraction * instance_light_luminosity / (4.0*pi*l*l)
			        		+ instance_surface_emission
			        	) / (4.0*pi*v*v); // TODO: remove assumption of unit irradiance illumination

			        	// solve for r at which signal==signal_cutoff to find fragment_clipspace_radius
			        	float intensity_cutoff = get_intensity_for_ldrtone(get_intensity_for_signal(signal_cutoff, gamma), exposure_intensity);
			        	float sigma = (psf_pixel_standard_deviation + psf_starburst_pixel_amplitude * exp(1.0)) / resolution.x;
			        	float standard_deviation_cutoff2 = -log(sqrt(2.0*pi) * intensity_cutoff / max3(fragment_point_intensity)) * (2.0*sigma*sigma);
			        	fragment_clipspace_radius = standard_deviation_cutoff2;

			            vec2 scale = sqrt(standard_deviation_cutoff2) * vec2(1, resolution.x / resolution.y);
			        	vec4 clip_for_element_origin = clip_for_view * view_for_global * global_for_local * vec4(instance_origin,1);
			        	vec4 clip_position = vec4(clip_for_element_origin.xy+element_position.xy*scale*clip_for_element_origin.z, 0, clip_for_element_origin.z);
			        	fragment_element_position = element_position;

			            gl_Position = clip_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform vec2  resolution;
			        uniform float psf_pixel_standard_deviation;
			        uniform float psf_starburst_pixel_amplitude;
			        uniform float psf_starburst_ray_count;
			        uniform float signal_cutoff;
			        uniform float exposure_intensity;
			        uniform float gamma;
			        in      vec3  fragment_element_position;
			        in      vec3  fragment_point_intensity;
			        in      float fragment_clipspace_radius;
			        out     vec4  fragment_color;

			        const   float pi = 3.141592653589793238462643383279;

			        float max3 (vec3 v) {
					  return max (max (v.x, v.y), v.z);
					}

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
			        	float r2 = dot(fragment_element_position.xy, fragment_element_position.xy) * fragment_clipspace_radius;
			        	float angle = atan(fragment_element_position.y/fragment_element_position.x);
			        	float sigma = (psf_pixel_standard_deviation + psf_starburst_pixel_amplitude*exp(cos(angle*psf_starburst_ray_count))) / resolution.x;
			        	vec3 intensity = vec3(fragment_point_intensity)*exp(-r2/(2.0*sigma*sigma))/sqrt(2.0*pi);
					    vec3 signal = 
					    	get_signal3_for_intensity3(
					    		get_ldrtone3_for_intensity3(
					    			intensity, 
					    			exposure_intensity
					    		), 
						    	gamma
					    	);
					    if(max3(signal) < signal_cutoff) {discard;};
					    fragment_color = vec4(signal, 1);
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
			psfPixelStandardDeviationLocation = glGetUniformLocation(shaderProgramId, "psf_pixel_standard_deviation");
			psfStarburstPixelAmplitudeLocation = glGetUniformLocation(shaderProgramId, "psf_starburst_pixel_amplitude");
			psfStarburstRayCountLocation = glGetUniformLocation(shaderProgramId, "psf_starburst_ray_count");
			signalCutoffLocation = glGetUniformLocation(shaderProgramId, "signal_cutoff");
			exposureIntensityLocation = glGetUniformLocation(shaderProgramId, "exposure_intensity");
			gammaLocation = glGetUniformLocation(shaderProgramId, "gamma");
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

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceLightSourceBufferId);
			instanceLightSourceLocation = glGetAttribLocation(shaderProgramId, "instance_light_source");
		    glEnableVertexAttribArray(instanceLightSourceLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceLightLuminosityBufferId);
			instanceLightLuminosityLocation = glGetAttribLocation(shaderProgramId, "instance_light_luminosity");
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
		        glDeleteBuffers(1, &instanceLightLuminosityBufferId);
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
			glBlendEquationSeparate(GL_MAX, GL_MAX);
			glEnable(GL_BLEND);

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
	        glUniform2fv      (resolutionLocation, 1, glm::value_ptr(view_state.resolution));
	        glUniform1f       (psfPixelStandardDeviationLocation, view_state.point_spread_function_pixel_standard_deviation);
	        glUniform1f       (psfStarburstPixelAmplitudeLocation, view_state.point_spread_function_starburst_pixel_amplitude);
	        glUniform1f       (psfStarburstRayCountLocation, (view_state.aperture_symmetry_axis_count%2?2:1) * view_state.aperture_symmetry_axis_count);
	        glUniform1f       (signalCutoffLocation, 0.02f);
	        glUniform1f       (exposureIntensityLocation, view_state.exposure_intensity);
	        glUniform1f       (gammaLocation, view_state.gamma);

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), origin.size());
		}
	};
}

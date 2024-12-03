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
		GLuint instanceIlluminationSourceBufferId;
		GLuint instanceIlluminationLuminosityBufferId;
		GLuint instanceBetaRayBufferId;
		GLuint instanceBetaMieBufferId;
		GLuint instanceBetaAbsBufferId;
		GLuint instanceSurfaceTemperatureBufferId;
		GLuint instanceTemperatureChangePerRadius2BufferId;
		GLuint instanceAtmosphereScaleHeightBufferId;

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceOriginLocation;
		GLuint instanceRadiusLocation;
		GLuint instanceIlluminationSourceLocation;
		GLuint instanceIlluminationLuminosityLocation;
		GLuint instanceBetaRayLocation;
		GLuint instanceBetaMieLocation;
		GLuint instanceBetaAbsLocation;
		GLuint instanceSurfaceTemperatureLocation;
		GLuint instanceTemperatureChangePerRadius2Location;
		GLuint instanceAtmosphereScaleHeightLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint exposureIntensityLocation;
		GLuint gammaLocation;
		GLuint wavelengthLocation;

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
			        in      vec3  instance_illumination_source;
			        in      vec3  instance_illumination_luminosity;

					in      vec3  instance_beta_abs;
					in      vec3  instance_beta_mie;
					in      vec3  instance_beta_ray;
			        in      float instance_radius;
					in      float instance_surface_temperature;
					in      float instance_temperature_change_per_radius2;
					in      float instance_atmosphere_scale_height;

			        out     vec3  fragment_element_position;
			        out     vec3  fragment_illumination_direction;
			        out     vec3  fragment_illumination_intensity;

					out     vec3  fragment_beta_abs;
					out     vec3  fragment_beta_mie;
					out     vec3  fragment_beta_ray;
					out     float fragment_radius;
					out     float fragment_surface_temperature;
					out     float fragment_temperature_change_per_radius2;
					out     float fragment_atmosphere_scale_height;

			        const   float PI = 3.141592653589793238462643383279;

					// from Carl Hansen et al., "Stellar Interiors"
					float get_fraction_of_radius_for_star_with_temperature(float temperature, float core_temperature)
					{
					    return sqrt(max(1.0 - (temperature / core_temperature), 0.0));
					}

			        void main(){
			        	/*
			        	spheres are billboards, which must always face the camera:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			        	global→view is assumed not to represent scaling, so
			            for scaling data:  local→view = local→global→view = local→global
			        	*/
					    float radius_temp0 = 
							max(
								instance_radius,
								instance_radius / 
								get_fraction_of_radius_for_star_with_temperature(
									instance_surface_temperature, 
									instance_temperature_change_per_radius2)
							); 
			            mat4 scale_map = mat4(radius_temp0);
			        	vec4 view_for_element_origin = view_for_global * global_for_local * vec4(instance_origin,1);
			        	mat4 view_for_element = mat4(scale_map[0], scale_map[1], scale_map[2], view_for_element_origin);
			        	vec4 clip_position = clip_for_view * view_for_element * vec4(element_position,1);
			            vec3 instance_illumination_offset = instance_illumination_source-instance_origin;
			        	float v = length(view_for_element_origin);
			        	float l = length(instance_illumination_offset);
			        	fragment_illumination_intensity = max(vec3(0),instance_illumination_luminosity) 
			        		/ (4.0*PI*l*l)
			        		// / (4.0*PI*v*v)
			        	;
			        	fragment_illumination_direction = (
			        		clip_for_view * 
			        		view_for_global * 
			        		global_for_local * 
			        		vec4(instance_illumination_offset/l,0)
			        	).xyz;
			        	fragment_element_position = element_position;
						fragment_beta_abs = instance_beta_abs;
						fragment_beta_mie = instance_beta_mie;
						fragment_beta_ray = instance_beta_ray;
						fragment_radius = instance_radius;
						fragment_surface_temperature = instance_surface_temperature;
						fragment_temperature_change_per_radius2 = instance_temperature_change_per_radius2;
						fragment_atmosphere_scale_height = instance_atmosphere_scale_height;
			            gl_Position = clip_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform float exposure_intensity;
			        uniform float gamma;
			        uniform vec3  wavelength3;
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        in      vec3  instance_origin;
			        in      vec3  fragment_element_position;
			        in      vec3  fragment_illumination_direction;
			        in      vec3  fragment_illumination_intensity;

					in      vec3  fragment_beta_abs;
					in      vec3  fragment_beta_mie;
					in      vec3  fragment_beta_ray;
					in      float fragment_radius;
					in      float fragment_surface_temperature;
					in      float fragment_temperature_change_per_radius2;
					in      float fragment_atmosphere_scale_height;

			        out     vec4  fragment_color;

			        const float PI = 3.141592653589793238462643383279;
					const float BIG = 1e20;
					const float SMALL = 1e-20;

					const float METER = 1.0;
					const float SECOND = 1.0;
					const float JOULE = 1.0;
					const float KELVIN = 1.0;

					const float SPEED_OF_LIGHT = 299792458. * METER / SECOND;
					const float BOLTZMANN_CONSTANT = 1.3806485279e-23 * JOULE / KELVIN;
					const float PLANCK_CONSTANT = 6.62607004e-34 * JOULE * SECOND;

					struct maybe_vec2
					{
					    vec2 value;
					    bool exists;
					};

					maybe_vec2 get_bounding_distances_along_ray(in maybe_vec2 distances_along_line){
					    return 
					      maybe_vec2(
					        vec2(
					          max(min(distances_along_line.value.x, distances_along_line.value.y), 0.0),
					          max(distances_along_line.value.x, distances_along_line.value.y)
					        ),
					        distances_along_line.exists && max(distances_along_line.value.x, distances_along_line.value.y) > 0.
					      );
					}

					maybe_vec2 get_distances_along_line_to_negation(
					    in maybe_vec2 positive,
					    in maybe_vec2 negative
					) {
					    // as long as intersection with positive exists, 
					    // and negative doesn't completely surround it, there will be an intersection
					    bool exists =
					        positive.exists && !(negative.value.x < positive.value.x && positive.value.y < negative.value.y);
					    // find the first region of intersection
					    float entrance = !negative.exists ? positive.value.x : min(negative.value.y, positive.value.x);
					    float exit = !negative.exists ? positive.value.y : min(negative.value.x, positive.value.y);
					    // if the first region is behind us, find the second region
					    if (exit < 0. && 0. < positive.value.y)
					    {
					        entrance = negative.value.y;
					        exit = positive.value.y;
					    }
					    return maybe_vec2( vec2(entrance, exit), exists );
					}

					/*
					A0 line reference
					A  line direction, normalized
					B0 sphere origin
					R  sphere radius
					*/
					maybe_vec2 get_distances_along_3d_line_to_sphere(
					    in vec3 A0,
					    in vec3 A,
					    in vec3 B0,
					    in float r
					){
					    float t = dot(B0 - A0, A);
					    vec3  At = A0 + A*t - B0;
					    float y2 = r*r - dot(At,At);
					    float dxr = sqrt(max(y2, SMALL));
					    return maybe_vec2(
					        vec2(t - dxr, t + dxr),
					        y2 > 0.
					    );
					}

					// from Carl Hansen et al., "Stellar Interiors"
					float get_fraction_of_radius_for_star_with_temperature(float temperature, float core_temperature)
					{
					    return sqrt(max(1.0 - (temperature / core_temperature), 0.0));
					}

					float get_temperature_of_star_at_fraction_of_radius(float fraction_of_radius, float core_temperature) 
					{
					    return max( core_temperature * (1.0 - fraction_of_radius*fraction_of_radius), 0.0 );
					}

					/*
					"approx_air_column_density_ratio_through_atmosphere" 
					  calculates the distance you would need to travel 
					  along the surface to encounter the same number of particles in the column. 
					It does this by finding an integral using integration by substitution, 
					  then tweaking that integral to prevent division by 0. 
					All distances are recorded in scale heights.
					"a" and "b" are distances along the ray from closest approach.
					  The ray is fired in the positive direction.
					  If there is no intersection with the planet, 
					  a and b are distances from the closest approach to the upper bound.
					"z2" is the closest distance from the ray to the center of the world, squared.
					"r0" is the radius of the world, in scale heights
					*/
					float approx_air_column_density_ratio_through_atmosphere(
					    in float a,
					    in float b,
					    in float z2,
					    in float r0
					){
						/*
					    GUIDE TO VARIABLE NAMES:
					     "x*" distance along the ray from closest approach
					     "z*" distance from the center of the world at closest approach
					     "r*" distance ("radius") from the center of the world
					     "*0" variable at reference point
					     "*2" the square of a variable
					     "ch" a nudge we give to prevent division by zero, analogous to the Chapman function
						*/
					    const float SQRT_HALF_PI = sqrt(PI/2.);
					    const float k = 0.6; // "k" is an empirically derived constant
					    float x0 = sqrt(max(r0*r0 - z2, SMALL));
					    // if obstructed by the world, approximate answer by using a ludicrously large number
					    if (a < x0 && -x0 < b && z2 < r0*r0) { return BIG; }
					    float abs_a  = abs(a);
					    float abs_b  = abs(b);
					    float z      = sqrt(z2);
					    float sqrt_z = sqrt(z);
					    float ra     = sqrt(a*a+z2);
					    float rb     = sqrt(b*b+z2);
					    float ch0    = (1. - 1./(2.*r0)) * SQRT_HALF_PI * sqrt_z + k*x0;
					    float cha    = (1. - 1./(2.*ra)) * SQRT_HALF_PI * sqrt_z + k*abs_a;
					    float chb    = (1. - 1./(2.*rb)) * SQRT_HALF_PI * sqrt_z + k*abs_b;
					    float s0     = min(exp(r0- z),1.) / (x0/r0 + 1./ch0);
					    float sa     = exp(r0-ra) / max(abs_a/ra + 1./cha, 0.01);
					    float sb     = exp(r0-rb) / max(abs_b/rb + 1./chb, 0.01);
					    return max( sign(b)*(s0-sb) - sign(a)*(s0-sa), 0.0 );
					}

					// TODO: support for light sources from within atmosphere
					vec3 get_intensity3_of_light_emitted_by_atmosphere(
					    float view_closest_distance2, float view_start_length, float view_stop_length, vec3 view_wavelengths,
					    float star_radius_temp0, //radius at which temperature is assumed 0
					    float star_radius_beta, //radius at which beta_* values are sampled
					    float star_core_temperature,
					    vec3 beta_ray, vec3 beta_mie, vec3 beta_abs,
					    float step_count
					){
					    // For an excellent introduction to what we're try to do here, see Alan Zucconi: 
					    //   https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
					    // We will be using most of the same terminology and variable names.
					    // GUIDE TO VARIABLE NAMES:
					    //  Uppercase letters indicate vectors.
					    //  Lowercase letters indicate scalars.
					    //  Going for terseness because I tried longhand names and trust me, you can't read them.
					    //  "*v*"    property of the view ray, the ray cast from the viewer to the object being viewed
					    //  "*l*"    property of the light ray, the ray cast from the object to the light source
					    //  "y*"     distance from the center of the world to the plane shared by view and light ray
					    //  "z*"     distance from the center of the world to along the plane shared by the view and light ray 
					    //  "r*"     a distance ("radius") from the center of the world
					    //  "h*"     the atmospheric scale height, the distance at which air density reduces by a factor of e
					    //  "*2"     the square of a variable
					    //  "*0"     property at the start of the raymarch
					    //  "*1"     property at the end of the raymarch
					    //  "*i"     property during an iteration of the raymarch
					    //  "d*"     the change in a property across iterations of the raymarch
					    //  "beta*"  a scattering coefficient, the number of e-foldings in light intensity per unit distance
					    //  "gamma*" a phase factor, the fraction of light that's scattered in a certain direction
					    //  "sigma*" a column density ratio, the density of a column of air relative to surface density
					    //  "F*"     fraction of source light that reaches the viewer due to scattering for each color channel
					    //  "*_ray"  property of rayleigh scattering
					    //  "*_mie"  property of mie scattering
					    //  "*_abs"  property of absorption
					    // setup variable shorthands
					    // all distances (except wavelength) are in scale heights 
					    // all positions are relative to world origin
					    float v0 = view_start_length;
					    float v1 = view_stop_length;
					    // "beta_*" indicates the rest of the fractional loss.
					    // it is dependant on wavelength, and the density ratio, which is dependant on height
					    // So all together, the fraction of sunlight that scatters to a given angle is: beta(wavelength) * gamma(angle) * density_ratio(height)
					    vec3 beta_sum = beta_ray + beta_mie + beta_abs;
					    // number of iterations within the raymarch
					    float dv = (v1 - v0) / step_count;
					    float vi = v0;
					    float z2 = view_closest_distance2;
					    float sigma; // columnar density encountered along the entire path, relative to surface density, effectively the distance along the surface needed to obtain a similar column density
					    float ri;
					    vec3 F = vec3(0); // total intensity for each color channel, found as the sum of light intensities for each path from the light source to the camera
					    for (float i = 0.; i < step_count; ++i)
					    {
					    	ri = sqrt(vi*vi+z2);
					        sigma = approx_air_column_density_ratio_through_atmosphere(v0, vi, z2, star_radius_beta);
					        float temperature = get_temperature_of_star_at_fraction_of_radius(ri / star_radius_temp0, star_core_temperature);
					        F += (
					            // incoming scattered light: the intensity of light that goes towards the camera
					            // I * exp(r-sqrt(vi*vi+y2+zv2)) * beta_gamma * dv 
					            // newly created emitted light: the intensity of light that is generated by the parcel towards the camera
					            + beta_abs * dv
					              * 2.0 * PLANCK_CONSTANT * SPEED_OF_LIGHT*SPEED_OF_LIGHT / (view_wavelengths*view_wavelengths*view_wavelengths*view_wavelengths*view_wavelengths)
					              * exp(star_radius_beta-ri) / (exp(PLANCK_CONSTANT * SPEED_OF_LIGHT/(view_wavelengths*BOLTZMANN_CONSTANT*temperature)) - 1.0) 
					            )
					            // outgoing scattered light: the fraction of light that scatters away from camera
					            * exp(-beta_sum * sigma);
					        vi += dv;
					    }
					    return F;
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
						return max(vec3(0.0), 1.0 - exp(-intensity/exposure_intensity));
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
    					float r  = fragment_radius; // radius at which "surface" temperature and beta_* variables are sampled
				    	float t  = fragment_surface_temperature;
				    	float dtdr2 = fragment_temperature_change_per_radius2;
    					float h  = fragment_atmosphere_scale_height;
					    // `r1` is the radius at which temperature is modeled as 0
					    float r1 = r / get_fraction_of_radius_for_star_with_temperature(t, dtdr2); 
    					float r0 = r; // innermost radius of the atmosphere march
    					vec2 X = fragment_element_position.xy;
			        	float XX = dot(X, X);
			        	if(XX>=1) { discard; }
			        	vec3 N = vec3(X*r1/r,-sqrt(max(0,1-dot(X*r1/r, X*r1/r))));
    					vec3 V0 = vec3(X,-2)*r1; // origin of view ray, assumes orthographic projection, anywhere outside the sphere is sufficiently distant
			        	vec3 L = fragment_illumination_direction;
			        	float l = length(L);
			        	vec3 E_gas_emitted = vec3(0);
			        	vec3 V  = vec3(0,0,1); 
    					vec3 O  = vec3(0);  // center of the star
					    maybe_vec2 air_along_view_ray = 
					        get_bounding_distances_along_ray
					        (
					            get_distances_along_line_to_negation(
					                get_distances_along_3d_line_to_sphere(V0, V, O, r1),
					                get_distances_along_3d_line_to_sphere(V0, V, O, r0)
					            )
					        );
				        float v0 = air_along_view_ray.value.x;
				        float v1 = air_along_view_ray.value.y;
				        // *B variables indicate coordinates relative to world origin
					    vec3 V0B = (V0 + V * v0 - O);
					    vec3 V1B = (V0 + V * v1 - O);
					    float v0B = dot(V0B,V);
					    float v1B = dot(V1B,V);
					    float z2 = XX*r1*r1;
					    if(air_along_view_ray.exists)
					    {
					        E_gas_emitted =
						        get_intensity3_of_light_emitted_by_atmosphere(
						            z2/h/h, v0B/h, v1B/h, vec3(650e-9*METER, 550e-9*METER, 450e-9*METER), 
						            r1/h, r/h, dtdr2, 
						            h*fragment_beta_ray, 
						            h*fragment_beta_mie, 
						            h*fragment_beta_abs,
						            32
						        );
					    }
			        	float fraction = dot(N,L);
			        	vec3 E_surface_reflected = length(X*r1/r)>1.0? vec3(0) : fraction * fragment_illumination_intensity;
			        	E_surface_reflected = any(isnan(E_surface_reflected))? vec3(0) : E_surface_reflected;
			        	vec4 view_for_element_origin = view_for_global * global_for_local * vec4(instance_origin,1);
			            mat4 scale_map = mat4(fragment_radius);
			        	mat4 view_for_element = mat4(scale_map[0], scale_map[1], scale_map[2], view_for_element_origin);
			        	vec4 clip_position = clip_for_view * view_for_element * vec4(N,1);
			            fragment_color = vec4(
					    	get_signal3_for_intensity3(
					    		get_ldrtone3_for_intensity3(
					    			E_surface_reflected
					    			+ E_gas_emitted
					    			, exposure_intensity
					    		), 
						    	gamma
					    	),
			            clip_position.z);
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
			wavelengthLocation = glGetUniformLocation(shaderProgramId, "wavelength3");

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
			glGenBuffers(1, &instanceIlluminationSourceBufferId);
			instanceIlluminationSourceLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_source");
		    glEnableVertexAttribArray(instanceIlluminationSourceLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceIlluminationLuminosityBufferId);
			instanceIlluminationLuminosityLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_luminosity");
		    glEnableVertexAttribArray(instanceIlluminationLuminosityLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceOriginBufferId);
			instanceOriginLocation = glGetAttribLocation(shaderProgramId, "instance_origin");
		    glEnableVertexAttribArray(instanceOriginLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceRadiusBufferId);
			instanceRadiusLocation = glGetAttribLocation(shaderProgramId, "instance_radius");
		    glEnableVertexAttribArray(instanceRadiusLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceSurfaceTemperatureBufferId);
			instanceSurfaceTemperatureLocation = glGetAttribLocation(shaderProgramId, "instance_surface_temperature");
		    glEnableVertexAttribArray(instanceSurfaceTemperatureLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceTemperatureChangePerRadius2BufferId);
			instanceTemperatureChangePerRadius2Location = glGetAttribLocation(shaderProgramId, "instance_temperature_change_per_radius2");
		    glEnableVertexAttribArray(instanceTemperatureChangePerRadius2Location);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceAtmosphereScaleHeightBufferId);
			instanceAtmosphereScaleHeightLocation = glGetAttribLocation(shaderProgramId, "instance_atmosphere_scale_height");
		    glEnableVertexAttribArray(instanceAtmosphereScaleHeightLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceBetaRayBufferId);
			instanceBetaRayLocation = glGetAttribLocation(shaderProgramId, "instance_beta_ray");
		    glEnableVertexAttribArray(instanceBetaRayLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceBetaMieBufferId);
			instanceBetaMieLocation = glGetAttribLocation(shaderProgramId, "instance_beta_mie");
		    glEnableVertexAttribArray(instanceBetaMieLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceBetaAbsBufferId);
			instanceBetaAbsLocation = glGetAttribLocation(shaderProgramId, "instance_beta_abs");
		    glEnableVertexAttribArray(instanceBetaAbsLocation);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &elementPositionBufferId);
		        glDeleteBuffers(1, &instanceOriginBufferId);
		        glDeleteBuffers(1, &instanceRadiusBufferId);
		        glDeleteBuffers(1, &instanceIlluminationSourceBufferId);
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
			const std::vector<float>& surface_temperature,
			const std::vector<float>& temperature_change_per_radius2,
			const std::vector<float>& atmosphere_scale_height,
			const std::vector<glm::vec3>& light_source,
			const std::vector<glm::vec3>& light_luminosity,
			const std::vector<glm::vec3>& beta_ray,
			const std::vector<glm::vec3>& beta_mie,
			const std::vector<glm::vec3>& beta_abs,
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
			// glBlendEquationSeparate(GL_MAX, GL_MAX);
			// glEnable(GL_BLEND);

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

			glBindBuffer(GL_ARRAY_BUFFER, instanceIlluminationSourceBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*light_source.size(), &light_source.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceIlluminationSourceLocation);
            glVertexAttribPointer(instanceIlluminationSourceLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceIlluminationSourceLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceIlluminationLuminosityBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*light_luminosity.size(), &light_luminosity.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceIlluminationLuminosityLocation);
            glVertexAttribPointer(instanceIlluminationLuminosityLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceIlluminationLuminosityLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBetaRayBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*beta_ray.size(), &beta_ray.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceBetaRayLocation);
            glVertexAttribPointer(instanceBetaRayLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceBetaRayLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBetaMieBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*beta_mie.size(), &beta_mie.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceBetaMieLocation);
            glVertexAttribPointer(instanceBetaMieLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceBetaMieLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBetaAbsBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*beta_abs.size(), &beta_abs.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceBetaAbsLocation);
            glVertexAttribPointer(instanceBetaAbsLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceBetaAbsLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceRadiusBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*radius.size(), &radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceRadiusLocation);
            glVertexAttribPointer(instanceRadiusLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceRadiusLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceSurfaceTemperatureBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*surface_temperature.size(), &surface_temperature.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceSurfaceTemperatureLocation);
            glVertexAttribPointer(instanceSurfaceTemperatureLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceSurfaceTemperatureLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceTemperatureChangePerRadius2BufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*temperature_change_per_radius2.size(), &temperature_change_per_radius2.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceTemperatureChangePerRadius2Location);
            glVertexAttribPointer(instanceTemperatureChangePerRadius2Location, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceTemperatureChangePerRadius2Location,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceAtmosphereScaleHeightBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*atmosphere_scale_height.size(), &atmosphere_scale_height.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceAtmosphereScaleHeightLocation);
            glVertexAttribPointer(instanceAtmosphereScaleHeightLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceAtmosphereScaleHeightLocation,1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform3fv      (wavelengthLocation,       1, glm::value_ptr(view_state.wavelength));
	        glUniform1f       (exposureIntensityLocation, view_state.exposure_intensity);
	        glUniform1f       (gammaLocation, view_state.gamma);

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), origin.size());
		}
	};
}

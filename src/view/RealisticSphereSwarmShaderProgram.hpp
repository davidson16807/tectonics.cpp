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
		GLuint instanceLightSourceLocation;
		GLuint instanceLightLuminosityLocation;
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
			        out     vec3  fragment_light_direction;
			        out     vec3  fragment_light_intensity;

					out     vec3  fragment_beta_abs;
					out     vec3  fragment_beta_mie;
					out     vec3  fragment_beta_ray;
					out     float fragment_radius;
					out     float fragment_surface_temperature;
					out     float fragment_temperature_change_per_radius2;
					out     float fragment_atmosphere_scale_height;

			        const   float PI = 3.141592653589793238462643383279;

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
			        	fragment_light_intensity = max(vec3(0),instance_illumination_luminosity) / (4.0*PI*l*l) / (4.0*PI*v*v);
			        	fragment_light_direction = (
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
			        in      vec3  fragment_element_position;
			        in      vec3  fragment_light_direction;
			        in      vec3  fragment_light_intensity;

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

					const float SPEED_OF_LIGHT = 299792.458 * METER / SECOND;
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
					R  sphere radius along each coordinate axis
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

					float get_fraction_of_radius_for_star_with_temperature(float temperature, float core_temperature)
					{
					    return sqrt(max(1.0 - (temperature / core_temperature), 0.0));
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
					"r0" is the radius of the world.
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

					vec3 get_intensity3_of_light_emitted_by_atmosphere(
					    vec3 view_origin, vec3 view_direction, float view_start_length, float view_stop_length, vec3 view_wavelengths,
					    vec3 position, 
					    float radius_temperature0, //radius at which temperature is effectively 0
					    float radius_beta, //radius at which beta_* values are sampled
					    float temperature_change_per_radius2, 
					    // ^^^ "radius" here is radius_temperature0
					    // for planets, radius_temperature0 can be set to the slope of a linear relationship to a suitable approximation
					    // for stars, this is equivalent to the core temperature of the star
					    float atmosphere_scale_height,
					    vec3 beta_ray, vec3 beta_mie, vec3 beta_abs,
					    int step_count
					){
						/*
					    For an excellent introduction to what we're try to do here, see Alan Zucconi: 
					      https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
					    We will be using most of the same terminology and variable names.
					    GUIDE TO VARIABLE NAMES:
					     Uppercase letters indicate vectors.
					     Lowercase letters indicate scalars.
					     Going for terseness because I tried longhand names and trust me, you can't read them.
					     "*v*"    property of the view ray, the ray cast from the viewer to the object being viewed
					     "*l*"    property of the light ray, the ray cast from the object to the light source
					     "y*"     distance from the center of the world to the plane shared by view and light ray
					     "z*"     distance from the center of the world to along the plane shared by the view and light ray 
					     "r*"     a distance ("radius") from the center of the world
					     "h*"     the atmospheric scale height, the distance at which air density reduces by a factor of e
					     "*2"     the square of a variable
					     "*0"     property at the start of the raymarch
					     "*1"     property at the end of the raymarch
					     "*i"     property during an iteration of the raymarch
					     "d*"     the change in a property across iterations of the raymarch
					     "beta*"  a scattering coefficient, the number of e-foldings in light intensity per unit distance
					     "gamma*" a phase factor, the fraction of light that's scattered in a certain direction
					     "sigma*" a column density ratio, the density of a column of air relative to surface density
					     "F*"     fraction of source light that reaches the viewer due to scattering for each color channel
					     "*_ray"  property of rayleigh scattering
					     "*_mie"  property of mie scattering
					     "*_abs"  property of absorption
					    setup variable shorthands
					    express all distances in scale heights 
					    express all positions relative to world origin
					    */
					    float h = atmosphere_scale_height;
					    vec3 V0 = (view_origin + view_direction * view_start_length - position) / h;
					    vec3 V1 = (view_origin + view_direction * view_stop_length - position) / h;
					    vec3 V = view_direction; // unit vector pointing to pixel being viewed
					    float v0 = dot(V0,V);
					    float v1 = dot(V1,V);
					    // "beta_*" indicates the rest of the fractional loss.
					    // it is dependant on wavelength, and the density ratio, which is dependant on height
					    // So all together, the fraction of sunlight that scatters to a given angle is: beta(wavelength) * gamma(angle) * density_ratio(height)
					    vec3 beta_sum = h*(beta_ray + beta_mie + beta_abs);
					    // number of iterations within the raymarch
					    float dv = (v1 - v0) / float(step_count);
					    float vi = v0;
					    float z2 = dot(V0,V0) - v0*v0;
					    float sigma; // columnar density encountered along the entire path, relative to surface density, effectively the distance along the surface needed to obtain a similar column density
					    vec3 F = vec3(0); // total intensity for each color channel, found as the sum of light intensities for each path from the light source to the camera
					    for (int i = 0; i < step_count; ++i)
					    {
					        sigma = approx_air_column_density_ratio_through_atmosphere(v0, vi, z2, radius_beta);
					        float ti = temperature_change_per_radius2 * (1.0 - pow(sqrt(vi*vi+z2) / radius_temperature0, 2));
					        F += (
					            // incoming scattered light: the intensity of light that goes towards the camera
					            // I * exp(r-sqrt(vi*vi+y2+zv2)) * beta_gamma * dv 
					            // newly created emitted light: the intensity of light that is generated by the parcel towards the camera
					            + beta_abs * dv
					              * 2.0 * PLANCK_CONSTANT * SPEED_OF_LIGHT*SPEED_OF_LIGHT / (view_wavelengths*view_wavelengths*view_wavelengths*view_wavelengths*view_wavelengths)
					              * exp(radius_beta-sqrt(vi*vi+z2)) / (exp(PLANCK_CONSTANT * SPEED_OF_LIGHT/(view_wavelengths*BOLTZMANN_CONSTANT*ti)) - 1.0) 
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
			        	vec3 N = vec3(fragment_element_position.xy,-z);
			        	vec3 L = fragment_light_direction;
			        	vec3 E_gas_emitted = vec3(0);
    					float r  = fragment_radius; // radius at which "surface" temperature and beta_* variables are sampled
			        	vec3  V  = vec3(0,0,1); 
    					vec3  V0 = (N+V)*r; // origin of view ray, assumes orthographic projection, anywhere outside the sphere is sufficiently distant
    					vec3  O  = vec3(0,0,0);  // center of the star
    					float h  = fragment_atmosphere_scale_height;
				    	float t  = fragment_surface_temperature;
				    	float dtdr2 = fragment_temperature_change_per_radius2;
					    // `r1` is the radius at which temperature is modeled as 0
					    float r1 = r / get_fraction_of_radius_for_star_with_temperature(t, dtdr2); 
    					float r0 = r - 5.0*h; // innermost radius of the atmosphere march
					    maybe_vec2 air_along_view_ray = 
					        get_bounding_distances_along_ray(
					            get_distances_along_line_to_negation(
					                get_distances_along_3d_line_to_sphere(V0, V, O, r1),
					                get_distances_along_3d_line_to_sphere(V0, V, O, r0)
					            )
					        );
					    if(air_along_view_ray.exists)
					    {
					        float v0 = air_along_view_ray.value.x;
					        float v1 = air_along_view_ray.value.y;
					        vec3 E_gas_emitted = get_intensity3_of_light_emitted_by_atmosphere(
					            V0, V, v0, v1, wavelength3,
					            O, r1, r, dtdr2, h,
					            fragment_beta_ray, 
					            fragment_beta_mie, 
					            fragment_beta_abs,
					            16
					        );
					    }
			        	vec3 fraction = vec3(dot(N,L));
			        	vec3 E_surface_reflected = fraction * fragment_light_intensity;
			        	fragment_color = vec4(1);
			            // fragment_color = vec4(
					    // 	get_signal3_for_intensity3(
					    // 		get_ldrtone3_for_intensity3(
					    // 			E_surface_reflected + E_gas_emitted, 
					    // 			exposure_intensity
					    // 		), 
						//     	gamma
					    // 	),
			            // 1);
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
			glGenBuffers(1, &instanceLightSourceBufferId);
			instanceLightSourceLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_source");
		    glEnableVertexAttribArray(instanceLightSourceLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceLightLuminosityBufferId);
			instanceLightLuminosityLocation = glGetAttribLocation(shaderProgramId, "instance_illumination_luminosity");
		    glEnableVertexAttribArray(instanceLightLuminosityLocation);

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
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*radius.size(), &radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceSurfaceTemperatureLocation);
            glVertexAttribPointer(instanceSurfaceTemperatureLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceSurfaceTemperatureLocation,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceTemperatureChangePerRadius2BufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*radius.size(), &radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceTemperatureChangePerRadius2Location);
            glVertexAttribPointer(instanceTemperatureChangePerRadius2Location, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceTemperatureChangePerRadius2Location,1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceAtmosphereScaleHeightBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*radius.size(), &radius.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceAtmosphereScaleHeightLocation);
            glVertexAttribPointer(instanceAtmosphereScaleHeightLocation, 1, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceAtmosphereScaleHeightLocation,1);

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

#pragma once

// std libraries
#include <iostream>       // std::cout
#include <regex>          // std::regex
#include <vector>         // std::vector
#include <unordered_set>  // std::unordered_set

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/mat4x4.hpp> // *mat4
#include <glm/gtc/type_ptr.hpp>// *vec4

// in house libraries
#include <many/types.hpp> // floats, etc.

namespace view 
{

	enum RenderPassType
	{
		solids,
		volumetrics,
		lens_effects,
	};
	enum ProjectionType
	{
		heads_up_display,
		perspective,
		equirectangular,
		equirectangular_texture
	};
	struct ViewState
	{
		glm::mat4 view_matrix;
		glm::mat4 model_matrix;
		glm::mat4 projection_matrix;
		ProjectionType projection_type;
		RenderPassType render_pass;

		ViewState():
			view_matrix(),
			model_matrix(),
			projection_matrix(),
			projection_type(ProjectionType::perspective),
			render_pass(RenderPassType::solids)
		{}
	};

	enum ColorscaleType
	{
		heatscale,
		topographic,
		monochrome
	};
	template <typename T>
	struct ColorscaleSurfacesViewState
	{
		glm::vec3 min_color;
		glm::vec3 max_color;
		T min_value;
		T max_value;
		float sealevel;
		ColorscaleType colorscale_type;

		ColorscaleSurfacesViewState():
			min_color(0),
			max_color(1),
			min_value(0),
			max_value(270),
			sealevel(0),
			colorscale_type(ColorscaleType::heatscale)
		{}
	};

    // TODO: move this to a dedicated namespace for projections
	std::string get_default_clipspace_position_glsl = R"(
	    vec4 get_default_clipspace_position (
	        in  vec4  local_position,
	        in  mat4  model_matrix,
	        in  mat4  view_matrix,
	        in  mat4  projection_matrix,
	        in  int   projection_type,
	        in  float map_projection_offset
	    ) {
	        const float PI = 3.14159265358979;
	        if (projection_type == 0)
	        {
	            return local_position;
	        } 
	        else if (projection_type == 1)
	        {
	            return projection_matrix * view_matrix * model_matrix * local_position;
	        }
	        else if (projection_type >= 2)
	        {
	            vec4 model_position = model_matrix * local_position;
	            vec4 view_position = view_matrix[3];
	            float focus = atan(-view_position.z, view_position.x) + PI + map_projection_offset;
	            float lon_focused = mod(atan(-model_position.z, model_position.x) + PI - focus, 2.*PI) - PI;
	            float lat_focused = asin(model_position.y / length(model_position)); //+ (map_projection_offset*PI);
	            bool is_on_edge = lon_focused >  PI*0.9 || lon_focused < -PI*0.9;
	            vec4 projected_position = vec4(
	                lon_focused, lat_focused, is_on_edge? 0.0 : length(model_position), 1
	            );
	            if (projection_type == 2) 
	            {
	                mat4 scale_matrix = mat4(1);
	                scale_matrix[3] = view_matrix[3];
	                return projection_matrix * scale_matrix * projected_position;
	            }
	            else if(projection_type == 3)
	            {
	                return projected_position;
	            }
	        }
	    }
    )";
	/*
	A "ColorscaleSurfacesProgram" seals off access to resources relating to an 
	OpenGL shader program within an OpenGL Context, 
	allowing view state to be managed statelessly elsewhere. 

	It guarantees the following:
	* all internal resources are created on initialization to minimize state transitions (RAII)
	* all internal resources are strictly encapsulated
	* the program can be in only one of two states: "created" and "disposed"
	* the disposed state can be entered at any time but never exited
	* all methods will continue to produce sensible, well defined behavior in the disposed state
	* the output that draw() sends to the currently bound framebuffer is 
	  a pure function of its input

	Its state transitions can be described with the following diagram:

	initialized
        ↓        dispose()
	 disposed
        ↻        dispose()

	Any attempt to relax guarantees made here will severely cripple 
	your ability to reason with the code base. 
	*/
	class ColorscaleSurfacesShaderProgram
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
		GLuint displacementBufferId;

		// attributes
	    int positionLocation;
	    int colorValueLocation;
	    int displacementLocation;

		// uniforms
	    int viewMatrixLocation;
		int modelMatrixLocation;
		int projectionMatrixLocation;
		int projectionTypeLocation;
		int colorscaleTypeLocation;
		int minColorLocation;
		int maxColorLocation;
		int minValueLocation;
		int maxValueLocation;
		int sealevelLocation;

		bool isDisposed;
	public:
		~ColorscaleSurfacesShaderProgram()
		{
		}
		explicit ColorscaleSurfacesShaderProgram() :
			vertexShaderGlsl(
				std::regex_replace(
					R"(#version 330
				        uniform   mat4  model_matrix;
				        uniform   mat4  view_matrix;
				        uniform   mat4  projection_matrix;
				        uniform   int   projection_type;
				        uniform   float map_projection_offset;
				        in        vec4  vertex_position;
				        in        float vertex_color_value;
				        in        float vertex_displacement;
				        out       float fragment_color_value;
				        out       float fragment_displacement;
				        {}
				        void main(){
				            fragment_color_value = vertex_color_value;
				            fragment_displacement = vertex_displacement;
				            gl_Position = get_default_clipspace_position(
				                vertex_position, model_matrix, view_matrix, 
				                projection_matrix, projection_type, map_projection_offset
				            );
				        };
					)",
					std::regex("\\{\\}"), 
					get_default_clipspace_position_glsl
				)
			),
			fragmentShaderGlsl(
				  R"(#version 330
			        precision mediump float;
			        uniform int   colorscale_type;
			        uniform vec3  min_color;
			        uniform vec3  max_color;
			        uniform float min_value;
			        uniform float max_value;
			        uniform float sealevel;
			        in      float fragment_color_value;
			        in      float fragment_displacement;
			        out     vec4  fragment_color;

			        /*
			        converts float from 0-1 to a heat map visualization
			        credit goes to Gaëtan Renaudeau: http://greweb.me/glsl.js/examples/heatmap/
			        */
			        vec4 get_rgb_signal_of_fraction_for_heatmap (float v) {
			            float value = 1.0-v;
			            return vec4((0.5+0.5*smoothstep(0.0, 0.1, value))*vec3(
			                smoothstep(0.5, 0.3, value),
			                value < 0.3 ? smoothstep(0.0, 0.3, value) : smoothstep(1.0, 0.6, value),
			                smoothstep(0.4, 0.6, value)
			            ), 1);
			        }

			        //converts a float ranging from [-1,1] to a topographic map visualization
			        vec4 get_rgb_signal_of_fraction_for_topomap(float value) {
			            //deep ocean
			            vec3 color = vec3(0,0,0.8);
			            //shallow ocean
			            color = mix(color, vec3(0.5,0.5,1), smoothstep(-1., -0.01, value));
			            //lowland
			            color = mix(color, vec3(0,0.55,0), smoothstep(-0.01, 0.01, value));
			            //highland
			            color = mix(color, vec3(0.95,0.95,0), smoothstep(0., 0.45, value));
			            //mountain
			            color = mix(color, vec3(0.5,0.5,0), smoothstep(0.2, 0.7, value));
			            //mountain
			            color = mix(color, vec3(0.5,0.5,0.5), smoothstep(0.4, 0.8, value));
			            //snow cap
			            color = mix(color, vec3(0.95), smoothstep(0.75, 1., value));
			            return vec4(color, 1.);
			        }

			        void main() {
			            vec4 color_without_ocean = 
			              colorscale_type == 0? get_rgb_signal_of_fraction_for_heatmap(fragment_color_value) 
			            : colorscale_type == 1? get_rgb_signal_of_fraction_for_topomap(fragment_color_value)
			            :                       vec4( mix( min_color, max_color, fragment_color_value ), 1.0);
			            vec4 color_with_ocean = mix(
			                vec4(0.), 
			                color_without_ocean, 
			                fragment_displacement < sealevel? 0.5 : 1.0
			            );
			            fragment_color = color_with_ocean;
			        }
					)"
			)
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
			projectionTypeLocation = glGetUniformLocation(shaderProgramId, "projection_type");

			colorscaleTypeLocation = glGetUniformLocation(shaderProgramId, "colorscale_type");
			minColorLocation = glGetUniformLocation(shaderProgramId, "min_color");
			maxColorLocation = glGetUniformLocation(shaderProgramId, "max_color");
			minValueLocation = glGetUniformLocation(shaderProgramId, "min_value");
			maxValueLocation = glGetUniformLocation(shaderProgramId, "max_value");
			sealevelLocation = glGetUniformLocation(shaderProgramId, "sealevel");

	        // ATTRIBUTES

			// generate new vertex array object, VAO
		    glGenVertexArrays(1, &attributeId);
		    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		    glBindVertexArray(attributeId);


			// create a new vertex array buffer, VBO
			glGenBuffers(1, &positionBufferId);
			// set current buffer in OpenGL's state machine to positionBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
			// attr #, 3 dimensions, floats
			glVertexAttribPointer(positionLocation, 3, GL_FLOAT, normalize, stride, offset);
			glBindAttribLocation(shaderProgramId, positionLocation, "vertex_position");


			// create a new vertex array buffer, VBO
			glGenBuffers(1, &colorValueBufferId);
			// set current buffer in OpenGL's state machine to colorValueBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, colorValueBufferId);
			// attr #, 3 dimensions, floats
			glVertexAttribPointer(colorValueLocation, 3, GL_FLOAT, normalize, stride, offset);
			glBindAttribLocation(shaderProgramId, colorValueLocation, "vertex_color_value");


			// create a new vertex array buffer, VBO
			glGenBuffers(1, &displacementBufferId);
			// set current buffer in OpenGL's state machine to displacementBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, displacementBufferId);
			// define layout of the first (current?) buffer
			// attr #, 3 dimensions, floats
			glVertexAttribPointer(displacementBufferId, 3, GL_FLOAT, normalize, stride, offset);
			glBindAttribLocation(shaderProgramId, displacementBufferId, "vertex_displacement");


		    glEnableVertexAttribArray(positionLocation);
		    glEnableVertexAttribArray(colorValueLocation);
		    glEnableVertexAttribArray(displacementBufferId);

		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &colorValueBufferId);
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
		template <typename T>
		bool canDepict(
			ColorscaleSurfacesViewState<T>& colorscale_state, 
			ViewState& view_state
		){
			return !isDisposed;
		}

		/*
	    `draw()` adds a depiction of a scalar field to the framebuffer that is currently 
	    bound to the program's context using options from a given view state.
	    Does nothing if unable to depict the given view state, as determined by canDepict().
		The only state that is allowed to be modified is that of the framebuffer
		*/
		template <typename T>
		void draw(
			std::vector<float>& flattened_face_vertex_coordinates, 
			std::vector<T>& flattened_face_vertex_color_values, 
			std::vector<float>& flattened_face_vertex_displacements, 
			ColorscaleSurfacesViewState<T>& colorscale_state, 
			ViewState& view_state
		){
			if (canDepict(colorscale_state, view_state))
			{
				return;
			}
			if (view_state.render_pass != RenderPassType::solids)
			{
				return; 
			}

			assert(flattened_face_vertex_color_values.size() == flattened_face_vertex_displacements.size());
			assert(flattened_face_vertex_color_values.size() == flattened_face_vertex_coordinates.size() * 3);

			glUseProgram(shaderProgramId);
			glBindVertexArray(attributeId);


			//ATTRIBUTES
			// set current buffer in OpenGL's state machine to positionBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
		    glEnableVertexAttribArray(positionLocation);
            // attr #, 3 dimensions, floats
            glVertexAttribPointer(positionLocation, 3, GL_FLOAT, normalize, 0, NULL);

			// set current buffer in OpenGL's state machine to colorValueBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, colorValueBufferId);
		    glEnableVertexAttribArray(colorValueLocation);
            glVertexAttribPointer(colorValueLocation, 1, GL_FLOAT, normalize, 0, NULL);

			// set current buffer in OpenGL's state machine to displacementBufferId (if not already)
			glBindBuffer(GL_ARRAY_BUFFER, displacementBufferId);
		    glEnableVertexAttribArray(displacementLocation);
            glVertexAttribPointer(displacementLocation, 1, GL_FLOAT, normalize, 0, NULL);


    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(view_state.model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform1i       (projectionTypeLocation, view_state.projection_type);

	        glUniform3fv(minColorLocation, 1, glm::value_ptr(colorscale_state.min_color));
	        glUniform3fv(maxColorLocation, 1, glm::value_ptr(colorscale_state.max_color));
	        glUniform1f (minValueLocation, colorscale_state.min_value);
	        glUniform1f (maxValueLocation, colorscale_state.max_value);
	        glUniform1f (sealevelLocation, colorscale_state.sealevel);
	        glUniform1i (colorscaleTypeLocation, colorscale_state.colorscale_type);

			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(flattened_face_vertex_coordinates), &flattened_face_vertex_coordinates.front(), GL_DYNAMIC_DRAW);
	        glBindBuffer(GL_ARRAY_BUFFER, colorValueBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(flattened_face_vertex_color_values), &flattened_face_vertex_color_values.front(), GL_DYNAMIC_DRAW);
	        glBindBuffer(GL_ARRAY_BUFFER, displacementBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(flattened_face_vertex_displacements), &flattened_face_vertex_displacements.front(), GL_DYNAMIC_DRAW);
			
			glDrawArrays(GL_TRIANGLES, /*array offset*/ 0, /*triangle count*/ flattened_face_vertex_color_values.size());
		}
	};
}
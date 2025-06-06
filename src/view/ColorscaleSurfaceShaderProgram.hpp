#pragma once

// c libraries
#include <cstddef>        // std::byte

// std libraries
#include <vector>         // std::vector
#include <unordered_set>  // std::unordered_set

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/gtc/type_ptr.hpp>// *vec4

// in house libraries
#include <view/ViewState.hpp> // ViewState

namespace view 
{

	enum ColorscaleType
	{
		heatscale,
		topographic,
		monochrome
	};
	struct ColorscaleSurfacesViewState
	{
		glm::vec3 min_color;
		glm::vec3 max_color;
		float min_color_value;
		float max_color_value;
		float darken_threshold;
		ColorscaleType colorscale_type;

		ColorscaleSurfacesViewState():
			min_color(0),
			max_color(1),
			min_color_value(0),
			max_color_value(1),
			colorscale_type(ColorscaleType::heatscale)
		{}
	};

	/*
	`ColorscaleSurfaceShader` renders a surface (i.e. where vertices are referenced by an element buffer) 
	in such a way that color represents an abstract concept as a function of a single scalar value.
	*/

	class ColorscaleSurfaceShaderProgram
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
		GLuint darkenBufferId;
		GLuint existsBufferId;
		GLuint elementVertexBufferId;

		// attributes
	    GLuint positionLocation;
	    GLuint colorValueLocation;
	    GLuint displacementLocation;
	    GLuint darkenLocation;
	    GLuint existsLocation;

		// uniforms
	    GLint viewMatrixLocation;
		GLint modelMatrixLocation;
		GLint projectionMatrixLocation;
		GLint existsThresholdLocation;
		GLint colorscaleTypeLocation;
		GLint minColorLocation;
		GLint maxColorLocation;
		GLint minColorValueLocation;
		GLint maxColorValueLocation;
		GLint darkenThresholdLocation;

		bool isDisposed;

	public:
		~ColorscaleSurfaceShaderProgram()
		{
		}
		explicit ColorscaleSurfaceShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform   mat4  model_matrix;
			        uniform   mat4  view_matrix;
			        uniform   mat4  projection_matrix;
			        in        vec4  vertex_position;
			        in        float vertex_color_value;
			        in        float vertex_displacement;
			        in        float vertex_darken;
			        in        uint  vertex_exists;
			        out       float fragment_color_value;
			        out       float fragment_darken;
			        out       float fragment_exists;
			        void main(){
			            fragment_color_value = vertex_color_value;
			            fragment_darken = vertex_darken;
			            fragment_exists = vertex_exists == 0u? 0.0 : 1.0;
			            /* 
			            NOTES: 
			            * For a heads up display, set all `*_matrix` parameters to identity.
			            * To disable a vertex, set vertex_exists to 0.0.
			            Alpha blending is disabled since it conflicts with other optimizations,
			            and it doesn't display well anyway on most grids we use,
			            To provide at least some method of exists, 
			            we set vertex position outside clipspace if vertex_exists is toggled off.
			            */
		            	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        uniform int   colorscale_type;
			        uniform vec3  min_color;
			        uniform vec3  max_color;
			        uniform float min_color_value;
			        uniform float max_color_value;
			        uniform float darken_threshold;
			        in      float fragment_color_value;
			        in      float fragment_darken;
			        in      float fragment_exists;
			        out     vec4  fragment_color;

			        /*
			        converts float from 0-1 to a heat map visualization, adapted from Gaëtan Renaudeau
			        */
			        vec3 get_rgb_signal_of_fraction_for_heatmap (float v) {
			            float value = 1.0-v;
			            return vec3((0.5+0.5*smoothstep(0.0, 0.1, value))*vec3(
			                smoothstep(0.5, 0.3, value),
			                value < 0.3 ? smoothstep(0.0, 0.3, value) : smoothstep(1.0, 0.6, value),
			                smoothstep(0.4, 0.6, value)
			            ));
			        }

			        //converts a float ranging from [-1,1] to a topographic map visualization
			        vec3 get_rgb_signal_of_fraction_for_topomap(float value) {
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
			            return color;
			        }

			        void main() {
			        	float color_value_fraction = smoothstep(min_color_value, max_color_value, fragment_color_value);
			            vec3 color_without_ocean = 
			              colorscale_type == 0? get_rgb_signal_of_fraction_for_heatmap(color_value_fraction) 
			            : colorscale_type == 1? get_rgb_signal_of_fraction_for_topomap(color_value_fraction)
			            :                       mix( min_color, max_color, color_value_fraction );
			            if(fragment_exists<0.5){discard;}
			            fragment_color = 
			            	isnan(fragment_color_value)? vec4(1,0,1,0.5)
			              : isinf(fragment_color_value)? vec4(1,1,1,0.5)
			              : isnan(color_value_fraction)? vec4(0,1,1,0.5)
			              : isinf(color_value_fraction)? vec4(1,1,1,1)/2.0f
				          : vec4(
					            mix(
					                vec3(0.), 
					                color_without_ocean, 
					                fragment_darken < darken_threshold? 0.5 : 1.0
					            ), 
					            fragment_exists
					        );
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

			colorscaleTypeLocation = glGetUniformLocation(shaderProgramId, "colorscale_type");
			minColorLocation = glGetUniformLocation(shaderProgramId, "min_color");
			maxColorLocation = glGetUniformLocation(shaderProgramId, "max_color");
			minColorValueLocation = glGetUniformLocation(shaderProgramId, "min_color_value");
			maxColorValueLocation = glGetUniformLocation(shaderProgramId, "max_color_value");
			darkenThresholdLocation = glGetUniformLocation(shaderProgramId, "darken_threshold");

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
			glGenBuffers(1, &colorValueBufferId);
			colorValueLocation = glGetAttribLocation(shaderProgramId, "vertex_color_value");
		    glEnableVertexAttribArray(colorValueLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &displacementBufferId);
			displacementLocation = glGetAttribLocation(shaderProgramId, "vertex_displacement");
		    glEnableVertexAttribArray(displacementLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &darkenBufferId);
			darkenLocation = glGetAttribLocation(shaderProgramId, "vertex_darken");
		    glEnableVertexAttribArray(darkenLocation);

			// create a new vertex array buffer, VBO
			glGenBuffers(1, &existsBufferId);
			existsLocation = glGetAttribLocation(shaderProgramId, "vertex_exists");
		    glEnableVertexAttribArray(existsLocation);

			glGenBuffers(1, &elementVertexBufferId);
		}

		void dispose()
		{
        	if (!isDisposed) 
        	{
		        isDisposed = true;
		        glDeleteBuffers(1, &colorValueBufferId);
		        glDeleteBuffers(1, &positionBufferId);
		        glDeleteBuffers(1, &displacementBufferId);
		        glDeleteBuffers(1, &darkenBufferId);
		        glDeleteBuffers(1, &existsBufferId);
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
			const ColorscaleSurfacesViewState& colorscale_state, 
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
		template <typename T>
		void draw(
			const std::vector<glm::vec3>& vertex_position, 
			const std::vector<T>& vertex_color_value, 
			const std::vector<float>& vertex_displacement, 
			const std::vector<float>& vertex_darken, 
			const std::vector<std::uint8_t>& vertex_exists, 
			const std::vector<unsigned int>& element_vertex_ids,
			const ColorscaleSurfacesViewState& colorscale_state,
			const glm::mat4& model_matrix,
			const ViewState& view_state,
			const unsigned int gl_mode=GL_TRIANGLES
		){
			if (!canDepict(colorscale_state, view_state))
			{
				return;
			}
			if (view_state.render_pass != RenderPassType::solids)
			{
				return; 
			}

			assert(vertex_position.size() == vertex_color_value.size());
			assert(vertex_position.size() == vertex_displacement.size());
			assert(vertex_position.size() == vertex_darken.size());

			glUseProgram(shaderProgramId);
			glBindVertexArray(attributeId);
			glEnable(GL_CULL_FACE);  
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			//ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertex_position.size(), &vertex_position.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(positionLocation);
            glVertexAttribPointer(positionLocation, 3, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, colorValueBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertex_color_value.size(), &vertex_color_value.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(colorValueLocation);
            glVertexAttribPointer(colorValueLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, displacementBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_displacement.size(), &vertex_displacement.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(displacementLocation);
            glVertexAttribPointer(displacementLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, darkenBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_darken.size(), &vertex_darken.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(darkenLocation);
            glVertexAttribPointer(darkenLocation, 1, GL_FLOAT, normalize, stride, offset);

			glBindBuffer(GL_ARRAY_BUFFER, existsBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(std::byte)*vertex_exists.size(), &vertex_exists.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(existsLocation);
            glVertexAttribIPointer(existsLocation, 1, GL_UNSIGNED_BYTE, stride, offset);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVertexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*element_vertex_ids.size(), &element_vertex_ids.front(), GL_DYNAMIC_DRAW);

    		// UNIFORMS
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(model_matrix));
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));

	        glUniform3fv(minColorLocation, 1, glm::value_ptr(colorscale_state.min_color));
	        glUniform3fv(maxColorLocation, 1, glm::value_ptr(colorscale_state.max_color));
	        glUniform1f (minColorValueLocation, colorscale_state.min_color_value);
	        glUniform1f (maxColorValueLocation, colorscale_state.max_color_value);
	        glUniform1f (darkenThresholdLocation, colorscale_state.darken_threshold);
	        glUniform1i (colorscaleTypeLocation, colorscale_state.colorscale_type);

			glDrawElements(gl_mode, /*element count*/ element_vertex_ids.size(), GL_UNSIGNED_INT, 0);
		}
	};
}


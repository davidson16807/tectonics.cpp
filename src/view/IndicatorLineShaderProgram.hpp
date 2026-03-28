#pragma once

// std libraries
#include <vector>         // std::vector

// glm libraries
#include <glm/vec2.hpp>   // *vec2
#include <glm/vec3.hpp>   // *vec3
#include <glm/vec4.hpp>   // *vec4
#include <glm/gtc/type_ptr.hpp>// value_ptr

// in house libraries
#include <view/ViewState.hpp> // ViewState

namespace view
{

	/*
	`IndicatorLineShaderProgram` renders a set of line segments that represent abstract concepts.
	Each segment is depicted as a camera-facing quad with constant pixel thickness.
	*/

	class IndicatorLineShaderProgram
	{
	    // properties shared by all attributes within the program
	    static constexpr GLenum normalize = GL_FALSE; // don't normalize the data
	    static constexpr GLsizei stride = 0;          // 0: advance size*sizeof(type) each iteration
	    static constexpr void* offset = NULL;         // NULL: start at the beginning of the buffer

		std::string vertexShaderGlsl;
		std::string fragmentShaderGlsl;

		std::vector<glm::vec2> elementPositions;

		GLuint shaderProgramId;
		GLuint vertexArrayId;

		// BUFFER IDS

		// element buffer ids
		GLuint elementPositionBufferId;

		// instance buffer ids
		GLuint instanceStartBufferId;
		GLuint instanceEndBufferId;
		GLuint instanceColorBufferId;

		// LOCATIONS

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceStartLocation;
		GLuint instanceEndLocation;
		GLuint instanceColorLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint resolutionLocation;
		GLuint pixelWidthLocation;

		bool isDisposed;

	public:
		explicit IndicatorLineShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        uniform vec2  resolution;
			        uniform float pixel_width;

			        in      vec2  element_position;
			        in      vec3  instance_start;
			        in      vec3  instance_end;
			        in      vec4  instance_color;

			        out     vec2  fragment_element_position;
			        out     vec4  fragment_color_in;

			        void main() {
			        	fragment_color_in = instance_color;
			        	fragment_element_position = element_position;

			        	vec4 global_start = global_for_local * vec4(instance_start,1);
			        	vec4 global_end   = global_for_local * vec4(instance_end,  1);

			        	vec4 view_start = view_for_global * global_start;
			        	vec4 view_end   = view_for_global * global_end;

			        	vec4 clip_start = clip_for_view * view_start;
			        	vec4 clip_end   = clip_for_view * view_end;

			        	// interpolate along the segment
			        	vec4 clip_center = mix(clip_start, clip_end, element_position.x);

			        	// compute a screen-space perpendicular so thickness stays constant in pixels
			        	vec2 ndc_start = clip_start.xy / clip_start.w;
			        	vec2 ndc_end   = clip_end.xy   / clip_end.w;

			        	vec2 screen_delta = (ndc_end - ndc_start) * resolution;

			        	float length2 = dot(screen_delta, screen_delta);
			        	vec2 screen_tangent = length2 > 0.0
			        		? normalize(screen_delta)
			        		: vec2(1.0, 0.0);

			        	vec2 screen_normal = vec2(-screen_tangent.y, screen_tangent.x);

			        	// convert pixel offset back into clip-space offset
			        	vec2 ndc_offset = vec2(
			        		2.0 * pixel_width * element_position.y * screen_normal.x / resolution.x,
			        		2.0 * pixel_width * element_position.y * screen_normal.y / resolution.y
			        	);

			        	vec4 clip_offset = vec4(ndc_offset * clip_center.w, 0.0, 0.0);
			        	gl_Position = clip_center + clip_offset;
			        };
				)"
			),
			fragmentShaderGlsl(
				R"(#version 330
			        precision mediump float;
			        in      vec2  fragment_element_position;
			        in      vec4  fragment_color_in;
			        out     vec4  fragment_color;
			        void main() {
			        	fragment_color = fragment_color_in
			        		* vec4(1,1,1,exp(-pow(fragment_element_position.y*2.0,2.0)));
			        }
				)"
			),
			isDisposed(false)
		{
			/*
			quad for a segment:
			x = interpolation from start(0) to end(1)
			y = signed half-width direction (-1 to +1)
			*/
			elementPositions = {
				glm::vec2(0,-1),
				glm::vec2(1,-1),
				glm::vec2(1, 1),
				glm::vec2(1, 1),
				glm::vec2(0, 1),
				glm::vec2(0,-1)
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
		    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		    if (!success)
		    {
		        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		    }

			shaderProgramId = glCreateProgram();
			glAttachShader(shaderProgramId, fragmentShaderId);
			glAttachShader(shaderProgramId, vertexShaderId);
    		glLinkProgram(shaderProgramId);
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
			pixelWidthLocation = glGetUniformLocation(shaderProgramId, "pixel_width");

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
			glGenBuffers(1, &instanceStartBufferId);
			instanceStartLocation = glGetAttribLocation(shaderProgramId, "instance_start");
		    glEnableVertexAttribArray(instanceStartLocation);

			// create a new vertex buffer object, VBO
			glGenBuffers(1, &instanceEndBufferId);
			instanceEndLocation = glGetAttribLocation(shaderProgramId, "instance_end");
		    glEnableVertexAttribArray(instanceEndLocation);

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
		        glDeleteBuffers(1, &instanceStartBufferId);
		        glDeleteBuffers(1, &instanceEndBufferId);
		        glDeleteBuffers(1, &instanceColorBufferId);
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
			const std::vector<glm::vec3>& instance_start,
			const std::vector<glm::vec3>& instance_end,
			const std::vector<glm::vec4>& instance_color,
			const float& pixel_width,
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

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, elementPositionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*elementPositions.size(), &elementPositions.front(), GL_STATIC_DRAW);
		    glEnableVertexAttribArray(elementPositionLocation);
            glVertexAttribPointer(elementPositionLocation, 2, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(elementPositionLocation, 0);

			glBindBuffer(GL_ARRAY_BUFFER, instanceStartBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*instance_start.size(), &instance_start.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceStartLocation);
            glVertexAttribPointer(instanceStartLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceStartLocation, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceEndBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*instance_end.size(), &instance_end.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceEndLocation);
            glVertexAttribPointer(instanceEndLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceEndLocation, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceColorBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*instance_color.size(), &instance_color.front(), GL_DYNAMIC_DRAW);
		    glEnableVertexAttribArray(instanceColorLocation);
            glVertexAttribPointer(instanceColorLocation, 4, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(instanceColorLocation, 1);

    		// UNIFORMS
	        glUniformMatrix4fv(viewMatrixLocation,       1, GL_FALSE, glm::value_ptr(view_state.view_matrix));
	        glUniformMatrix4fv(modelMatrixLocation,      1, GL_FALSE, glm::value_ptr(model_matrix));
	        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(view_state.projection_matrix));
	        glUniform2fv      (resolutionLocation,       1, glm::value_ptr(view_state.resolution));
	        glUniform1f       (pixelWidthLocation, pixel_width);

			glDrawArraysInstanced(
				GL_TRIANGLES,
				/*array offset*/ 0,
				/*vertex count*/ elementPositions.size(),
				instance_start.size()
			);
		}

		/*
		Convenience wrapper for the `draw` method above.
		`node_positions` are the N+1 positions for bounds of N abutted line segments.
		`segment_colors` are the N colors of line segments.
		*/
		void draw(
			const std::vector<glm::vec3>& node_positions,
			const std::vector<glm::vec4>& segment_colors,
			const float& pixel_width,
			const glm::mat4 model_matrix,
			const ViewState& view_state
		){
			if (node_positions.size() < 2)
			{
				return;
			}

			const std::size_t segment_count = node_positions.size() - 1;
			if (segment_colors.size() != segment_count)
			{
				return;
			}

			std::vector<glm::vec3> instance_start;
			std::vector<glm::vec3> instance_end;

			instance_start.reserve(segment_count);
			instance_end.reserve(segment_count);

			for (std::size_t i = 0; i < segment_count; ++i)
			{
				instance_start.push_back(node_positions[i]);
				instance_end.push_back(node_positions[i+1]);
			}

			draw(
				instance_start,
				instance_end,
				segment_colors,
				pixel_width,
				model_matrix,
				view_state
			);
		}

		/*
		Convenience wrapper for the `draw` method above.
		`node_positions` are the N+1 positions for bounds of N abutted line segments.
		`color` is the color applied to all line segments.
		*/
		void draw(
			const std::vector<glm::vec3>& node_positions,
			const glm::vec4& color,
			const float& pixel_width,
			const glm::mat4 model_matrix,
			const ViewState& view_state
		){
			if (node_positions.size() < 2)
			{
				return;
			}

			const std::size_t segment_count = node_positions.size() - 1;

			std::vector<glm::vec4> segment_colors(segment_count, color);

			draw(
				node_positions,
				segment_colors,
				pixel_width,
				model_matrix,
				view_state
			);
		}

	};
}


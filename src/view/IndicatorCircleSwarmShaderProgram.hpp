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
	`IndicatorCircleSwarmShaderProgram` renders a swarm of circles that represent abstract concepts
	The circles have a uniform inner and outer radius measured as a fraction of screen height,
	and a unique color per instance.
	*/

	class IndicatorCircleSwarmShaderProgram
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
		GLuint instanceRadiusBufferId;
		GLuint instanceColorBufferId;

		// LOCATIONS

		// element attributes
	    GLuint elementPositionLocation;

	    // instance attributes
		GLuint instanceOriginLocation;
		GLuint instanceRadiusLocation;
		GLuint instanceColorLocation;
		GLuint pixelWidthLocation;
		GLuint pixelSeparationLocation;
		GLuint scaleHeightCountLocation;

		// uniforms
	    GLuint modelMatrixLocation;
	    GLuint viewMatrixLocation;
		GLuint projectionMatrixLocation;
		GLuint resolutionLocation;

		bool isDisposed;

	public:
		explicit IndicatorCircleSwarmShaderProgram() :
			vertexShaderGlsl(
				R"(#version 330
			        uniform mat4  global_for_local;
			        uniform mat4  view_for_global;
			        uniform mat4  clip_for_view;
			        uniform vec2  resolution;
			        uniform float pixel_width;
			        uniform float pixel_separation;
			        in      vec3  element_position;
			        in      vec3  instance_origin;
			        in      float instance_radius;
			        in      vec4  instance_color;
			        out     float fragment_inner_pixel_radius;
					out     float fragment_outer_pixel_radius;
			        out     vec3  fragment_element_position;
			        out     vec4  fragment_color_in;
			        void main(){
			        	/*
			        	spheres are billboards, which must always face the camera:
			        	for position data: the local→view map is the usual implementation
			        	for rotation data: the local→view map is identity
			        	for scaling data: the element→clip map is a scaling operation,
			            	except we specify width and separation in clip space
			        	*/
			            mat4 scale_map = mat4(instance_radius);
			        	vec4 view_for_element_origin = view_for_global * global_for_local * vec4(instance_origin,1);
			        	mat4 view_for_element = mat4(scale_map[0], scale_map[1], scale_map[2], view_for_element_origin);
			        	vec4 clip_origin = clip_for_view * view_for_element_origin;
			        	vec4 clip_position = clip_for_view * view_for_element * vec4(element_position,1)
			        	    + vec4((pixel_separation+pixel_width)/resolution.y * 
			        			element_position * 
			        			clip_origin.w / vec3(resolution.x/resolution.y,1,1), 
			        			0.0);

						float clip_radius = (clip_for_view * view_for_element * vec4(1,0,0,0)).x;
						float pixel_radius = clip_radius / clip_origin.w * resolution.y;

						fragment_inner_pixel_radius = pixel_radius + pixel_separation;
						fragment_outer_pixel_radius = pixel_radius + pixel_separation + pixel_width;

			            fragment_color_in = instance_color;
			        	fragment_element_position = element_position;

			            gl_Position = clip_position;
			        };
				)"
			),

			fragmentShaderGlsl(
				R"(#version 330
			        uniform vec2  resolution;
			        uniform float scale_height_count;
			        in      vec4  fragment_color_in;
			        in      vec3  fragment_element_position;
			        in      float  fragment_inner_pixel_radius;
					in      float  fragment_outer_pixel_radius;
			        out     vec4  fragment_color;

			        void main() {
			        	const float rhi = 1.0;
			        	float rlo = fragment_inner_pixel_radius / fragment_outer_pixel_radius;
			        	float rmid = 0.5*(rlo+rhi);
			        	float r2 = dot(fragment_element_position.xy, fragment_element_position.xy);
			        	float r = sqrt(r2);
			        	if(!(rlo*rlo<r2&&r2<1.0)) { discard; }
			        	float width = rhi-rlo;
			        	float scale_height = width/scale_height_count;
			            fragment_color = fragment_color_in * vec4(1,1,1, exp(-pow((r-rmid)/scale_height,2.0)));
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
		        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
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
			pixelWidthLocation = glGetUniformLocation(shaderProgramId, "pixel_width");
			pixelSeparationLocation = glGetUniformLocation(shaderProgramId, "pixel_separation");
			scaleHeightCountLocation = glGetUniformLocation(shaderProgramId, "scale_height_count");

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
		A circle with sharp boundaries is accounted for by the degenerate case where scale_height_count = 1
		*/
		void draw(
			const std::vector<glm::vec3>& instance_origin,
			const std::vector<float>& instance_radius,
			const std::vector<glm::vec4>& instance_color,
			const float& pixel_separation,
			const float& pixel_width,
			const float& scale_height_count,
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

			//ATTRIBUTES
			glBindBuffer(GL_ARRAY_BUFFER, elementPositionBufferId);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elementPositions.size(), &elementPositions.front(), GL_STATIC_DRAW);
		    glEnableVertexAttribArray(elementPositionLocation);
            glVertexAttribPointer(elementPositionLocation, 3, GL_FLOAT, normalize, stride, offset);
		    glVertexAttribDivisor(elementPositionLocation,0);

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
	        glUniform1f       (pixelSeparationLocation,  pixel_separation);
	        glUniform1f       (pixelWidthLocation,       pixel_width);
	        glUniform1f       (scaleHeightCountLocation, scale_height_count);

			glDrawArraysInstanced(GL_TRIANGLES, /*array offset*/ 0, /*vertex count*/ elementPositions.size(), instance_origin.size());
		}

	};
}

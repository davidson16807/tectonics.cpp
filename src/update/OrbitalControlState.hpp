#pragma once

#include <cmath>          // log2

#include <algorithm>      // clamp

// glm libraries
#include <glm/vec4.hpp>   // *vec4
#include <glm/mat4x4.hpp> // *mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

namespace update 
{
	enum OrbitalControlDragState
	{
		released,
		zoom,
		pan,
		rotate
	};
	struct OrbitalControlState
	{
		// latitude/longitude of geographic coordinates indicating position over world
		glm::vec2 angular_position;
		// altitude/azimuth of horizontal coordinates indicating direction of viewer relative to "up" axis
		glm::vec2 angular_direction;
		// closest distance to origin
		float min_zoom_distance;
		// log2 of height above the min_zoom_distance
		float log2_height;
		// the last mouse button event recorded
		OrbitalControlDragState drag_state;

		OrbitalControlState(
			glm::vec2 angular_position = glm::vec2(0),
			glm::vec2 angular_direction = glm::vec2(0),
			float min_zoom_distance = 1.0, 
			float log2_height = 2
		):
			angular_position(angular_position),
			angular_direction(angular_direction),
			min_zoom_distance(min_zoom_distance),
			log2_height(log2_height)
		{}

		// DERIVED ATTRIBUTES, functions of the form: OrbitalControlState → T
		glm::mat4 get_view_matrix()
		{
			glm::mat4 I   = glm::mat4(1.0f);
			glm::mat4 out = glm::mat4(1.0f);
			out = glm::translate(I, glm::vec3(0,0,-(std::exp2(log2_height)+min_zoom_distance)));
			out = out * glm::rotate(I, angular_position.y-float(M_PI)/2.0f, glm::vec3(1,0,0));
			out = out * glm::rotate(I, angular_position.x, glm::vec3(0,0,1));
			out = glm::rotate(I, angular_direction.y, glm::vec3(1,0,0)) * out;
			out = glm::rotate(I, angular_direction.x, glm::vec3(0,1,0)) * out;
			return out;
		}

		static void zoom(const OrbitalControlState& state_in, float log_outward_motion, OrbitalControlState& state_out)
		{
			if (&state_in != &state_out) { state_out = state_in; }
			state_out.log2_height += log_outward_motion;
		}

		static void pan(const OrbitalControlState& state_in, glm::vec2 motion, OrbitalControlState& state_out)
		{
			if (&state_in != &state_out) { state_out = state_in; }
			motion = 0.2f * motion * std::exp2(state_in.log2_height)/state_in.min_zoom_distance;
			state_out.angular_position = state_in.angular_position + motion;
			state_out.angular_position.y = std::clamp(state_out.angular_position.y, -float(M_PI)/2.0f, float(M_PI)/2.0f);
		}
	};

}
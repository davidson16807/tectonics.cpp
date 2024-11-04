#pragma once

// glm libraries
#include <glm/mat4x4.hpp> // *mat4

namespace view 
{
	enum RenderPassType
	{
		background,
		solids,
		volumetrics,
		lens_effects,
	};
	enum ProjectionType
	{
		heads_up_display,
		perspective
	};
	struct ViewState
	{
		glm::mat4 view_matrix;
		glm::mat4 model_matrix;
		glm::mat4 projection_matrix;
		glm::vec2 resolution;
		float point_spread_function_pixel_standard_deviation;
		float exposure_intensity;
		float gamma;
		ProjectionType projection_type;
		RenderPassType render_pass;

		ViewState():
			view_matrix(1),
			model_matrix(1),
			projection_matrix(1),
			resolution(1),
			point_spread_function_pixel_standard_deviation(1.0),
			exposure_intensity(1.0),
			gamma(2.2),
			projection_type(ProjectionType::perspective),
			render_pass(RenderPassType::solids)
		{}
	};

}


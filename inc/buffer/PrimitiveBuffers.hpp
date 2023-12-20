#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace buffer {

	template<typename id, glm::qualifier Q=glm::defaultp>
	struct PrimitiveBuffers
	{

		template<typename T>
		constexpr inline id point_size(const T value) const
		{
			return 1;
		}

		template<typename T, typename Buffer>
		constexpr inline id storePoint(const T value, Buffer& output, const id buffer_start_id=0) const
		{
			output[buffer_start_id] = value;
			return buffer_start_id + point_size(value);
		}

		template<typename T>
		constexpr inline id triangle_size(const T CW, const T O, const T CCW) const
		{
			return 3;
		}

		template<typename T, typename Buffer>
		constexpr id storeTriangle(const T CW, const T O, const T CCW, Buffer& output, const id buffer_start_id=0) const
		{
			id buffer_id = buffer_start_id;
			buffer_id = storePoint(CW, output, buffer_id);
			buffer_id = storePoint(O, output, buffer_id);
			buffer_id = storePoint(CCW, output, buffer_id);
			return buffer_id;
		}

	};

}
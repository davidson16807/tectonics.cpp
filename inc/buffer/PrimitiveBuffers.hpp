#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace buffer {

	template<typename id, glm::qualifier Q=glm::defaultp>
	struct PrimitiveBuffers
	{

		constexpr inline id point_size(const float value) const
		{
			return 1;
		}

		template<int L, typename T>
		constexpr inline id point_size(const glm::vec<L,T,Q> value) const
		{
			return L;
		}

		template<typename Buffer>
		constexpr inline id storePoint(const float value, Buffer& output, const id buffer_start_id=0) const
		{
			output[buffer_start_id] = value;
			return buffer_start_id + point_size(value);
		}

		template<int L, typename T, typename Buffer>
		constexpr id storePoint(const glm::vec<L,T,Q> value, Buffer& output, const id buffer_start_id=0) const
		{
			id buffer_id = buffer_start_id;
			for (int i = 0; i < L; ++i)
			{
				output[buffer_id+i] = value[i];
			}
			return buffer_start_id + point_size(value);
		}

		constexpr inline id triangle_size(const float CW, const float O, const float CCW) const
		{
			return 3;
		}

		template<typename Buffer>
		constexpr id storeTriangle(const float CW, const float O, const float CCW, Buffer& output, const id buffer_start_id=0) const
		{
			id buffer_id = buffer_start_id;
			buffer_id = storePoint(CW, output, buffer_id);
			buffer_id = storePoint(O, output, buffer_id);
			buffer_id = storePoint(CCW, output, buffer_id);
			return buffer_id;
		}

		template<int L, typename T>
		constexpr inline id triangle_size(const glm::vec<L,T,Q> CW, const glm::vec<L,T,Q> O, const glm::vec<L,T,Q> CCW) const
		{
			return 3*L;
		}

		template<int L, typename T, typename Buffer>
		constexpr id storeTriangle(const glm::vec<L,T,Q> CW, const glm::vec<L,T,Q> O, const glm::vec<L,T,Q> CCW, Buffer& output, const id buffer_start_id=0) const
		{
			id buffer_id = buffer_start_id;
			buffer_id = storePoint(CW, output, buffer_id);
			buffer_id = storePoint(O, output, buffer_id);
			buffer_id = storePoint(CCW, output, buffer_id);
			return buffer_id;
		}

	};

}
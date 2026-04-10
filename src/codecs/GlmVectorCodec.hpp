#pragma once

#include <format>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>

#include <glm/vec.hpp>

namespace codecs {

	template <glm::length_t L, typename scalar, glm::qualifier Q = glm::defaultp>
	class GlmVectorCodec
	{

		using vec = glm::vec<L, scalar, Q>;

		using strings = std::vector<std::string>;

		static constexpr ScalarStringCodec<scalar> scalars = {};

	public:
		GlmVectorCodec() = default;

		vec decode(const strings& fields) const
		{
			if (fields.size() < std::size_t(L)) {
				throw std::invalid_argument(std::format(
					"A glm::vec codec requires at least {} fields", L
				));
			}

			vec result(scalar(0));
			for (glm::length_t i = 0; i < L; ++i) {
				result[i] = scalars.decode(fields[std::size_t(i)]);
			}
			return result;
		}

		void encode(const vec& value, strings& fields) const
		{
			fields.clear();
			fields.reserve(L);
			for (glm::length_t i = 0; i < L; ++i) {
				fields.push_back(scalars.encode(value[i]));
			}
		}
	};

}


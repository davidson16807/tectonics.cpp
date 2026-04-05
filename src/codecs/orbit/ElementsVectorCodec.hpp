#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <model/orbit/Elements.hpp>

namespace orbit {

	template <typename scalar>
	class ElementsVectorCodec
	{

		using strings = std::vector<std::string>;

	public:
		ElementsVectorCodec() = default;

		Elements<scalar> decode(const std::vector<std::string>& vector) const
		{
			if (vector.size() != 6) {
				throw std::invalid_argument("ElementsVectorCodec::decode requires exactly 6 fields");
			}

			return Elements<scalar>(
				scalar(std::stod(vector[0])),
				scalar(std::stod(vector[1])),
				scalar(std::stod(vector[2])),
				scalar(std::stod(vector[3])),
				scalar(std::stod(vector[4])),
				scalar(std::stod(vector[5]))
			);
		}

		void encode(const Elements<scalar>& elements, strings& vector) const
		{
			vector.push_back(std::to_string(elements.semi_major_axis));
			vector.push_back(std::to_string(elements.eccentricity));
			vector.push_back(std::to_string(elements.inclination));
			vector.push_back(std::to_string(elements.longitude_of_ascending_node));
			vector.push_back(std::to_string(elements.argument_of_periapsis));
			vector.push_back(std::to_string(elements.mean_anomaly));
		}
	};

}
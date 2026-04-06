#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>

#include <model/orbit/Elements.hpp>

namespace codecs {

	template <typename scalar>
	class ElementsVectorCodec
	{

		using strings = std::vector<std::string>;

	    static scalar decode(const std::string& s) {
	        std::istringstream iss(s);
	        scalar value;
	        iss >> value;
	        return value;
	    }

	    static std::string encode(const scalar& value) {
	        std::ostringstream oss;
	        oss << std::setprecision(std::numeric_limits<scalar>::max_digits10) << value;
	        return oss.str();
	    }

	public:
		ElementsVectorCodec(){};

		orbit::Elements<scalar> decode(const std::vector<std::string>& fields) const
		{
			if (fields.size() != 6) {
				throw std::invalid_argument("Exactly 6 fields must be supplied to create an Element");
			}

			return orbit::Elements<scalar>(
				decode(fields[0]),
				decode(fields[1]),
				decode(fields[2]),
				decode(fields[3]),
				decode(fields[4]),
				decode(fields[5])
			);
		}

		void encode(const orbit::Elements<scalar>& elements, strings& fields) const
		{
			fields.clear();
			fields.push_back(encode(elements.semi_major_axis));
			fields.push_back(encode(elements.eccentricity));
			fields.push_back(encode(elements.inclination));
			fields.push_back(encode(elements.longitude_of_ascending_node));
			fields.push_back(encode(elements.argument_of_periapsis));
			fields.push_back(encode(elements.mean_anomaly));
		}

	};

}
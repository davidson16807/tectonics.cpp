#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>

#include <unit/si.hpp>

#include <model/orbit/Elements.hpp>

namespace codecs {

	template <typename scalar>
	class ElementsVectorCodec
	{

		static constexpr scalar pi = 3.141592653589793238462643383279502884197;
		static constexpr scalar radians_per_degree = pi/180.0;
		static constexpr scalar s0   =   0.0;

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
			if (fields.size() < 1) {
				throw std::invalid_argument("An orbit must have at least a semi-major axis");
			}

			return orbit::Elements<scalar>(
				decode(fields[0]), // semi major axis
				fields.size() > 1? decode(fields[1])            : s0, // eccentricity
				fields.size() > 2? decode(fields[2])*si::degree : s0, // inclination
				fields.size() > 3? decode(fields[3])*si::degree : s0, // longitude of ascending node
				fields.size() > 4? decode(fields[4])*si::degree : s0, // argument of periapsis
				fields.size() > 5? decode(fields[5])*si::degree : s0  // mean anomaly
			);

		}

		void encode(const orbit::Elements<scalar>& elements, strings& fields) const
		{
			fields.clear();
			fields.push_back(encode(elements.semi_major_axis));
			fields.push_back(encode(elements.eccentricity));
			fields.push_back(encode(elements.inclination                 / si::degree));
			fields.push_back(encode(elements.longitude_of_ascending_node / si::degree));
			fields.push_back(encode(elements.argument_of_periapsis       / si::degree));
			fields.push_back(encode(elements.mean_anomaly                / si::degree));
		}

	};

}


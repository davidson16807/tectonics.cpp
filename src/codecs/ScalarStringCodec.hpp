#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>

namespace codecs {

	template <typename scalar>
	struct ScalarStringCodec
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

	};

}


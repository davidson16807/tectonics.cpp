#pragma once

namespace rock
{
	namespace formations {
		enum FormationType
		{
	        sediment,
	        sedimentary,
	        metasedimentary,
	        igneous,
	        metaigneous
		};

		const std::size_t count = 5;

		std::array<std::string, count> names{
	        "sediment",
	        "sedimentary",
	        "metasedimentary",
	        "igneous",
	        "metaigneous"
		};

	}


}

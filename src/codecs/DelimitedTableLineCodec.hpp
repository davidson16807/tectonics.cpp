#pragma once

#include <string>    // std::string
#include <vector>    // std::vector
#include <utility>   // std::move
#include <stdexcept> // std::invalid_argument

namespace codecs {

	class DelimitedTableLineCodec
	{

		std::string column_delimiter;

	public:
		explicit DelimitedTableLineCodec(
			std::string column_delimiter_ = "\t"
		) :
			column_delimiter(std::move(column_delimiter_))
		{
		    if (column_delimiter.empty()) {
       			throw std::invalid_argument("column delimiter must not be empty");
    		}
		}

		void decode(const std::string& line, std::vector<std::string>& vector) const
		{
			vector.clear();

			std::string trimmed = line;
			while (!trimmed.empty() && (trimmed.back() == '\n' || trimmed.back() == '\r')) {
				trimmed.pop_back();
			}

			std::size_t start = 0;
			while (true) {
				const std::size_t pos = trimmed.find(column_delimiter, start);
				if (pos == std::string::npos) {
					vector.push_back(trimmed.substr(start));
					break;
				}
				vector.push_back(trimmed.substr(start, pos - start));
				start = pos + column_delimiter.size();
			}
		}

		[[nodiscard]]
		std::string encode(const std::vector<std::string>& cells) const
		{
			std::string result;

			for (std::size_t i = 0; i < cells.size(); ++i) {
				if (i != 0) {
					result += column_delimiter;
				}
				result += cells[i];
			}

			return result;
		}

	};

}


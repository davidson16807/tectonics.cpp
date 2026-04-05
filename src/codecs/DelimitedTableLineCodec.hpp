#pragma once

#include <string>
#include <vector>

namespace orbit {

	class DelimitedTableLineCodec
	{
	public:
		explicit DelimitedTableLineCodec(
			std::string row_delimiter = "\n",
			std::string column_delimiter = "\t"
		) noexcept :
			row_delimiter_(std::move(row_delimiter)),
			column_delimiter_(std::move(column_delimiter))
		{}

		void decode(const std::string& line, std::vector<std::string>& vector) const
		{
			vector.clear();

			std::string trimmed = line;
			while (!trimmed.empty() && (trimmed.back() == '\n' || trimmed.back() == '\r')) {
				trimmed.pop_back();
			}

			std::size_t start = 0;
			while (true) {
				const std::size_t pos = trimmed.find(column_delimiter_, start);
				if (pos == std::string::npos) {
					vector.push_back(trimmed.substr(start));
					break;
				}
				vector.push_back(trimmed.substr(start, pos - start));
				start = pos + column_delimiter_.size();
			}
		}

		[[nodiscard]]
		std::string encode(const std::vector<std::string>& cells) const
		{
			std::string result;

			for (std::size_t i = 0; i < cells.size(); ++i) {
				if (i != 0) {
					result += column_delimiter_;
				}
				result += cells[i];
			}

			result += row_delimiter_;
			return result;
		}

	private:
		std::string row_delimiter_;
		std::string column_delimiter_;
	};

}


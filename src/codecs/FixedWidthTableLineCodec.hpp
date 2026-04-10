#pragma once

#include <string>    // std::string
#include <vector>    // std::vector
#include <utility>   // std::move
#include <stdexcept> // std::invalid_argument
#include <cstddef>   // std::size_t

namespace codecs {

	class FixedWidthTableLineCodec
	{
		std::vector<std::size_t> column_widths;
		std::size_t padding_width;
		char padding_character;

	public:
		explicit FixedWidthTableLineCodec(
			std::vector<std::size_t> column_widths_,
			std::size_t padding_width_ = 1,
			char padding_character_ = ' '
		) :
			column_widths(std::move(column_widths_)),
			padding_width(padding_width_),
			padding_character(padding_character_)
		{
			if (column_widths.empty()) {
				throw std::invalid_argument("column widths must not be empty");
			}

			for (const std::size_t width : column_widths) {
				if (width == 0) {
					throw std::invalid_argument("column widths must all be greater than zero");
				}
			}
		}

		void decode(const std::string& line, std::vector<std::string>& vector) const
		{
			vector.clear();
			std::size_t start = 0;
			for (std::size_t i = 0; i < column_widths.size(); ++i) {
				const std::size_t width = column_widths[i];

				if (start >= line.size()) {
					vector.emplace_back();
				}
				else {
					vector.push_back(line.substr(start, width));
				}

				start += width;

				if (i < column_widths.size()-1) {
					start += padding_width;
				}
			}
		}

		[[nodiscard]]
		std::string encode(const std::vector<std::string>& cells) const
		{
			if (cells.size() != column_widths.size()) {
				throw std::invalid_argument("number of cells does not match number of column widths");
			}

			std::string result;

			for (std::size_t i = 0; i < cells.size(); ++i) {
				const std::string& cell = cells[i];
				const std::size_t width = column_widths[i];

				if (cell.size() > width) {
					throw std::invalid_argument("cell content exceeds fixed column width");
				}

				result += cell;
				result.append(width - cell.size(), padding_character);

				if (i < cells.size()-1) {
					result.append(padding_width, padding_character);
				}
			}

			return result;
		}
	};

}
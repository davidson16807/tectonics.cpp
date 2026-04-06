#pragma once

#include <fstream>   // std::ofstream, std::ifstream
#include <string>    // std:;string
#include <vector>    // std::vector
#include <utility>   // std::move
#include <stdexcept> // std::runtime_error

namespace files {

	template<typename TableLineCodec>
	class VectorTableFiles
	{

		const TableLineCodec codec;

	public:
		VectorTableFiles(TableLineCodec codec): codec(codec) {}

		using Table = std::vector<std::vector<std::string>>;

		void read(
			const std::string& filename,
			Table& table
		) const
		{
			table.clear();
			std::ifstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}
			std::string line;
			while (std::getline(file, line)) {
				std::vector<std::string> cells;
				codec.decode(line, cells);
				table.push_back(std::move(cells));
			}
		}

		void write(
			const std::string& filename,
			const Table& table
		) const
		{
			std::ofstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}
			for (std::size_t i = 0; i < table.size(); ++i) {
				file << codec.encode(table[i]) << "\n";
			}
		}

	};

}


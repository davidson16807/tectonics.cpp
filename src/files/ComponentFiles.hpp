#pragma once

#include <cstddef>       // std::size_t

#include <format>        // std::format
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
#include <stdexcept>     // std::runtime_error
#include <utility>       // std::move
#include <fstream>       // std::ofstream, std::ifstream

namespace files {

	template <typename scalar, typename TableLineCodec, typename ComponentVectorCodec>
	class ComponentFiles
	{

		const TableLineCodec table_line_codec;
		const ComponentVectorCodec component_vector_codec;
		std::size_t header_count;

	public:

		ComponentFiles(
			const TableLineCodec table_line_codec,
			const ComponentVectorCodec component_vector_codec,
			const std::size_t header_count = 1
		) noexcept :
			table_line_codec(std::move(table_line_codec)),
			component_vector_codec(std::move(component_vector_codec)),
			header_count(header_count)
		{}

		template<typename id, typename Components>
		void read(
			const std::string& filename,
			const std::unordered_map<std::string, id>& id_for_label,
			Components& components
		) const {

			std::ifstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}

			std::string line;
			for (std::size_t row_id = 0; row_id < header_count; ++row_id)
			{
			    if (!std::getline(file, line)) {
			        throw std::runtime_error("File ended before the header: " + filename);
			    }
			}

			std::size_t row_id(0);
			std::size_t record_id(0);
			std::vector<std::string> row;
			// components.clear();
			while (std::getline(file, line)) {

				row_id = record_id + header_count;

				table_line_codec.decode(line, row);

				if (row.empty()) {
				    throw std::runtime_error(std::format("Row {} of file {} is empty", row_id, filename));
				}

				const std::string& label = row[0];
			    auto item = id_for_label.find(label);
		        if (item != id_for_label.end()) {
					std::vector<std::string> subrow(row.begin() + 1, row.end());
					components.add(item->second, component_vector_codec.decode(subrow));
			    }

				++record_id;

			}

		}

		template<typename Components>
		void write(
			const std::string& filename,
			const std::vector<std::string>& label_for_id,
			const Components& components,
			const std::string header=""
		) const {

			std::ofstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}

			file << header;

			std::vector<std::string> row;
			std::vector<std::string> subrow;

			for (std::size_t record_id = 0; record_id < components.entity_count(); ++record_id) {

				if (components.has(record_id))
				{

					row.clear();
					subrow.clear();

					row.push_back(label_for_id[record_id]);
					component_vector_codec.encode(components[record_id], subrow);
					row.insert(row.end(), subrow.begin(), subrow.end());

					file << table_line_codec.encode(row) << "\n";

				}

			}

			file.flush();
			if (!file) {
			    throw std::runtime_error("Failed to finalize file: " + filename);
			}

		}

	};

}


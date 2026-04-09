#pragma once

#include <cstddef>       // std::size_t

#include <format>        // std::format
#include <string>        // std:;string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
#include <stdexcept>     // std::runtime_error
#include <utility>       // std::move
#include <fstream>       // std::ofstream, std::ifstream

#include <glm/vec4.hpp>

#include <unit/si.hpp>
#include <model/orbit/Elements.hpp>
#include <codecs/orbit/ElementsVectorCodec.hpp>

namespace files {

	template <typename id, typename scalar, typename TableLineCodec>
	class BodyFiles
	{

		using mass = si::mass<scalar>;

		using vec4 = glm::vec4;

		using Row = std::vector<std::string>;

		static constexpr mass kg = si::kilogram;

		const TableLineCodec table_line_codec;
		const codecs::ElementsVectorCodec<scalar> elements_vector_codec;
		std::size_t header_count;

	public:

		BodyFiles(
			const TableLineCodec table_line_codec = {},
			const codecs::ElementsVectorCodec<scalar> elements_vector_codec = {},
			const std::size_t header_count = 1
		) noexcept :
			table_line_codec(std::move(table_line_codec)),
			elements_vector_codec(std::move(elements_vector_codec)),
			header_count(header_count)
		{}

		void read(
			const std::string& filename,
			std::vector<orbit::Elements<scalar>>& elements,
			std::vector<id>& parent_ids,
			std::vector<mass>& masses,
			std::vector<std::string>& label_for_id,
			std::vector<vec4>& colors
		) const
		{

			std::ifstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}

			std::unordered_map<std::string, id> id_for_label;
			label_for_id.clear();
			parent_ids.clear();
			masses.clear();
			colors.clear();
			elements.clear();

			std::string line;
			for (std::size_t row_id = 0; row_id < header_count; ++row_id)
			{
				std::getline(file, line);
			}

			std::size_t row_id(0);
			std::size_t record_id(0);
			std::vector<std::string> row;
			while (std::getline(file, line)) {
				row_id = record_id + header_count;

				std::vector<std::string> cells;

				table_line_codec.decode(line, row);

				if (row.size() < 1) {
					throw std::runtime_error(
						std::format("Body rows require at least 8 columns, {} were found, row {}", 
							row.size(), row_id)
					);
				} else if (row.size() < 8) { 
					throw std::runtime_error(
						std::format("Body rows require at least 8 columns, {} were found, row {}, label {} ", 
							row.size(), row_id, row[0])
					);
				}

				const std::string& label = row[0];
				label_for_id.push_back(row[0]);
				id_for_label[label] = id(record_id);

				// next, build `parent_ids` output
				const auto it = id_for_label.find(row[1]);
				if (it == id_for_label.end()) {
					throw std::runtime_error(std::format("Encountered unknown parent label, row {}, label {}", row_id, row[1]));
				}

				parent_ids.push_back(it->second);
				try
				{
					colors.push_back(vec4(
						scalar(std::stod(row[2])), 
						scalar(std::stod(row[3])), 
						scalar(std::stod(row[4])), 
						scalar(std::stod(row[5]))
					));
				}
				catch (const std::invalid_argument& e)
				{
					throw std::runtime_error(std::format("Error parsing number in color value, row {}, values {},{},{},{}", row_id, row[3], row[4], row[5], row[6]));
				}

				try
				{
					masses.push_back(scalar(row[6].size() < 1? 0.0: std::stod(row[6]))*kg);
				}
				catch (const std::invalid_argument& e)
				{
					throw std::runtime_error(std::format("Error parsing mass, row {}, value {}", row_id, row[6]));
				}

				std::vector<std::string> element_vector(row.begin() + 7, row.end());
				elements.push_back(elements_vector_codec.decode(element_vector));

				++record_id;
			}

		}

		void write(
			const std::string& filename,
			const std::vector<orbit::Elements<scalar>>& elements,
			const std::vector<id>& parent_ids,
			const std::vector<mass>& masses,
			const std::vector<std::string>& label_for_id,
			const std::vector<vec4>& colors
		) const {

			if (elements.size() != parent_ids.size() || 
				elements.size() != label_for_id.size() || 
				elements.size() != masses.size() || 
				elements.size() != colors.size()) {
				throw std::invalid_argument("Vectors must be of equal length");
			}

			std::vector<std::string> element_vector;

			std::ofstream file(filename);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}

			file << "label\tparent\tr\tg\tb\ta\tmass (kg)\tsma (m)\tecc (m)\tinc (°)\tlan (°)\taop (°)\tma (°)\n";

			for (std::size_t record_id = 0; record_id < elements.size(); ++record_id) {
				std::vector<std::string> row_vector;

				const id parent_id = parent_ids[record_id];
				if (std::size_t(parent_id) >= label_for_id.size()) {
					throw std::out_of_range("parent_id out of range");
				}

				row_vector.clear();
				element_vector.clear();

				row_vector.push_back(label_for_id[record_id]);
				row_vector.push_back(label_for_id[std::size_t(parent_id)]);
				row_vector.push_back(std::to_string(colors[record_id].r));
				row_vector.push_back(std::to_string(colors[record_id].g));
				row_vector.push_back(std::to_string(colors[record_id].b));
				row_vector.push_back(std::to_string(colors[record_id].a));
				row_vector.push_back(std::to_string(masses[record_id]/kg));

				elements_vector_codec.encode(elements[record_id], element_vector);
				row_vector.insert(row_vector.end(), element_vector.begin(), element_vector.end());

				file << table_line_codec.encode(row_vector) << "\n";
			}

			file.flush();
			if (!file) {
			    throw std::runtime_error("Failed to finalize file: " + filename);
			}

		}

	};

}


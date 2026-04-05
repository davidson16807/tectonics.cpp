#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "Elements.hpp"
#include "ElementsVectorCodec.hpp"

namespace orbit {

	template <typename scalar, typename id, typename TableLineCodec>
	class ElementsFiles
	{
	public:
		ElementsFiles(
			TableLineCodec delimited_table_line_codec = {},
			ElementsVectorCodec<scalar> elements_vector_codec = {}
		) noexcept :
			delimited_table_line_codec_(std::move(delimited_table_line_codec)),
			elements_vector_codec_(std::move(elements_vector_codec))
		{}

		void read(
			const std::string& filename,
			std::vector<Elements<scalar>>& elements,
			std::vector<id>& parent_ids,
			std::vector<std::string>& label_for_id
		) const
		{
			std::vector<std::vector<std::string>> scratch;

			{
				std::ifstream file(filename);
				if (!file) {
					throw std::runtime_error("ElementsFiles::read failed to open file: " + filename);
				}

				std::string line;
				while (std::getline(file, line)) {
					std::vector<std::string> cells;
					delimited_table_line_codec_.decode(line, cells);
					scratch.push_back(std::move(cells));
				}
			}

			std::unordered_map<std::string, id> id_for_label;

			label_for_id.clear();
			label_for_id.reserve(scratch.size());

			for (id i = 0; i < static_cast<id>(scratch.size()); ++i) {
				if (scratch[static_cast<std::size_t>(i)].size() < 2) {
					throw std::runtime_error("ElementsFiles::read encountered row with fewer than 2 columns");
				}

				const std::string& label = scratch[static_cast<std::size_t>(i)][0];
				label_for_id.push_back(label);
				id_for_label[label] = i;
			}

			parent_ids.clear();
			parent_ids.reserve(scratch.size());

			for (const auto& row : scratch) {
				const auto it = id_for_label.find(row[1]);
				if (it == id_for_label.end()) {
					throw std::runtime_error("ElementsFiles::read encountered unknown parent label: " + row[1]);
				}
				parent_ids.push_back(it->second);
			}

			elements.clear();
			elements.reserve(scratch.size());

			for (const auto& row : scratch) {
				if (row.size() < 8) {
					throw std::runtime_error("ElementsFiles::read expects at least 8 columns per row");
				}

				std::vector<std::string> encoded_elements(row.begin() + 2, row.begin() + 8);
				elements.push_back(elements_vector_codec_.decode(encoded_elements));
			}
		}

		void write(
			const std::string& filename,
			const std::vector<Elements<scalar>>& elements,
			const std::vector<id>& parent_ids,
			const std::vector<std::string>& label_for_id
		) const
		{
			if (elements.size() != parent_ids.size() || elements.size() != label_for_id.size()) {
				throw std::invalid_argument("ElementsFiles::write expects vectors of equal length");
			}

			std::ofstream file(filename);
			if (!file) {
				throw std::runtime_error("ElementsFiles::write failed to open file: " + filename);
			}

			std::vector<std::string> vector;
			std::vector<std::string> subvector;

			for (std::size_t i = 0; i < elements.size(); ++i) {
				const id parent_id = parent_ids[i];
				if (static_cast<std::size_t>(parent_id) >= label_for_id.size()) {
					throw std::out_of_range("ElementsFiles::write parent_id out of range");
				}

				vector.clear();
				subvector.clear();

				vector.push_back(label_for_id[i]);
				vector.push_back(label_for_id[static_cast<std::size_t>(parent_id)]);

				elements_vector_codec_.encode(elements[i], subvector);
				vector.insert(vector.end(), subvector.begin(), subvector.end());

				file << delimited_table_line_codec_.encode(vector);
			}
		}

	private:
		DelimitedTableLineCodec delimited_table_line_codec_;
		ElementsVectorCodec<scalar> elements_vector_codec_;
	};

}
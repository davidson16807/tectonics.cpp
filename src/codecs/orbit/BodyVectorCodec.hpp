#pragma once

#include <cstddef>       // std::size_t

#include <string>
#include <vector>
#include <unordered_map> // 
#include <stdexcept>     // std::runtime_error
#include <utility>       // std::move

#include <glm/vec4.hpp>

#include <unit/si.hpp>
#include <model/orbit/Elements.hpp>
#include <codecs/orbit/ElementsVectorCodec.hpp>

namespace codecs {

	template <typename id, typename scalar, typename TableLineCodec>
	class BodyVectorCodec
	{

		using mass = si::mass<scalar>;

		using vec4 = glm::vec4;

		using Table = std::vector<std::vector<std::string>>;

		static constexpr mass kg = si::kilogram;

		ElementsVectorCodec<scalar> elements_vector_codec;

	public:
		BodyVectorCodec(
			ElementsVectorCodec<scalar> elements_vector_codec = {}
		) noexcept :
			elements_vector_codec(std::move(elements_vector_codec))
		{}

		void decode(
			const Table& table,
			std::vector<orbit::Elements<scalar>>& elements,
			std::vector<id>& parent_ids,
			std::vector<mass>& masses,
			std::vector<std::string>& label_for_id,
			std::vector<vec4>& colors
		) const
		{

			// first, build lookups for the human-readable primary keys, the "labels"
			std::unordered_map<std::string, id> id_for_label;
			label_for_id.clear();
			label_for_id.reserve(table.size());
			for (std::size_t i = 0; i < table.size(); ++i) {
				if (table[i].size() != 13) { 
					throw std::runtime_error("Body rows require 13 columns.");
				}
				const std::string& label = table[i][0];
				label_for_id.push_back(table[i][0]);
				id_for_label[label] = id(i);
			}

			// next, build `parent_ids` output
			parent_ids.clear();
			parent_ids.reserve(table.size());
			masses.clear();
			masses.reserve(table.size());
			colors.clear();
			colors.reserve(table.size());
			elements.clear();
			elements.reserve(table.size());
			for (const auto& row : table) {
				const auto it = id_for_label.find(row[1]);
				if (it == id_for_label.end()) {
					throw std::runtime_error("Encountered unknown parent label: " + row[1]);
				}
				std::vector<std::string> element_vector(row.begin() + 7, row.end());
				parent_ids.push_back(it->second);
				masses.push_back(
					scalar(std::stod(row[2]))*kg
				);
				colors.push_back(vec4(
					scalar(std::stod(row[3])), 
					scalar(std::stod(row[4])), 
					scalar(std::stod(row[5])), 
					scalar(std::stod(row[6]))
				));
				elements.push_back(elements_vector_codec.decode(element_vector));
			}

		}

		void encode(
			Table& table,
			const std::vector<orbit::Elements<scalar>>& elements,
			const std::vector<id>& parent_ids,
			const std::vector<mass>& masses,
			const std::vector<std::string>& label_for_id,
			const std::vector<vec4>& colors
		) const
		{

			if (elements.size() != parent_ids.size() || 
				elements.size() != label_for_id.size() || 
				elements.size() != masses.size() || 
				elements.size() != colors.size()) {
				throw std::invalid_argument("Vectors must be of equal length");
			}

			std::vector<std::string> row_vector;
			std::vector<std::string> element_vector;

			table.clear();
			table.reserve(elements.size());
			for (std::size_t i = 0; i < elements.size(); ++i) {

				const id parent_id = parent_ids[i];
				if (std::size_t(parent_id) >= label_for_id.size()) {
					throw std::out_of_range("parent_id out of range");
				}

				row_vector.clear();
				element_vector.clear();

				row_vector.push_back(label_for_id[i]);
				row_vector.push_back(label_for_id[std::size_t(parent_id)]);
				row_vector.push_back(std::to_string(masses[i]/kg));
				row_vector.push_back(std::to_string(colors[i].r));
				row_vector.push_back(std::to_string(colors[i].g));
				row_vector.push_back(std::to_string(colors[i].b));
				row_vector.push_back(std::to_string(colors[i].a));

				elements_vector_codec.encode(elements[i], element_vector);
				row_vector.insert(row_vector.end(), element_vector.begin(), element_vector.end());

				table.push_back(row_vector);

			}
		}

	};

}


#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/vector_relational.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>

#include <grid/cartesian/UnboundedIndexing.hpp>

namespace field
{


	/*
	Given a `Noise` instance: ℕ→ℝ, `MosaicNoise` maps ℝᴺ→ℤᴺ→ℕ→ℝ,
	`MosaicNoise` represents what is known as "square noise" or "mosaic noise"
	where a space is broken down into cells, and all points within a cell assume a unique random value.
	*/
	template<typename Noise, typename Indexing>
	class MosaicNoise
	{
	public:
		Noise noise;
		Indexing indexing;

        constexpr MosaicNoise(const Noise noise, const Indexing indexing) :
        	noise(noise),
        	indexing(indexing)
        {
        }

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		MosaicNoise(const MosaicNoise& mosaic_noise):
			noise(mosaic_noise.noise),
			indexing(mosaic_noise.indexing)
		{}

		using value_type = typename Noise::value_type;

		template<typename T>
		auto operator()(const T V) const {
		    return noise(indexing.memory_id(V));
		}

	};

	template<typename Noise>
	constexpr inline auto mosaic_noise(const Noise& noise)
	{
		return MosaicNoise(noise, cartesian::UnboundedIndexing<int>());
	}

	template<typename Noise, typename Indexing>
	constexpr inline auto mosaic_noise(const Noise& noise, const Indexing& indexing)
	{
		return MosaicNoise<Noise,Indexing>(noise, indexing);
	}

}


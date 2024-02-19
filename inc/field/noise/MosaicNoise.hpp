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

	/*
	`VectorMosaicOps` considers operations on ℝ³ that are required for other kinds of noise functions 
	that depend on `MosaicNoise`, such as `ValueNoise`, `PerlinNoise`, etc.
	It is analogous to other possible classes that can be 
	used to extend these noise functions to operate on other domains.
	As such, it can be considered an implementation of the adapter pattern for vectors.
	*/
	template<typename id, typename scalar>
	struct VectorMosaicOps
	{
		template<int L, glm::qualifier Q>
		inline glm::vec<L,id,Q> floor(const glm::vec<L,scalar,Q> grid_id) const
		{
			return glm::vec<L,id,Q>(glm::floor(grid_id));
		}
		template<int L, glm::qualifier Q>
		inline glm::vec<L,scalar,Q> fract(const glm::vec<L,scalar,Q> grid_id) const
		{
			return glm::fract(grid_id);
		}
		template<int L, glm::qualifier Q>
		inline auto add(const glm::vec<L,id,Q> grid_id, glm::vec<L,scalar,Q> offset) const
		{
			return glm::vec<L,scalar,Q>(grid_id)+glm::vec<L,scalar,Q>(offset);
		}
		template<int L, glm::qualifier Q>
		inline auto add(const glm::vec<L,id,Q> grid_id, glm::vec<L,id,Q> offset) const
		{
			return grid_id+offset;
		}
		template<int L, glm::qualifier Q>
		inline scalar distance(const glm::vec<L,scalar,Q> grid_id1, glm::vec<L,scalar,Q> grid_id2) const
		{
			return glm::distance(grid_id1, grid_id2);
		}
	};

	template<typename id, typename scalar>
	constexpr inline auto vector_mosaic_ops()
	{
		return VectorMosaicOps<id,scalar>();
	}

}


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

	// declaring private namespace since we haven't yet gathered enough functions like this to find a public namespace for them
	namespace {

		template<glm::qualifier Q>
		int hash(glm::vec<2,int,Q> V){
			using ivec2 = glm::vec<2,int,Q>;
			using bvec2 = glm::vec<2,bool,Q>;
			// hash vectors using the dovetail technique
			bvec2 U(glm::greaterThan(V,ivec2(0)));
			ivec2 W(glm::abs(V));
			int abs_hash = (W.x+W.y+1) * ((W.x+W.y)/2) + W.y;
			int sign_hash = (U.y<<1) + U.x;
			return 4*abs_hash + sign_hash;
		}

		template<glm::qualifier Q>
		int hash(glm::vec<3,int,Q> V){
			using ivec2 = glm::vec<2,int,Q>;
			return hash(ivec2(
					hash(V.xy()), V.z));
		}

		template<glm::qualifier Q>
		int hash(glm::vec<4,int,Q> V){
			using ivec2 = glm::vec<2,int,Q>;
			return hash(ivec2(
					hash(V.xyx()), V.w));
		}

	}

	/*
	Given a `Noise` instance: ℕ→ℝ, `SquareNoise` maps ℝᴺ→ℤᴺ→ℕ→ℝ,
	`SquareNoise` represents what is known as "square noise" or "mosaic noise"
	where a space is broken down into cells, and all points within a cell assume a unique random value.
	*/
	template<typename Noise, typename Indexing=cartesian::UnboundedIndexing<int>>
	class SquareNoise
	{
	public:
		Noise noise;
		Indexing indexing;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit SquareNoise(const Noise& noise): noise(noise) {}
		SquareNoise(const SquareNoise& square_noise): noise(square_noise.noise) {}

		using value_type = typename Noise::value_type;

		template<int N, typename T, glm::qualifier Q>
		auto operator()(const glm::vec<N,T,Q> V) const {
		    return noise(indexing.memory_id(glm::vec<N,int,Q>(V)));
		}

	};

	template<typename Noise>
	constexpr inline auto square_noise(const Noise& noise)
	{
		return SquareNoise<Noise>(noise);
	}

}


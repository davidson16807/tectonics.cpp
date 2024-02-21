#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace dymaxion
{

	template<typename id, typename scalar> 
	struct Noise 
	{ 
		Grid<id,scalar> grid; 
		constexpr inline explicit Noise(const Grid<id,scalar> grid): grid(grid) {} 
	    using size_type = std::size_t; 
		using value_type = TYPE; 
		constexpr inline size_type size() const { return indexing.vertex_count; } 
		constexpr inline value_type operator()(const size_type memory_id ) const { return call(memory_id); } 
		constexpr inline value_type operator[](const size_type memory_id ) const { return call(memory_id); } 
		constexpr value_type call(const size_type memory_id ) const { 
			auto grid_id = indexing.grid_id(memory_id);
			return noise(grid_id);
		} 
	};
	template<typename id, typename scalar>
	constexpr inline Noise<id,scalar> LOWER(const Grid<id,scalar> grid)
	{
		return Noise<id,scalar>(grid);
	}

}



/*
namespace dymaxion
{


	template<typename Noise, typename Indexing>
	class FractalBrownianNoise
	{
	public:
		Noise noise;
		Indexing indexing;

        constexpr FractalBrownianNoise(const Noise noise, const Indexing indexing) :
        	noise(noise),
        	indexing(indexing)
        {
        }

		FractalBrownianNoise(const FractalBrownianNoise& mosaic_noise):
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
		return FractalBrownianNoise(noise, cartesian::UnboundedIndexing<int>());
	}

	template<typename Noise, typename Indexing>
	constexpr inline auto mosaic_noise(const Noise& noise, const Indexing& indexing)
	{
		return FractalBrownianNoise<Noise,Indexing>(noise, indexing);
	}

}

	*/

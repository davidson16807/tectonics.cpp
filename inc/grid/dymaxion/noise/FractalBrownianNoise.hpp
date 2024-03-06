#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace dymaxion
{

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	class FractalBrownianNoise
	{

        using vec2 = glm::vec<2,scalar,precision>;
        using ivec2 = glm::vec<2,id,precision>;

        using point = Point<id,scalar,precision>;
        using ipoint = Point<id,id,precision>;

	public:
		Noise noise;
		scalar radius;
		id vertices_per_square_side;

        constexpr FractalBrownianNoise(
    		const Noise noise, 
    		const scalar radius, 
    		const id vertices_per_square_side
        ) :
        	noise(noise),
        	radius(radius),
        	vertices_per_square_side(vertices_per_square_side)
        {
        }

		FractalBrownianNoise(const FractalBrownianNoise& noise):
			noise(noise.noise),
        	radius(noise.radius),
			vertices_per_square_side(noise.vertices_per_square_side)
		{}

		using value_type = scalar;

		template<typename tpoint>
		scalar operator()(const tpoint V) const {

		    // return dymaxion::mosaic_ops<id,scalar>(
		    //         dymaxion::Voronoi<id,scalar>(radius, vertices_per_square_side/downsample)
		    //     ).fract(point(V.square_id, (vec2(V.square_position)/scalar(downsample)))).x;

			const id downsample(2);
			return field::value_noise<2,scalar>(
			// return field::perlin_noise<2,scalar>(
		        field::mosaic_noise(noise,
		            dymaxion::Indexing<id,scalar>(vertices_per_square_side/downsample)
		        ),
		        dymaxion::mosaic_ops<id,scalar>(
		            dymaxion::Voronoi<id,scalar>(radius, vertices_per_square_side/downsample)
		        )
		    )(point(V.square_id, ((vec2(V.square_position))/scalar(downsample))));
		}

	};

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	constexpr inline auto fractal_brownian_noise(
		const Noise noise, 
		const scalar radius, 
		const id vertices_per_square_side
	) {
		return FractalBrownianNoise<id,scalar,Noise,precision>(noise, radius, vertices_per_square_side);
	}

}


#pragma once

// C libraries

// std libraries
#include <iostream>

// 3rd party libraries

// in-house libraries

namespace field
{

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	class FractalBrownianNoise
	{

	public:
		const Noise noise;
		const id octave_count;
		const scalar amplitude_scale_per_frequency_doubling;
		const scalar coarsest_frequency;

        constexpr FractalBrownianNoise(
    		const Noise noise, 
			const scalar octave_count, 
			const scalar amplitude_scale_per_frequency_doubling,
			const scalar coarsest_frequency
        ) :
        	noise(noise),
        	octave_count(octave_count),
        	amplitude_scale_per_frequency_doubling(amplitude_scale_per_frequency_doubling),
        	coarsest_frequency(coarsest_frequency)
        {
        }

		FractalBrownianNoise(
			const Noise& noise
		):
			noise(noise.noise),
			octave_count(noise.octave_count),
			amplitude_scale_per_frequency_doubling(noise.amplitude_scale_per_frequency_doubling),
			coarsest_frequency(noise.coarsest_frequency)
		{}

		using value_type = scalar;

		template<typename tpoint>
		auto operator()(const tpoint V) const {
		    tpoint position(coarsest_frequency*V);
		    scalar amplitude(1);
			auto output = noise(position);
			for (int i = 0; i < octave_count; ++i)
			{
		        position  *= scalar(2);
		        amplitude *= amplitude_scale_per_frequency_doubling;
		        output += amplitude*noise(position);
			}
		    return output;
		}

	};

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	constexpr inline auto fractal_brownian_noise(
		const Noise noise, 
		const id octave_count, 
		const scalar amplitude_scale_per_frequency_doubling,
		const scalar coarsest_frequency
	) {
		return FractalBrownianNoise<id,scalar,Noise,precision>(noise, octave_count, amplitude_scale_per_frequency_doubling, coarsest_frequency);
	}

}


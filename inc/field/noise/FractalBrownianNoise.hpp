#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace field
{

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	class FractalBrownianNoise
	{

        using vec2 = glm::vec<2,scalar,precision>;

	public:
		Noise noise;
		id octave_count;
		scalar amplitude_scale_per_frequency_doubling;

        constexpr FractalBrownianNoise(
    		const Noise noise, 
			const scalar octave_count, 
			const scalar amplitude_scale_per_frequency_doubling
        ) :
        	noise(noise),
        	octave_count(octave_count),
        	amplitude_scale_per_frequency_doubling(amplitude_scale_per_frequency_doubling)
        {
        }

		FractalBrownianNoise(
			const Noise& noise
		):
			noise(noise.noise),
			octave_count(noise.octave_count),
			amplitude_scale_per_frequency_doubling(noise.amplitude_scale_per_frequency_doubling)
		{}

		using value_type = scalar;

		template<typename tpoint>
		auto operator()(const tpoint V) const {
		    scalar frequency(1);
		    scalar amplitude(1);
			auto output = noise(V);
			for (int i = 0; i < octave_count; ++i)
			{
		        frequency *= scalar(2);
		        amplitude *= amplitude_scale_per_frequency_doubling;
		        output += amplitude*noise(frequency*V);
			}
		    return output;
		}

	};

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	constexpr inline auto fractal_brownian_noise(
		const Noise noise, 
		const id octave_count, 
		const scalar amplitude_scale_per_frequency_doubling
	) {
		return FractalBrownianNoise<id,scalar,Noise,precision>(noise, octave_count, amplitude_scale_per_frequency_doubling);
	}

}


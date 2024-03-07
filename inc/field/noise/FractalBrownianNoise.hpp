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
		scalar amplitude_factor_per_octave;

        constexpr FractalBrownianNoise(
    		const Noise noise, 
			const scalar octave_count, 
			const scalar amplitude_factor_per_octave
        ) :
        	noise(noise),
        	octave_count(octave_count),
        	amplitude_factor_per_octave(amplitude_factor_per_octave)
        {
        }

		FractalBrownianNoise(
			const Noise& noise
		):
			noise(noise.noise),
			octave_count(noise.octave_count),
			amplitude_factor_per_octave(noise.amplitude_factor_per_octave)
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
		        amplitude *= amplitude_factor_per_octave;
		        output += amplitude*noise(frequency*V);
			}
		    return output;
		}

	};

	template<typename id, typename scalar, typename Noise, glm::qualifier precision=glm::defaultp>
	constexpr inline auto fractal_brownian_noise(
		const Noise noise, 
		const scalar octave_count, 
		const scalar amplitude_factor_per_octave
	) {
		return FractalBrownianNoise<id,scalar,Noise,precision>(noise, octave_count, amplitude_factor_per_octave);
	}

}


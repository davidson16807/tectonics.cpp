#pragma once

// C libraries
#include <cmath>

// std libraries

// 3rd party libraries

// in house libraries
#include <math/analytic/Gaussian.hpp>
#include <math/analytic/Error.hpp>

#include <index/procedural/noise/UnitIntervalNoise.hpp> // UnitIntervalNoise
#include <index/procedural/noise/GaussianNoise.hpp>

#include <field/noise/ValueNoise.hpp>               // ValueNoise
#include <field/noise/MosaicNoise.hpp>              // MosaicNoise
#include <field/Compose.hpp>                        // Compose
#include <field/noise/MosaicOps.hpp>                // field::VectorMosaicOps
#include <field/noise/FractalBrownianNoise.hpp>     // dymaxion::FractalBrownianNoise

namespace field
{

	/*
	`ranked_fractal_brownian_noise` (abbreviated "RFBN") is a `FractalBrownianNoise` rfbn:ℝⁿ→ℝ 
	that is constructed in such a way that for each v∈ℝⁿ, rfbn(v)~𝓤(0,1) where 𝓤(0,1) is the uniform unit interval distribution.
	In other words, when sampling over a sufficiently large amount of evenly distributed input,
	the output of RFBN is guaranteed to be evenly distributed from 0 to 1.
	RFBN can therefore be easily composed with the inverse CDF of any statistical distribution to produce 
	noise whose output follows the PDF of that distribution. 
	This makes an easy and very flexible way to produce spatial noise of any distribution.
	The word "ranked" in RFBN refers to the fact that output of the FBN is "ranked" from lowest to highest on a scale from 0 to 1.

	The implementation of RFBN requires precise coordination between the distribution that underlies the FBN 
	and the CDF that's used to map to the unit interval. 
	To safely abstract over this coordination, decisions have to be made about the RFBN's implementation.
	The CDF of an FBN is easily analyzed for precious few circumstances, 
	one of which is where a gaussian is the underlying noise function.
	Although our implementation of FBN is generalized to work with any underlying noise function,
	FBN is classically defined as only using gaussians,
	so a gaussian is chosen as the underlying noise for the FBN since this distribution is common and easy to reason with
	(gaussians of the same mean and standard deviation are closed under linear combinations)
	Other design decisions (such as the underlying use of value and mosaic noise) 
	are pragmatic decisions that are made to simplify implementation (this problem is hard enough as it is),
	however care should be taken when implementing other forms of RFBN to ensure its "ranked" property is preserved.
	*/
	template<int L, typename id, typename scalar>
	constexpr inline auto ranked_fractal_brownian_noise(
		const id octave_count, 
		const scalar amplitude_scale_per_frequency_doubling,
		const scalar coarsest_frequency,
		const scalar seed1,
		const scalar seed2
	) {
		const scalar pi(3.141592653589793238462643383279L);
		const scalar turn(2.0*pi);
	    auto fbm_cdf = analytic::Error(scalar(0), scalar(1), (scalar(1)/(std::sqrt(turn))));
	    auto fbm = field::fractal_brownian_noise<id,scalar>(
	      field::value_noise<L,scalar>(
	          field::mosaic_noise(
	            procedural::gaussian(procedural::unit_interval_noise(seed1,seed2)), 
	            cartesian::UnboundedIndexing<id>()),
	          field::vector_mosaic_ops<L,id,scalar>()
	      ), octave_count, amplitude_scale_per_frequency_doubling, coarsest_frequency);
	    return field::compose(fbm_cdf, fbm);
	}

}


#pragma once

template <
	typename scalar,
	typename speed,
	typename energy_per_temperature,
	typename energy_second,
	typename intensity_per_temperature4,
	glm::qualifier quality=Q
>
class ThermalEmission
{

	using vec3 = glm::vec<3,scalar,quality>;

	const scalar pi;
	const speed c; // speed of light
	const energy_per_temperature k; // boltzmann constant
	const energy_second h; // planck constant
	const intensity_per_temperature4 sigma; // stephan-boltzmann constant

	ThermalEmission(
		const scalar pi,
		const speed speed_of_light,
		const energy_per_temperature boltzmann_constant,
		const energy_second planck_constant
	):
		pi(pi),
		c(speed_of_light),
		k(boltzmann_constant),
		h(planck_constant),
		sigma(
			 scalar(2)*pi*pi*pi*pi*pi*k*k*k*k/
			(scalar(15)*h*h*h*c*c)
		) // stephan-boltzmann constant
	{}

	// see Lawson 2004, "The Blackbody Fraction, Infinite Series and Spreadsheets"
	// we only do a single iteration with n=1, because it doesn't have a noticeable effect on output
	scalar solve_fraction_of_light_emitted_by_black_body_below_wavelength(
	    const scalar wavelength, 
	    const scalar temperature,
	    const scalar iterations=2
	) const {

	    scalar L = wavelength;
	    scalar T = temperature;

	    scalar C2 = h*c/k;
	    scalar z  = C2 / (L*T);
	    scalar z2 = z*z;
	    scalar z3 = z2*z;

	    scalar sum(0);
	    scalar n2(0);
	    scalar n3(0);
	    for (scalar n=1.; n <= iterations; n++) {
	        n2 = n*n;
	        n3 = n2*n;
	        sum += (z3 + 3.*z2/n + 6.*z/n2 + 6./n3) * exp(-n*z) / n;
	    }
	    return 15.*sum/(pi*pi*pi*pi);
	}

	scalar solve_fraction_of_light_emitted_by_black_body_between_wavelengths(
	    const scalar lo, 
	    const scalar hi, 
	    const scalar temperature
	) const {
	    return  solve_fraction_of_light_emitted_by_black_body_below_wavelength(hi, temperature) - 
	            solve_fraction_of_light_emitted_by_black_body_below_wavelength(lo, temperature);
	}

	// This calculates the radiation (in watts/m^2) that's emitted 
	// by a single object using the Stephan-Boltzmann equation
	scalar get_intensity_of_light_emitted_by_black_body(
	    const scalar temperature
	) const {
	    return sigma * temperature*temperature*temperature*temperature;
	}

	vec3 solve_intensity3_of_light_emitted_by_black_body_between_wavelengths(
	    const vec3 lo3,
	    const vec3 hi3,
	    const scalar temperature
	) const {
	    return get_intensity_of_light_emitted_by_black_body(temperature)
	         * vec3(
	             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.x, hi3.x, temperature),
	             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.y, hi3.y, temperature),
	             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.z, hi3.z, temperature)
	           );
	}

	// from Carl Hansen et al., "Stellar Interiors"
	scalar get_fraction_of_radius_for_star_with_temperature(
		const scalar temperature, 
		const scalar core_temperature
	) const {
	    return sqrt(max(scalar(1) - (temperature / core_temperature), scalar(0)));
	}

}

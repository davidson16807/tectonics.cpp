#pragma once

#include <units.hpp>

namespace physics {

	constexpr float speed_of_light(299792458.0f * units::meter / units::second);
	constexpr float boltzmann_constant(1.3806485279e-23f * units::joule / units::kelvin);
	constexpr float stephan_boltzmann_constant(5.670373e-8f * units::watt / (units::meter2*units::kelvin4));
	constexpr float planck_constant(6.62607004e-34f * units::joule * units::second);
	constexpr float gravitational_constant(6.67428e-11f * units::meter3/(units::kilogram*units::second*units::second));

}

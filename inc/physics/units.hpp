#pragma once

namespace units {

	const     float degree = 3.141592653589793238462643383279502884197169399/180.0f;
	const     float radian = 1.0f;

	const     float kelvin = 1.0f;

	const     float microgram = 1e-9f;                  // kilograms
	const     float milligram = 1e-6f;                  // kilograms
	const     float gram      = 1e-3f;                  // kilograms
	constexpr float kilogram  = 1.0f;                   // kilograms
	const     float ton       = 1000.f;                 // kilograms

	const     float femtometer = 1e-12f;                // meters
	const     float nanometer  = 1e-9f;                 // meters
	const     float micrometer = 1e-6f;                 // meters
	const     float millimeter = 1e-3f;                 // meters
	constexpr float meter      = 1.0f;                  // meters
	const     float kilometer  = 1000.f;                // meters

	constexpr float mole       = 6.02214076e23f;
	constexpr float millimole  = mole / 1e3f;
	constexpr float micromole  = mole / 1e6f;
	constexpr float nanomole   = mole / 1e9f;
	constexpr float femtomole  = mole / 1e12f;

	constexpr float second     = 1.0f;                  // seconds
	constexpr float minute     = 60.0f;                 // seconds
	constexpr float hour       = minute*60.0f;          // seconds
	constexpr float day        = hour*24.0f;            // seconds
	constexpr float week       = day*7.0f;              // seconds
	constexpr float month      = day*29.53059f;         // seconds
	constexpr float year       = day*365.256363004f;    // seconds
	constexpr float megayear   = year*1e6f;             // seconds

	constexpr float newton     = kilogram * meter / (second * second);
	constexpr float joule      = newton * meter;
	constexpr float watt       = joule / second;

	constexpr float pascal     = newton / (meter * meter);
	constexpr float kilopascal = 1e3f * pascal;
	constexpr float megapascal = 1e6f * pascal;
	constexpr float gigapascal = 1e9f * pascal;

	constexpr float earth_mass            = 5.972e24f;    // kilograms
	constexpr float earth_radius          = 6.367e6f;     // meters
	constexpr float standard_gravity      = 9.80665f;     // meters/second^2
	constexpr float standard_temperature  = 273.15f;      // kelvin
	constexpr float standard_pressure     = 101325.0f;    // pascals
	constexpr float astronomical_unit     = 149597870700.0f;// meters
	constexpr float global_solar_constant = 1361.0f;      // watts/meter^2

	constexpr float jupiter_mass = 1.898e27f;             // kilograms
	constexpr float jupiter_radius = 71e6f;               // meters

	constexpr float solar_mass = 2e30f;                   // kilograms
	constexpr float solar_radius = 695.7e6f;              // meters
	constexpr float solar_luminosity = 3.828e26f;         // watts
	constexpr float solar_temperature = 5772.0f;          // kelvin

}

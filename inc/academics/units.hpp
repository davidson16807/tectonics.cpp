#pragma once

namespace units {

	constexpr float degree = 3.141592653589793238462643383279502884197169399/180.;
	constexpr float radian = 1.;

	constexpr float kelvin = 1.;

	constexpr float microgram = 1e-9;                  // kilograms
	constexpr float milligram = 1e-6;                  // kilograms
	constexpr float gram      = 1e-3;                  // kilograms
	constexpr float kilogram  = 1.;                    // kilograms
	constexpr float ton       = 1000.;                 // kilograms

	constexpr float femtometer = 1e-12;                 // meters
	constexpr float nanometer  = 1e-9;                 // meters
	constexpr float micrometer = 1e-6;                 // meters
	constexpr float millimeter = 1e-3;                 // meters
	constexpr float meter      = 1.;                   // meters
	constexpr float kilometer  = 1000.;                // meters

	constexpr float mole       = 6.02214076e23;
	constexpr float millimole  = mole / 1e3;
	constexpr float micromole  = mole / 1e6;
	constexpr float nanomole   = mole / 1e9;
	constexpr float femtomole  = mole / 1e12;

	constexpr float second     = 1.;                   // seconds
	constexpr float minute     = 60.;                  // seconds
	constexpr float hour       = minute*60.;           // seconds
	constexpr float day        = hour*24.;             // seconds
	constexpr float week       = day*7.;               // seconds
	constexpr float month      = day*29.53059;         // seconds
	constexpr float year       = day*365.256363004;    // seconds
	constexpr float megayear   = year*1e6;             // seconds

	constexpr float newton     = kilogram * meter / (second * second);
	constexpr float joule      = newton * meter;
	constexpr float watt       = joule / second;

	constexpr float pascal     = newton / (meter * meter);
	constexpr float kilopascal = 1e3 * pascal;
	constexpr float megapascal = 1e6 * pascal;
	constexpr float gigapascal = 1e9 * pascal;

	constexpr float earth_mass            = 5.972e24;  // kilograms
	constexpr float earth_radius          = 6.367e6;   // meters
	constexpr float standard_gravity      = 9.80665;   // meters/second^2
	constexpr float standard_temperature  = 273.15;    // kelvin
	constexpr float standard_pressure     = 101325.;   // pascals
	constexpr float astronomical_unit     = 149597870700.;// meters
	constexpr float global_solar_constant = 1361.;     // watts/meter^2

	constexpr float jupiter_mass = 1.898e27;           // kilograms
	constexpr float jupiter_radius = 71e6;             // meters

	constexpr float solar_mass = 2e30;                 // kilograms
	constexpr float solar_radius = 695.7e6;            // meters
	constexpr float solar_luminosity = 3.828e26;       // watts
	constexpr float solar_temperature = 5772.;         // kelvin

}

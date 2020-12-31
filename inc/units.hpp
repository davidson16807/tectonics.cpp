#pragma once

namespace units {

	// NOTE: we declare pi directly to avoid introducing dependency
	constexpr float degree ( 3.141592653589793238462643383279502884197169399/180.0f ); 
	constexpr float radian ( 1.0f );

	constexpr float kelvin  ( 1.0f );
	constexpr float kelvin4 ( kelvin*kelvin*kelvin*kelvin );

	constexpr float gram        ( 1e-3f         );          // kilograms
	constexpr float microgram   ( 1e-6f  * gram );          // kilograms
	constexpr float milligram   ( 1e-3f  * gram );          // kilograms
	constexpr float kilogram    ( 1e3f   * gram );          // kilograms
	constexpr float ton         ( 1e3f   * kilogram );      // kilograms
	constexpr float dalton      ( 1.66054e-27 * kilogram ); // kilograms

	constexpr float meter       ( 1.0f           );         // meters
	constexpr float femtometer  ( 1e-15f * meter );         // meters
	constexpr float picometer   ( 1e-12f * meter );         // meters
	constexpr float nanometer   ( 1e-9f  * meter );         // meters
	constexpr float micrometer  ( 1e-6f  * meter );         // meters
	constexpr float millimeter  ( 1e-3f  * meter );         // meters
	constexpr float centimeter  ( 1e-2f  * meter );         // meters
	constexpr float kilometer   ( 1000.f * meter );         // meters
	constexpr float angstrom    ( 0.1 * nanometer );        // meters

	constexpr float femtometer2 ( femtometer*femtometer );  // meters^2
	constexpr float picometer2  ( picometer*picometer );    // meters^2
	constexpr float nanometer2  ( nanometer*nanometer );    // meters^2
	constexpr float micrometer2 ( micrometer*micrometer );  // meters^2
	constexpr float millimeter2 ( millimeter*millimeter );  // meters^2
	constexpr float meter2      ( meter*meter );            // meters^2
	constexpr float centimeter2 ( centimeter*centimeter );  // meters^2
	constexpr float kilometer2  ( kilometer*kilometer );    // meters^2

	constexpr float femtometer3 ( femtometer*femtometer*femtometer ); // meters^3
	constexpr float picometer3  ( picometer*picometer*picometer );    // meters^3
	constexpr float nanometer3  ( nanometer*nanometer*nanometer );    // meters^3
	constexpr float micrometer3 ( micrometer*micrometer*micrometer ); // meters^3
	constexpr float millimeter3 ( millimeter*millimeter*millimeter ); // meters^3
	constexpr float centimeter3 ( centimeter*centimeter*centimeter ); // meters^3
	constexpr float meter3      ( meter*meter*meter );                // meters^3
	constexpr float kilometer3  ( kilometer*kilometer*kilometer );    // meters^3
	constexpr float liter       ( 0.001*meter3 );                     // meters^3
	constexpr float milliliter  ( 0.001*liter );                      // meters^3

	constexpr float mole        ( 6.02214076e23f );
	constexpr float millimole   ( mole / 1e3f );
	constexpr float micromole   ( mole / 1e6f );
	constexpr float nanomole    ( mole / 1e9f );
	constexpr float femtomole   ( mole / 1e12f );

	constexpr float second      ( 1.0f );                  // seconds
	constexpr float minute      ( 60.0f );                 // seconds
	constexpr float hour        ( minute*60.0f );          // seconds
	constexpr float day         ( hour*24.0f );            // seconds
	constexpr float week        ( day*7.0f );              // seconds
	constexpr float month       ( day*29.53059f );         // seconds
	constexpr float year        ( day*365.256363004f );    // seconds
	constexpr float megayear    ( year*1e6f );             // seconds

	constexpr float newton      ( kilogram * meter / (second * second) );
	constexpr float millinewton ( 1e-3f* newton );
	constexpr float kilonewton  ( 1e3f * newton );
	constexpr float meganewton  ( 1e6f * newton );
	constexpr float giganewton  ( 1e9f * newton );

	constexpr float joule       ( newton * meter );
	constexpr float millijoule  ( 1e-3f* joule  );
	constexpr float kilojoule   ( 1e3f * joule  );
	constexpr float megajoule   ( 1e6f * joule  );
	constexpr float gigajoule   ( 1e9f * joule  );
	constexpr float calorie     ( 4.184 * joule );

	constexpr float watt        ( joule / second );
	constexpr float milliwatt   ( 1e-3f* watt   );
	constexpr float kilowatt    ( 1e3f * watt   );
	constexpr float megawatt    ( 1e6f * watt   );
	constexpr float gigawatt    ( 1e9f * watt   );

	constexpr float pascal      ( newton / (meter * meter) );
	constexpr float millipascal ( 1e-3f* pascal );
	constexpr float kilopascal  ( 1e3f * pascal );
	constexpr float megapascal  ( 1e6f * pascal );
	constexpr float gigapascal  ( 1e9f * pascal );
	constexpr float bar         ( 1e5f * pascal );

	constexpr float earth_age             ( 4.54e9f*year );        // seconds
	constexpr float earth_mass            ( 5.972e24f );           // kilograms
	constexpr float earth_radius          ( 6.367e6f );            // meters
	constexpr float standard_gravity      ( 9.80665f );            // meters/second^2
	constexpr float standard_temperature  ( 273.15f );             // kelvin
	constexpr float standard_pressure     ( 101325.0f );           // pascals
	constexpr float standard_molar_volume ( 22.414*units::liter ); // meter^3
	constexpr float astronomical_unit     ( 149597870700.0f );     // meters
	constexpr float global_solar_constant ( 1361.0f );             // watts/meter^2

	constexpr float jupiter_mass ( 1.898e27f );                    // kilograms
	constexpr float jupiter_radius ( 71e6f );                      // meters

	constexpr float solar_mass ( 2e30f );                          // kilograms
	constexpr float solar_radius ( 695.7e6f );                     // meters
	constexpr float solar_luminosity ( 3.828e26f );                // watts
	constexpr float solar_temperature ( 5772.0f );                 // kelvin

}

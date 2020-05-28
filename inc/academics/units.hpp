
namespace units {

	constexpr float DEGREE = 3.141592653589793238462643383279502884197169399/180.;
	constexpr float RADIAN = 1.;

	constexpr float KELVIN = 1.;

	constexpr float MICROGRAM = 1e-9;                  // kilograms
	constexpr float MILLIGRAM = 1e-6;                  // kilograms
	constexpr float GRAM      = 1e-3;                  // kilograms
	constexpr float KILOGRAM  = 1.;                    // kilograms
	constexpr float TON       = 1000.;                 // kilograms

	constexpr float FEMTOMETER = 1e-12;                 // meters
	constexpr float NANOMETER  = 1e-9;                 // meters
	constexpr float MICROMETER = 1e-6;                 // meters
	constexpr float MILLIMETER = 1e-3;                 // meters
	constexpr float METER      = 1.;                   // meters
	constexpr float KILOMETER  = 1000.;                // meters

	constexpr float MOLE       = 6.02214076e23;
	constexpr float MILLIMOLE  = MOLE / 1e3;
	constexpr float MICROMOLE  = MOLE / 1e6;
	constexpr float NANOMOLE   = MOLE / 1e9;
	constexpr float FEMTOMOLE  = MOLE / 1e12;

	constexpr float SECOND     = 1.;                   // seconds
	constexpr float MINUTE     = 60.;                  // seconds
	constexpr float HOUR       = MINUTE*60.;           // seconds
	constexpr float DAY        = HOUR*24.;             // seconds
	constexpr float WEEK       = DAY*7.;               // seconds
	constexpr float MONTH      = DAY*29.53059;         // seconds
	constexpr float YEAR       = DAY*365.256363004;    // seconds
	constexpr float MEGAYEAR   = YEAR*1e6;             // seconds

	constexpr float NEWTON     = KILOGRAM * METER / (SECOND * SECOND);
	constexpr float JOULE      = NEWTON * METER;
	constexpr float WATT       = JOULE / SECOND;

	constexpr float PASCAL     = NEWTON / (METER * METER);
	constexpr float KILOPASCAL = 1e3 * PASCAL;
	constexpr float MEGAPASCAL = 1e6 * PASCAL;
	constexpr float GIGAPASCAL = 1e9 * PASCAL;

	constexpr float EARTH_MASS            = 5.972e24;  // kilograms
	constexpr float EARTH_RADIUS          = 6.367e6;   // meters
	constexpr float STANDARD_GRAVITY      = 9.80665;   // meters/second^2
	constexpr float STANDARD_TEMPERATURE  = 273.15;    // kelvin
	constexpr float STANDARD_PRESSURE     = 101325.;   // pascals
	constexpr float ASTRONOMICAL_UNIT     = 149597870700.;// meters
	constexpr float GLOBAL_SOLAR_CONSTANT = 1361.;     // watts/meter^2

	constexpr float JUPITER_MASS = 1.898e27;           // kilograms
	constexpr float JUPITER_RADIUS = 71e6;             // meters

	constexpr float SOLAR_MASS = 2e30;                 // kilograms
	constexpr float SOLAR_RADIUS = 695.7e6;            // meters
	constexpr float SOLAR_LUMINOSITY = 3.828e26;       // watts
	constexpr float SOLAR_TEMPERATURE = 5772.;         // kelvin

}

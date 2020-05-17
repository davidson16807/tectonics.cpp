#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace tectonics
{
	struct stratum_mass_pool
	{
		float mass; 
		float max_pressure_received;
		float max_temperature_received;
		float grain_size;
		float mass_pool_id;
	};
	/*
	"stratum_mass_pool_store" is a memory efficient variant of the 
	statum_mass_pool data structure. Isn't it adorable!?
	It would take ridiculous amounts of memory to store a stratum_mass_pool 
	for every stratum within a raster, so we store each stratum in a raster
	as a stratum_mass_pool_store, then convert back to stratum_mass_pool when
	we want to perform some operation on it. 

	so a commutative diagram of its morphisms is:
	stratum_mass_pool_store <=> stratum_mass_pool
	*/
	struct stratum_mass_pool_store
	{
		float mass; 
		std::uint8_t max_pressure_received;
		std::uint8_t max_temperature_received;
		std::uint8_t grain_size;
		std::uint8_t mass_pool_id;

	private:
		// Represent pressures up to 6e12 Pascals with a precision of 12%.
		// This value was chosen to reflect the pressure of Jupiter's core. 
		constexpr float LOG_REF_TEMPERATURE = 17.;
		// Represent temperatures up to 32768 Kelvin with a precision of 12%.
		// This value was chosen to reflect the temperature of Jupiter's core. 
		constexpr float LOG_REF_PRESSURE    = 6.;
		// Represent grain sizes down to 1μm with a precision of 12%.
		// This value was chosen to reflect the size of clay particles. 
		constexpr float LOG_REF_GRAIN_SIZE  = 13.;

		constexpr float TEMPERATURE_MAX = exp2(255./LOG_REF_TEMPERATURE);
		constexpr float PRESSURE_MAX    = exp2(255./LOG_REF_PRESSURE);
		constexpr float GRAIN_SIZE_MAX  = exp2(-0. /LOG_REF_GRAIN_SIZE);

		constexpr float TEMPERATURE_MIN = exp2(0./LOG_REF_TEMPERATURE);
		constexpr float PRESSURE_MIN    = exp2(0./LOG_REF_PRESSURE);
		constexpr float GRAIN_SIZE_MIN  = exp2(-255./LOG_REF_GRAIN_SIZE);

	public:
		~stratum_mass_pool_store()
		{
		}
		explicit stratum_mass_pool_store(const stratum_mass_pool& input) :
			max_pressure_received   (exp2(float(input.max_pressure_received)/LOG_REF_PRESSURE)),
			max_temperature_received(exp2(float(input.max_temperature_received)/LOG_REF_PRESSURE)),
			grain_size              (exp2(-float(grain_size_)/LOG_REF_GRAIN_SIZE)),
		{
		}
		void decompress(stratum_mass_pool& output)
		{
			output.max_pressure_received    = std::uint8_t(std::clamp(log2(max_pressure_received)*LOG_REF_PRESSURE,  0., 255.));
			output.max_temperature_received = std::uint8_t(std::clamp(log2(max_temperature_received)*LOG_REF_TEMPERATURE, 0., 255.));
			output.grain_size               = std::uint8_t(std::clamp(-log2(grain_size)*LOG_REF_GRAIN_SIZE, 0., 255.));
		}
	}

	const std::size_t STRATA_MASS_POOL_COUNT = 8;
	struct stratum
	{
		float particle_size;
		std::array<stratum_mass_pool_store, STRATA_MASS_POOL_COUNT>  mass_pools;
	};

	const std::size_t STRATA_STRATUM_COUNT = 16;
	struct strata
	{
		std::array<stratum, STRATA_STRATUM_COUNT> strata;
	};

	// typedef rasters<Strata> Crust;

}

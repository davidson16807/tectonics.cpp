#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace tectonics
{

	struct stratum_mass_pool
	{
		float mass; 
	private:
		std::uint8_t max_pressure_received_;
		std::uint8_t max_temperature_received_;
		std::uint8_t grain_size_;
	public: 
		std::uint8_t mass_pool_id;

	private:
		static constexpr float LOG_REF_TEMPERATURE = 17.;
		static constexpr float LOG_REF_PRESSURE    = 6.;
		static constexpr float LOG_REF_GRAIN_SIZE  = 13.;

	public: 
		static constexpr float TEMPERATURE_MAX = exp2(255./LOG_REF_TEMPERATURE);
		static constexpr float PRESSURE_MAX    = exp2(255./LOG_REF_PRESSURE);
		static constexpr float GRAIN_SIZE_MAX  = exp2(-0. /LOG_REF_GRAIN_SIZE);

		static constexpr float TEMPERATURE_MIN = exp2(0./LOG_REF_TEMPERATURE);
		static constexpr float PRESSURE_MIN    = exp2(0./LOG_REF_PRESSURE);
		static constexpr float GRAIN_SIZE_MIN  = exp2(-255./LOG_REF_GRAIN_SIZE);

	public: 
		inline float max_pressure_received() const {
			// Represent pressures up to 6e12 Pascals with a precision of 12%.
			// This value was chosen to reflect the pressure of Jupiter's core. 
			return exp2(float(max_pressure_received_)/LOG_REF_PRESSURE);
		}
		inline float max_temperature_received() const {
			// represent temperatures up to 32768 Kelvin with a precision of 12%.
			// This value was chosen to reflect the temperature of Jupiter's core. 
			return exp2(float(max_temperature_received_)/LOG_REF_TEMPERATURE);
		}
		inline float grain_size() const {
			// represent grain sizes down to 1μm with a precision of 12%.
			// This value was chosen to reflect the size of clay particles. 
			return exp2(-float(grain_size_)/LOG_REF_GRAIN_SIZE);
		}

		inline float max_pressure_received(const float value) {
			max_pressure_received_   = std::uint8_t(std::clamp(log2(value)*LOG_REF_PRESSURE,  0., 255.));
		}
		inline float max_temperature_received(const float value) {
			max_temperature_received_= std::uint8_t(std::clamp(log2(value)*LOG_REF_TEMPERATURE, 0., 255.));
		}
		inline float grain_size(const float value) {
			grain_size_ = std::uint8_t(std::clamp(-log2(value)*LOG_REF_GRAIN_SIZE, 0., 255.));
		}
	};

	const std::size_t STRATA_MASS_POOL_COUNT = 8;
	struct stratum
	{
		float particle_size;
		std::array<stratum_mass_pool, STRATA_MASS_POOL_COUNT>  mass_pools;
	};

	const std::size_t STRATA_STRATUM_COUNT = 16;
	struct strata
	{
		std::array<stratum, STRATA_STRATUM_COUNT> strata;
	};

	// typedef rasters<Strata> Crust;

}

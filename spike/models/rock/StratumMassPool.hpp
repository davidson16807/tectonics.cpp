#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace rock
{
	struct StratumMassPool
	{
		float mass; 
		float max_pressure_received;
		float max_temperature_received;
		float intrusive_grain_bin_relative_volume;
		float extrusive_grain_bin_relative_volume;
		/*
		Constructs an empty mass pool with minimum possible pressure and temperature,
		min and max grain sizes are undefined.
		*/
		StratumMassPool():
			mass(0),
			max_pressure_received(0),
			max_temperature_received(0),
			intrusive_grain_bin_relative_volume(0),
			extrusive_grain_bin_relative_volume(0),
		{
		}
		static void combine(const StratumMassPool& a, const StratumMassPool& b, StratumMassPool& output)
		{
			output.mass = a.mass + b.mass;
			output.max_pressure_received    = max(a.max_pressure_received,    b.max_pressure_received   );
			output.max_temperature_received = max(a.max_temperature_received, b.max_temperature_received);
			output.extrusive_grain_bin_relative_volume = a.extrusive_grain_bin_relative_volume + b.extrusive_grain_bin_relative_volume;
			output.intrusive_grain_bin_relative_volume = a.intrusive_grain_bin_relative_volume + b.intrusive_grain_bin_relative_volume;
		}
	};
	/*
	"StratumMassPoolStore" is a memory efficient variant of the 
	StratumMassPool data structure. Isn't it adorable!?
	It would take ridiculous amounts of memory to store a StratumMassPool 
	for every stratum within a raster, so we store each stratum in a raster
	as a StratumMassPoolStore, then convert back to StratumMassPool when
	we want to perform some operation on it. 

	so a commutative diagram of its morphisms is:
	StratumMassPoolStore <=> StratumMassPool
	*/
	class StratumMassPoolStore
	{
		float mass; 
		std::uint8_t max_pressure_received;
		std::uint8_t max_temperature_received;
		std::uint8_t intrusive_grain_bin_relative_volume;
		std::uint8_t extrusive_grain_bin_relative_volume;

	private:
		// Represent pressures up to 6e12 Pascals with a precision of 12%.
		// This value was chosen to reflect the pressure of Jupiter's core. 
		constexpr static float log_ref_temperature = 17.;
		// Represent temperatures up to 32768 Kelvin with a precision of 12%.
		// This value was chosen to reflect the temperature of Jupiter's core. 
		constexpr static float log_ref_pressure    = 6.;

		constexpr static float temperature_max = exp2(255./log_ref_temperature);
		constexpr static float pressure_max    = exp2(255./log_ref_pressure);

		constexpr static float temperature_min = exp2(0./log_ref_temperature);
		constexpr static float pressure_min    = exp2(0./log_ref_pressure);

	public:
		~StratumMassPoolStore()
		{
		}
		void decompress(StratumMassPool& output) const
		{
		    output.mass                     = mass;
			output.max_pressure_received    = exp2( log_ref_pressure    * float(max_pressure_received)    / 255.0f);
			output.max_temperature_received = exp2( log_ref_temperature * float(max_temperature_received) / 255.0f);
			output.extrusive_grain_bin_relative_volume = extrusive_grain_bin_relative_volume;
			output.intrusive_grain_bin_relative_volume = intrusive_grain_bin_relative_volume;
		}
		void compress(const StratumMassPool& input)
		{
			mass                     = input.mass;
			max_pressure_received    = std::uint8_t(255.0f*std::clamp( log2(input.max_pressure_received)    / log_ref_pressure,   0., 1.0));
			max_temperature_received = std::uint8_t(255.0f*std::clamp( log2(input.max_temperature_received) / log_ref_temperature,0., 1.0));
			float grain_size_bin_relative_volume_max = max(input.extrusive_grain_bin_relative_volume, input.intrusive_grain_bin_relative_volume);
			extrusive_grain_bin_relative_volume = std::clamp(input.extrusive_grain_bin_relative_volume * 255.0f / grain_size_bin_relative_volume_max, 0.0f, 255.0f);
			intrusive_grain_bin_relative_volume = std::clamp(input.intrusive_grain_bin_relative_volume * 255.0f / grain_size_bin_relative_volume_max, 0.0f, 255.0f);
		}
	}
}

#pragma once

#include <math.h>       /* log10, floor, pow, abs */
#include <algorithm>    // std::min

namespace si{
	/*
	`si` is a principled units library. It minimizes class constructs, maximizes readability, and has a single well-defined purpose: 
	  To track scalar quantities that are stored in terms of the six Standard International ("SI") base units.

	`si` is built on the following principles:

	* Reusability in concepts supercedes reusability in libraries.
	  Software libraries come and go, but the concepts that underlie them can endure if they are simple, formally defined, and effective.
      In this vein, the constructs within this library should be as simple to understand and reimplement as possible. 

	* The library should limit itself to a single well defined scope.
	  We choose SI since it is a simple and well defined standard with broad applicability. 
	  An SI units library is usually enough to satisfy the guarantees that are needed 
	  for even large scale scientific software applications, 
	  yet doing so might only require a single class template in order to implement.

	* The library should use the fewest class constructs needed to create a constexpr implementation 
	  of the full SI standard that still provides all valid operator overloads 
	  and guarantees that raw scalar representations will never be exposed unless they describe unitless values.

    * The library should work well with other units libraries, so that libraries written on top of it
      do not require their users to choose this units library over another one. 
      The name of the library should be distinct and descriptive to reflect this.

    * The library also should enforce that measurements be easy to read and interpret without requiring the user to drop namespaces.
      This includes measurements formed by composing several units of measure, like km/s.
      We find that measurements are easier to read this way when using a flattened and abbreviated namespace 
      with unabbreviated and unambiguous units of measure, and unique variable names for prefixed units of measure.
      In other words, this:

      ```
      si::kilometer / si::second
      ```

	  as opposed to:

      ```

        si::km / si::s

        si::k*si::m / si::s

        si::kilo*si::meter / si::second

        units::km / units::s

        units::k*units::m / units::s

        units::kilometer / units::second

        units::kilo*units::meter / units::second

        units::si::km / units::si::s

        units::si::k*units::si::m / units::si::s

        units::si::kilo*units::si::meter / units::si::second

        units::si::kilometer / units::si::second

      ```
      
	  `units::kilometer / units::second` is also readable but violates the principle that the library work well with others,
	  since many unit libraries have already taken that name.
	  Since abbreviated namespaces are preferred, we should pick the shortest abbreviation
      recognized by an official standards institute. This is why we call it `si`.

    * The library should minimize the amount of unit conversion that occurs when reporting measurements.
      This applies for both unit conversion in the developer's head as well as in the developer's code.
      So in contrast to the `si::units` class template, which is as simple as possible, 
      the list of built-in measures (e.g. `si::length`) and units of measure (e.g. `si::kilometer`) 
      should be deliberately made as *extensive* as possible.
      Combined with readability constraints mentioned above, this means the library should include all combinations 
      of officially recognized SI prefixes and units of measure as unique variables
      (e.g. `si::hectometer`, not `si::hecto*si::meter`), 
      and units of measure from other common measurement systems (e.g. `si::hour`, `si::poise`, `si::earth_radius`),
      or common exponents of measures (e.g. `si::kelvin4`)
      Please note this principle does not apply to plurality variants that are meant for convenience (e.g. not `si::kilometers`)
      or variants that could be written using operators instead (e.g. not `si::kilometer_per_second`)
	  We settle upon including all common units of time, units from the CGS, and astronomical units from the IAU standard.

    * The library should provide default measurement types (e.g. `si::length`) 
      that allow work over the same range and precision afforded by the underlying system of measurements. 
      Since SI is designed to express measurements over a very wide range in orders of magnitude,
      some extremes cannot be expressed using floats (e.g. cubic yottameters)
	  so all built-in measures (e.g. `si::length`) should handle doubles by default.
      However, template aliases should be provided for all built in measures 
      to allow easily specifying other underlying types (e.g. `si::length_type<float>`)
	*/

	template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1=double>
	class units
	{
		T1 raw;
	public:
		constexpr explicit units(T1 raw): raw(raw)
		{

		}
		template<typename T2>
		constexpr explicit units(T2 raw): raw(raw)
		{

		}
		// implicit cast is only allowed if two objects match signature but differ in type
		template<typename T2>
		constexpr units(units<M1,KG1,S1,K1,MOL1,A1,CD1,T2> raw): raw(raw)
		{

		}

		template <int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2>
		constexpr units<M1+M2,  KG1+KG2,  S1+S2,  K1+K2,  MOL1+MOL2,  A1+A2,  CD1+CD2,  T1> 
		multiply(const units<M2,KG2,S2,K2,MOL2,A2,CD2,T1> other) const
		{
			return units<M1+M2,  KG1+KG2,  S1+S2,  K1+K2,  MOL1+MOL2,  A1+A2,  CD1+CD2,  T1>(raw * other.raw);
		}
		constexpr units<-M1,-KG1,-S1,-K1,-MOL1,-A1,-CD1,T1>  invert() const
		{
			return units<-M1,-KG1,-S1,-K1,-MOL1,-A1,-CD1,T1>(T1(1)/raw);
		}
		template<typename T2>
		constexpr units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> multiply(const T2 scalar) const
		{
			return units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(raw*T1(scalar));
		}
		template<typename T2>
		constexpr units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> divide(const T2 scalar) const
		{
			return units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(T1(scalar)/scalar);
		}

		constexpr bool operator==(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw == other.raw;
		}
		constexpr bool operator!=(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw != other.raw;
		}

		constexpr bool operator>(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw > other.raw;
		}
		constexpr bool operator>=(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw >= other.raw;
		}
		constexpr bool operator<(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw < other.raw;
		}
		constexpr bool operator<=(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return raw <= other.raw;
		}

		constexpr units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> operator+(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(raw + other.raw);
		}
		constexpr units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> operator-(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) const
		{
			return units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(raw - other.raw);
		}
		constexpr units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> operator-() const
		{
			return units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(-raw);
		}


		units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>& operator+=(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) 
		{
			raw += other.raw;
			return *this;
		}
		units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>& operator-=(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> other) 
		{
			raw -= other.raw;
			return *this;
		}

		template<typename T2>
		units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>& operator*=(const T2 other) 
		{
			raw *= T1(other);
			return *this;
		}
		template<typename T2>
		units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>& operator/=(const T2 other) 
		{
			raw /= T1(other);
			return *this;
		}

	    std::string to_string() const 
	    {
	    	std::array<std::string, 17> prefixes {"y","z","a","","p","n","u","m","","k","M","G","T","P","E","Z","Y"};

	    	// customize formatting for area and volume, which follow separate rules for prefix conversion
    		if( M1 != 0 && (KG1|S1|K1|MOL1|A1|CD1) == 0)
    		{
		    	std::string prefixed_value = std::to_string(raw / pow(1000.0, std::min(8.0, floor(log10(raw)/(3.0*M1)))));
		    	std::string prefix(prefixes[int(log10(raw)/(3.0*M1))+8]);
    			return prefixed_value + prefix + (M1<0? "m"  + (  M1<1? std::to_string(abs(M1  )) : "") : "");
    		}

	    	// customize formatting for common derived units for pretty printing
	    	std::array<std::pair<std::array<int,3>, std::string>, 9> named_mks {
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 1,1,-2}, "N"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 2,1,-2}, "N*m"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{-1,1,-2}, "Pa"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{-1,1,-1}, "Pa*s"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 2,1,-2}, "J"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 2,0,-2}, "J/kg"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 2,1,-3}, "W"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 1,1,-3}, "W/m"},
	    		std::pair<std::array<int,3>, std::string>{std::array<int,3>{ 0,1,-3}, "W/m2"},
	    	};
	    	for(std::size_t i(0); i < named_mks.size(); ++i)
	    	{
	    		if(named_mks[i].first == std::array<int,3>{M1,KG1,S1})
	    		{
			    	std::string prefixed_value =  std::to_string(raw / pow(1000.0, std::min(8.0, floor(log10(raw)/3.0))));
			    	std::string prefix(prefixes[int(log10(raw)/3.0)+8]);
	    			std::string result = prefixed_value + prefix;
	    			result += named_mks[i].second;
			    	result +=   K1<0? "K"  + (  K1<1? std::to_string(abs(K1  )) : "") : ""; 
			    	result += MOL1<0? "mol"+ (MOL1<1? std::to_string(abs(MOL1)) : "") : ""; 
			    	result +=   A1<0? "A"  + (  A1<1? std::to_string(abs(  A1)) : "") : ""; 
			    	result +=  CD1<0? "Cd" + ( CD1<1? std::to_string(abs( CD1)) : "") : ""; 
			    	result += K1<0 || MOL1<0? "/" : "";
			    	result +=   K1<0? "K"  + (  K1<1? std::to_string(abs(K1  )) : "") : ""; 
			    	result += MOL1<0? "mol"+ (MOL1<1? std::to_string(abs(MOL1)) : "") : ""; 
			    	result +=   A1<0? "A"  + (  A1<1? std::to_string(abs(  A1)) : "") : ""; 
			    	result +=  CD1<0? "Cd" + ( CD1<1? std::to_string(abs( CD1)) : "") : ""; 
			    	return result;
	    		}
	    	}

	    	// fall back on SI base units 
			std::string result = std::to_string(raw);
	    	result += M1<0 && KG1<0 && S1<0 && K1<0 && MOL1<0? "1" : "";
	    	result +=   M1<0? "m"  + (  M1<1? std::to_string(abs(M1  )) : "") : ""; 
	    	result +=  KG1<0? "kg" + ( KG1<1? std::to_string(abs(KG1 )) : "") : ""; 
	    	result +=   S1<0? "s"  + (  S1<1? std::to_string(abs(S1  )) : "") : ""; 
	    	result +=   K1<0? "K"  + (  K1<1? std::to_string(abs(K1  )) : "") : ""; 
	    	result += MOL1<0? "mol"+ (MOL1<1? std::to_string(abs(MOL1)) : "") : ""; 
	    	result +=   A1<0? "A"  + (  A1<1? std::to_string(abs(  A1)) : "") : ""; 
	    	result +=  CD1<0? "Cd" + ( CD1<1? std::to_string(abs( CD1)) : "") : ""; 
	    	result += M1<0 || KG1<0 || S1<0 || K1<0 || MOL1<0? "/" : "";
	    	result +=   M1<0? "m"  + (  M1<1? std::to_string(abs(M1  )) : "") : ""; 
	    	result +=  KG1<0? "kg" + ( KG1<1? std::to_string(abs(KG1 )) : "") : ""; 
	    	result +=   S1<0? "s"  + (  S1<1? std::to_string(abs(S1  )) : "") : ""; 
	    	result +=   K1<0? "K"  + (  K1<1? std::to_string(abs(K1  )) : "") : ""; 
	    	result += MOL1<0? "mol"+ (MOL1<1? std::to_string(abs(MOL1)) : "") : ""; 
	    	result +=   A1<0? "A"  + (  A1<1? std::to_string(abs(  A1)) : "") : ""; 
	    	result +=  CD1<0? "Cd" + ( CD1<1? std::to_string(abs( CD1)) : "") : ""; 
	    	return result;
    	}

		template <int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2, typename T2>
		friend class units;
		template <typename T2>
		friend constexpr T2 unitless(units<0,0,0,0,0,0,0,T2> a);
	};

	template <typename T1>
	constexpr T1 unitless(units<0,0,0,0,0,0,0,T1> a)
	{
		return a.raw;
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto operator*(const T1 a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
	{
		return b.multiply(a);
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto operator/(const T1 a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
	{
		return b.invert().multiply(a);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto operator*(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const T1 b)
	{
		return a.multiply(b);
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto operator/(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const T1 b)
	{
		return a.divide(b);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1,  int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2, typename T1>
	constexpr auto operator*(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M2,KG2,S2,K2,MOL2,A2,CD2,T1> b)
	{
		return a.multiply(b);
	}
	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1,  int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2, typename T1>
	constexpr auto operator/(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M2,KG2,S2,K2,MOL2,A2,CD2,T1> b)
	{
		return b.invert().multiply(a);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto abs(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
	{
		return a > units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(0)? a : a.multiply(-1.0);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto min(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
	{
		return a < b? a : b;
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	constexpr auto max(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
	{
		return a > b? a : b;
	}




















														//           m kg  s  K mol A Cd
	template <typename T> using length_type                    = units< 1, 0, 0, 0, 0, 0, 0, T>;
	template <typename T> using mass_type                      = units< 0, 1, 0, 0, 0, 0, 0, T>;
	template <typename T> using time_type                      = units< 0, 0, 1, 0, 0, 0, 0, T>;
	template <typename T> using temperature_type               = units< 0, 0, 0, 1, 0, 0, 0, T>;
	template <typename T> using amount_type                    = units< 0, 0, 0, 0, 1, 0, 0, T>;
	template <typename T> using current_type                   = units< 0, 0, 0, 0, 0, 1, 0, T>;
	template <typename T> using luminous_intensity_type        = units< 0, 0, 0, 0, 0, 0, 1, T>;
	template <typename T> using area_type                      = units< 2, 0, 0, 0, 0, 0, 0, T>;
	template <typename T> using volume_type                    = units< 3, 0, 0, 0, 0, 0, 0, T>;
	template <typename T> using volumetric_flow_type           = units< 3, 0,-1, 0, 0, 0, 0, T>;
	template <typename T> using area_density_type              = units<-2, 1, 0, 0, 0, 0, 0, T>;
	template <typename T> using density_type                   = units<-3, 1, 0, 0, 0, 0, 0, T>;
	template <typename T> using frequency_type                 = units< 0, 0,-1, 0, 0, 0, 0, T>;
	template <typename T> using speed_type                     = units< 1, 0,-1, 0, 0, 0, 0, T>;
	template <typename T> using acceleration_type              = units< 1, 0,-2, 0, 0, 0, 0, T>;
	template <typename T> using momentum_type                  = units< 1, 1,-1, 0, 0, 0, 0, T>;
	template <typename T> using force_type                     = units< 1, 1,-2, 0, 0, 0, 0, T>;
	template <typename T> using angular_velocity_type          = units< 0, 0,-1, 0, 0, 0, 0, T>;
	template <typename T> using angular_acceleration_type      = units< 0, 0,-2, 0, 0, 0, 0, T>;
	template <typename T> using angular_momentum_type          = units< 2, 1,-1, 0, 0, 0, 0, T>;
	template <typename T> using torque_type                    = units< 2, 1,-2, 0, 0, 0, 0, T>;
	template <typename T> using pressure_type                  = units<-1, 1,-2, 0, 0, 0, 0, T>;
	template <typename T> using kinematic_viscosity_type       = units< 2, 0,-1, 0, 0, 0, 0, T>;
	template <typename T> using dynamic_viscosity_type         = units<-1, 1,-1, 0, 0, 0, 0, T>;
	template <typename T> using energy_type                    = units< 2, 1,-2, 0, 0, 0, 0, T>;
	template <typename T> using power_type                     = units< 2, 1,-3, 0, 0, 0, 0, T>;
	template <typename T> using intensity_type                 = units< 0, 1,-3, 0, 0, 0, 0, T>;
	template <typename T> using molar_mass_type                = units< 0, 1, 0, 0,-1, 0, 0, T>;
	template <typename T> using molar_volume_type              = units< 3, 0, 0, 0,-1, 0, 0, T>;
	template <typename T> using molar_heat_capacity_type       = units< 2, 1,-2,-1,-1, 0, 0, T>;
	template <typename T> using specific_heat_capacity_type    = units< 2, 0,-2,-1, 0, 0, 0, T>;
	template <typename T> using molar_energy_type              = units< 2, 1,-2, 0,-1, 0, 0, T>;
	template <typename T> using specific_energy_type           = units< 2, 0,-2, 0, 0, 0, 0, T>;
	template <typename T> using thermal_conductivity_type      = units< 1, 1,-3,-1, 0, 0, 0, T>;
	template <typename T> using wavenumber_type                = units<-1, 0, 0, 0, 0, 0, 0, T>;
	template <typename T> using voltage_type                   = units< 2, 1,-3, 0, 0,-1, 0, T>;
	template <typename T> using electric_charge_type           = units< 0, 0, 1, 0, 0, 1, 0, T>;
	template <typename T> using electric_capacitance_type      = units<-2,-1, 4, 0, 0, 2, 0, T>;
	template <typename T> using electric_resistance_type       = units< 2, 1,-3, 0, 0,-2, 0, T>;
	template <typename T> using electric_conductance_type      = units<-2,-1, 3, 0, 0, 2, 0, T>;
	template <typename T> using electric_induction_type        = units< 2, 1,-2, 0, 0,-2, 0, T>;
	template <typename T> using magnetic_flux_type             = units< 2, 1,-2, 0, 0,-2, 0, T>;
	template <typename T> using magnetic_induction_type        = units< 0, 1,-2, 0, 0,-1, 0, T>;
	template <typename T> using magnetic_flux_density_type     = units< 0, 1,-2, 0, 0,-1, 0, T>;
	template <typename T> using luminous_flux_type             = units< 0, 0, 0, 0, 0, 0, 1, T>;
	template <typename T> using illuminance_type               = units<-2, 0, 0, 0, 0, 0, 1, T>;
	template <typename T> using catalytic_activity_type        = units< 0, 0,-1, 0, 1, 0, 0, T>;
	template <typename T> using ionizing_radiation_dosage_type = units< 2, 0,-2, 0, 0, 0, 0, T>;
	template <typename T> using radioactivity_type             = units< 0, 0,-1, 0, 0, 0, 0, T>;


	//             m kg  s  K mol A Cd
	typedef units< 1, 0, 0, 0, 0, 0, 0> length                       ;
	typedef units< 0, 1, 0, 0, 0, 0, 0> mass                         ;
	typedef units< 0, 0, 1, 0, 0, 0, 0> time                         ;
	typedef units< 0, 0, 0, 1, 0, 0, 0> temperature                  ;
	typedef units< 0, 0, 0, 0, 1, 0, 0> amount                       ;
	typedef units< 0, 0, 0, 0, 0, 1, 0> current                      ;
	typedef units< 0, 0, 0, 0, 0, 0, 1> luminous_intensity           ;
	typedef units< 2, 0, 0, 0, 0, 0, 0> area                         ;
	typedef units< 3, 0, 0, 0, 0, 0, 0> volume                       ;
	typedef units< 3, 0,-1, 0, 0, 0, 0> volumetric_flow              ;
	typedef units<-2, 1, 0, 0, 0, 0, 0> area_density                 ;
	typedef units<-3, 1, 0, 0, 0, 0, 0> density                      ;
	typedef units< 0, 0,-1, 0, 0, 0, 0> frequency                    ;
	typedef units< 1, 0,-1, 0, 0, 0, 0> speed                        ;
	typedef units< 1, 0,-2, 0, 0, 0, 0> acceleration                 ;
	typedef units< 1, 1,-1, 0, 0, 0, 0> momentum                     ;
	typedef units< 1, 1,-2, 0, 0, 0, 0> force                        ;
	typedef units< 0, 0,-1, 0, 0, 0, 0> angular_velocity             ;
	typedef units< 0, 0,-2, 0, 0, 0, 0> angular_acceleration         ;
	typedef units< 2, 1,-1, 0, 0, 0, 0> angular_momentum             ;
	typedef units< 2, 1,-2, 0, 0, 0, 0> torque                       ;
	typedef units<-1, 1,-2, 0, 0, 0, 0> pressure                     ;
	typedef units< 2, 0,-1, 0, 0, 0, 0> kinematic_viscosity          ;
	typedef units<-1, 1,-1, 0, 0, 0, 0> dynamic_viscosity            ;
	typedef units< 2, 1,-2, 0, 0, 0, 0> energy                       ;
	typedef units< 2, 1,-3, 0, 0, 0, 0> power                        ;
	typedef units< 0, 1,-3, 0, 0, 0, 0> intensity                    ;
	typedef units< 0, 1, 0, 0,-1, 0, 0> molar_mass                   ;
	typedef units< 3, 0, 0, 0,-1, 0, 0> molar_volume                 ;
	typedef units< 2, 1,-2,-1,-1, 0, 0> molar_heat_capacity          ;
	typedef units< 2, 0,-2,-1, 0, 0, 0> specific_heat_capacity       ;
	typedef units< 2, 1,-2, 0,-1, 0, 0> molar_energy                 ;
	typedef units< 2, 0,-2, 0, 0, 0, 0> specific_energy              ;
	typedef units< 1, 1,-3,-1, 0, 0, 0> thermal_conductivity         ;
	typedef units<-1, 0, 0, 0, 0, 0, 0> wavenumber                   ;
	typedef units< 2, 1,-3, 0, 0,-1, 0> voltage                      ;
	typedef units< 0, 0, 1, 0, 0, 1, 0> electric_charge              ;
	typedef units<-2,-1, 4, 0, 0, 2, 0> electric_capacitance         ;
	typedef units< 2, 1,-3, 0, 0,-2, 0> electric_resistance          ;
	typedef units<-2,-1, 3, 0, 0, 2, 0> electric_conductance         ;
	typedef units< 2, 1,-2, 0, 0,-2, 0> electric_inductance          ;
	typedef units< 2, 1,-2, 0, 0,-2, 0> magnetic_flux                ;
	typedef units< 0, 1,-2, 0, 0,-1, 0> magnetic_induction           ;
	typedef units< 0, 1,-2, 0, 0,-1, 0> magnetic_flux_density        ;
	typedef units< 0, 0, 0, 0, 0, 0, 1> luminous_flux                ;
	typedef units<-2, 0, 0, 0, 0, 0, 1> illuminance                  ;
	typedef units< 0, 0,-1, 0, 1, 0, 0> catalytic_activity           ;
	typedef units< 2, 0,-2, 0, 0, 0, 0> ionizing_radiation_dosage    ;
	typedef units< 0, 0,-1, 0, 0, 0, 0> radioactivity                ;



	#define SI_MODERN_PREFIXED_UNITS(quantity, suffix) \
	constexpr quantity yocto ## suffix (1e-24 * suffix); \
	constexpr quantity zepto ## suffix (1e-21 * suffix); \
	constexpr quantity  atto ## suffix (1e-18 * suffix); \
	constexpr quantity femto ## suffix (1e-15 * suffix); \
	constexpr quantity  pico ## suffix (1e-12 * suffix); \
	constexpr quantity  nano ## suffix (1e-9  * suffix); \
	constexpr quantity micro ## suffix (1e-6  * suffix); \
	constexpr quantity milli ## suffix (1e-3  * suffix); \
	constexpr quantity centi ## suffix (1e-2  * suffix); \
	constexpr quantity  deci ## suffix (1e-1  * suffix); \
	constexpr quantity  deca ## suffix (1e1   * suffix); \
	constexpr quantity hecto ## suffix (1e2   * suffix); \
	constexpr quantity  kilo ## suffix (1e2   * suffix); \
	constexpr quantity  mega ## suffix (1e6   * suffix); \
	constexpr quantity  giga ## suffix (1e9   * suffix); \
	constexpr quantity  tera ## suffix (1e12  * suffix); \
	constexpr quantity  peta ## suffix (1e15  * suffix); \
	constexpr quantity   exa ## suffix (1e18  * suffix); \
	constexpr quantity zetta ## suffix (1e21  * suffix); \
	constexpr quantity yotta ## suffix (1e24  * suffix); 

	#define SI_MODERN_DOUBLE_PREFIXED_UNITS(quantity, prefix, suffix) \
	constexpr quantity prefix ## yocto ## suffix (1e-24 * prefix ## suffix); \
	constexpr quantity prefix ## zepto ## suffix (1e-21 * prefix ## suffix); \
	constexpr quantity prefix ##  atto ## suffix (1e-18 * prefix ## suffix); \
	constexpr quantity prefix ## femto ## suffix (1e-15 * prefix ## suffix); \
	constexpr quantity prefix ##  pico ## suffix (1e-12 * prefix ## suffix); \
	constexpr quantity prefix ##  nano ## suffix (1e-9  * prefix ## suffix); \
	constexpr quantity prefix ## micro ## suffix (1e-6  * prefix ## suffix); \
	constexpr quantity prefix ## milli ## suffix (1e-3  * prefix ## suffix); \
	constexpr quantity prefix ## centi ## suffix (1e-2  * prefix ## suffix); \
	constexpr quantity prefix ##  deci ## suffix (1e-1  * prefix ## suffix); \
	constexpr quantity prefix ##  deca ## suffix (1e1   * prefix ## suffix); \
	constexpr quantity prefix ## hecto ## suffix (1e2   * prefix ## suffix); \
	constexpr quantity prefix ##  kilo ## suffix (1e2   * prefix ## suffix); \
	constexpr quantity prefix ##  mega ## suffix (1e6   * prefix ## suffix); \
	constexpr quantity prefix ##  giga ## suffix (1e9   * prefix ## suffix); \
	constexpr quantity prefix ##  tera ## suffix (1e12  * prefix ## suffix); \
	constexpr quantity prefix ##  peta ## suffix (1e15  * prefix ## suffix); \
	constexpr quantity prefix ##   exa ## suffix (1e18  * prefix ## suffix); \
	constexpr quantity prefix ## zetta ## suffix (1e21  * prefix ## suffix); \
	constexpr quantity prefix ## yotta ## suffix (1e24  * prefix ## suffix); 

	constexpr mass gram(1e-3);
	SI_MODERN_PREFIXED_UNITS(mass, gram)

	constexpr length  meter(1.0);
	SI_MODERN_PREFIXED_UNITS(length, meter)

	constexpr length  metre (1.0);
	SI_MODERN_PREFIXED_UNITS(length, metre)

	constexpr time  second (1.0);
	SI_MODERN_PREFIXED_UNITS(time, second)

	constexpr temperature  kelvin (1.0);
	SI_MODERN_PREFIXED_UNITS(temperature, kelvin)

	constexpr amount  mole (1.0);
	SI_MODERN_PREFIXED_UNITS(amount, mole)

	constexpr force  newton (1.0);
	SI_MODERN_PREFIXED_UNITS(force, newton)

	constexpr energy  joule (1.0);
	SI_MODERN_PREFIXED_UNITS(energy, joule)

	constexpr power  watt (1.0);
	SI_MODERN_PREFIXED_UNITS(power, watt)

	constexpr pressure  pascal (1.0);
	SI_MODERN_PREFIXED_UNITS(pressure, pascal)

	constexpr frequency  hertz (1.0);
	SI_MODERN_PREFIXED_UNITS(frequency, hertz)

	constexpr current  ampere (1.0);
	SI_MODERN_PREFIXED_UNITS(current, ampere)

	constexpr electric_charge  coulomb (1.0);
	SI_MODERN_PREFIXED_UNITS(electric_charge, coulomb)

	constexpr voltage  volt (1.0);
	SI_MODERN_PREFIXED_UNITS(voltage, volt)

	constexpr electric_capacitance farad (1.0);
	SI_MODERN_PREFIXED_UNITS(electric_capacitance, farad)

	constexpr electric_resistance  ohm (1.0);
	SI_MODERN_PREFIXED_UNITS(electric_resistance, ohm)

	constexpr electric_conductance  siemens (1.0);
	SI_MODERN_PREFIXED_UNITS(electric_conductance, siemens)

	constexpr magnetic_flux  weber (1.0);
	SI_MODERN_PREFIXED_UNITS(magnetic_flux, weber)

	constexpr magnetic_induction  tesla (1.0);
	SI_MODERN_PREFIXED_UNITS(magnetic_induction, tesla)

	constexpr electric_inductance  henry (1.0);
	SI_MODERN_PREFIXED_UNITS(electric_inductance, henry)

	constexpr catalytic_activity  katal (1.0);
	SI_MODERN_PREFIXED_UNITS(catalytic_activity, katal)

	constexpr luminous_intensity  candela (1.0);
	SI_MODERN_PREFIXED_UNITS(luminous_intensity, candela)

	constexpr luminous_flux  lumen (1.0);
	SI_MODERN_PREFIXED_UNITS(luminous_flux, lumen)

	constexpr illuminance  lux (1.0);
	SI_MODERN_PREFIXED_UNITS(illuminance, lux)

	constexpr radioactivity  becquerel (1.0);
	SI_MODERN_PREFIXED_UNITS(radioactivity, becquerel)

	constexpr ionizing_radiation_dosage  gray (1.0);
	SI_MODERN_PREFIXED_UNITS(ionizing_radiation_dosage, gray)

	constexpr ionizing_radiation_dosage  sievert (1.0);
	SI_MODERN_PREFIXED_UNITS(ionizing_radiation_dosage, sievert)

	/*
	special consideration is needed for exponents of base units, 
	whose values do not scale as described with SI_MODERN_PREFIXED_UNITS
	*/
	constexpr area       meter2 (1.0);
	constexpr  area yoctometer2 (1e-48);
	constexpr  area zeptometer2 (1e-42);
	constexpr  area  attometer2 (1e-36);
	constexpr  area femtometer2 (1e-30);
	constexpr  area  picometer2 (1e-24);
	constexpr  area  nanometer2 (1e-18);
	constexpr  area micrometer2 (1e-12);
	constexpr  area millimeter2 (1e-6 );
	constexpr  area centimeter2 (1e-4 );
	constexpr  area  decimeter2 (1e2  );
	constexpr  area  decameter2 (1e-2 );
	constexpr  area hectometer2 (1e4  );
	constexpr  area  kilometer2 (1e6  );
	constexpr  area  megameter2 (1e12 );
	constexpr  area  gigameter2 (1e18 );
	constexpr  area  terameter2 (1e24 );
	constexpr  area  petameter2 (1e30 );
	constexpr  area   exameter2 (1e36 );
	constexpr  area zettameter2 (1e42 );
	constexpr  area yottameter2 (1e48 );

	constexpr volume      meter3 (1.0);
	constexpr volume yoctometer3 (1e-72);
	constexpr volume zeptometer3 (1e-63);
	constexpr volume  attometer3 (1e-54);
	constexpr volume femtometer3 (1e-45);
	constexpr volume  picometer3 (1e-36);
	constexpr volume  nanometer3 (1e-27);
	constexpr volume micrometer3 (1e-18);
	constexpr volume millimeter3 (1e-9 );
	constexpr volume centimeter3 (1e-6 );
	constexpr volume  decimeter3 (1e3  );
	constexpr volume  decameter3 (1e-3 );
	constexpr volume hectometer3 (1e6  );
	constexpr volume  kilometer3 (1e9  );
	constexpr volume  megameter3 (1e18 );
	constexpr volume  gigameter3 (1e27 );
	constexpr volume  terameter3 (1e36 );
	constexpr volume  petameter3 (1e45 );
	constexpr volume   exameter3 (1e54 );
	constexpr volume zettameter3 (1e63 );
	constexpr volume yottameter3 (1e72 );

	constexpr units<0,0,2,0,0,0,0> second2  (1.0);
	constexpr units<4,0,0,0,0,0,0> kelvin4 (1.0);
	constexpr amount molecule = mole/6.02214076e23;

	// SI DEFINED PHYSICAL CONSTANTS
	// all values for constants taken from https://www.nist.gov/pml/special-publication-330/sp-330-section-2#2.1

	constexpr electric_charge elementary_charge_constant (1.602176634e-19); // coulomb
	constexpr units<0, 0, 0, 0,-1, 0, 0> avogadro_constant          (6.02214076e23);  // 1/mole
	constexpr units<1, 0,-1, 0, 0, 0, 0> speed_of_light             (299792458.0);    // meter/second
	constexpr units<2, 1,-2,-1, 0, 0, 0> boltzmann_constant         (1.380649e-23);   // joule/kelvin
	constexpr units<2, 1,-2,-1,-1, 0, 0> universal_gas_constant     (8.314472);       // joule/kelvin/mole
	constexpr units<0, 1,-3,-4, 0, 0, 0> stephan_boltzmann_constant (5.670373e-8);    // watt/(meter2*kelvin4)
	constexpr units<2, 1,-1, 0, 0, 0, 0> planck_constant            (6.62607015e-34); // joule*second
	constexpr units<3,-1,-2, 0, 0, 0, 0> gravitational_constant     (6.67428e-11);    // meter3/(kilogram*second2)

	// NON SI UNITS MENTIONED IN THE SI
	// see https://en.wikipedia.org/wiki/Non-SI_units_mentioned_in_the_SI
	// NOTE: the "astronomical unit" is defined under "IAU units for astronomy"
	constexpr time minute    (second*60.0);
	constexpr time hour      (minute*60.0);
	constexpr time day       (hour*24.0);
	constexpr float radian   (1.0);
	constexpr float degree   (3.1415926535897932384626433 / 180.0);
	constexpr float arcminute(degree/60);
	constexpr float arcsecond(arcminute/60);
	constexpr mass dalton    (1.660538921e-27);

	constexpr area are (100.0);
	SI_MODERN_PREFIXED_UNITS(area, are)

	constexpr volume  liter (0.001);
	SI_MODERN_PREFIXED_UNITS(volume, liter)

	constexpr volume  litre (0.001);
	SI_MODERN_PREFIXED_UNITS(volume, litre)

	constexpr mass  tonne (1e3);
	SI_MODERN_PREFIXED_UNITS(mass, tonne)

	constexpr mass  metric_ton (1e3);
	SI_MODERN_DOUBLE_PREFIXED_UNITS(mass, metric_, ton)

	constexpr energy  electronvolt (1e3* 1.602176634e-19);
	SI_MODERN_PREFIXED_UNITS(energy, electronvolt)

	/*
	NON SI UNITS MENTIONED IN PAST VERSIONS OF THE SI
	NOTE: Our purpose in providing deprecated units is to allow reporting measurements from as many published sources 
	as possible without requiring conversion or subnamespaces.
	SI prefixes that were ratified after 1960 (such as "nano" and "giga" ) were never incorporated into CGS as a standard, 
	so no historical literature exists that could have used CGS with these prefixes.
	Use of these prefixes with CGS might indicate an error and should not be permitted by introducing these units.
	*/
	#define SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(quantity, suffix) \
	constexpr quantity micro ## suffix ( 1e-6  * suffix ); \
	constexpr quantity milli ## suffix ( 1e-3  * suffix ); \
	constexpr quantity centi ## suffix ( 1e-2  * suffix ); \
	constexpr quantity  deci ## suffix ( 1e-1  * suffix ); \
	constexpr quantity  deca ## suffix ( 1e1   * suffix ); \
	constexpr quantity hecto ## suffix ( 1e2   * suffix ); \
	constexpr quantity  kilo ## suffix ( 1e2   * suffix ); \
	constexpr quantity  mega ## suffix ( 1e6   * suffix );

	// see https://en.wikipedia.org/wiki/Non-SI_units_mentioned_in_the_SI
	constexpr length   angstrom ( 1e-10 );
	constexpr length   micron   ( 1e-6 );
	constexpr length   nautical_mile ( 1852.0 );
	constexpr speed    knots    ( speed(0.514444) );

	constexpr area  barn (100.0 * femtometer2);
	SI_MODERN_PREFIXED_UNITS(area, barn)

	constexpr pressure bar (1e5);
	SI_MODERN_PREFIXED_UNITS(pressure,  bar)

	constexpr pressure meter_mercury (1000.0 * 133.322387415);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure, meter_mercury)

	constexpr acceleration  gal (acceleration(0.1));
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(acceleration, gal)

	constexpr force  dyne (1e-5);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(force, dyne)

	constexpr energy erg (1e-7);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy, erg)

	constexpr pressure barye (1e-1);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure, barye)

	constexpr auto kayser (100.0 / meter);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, kayser)

	constexpr kinematic_viscosity stokes (1e-4);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(kinematic_viscosity, stokes)

	constexpr dynamic_viscosity poise (0.1);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(dynamic_viscosity, poise)

    constexpr units<-1, 0, 0, 0, 0, 1, 0> oersted (79.57747);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, oersted)

	constexpr auto gauss (1e-4 * tesla);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, gauss)

	constexpr auto maxwell (1e-8 * weber);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, maxwell)

	constexpr auto phot (1e4 * lux);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, phot)

	constexpr units<-2, 0, 0,   0, 0,   0, 1> stlib (1e-4);  // 1 candela/centimeter2
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, stlib)

	/*
	METRIC UNITS IN SCIENTIFIC LITERATURE UNMENTIONED BY SI
	NOTE: as mentioned above, the namespace is only concerned with *storing* measurements on disk using base SI units.
	Providing other measurement systems allows developers to report measurements from as many published sources 
	as possible without requiring conversion or subnamespaces.
	So we do include units of measure from other measurement systems,
	however they must have precedence within existing in scientific literature 
	that was targeted for international consumption sometime in the past century (that is, after 1920). 
	This includes calories, units of time, and units from the IAU standard, 
	but does not include U.S. customary units since any publication that references those units is not meant for international consumption.
	*/

	constexpr pressure torr (133.32);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure, torr)

	constexpr volumetric_flow sverdrup (1e6);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(volumetric_flow, sverdrup)

	constexpr force pond (9.806650*millinewton);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(force, pond)

	constexpr power metric_horsepower (735.49875);
	SI_MODERN_DOUBLE_PREFIXED_UNITS(power, metric_, horsepower)

	constexpr auto eotvos = units<0,0,-2, 0,0, 0,0>(1e-9);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, eotvos)

	constexpr auto jansky = units<0,1,-4, 0,0, 0,0>(1e-26);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, jansky)

	constexpr voltage abvolt (1e-8);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(voltage, abvolt)

	constexpr current abampere (10.0);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(current, abampere)

	constexpr electric_charge abcoulomb (10.0);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_charge, abcoulomb)

	constexpr electric_conductance absiemens (1e9);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_conductance, absiemens)

	constexpr electric_resistance abohm (1e-9);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_resistance, abohm)

	constexpr auto gilbert (1e-4);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, gilbert)

	constexpr electric_capacitance abfarad (1e9);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_capacitance, abfarad)


	constexpr electric_charge statcoulomb (3.33564e-10 );
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_charge, statcoulomb)

	constexpr electric_conductance statsiemens (1.112650*picosiemens);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_conductance, statsiemens)

	constexpr electric_resistance statohm (8.987551787e11);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_resistance, statohm)

	constexpr electric_capacitance statfarad (1.1126*picofarad);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_capacitance, statfarad)

	constexpr voltage statvolt (299.792458);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(voltage, statvolt)


	constexpr radioactivity curie (3.7e10);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(radioactivity, curie)

	constexpr radioactivity rutherford (1e6);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(radioactivity, rutherford)

	constexpr ionizing_radiation_dosage rad (0.010);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(ionizing_radiation_dosage, rad)

	constexpr ionizing_radiation_dosage rem (0.010);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(ionizing_radiation_dosage, rem)

	constexpr auto rontgen = units< 0,-1, 1, 0, 0, 1, 0>(2.58e-4);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, rontgen)


	/*
	NOTE: 
	"calorie" is an ambiguous unit that is conventionally distinguished only by its capitalization.
	all other units within the library follow SI rules regarding capitalization, 
	which is to never capitalize units, even when named after individuals, unless the unit is abbreviated.
	See https://english.stackexchange.com/questions/173906/should-units-of-measure-be-capitalized
	From the SI specification:
	  Unit names are normally printed in roman (upright) type, and they are treated like ordinary nouns. 
	  In English, the names of units start with a lower-case letter (even when the symbol for the unit begins with a capital letter), 
	  except at the beginning of a sentence or in capitalized material such as a title. 
	  In keeping with this rule, the correct spelling of the name of the unit with the symbol °C is "degree Celsius" 
  	  (the unit degree begins with a lower-case d and the modifier Celsius begins with an upper-case C because it is a proper name).
	*/
	constexpr energy      calorie (4.184);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy, calorie)

	constexpr energy      Calorie (4184.0);
	SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy, Calorie)

	/*
	COMMON UNITS OF TIME
	Some units of time are ambiguous. 
	If a unit has a commonly accepted definition in everyday parlance then we adopt that definition as a shorthand but provide longhand alternatives for clarity to readers.
	If there is no commonly accepted definition for everyday parlance then we deliberately omit the shorthand.
	*/
	constexpr time week     (7.0*day);

	/*
    There are several definitions for a "month", but in everyday parlance it refers to the time it takes 
    for the moon to return to a phase for an observer on earth, i.e. the "synodic month".
	*/
	constexpr time synodic_month (29.53059 * day);
	constexpr time sidereal_month (27.32166 * day);
	constexpr time tropical_month (27.32158 * day);
	constexpr time anomalistic_month (27.55455 * day);
	constexpr time nodal_month (27.21222 * day);
	constexpr time month (synodic_month);
	/*
    A "year" has been referenced in scientific literature, predominantly geology, anthropology, and astronomy.
    It is commonly used in combination with modern metric prefixes, namely "gigayear".
	As with "ton", when used with prefixes in literature the prefixed units are always larger than the base unit
    however the definition of the base "year" is ambiguous.
    In everyday parlance it refers to the time reported on a calendar, and since the Gregorian calendar is internationally adopted, this refers to the average Gregorian year.
	Both the IUPAC and the IUGS recommend adopting "annum" for clarity, which is 31556925.445 seconds or ~365.24219265 ephemeris days.
	*/

	constexpr time      annum (31556925.445 * second); // ~365.24219265 days
	SI_MODERN_PREFIXED_UNITS(time, annum)

	constexpr time gregorian_year (365.2425 * day);
	SI_MODERN_DOUBLE_PREFIXED_UNITS(time, gregorian_, year)

	constexpr time      julian_year (365.25 * day);
	SI_MODERN_DOUBLE_PREFIXED_UNITS(time, julian_, year)

	constexpr time      j2000_sidereal_year (365.25 * day);
	SI_MODERN_DOUBLE_PREFIXED_UNITS(time, j2000_sidereal_, year)

	constexpr time year      (gregorian_year);
	constexpr time decade    (10.0 * year);
	constexpr time century   (100.0* year);
	constexpr time millenium (1e3  * year);
	SI_MODERN_PREFIXED_UNITS(time, year)

	/*
	IAU UNITS FOR ASTRONOMY AND CONSTANTS FOR EARTH
	*/
	constexpr time         earth_age            ( 4.54e9 * year );
	constexpr mass         earth_mass           ( 5.972e24 );
	constexpr length       earth_radius         ( 6.367e6 );
	constexpr acceleration standard_gravity     ( 9.80665 );
	constexpr temperature  standard_temperature ( 273.15 );
	constexpr pressure     standard_pressure    ( 101325.0 );
	constexpr molar_volume standard_molar_volume( 22.414);
	constexpr length       astronomical_unit    ( 149597870700.0 );
	constexpr intensity    global_solar_constant( 1361.0 );

	constexpr mass         jupiter_mass         ( 1.898e27 );
	constexpr length       jupiter_radius       ( 71e6 ); 

	constexpr mass         solar_mass           ( 2e30 );
	constexpr length       solar_radius         ( 695.7e6 );
	constexpr power        solar_luminosity     ( 3.828e26 );
	constexpr temperature  solar_temperature    ( 5772.0 );

	constexpr length       lightyear            ( 9460730472580800.0 );
	constexpr length       parsec               ( 3.0857e16 );

	constexpr time         observable_universe_age          ( 13.787e9 * year );
	constexpr length       observable_universe_radius       ( 4.4e26 );
	constexpr mass         observable_universe_ordinary_mass( 1.5e53 );
	constexpr volume       observable_universe_volume       ( 4e80 );
}
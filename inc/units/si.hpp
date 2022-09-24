#pragma once

#include <cmath>       /* log10, floor, pow, abs, sqrt */
#include <algorithm>    // std::min

namespace si{
  /*
  `si` is a principled units library. It minimizes class constructs, maximizes readability, and has a single well-defined purpose: 
    To store scalar quantities in terms of the six Standard International ("SI") base units.

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

    * The library should minimize the amount of explicit unit conversion that occurs when reporting measurements.
      This applies for both unit conversion in the developer's head as well as in the developer's code.
      So in contrast to the `si::units` class template, which is as simple as possible, 
      the list of built-in measures (e.g. `si::length<double>`) and units of measure (e.g. `si::kilometer`) 
      should be deliberately made as *extensive* as possible.
      Combined with readability constraints mentioned above, this means the library should include all combinations 
      of officially recognized SI prefixes and units of measure as unique variables
      (e.g. `si::hectometer`, not `si::hecto*si::meter`), 
      and units of measure from other common measurement systems (e.g. `si::hour`, `si::poise`, `si::earth_radius`),
      or common exponents of measures (e.g. `si::kelvin4`)
      Please note this principle does not apply to plurality variants that are meant for convenience (e.g. not `si::kilometers`)
      or variants that could be written using operators instead (e.g. not `si::kilometer_per_second` or `si::kmps`)
    We settle upon including all common units of time, units from the CGS, and astronomical units from the IAU standard.

    * The library should provide default measurement types (e.g. `si::length<double>`) 
      that allow work over the same range and precision afforded by the underlying system of measurements. 
      Since SI is designed to express measurements over a very wide range in orders of magnitude,
      some extremes cannot be expressed using floats (e.g. cubic yottameters)
    so all built-in measures (e.g. `si::length<double>`) should handle doubles by default.
      However, template aliases should be provided for all built in measures 
      to allow easily specifying other underlying types (e.g. `si::length<float>`)
  */

  template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1=double>
  class units
  {
    T1 raw;
  public:
    // zero constructor
    constexpr explicit units(): raw(T1(0))
    {

    }

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
    constexpr units<M1/2,KG1/2,S1/2,K1/2,MOL1/2,A1/2,CD1/2,T1> sqrt() const
    {
      return units<M1/2,KG1/2,S1/2,K1/2,MOL1/2,A1/2,CD1/2,T1>(std::sqrt(raw));
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
      std::array<std::string, 17> prefixes {"y","z","a","f","p","n","μ","m","","k","M","G","T","P","E","Z","Y"};

      // customize formatting for area and volume, which follow separate rules for prefix conversion
      if( M1 != 0 && (KG1|S1|K1|MOL1|A1|CD1) == 0)
      {
        T1 prefix_id = std::floor(std::log10(raw)/3.0); prefix_id = std::abs(prefix_id) < 8? prefix_id : 0;
        std::string prefixed_value = std::to_string(raw / std::pow(1000.0, M1*std::min(8.0, prefix_id)));
        std::string prefix(prefixes[int(prefix_id)+8]);
        return prefixed_value + " " + (M1<0? "1/":"") + prefix + "m" + (std::abs(M1)!=1? std::to_string(std::abs(M1)) : "");
      }
      // customize formatting for mass, which must be converted to grams
      if( KG1 != 0 && (M1|S1|K1|MOL1|A1|CD1) == 0)
      {
        T1 prefix_id = std::floor(std::log10(raw)/3.0); prefix_id = std::abs(prefix_id) < 8? prefix_id : 0;
        std::string prefixed_value = std::to_string(raw / std::pow(1000.0, KG1*std::min(8.0, prefix_id)));
        std::string prefix(prefixes[int(prefix_id)+KG1+8]);
        return prefixed_value + " " + (KG1<0? "1/":"") + prefix + "g" + (std::abs(KG1)!=1? std::to_string(std::abs(KG1)) : "");
      }
      // customize formatting for temperature, which conventionally avoids the "kilo" suffix
      if( K1 == 1 && (M1|KG1|S1|MOL1|A1|CD1) == 0)
      {
        T1 prefix_id = std::floor(std::log10(raw)/3.0); prefix_id = std::abs(prefix_id) < 8? prefix_id : 0;
        prefix_id = prefixes[int(prefix_id)+8] == "k"? prefix_id - 1 : prefix_id;
        std::string prefixed_value =  std::to_string(raw / std::pow(1000.0, std::min(8.0, prefix_id)));
        std::string prefix(prefixes[int(prefix_id)+8]);
        return prefixed_value + " " + prefix + "K";
      }

      using triple = std::array<int,3>;
      using pair = std::pair<triple, std::string>;
      // customize formatting for common derived units for pretty printing
      std::array<pair, 10> named_mks {
        pair({ 1,1,-2}, "N"),
        pair({ 2,1,-2}, "N*m"),
        pair({-1,1,-2}, "Pa"),
        pair({-1,1,-1}, "Pa*s"),
        pair({ 2,1,-2}, "J"),
        pair({ 2,0,-2}, "J/kg"),
        pair({ 2,1,-3}, "W"),
        pair({ 1,1,-3}, "W/m"),
        pair({ 0,1,-3}, "W/m2"),
        pair({ 0,1,-2}, "N/m"),
      };
      for(std::size_t i(0); i < named_mks.size(); ++i)
      {
        if(named_mks[i].first == std::array<int,3>{M1,KG1,S1})
        {
          T1 prefix_id = std::floor(std::log10(raw)/3.0); prefix_id = std::abs(prefix_id) < 8? prefix_id : 0;
          std::string prefixed_value =  std::to_string(raw / std::pow(1000.0, std::min(8.0, prefix_id)));
          std::string prefix(prefixes[int(prefix_id)+8]);
          std::string result = prefixed_value + " " + prefix;
          result += named_mks[i].second;
          result +=   K1>0? "K"  + (std::abs(  K1)>1? std::to_string(std::abs(K1  )) : "") : ""; 
          result += MOL1>0? "mol"+ (std::abs(MOL1)>1? std::to_string(std::abs(MOL1)) : "") : ""; 
          result +=   A1>0? "A"  + (std::abs(  A1)>1? std::to_string(std::abs(  A1)) : "") : ""; 
          result +=  CD1>0? "Cd" + (std::abs( CD1)>1? std::to_string(std::abs( CD1)) : "") : ""; 
          result += K1<0 || MOL1<0 || A1<0 || CD1<0? "/" : "";
          result +=   K1<0? "K"  + (std::abs(  K1)<1? std::to_string(std::abs(K1  )) : "") : ""; 
          result += MOL1<0? "mol"+ (std::abs(MOL1)<1? std::to_string(std::abs(MOL1)) : "") : ""; 
          result +=   A1<0? "A"  + (std::abs(  A1)<1? std::to_string(std::abs(  A1)) : "") : ""; 
          result +=  CD1<0? "Cd" + (std::abs( CD1)<1? std::to_string(std::abs( CD1)) : "") : ""; 
          return result;
        }
      }

      // fall back on SI base units 
      std::string result = std::to_string(raw) + " ";
      result += M1<0 && KG1<0 && S1<0 && K1<0 && MOL1<0 && A1<0 && CD1<0? "1" : "";
      result +=   M1>0? "m"  + (std::abs(  M1)>1? std::to_string(std::abs(M1  )) : "") : ""; 
      result +=  KG1>0? "kg" + (std::abs( KG1)>1? std::to_string(std::abs(KG1 )) : "") : ""; 
      result +=   S1>0? "s"  + (std::abs(  S1)>1? std::to_string(std::abs(S1  )) : "") : ""; 
      result +=   K1>0? "K"  + (std::abs(  K1)>1? std::to_string(std::abs(K1  )) : "") : ""; 
      result += MOL1>0? "mol"+ (std::abs(MOL1)>1? std::to_string(std::abs(MOL1)) : "") : ""; 
      result +=   A1>0? "A"  + (std::abs(  A1)>1? std::to_string(std::abs(  A1)) : "") : ""; 
      result +=  CD1>0? "Cd" + (std::abs( CD1)>1? std::to_string(std::abs( CD1)) : "") : ""; 
      result += M1<0 || KG1<0 || S1<0 || K1<0 || MOL1<0 || A1<0 || CD1<0? "/" : "";
      result +=   M1<0? "m"  + (std::abs(  M1)<1? std::to_string(std::abs(M1  )) : "") : ""; 
      result +=  KG1<0? "kg" + (std::abs( KG1)<1? std::to_string(std::abs(KG1 )) : "") : ""; 
      result +=   S1<0? "s"  + (std::abs(  S1)<1? std::to_string(std::abs(S1  )) : "") : ""; 
      result +=   K1<0? "K"  + (std::abs(  K1)<1? std::to_string(std::abs(K1  )) : "") : ""; 
      result += MOL1<0? "mol"+ (std::abs(MOL1)<1? std::to_string(std::abs(MOL1)) : "") : ""; 
      result +=   A1<0? "A"  + (std::abs(  A1)<1? std::to_string(std::abs(  A1)) : "") : ""; 
      result +=  CD1<0? "Cd" + (std::abs( CD1)<1? std::to_string(std::abs( CD1)) : "") : ""; 
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
  constexpr auto sqrt(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
  {
    return a.sqrt();
  }

  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1, typename T2>
  constexpr auto operator*(const T2 a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
  {
    return b.multiply(a);
  }
  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1, typename T2>
  constexpr auto operator/(const T2 a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
  {
    return b.invert().multiply(a);
  }

  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1, typename T2>
  constexpr auto operator*(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const T2 b)
  {
    return a.multiply(b);
  }
  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1, typename T2>
  constexpr auto operator/(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const T2 b)
  {
    return a.multiply(T1(1)/b);
  }

  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1,  int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2, typename T1,
    std::enable_if_t<M1!=-M2||KG1!=-KG2||S1!=-S2||K1!=-K2||MOL1!=-MOL2||A1!=-A2||CD1!=-CD2, int> = 0>
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
  constexpr T1 operator*(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<-M1,-KG1,-S1,-K1,-MOL1,-A1,-CD1,T1> b)
  {
    return si::unitless(b.multiply(a));
  }
  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
  constexpr T1 operator/(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
  {
    return si::unitless(b.invert().multiply(a));
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

  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
  constexpr auto clamp(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> lo, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> hi)
  {
    return a < lo? lo : a > hi? hi : a;
  }

  template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
  std::string to_string(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
  {
    return a.to_string();
  }


















                                                    //           m kg  s  K mol A Cd
  template <typename T> using length                    = units< 1, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using mass                      = units< 0, 1, 0, 0, 0, 0, 0, T>;
  template <typename T> using time                      = units< 0, 0, 1, 0, 0, 0, 0, T>;
  template <typename T> using temperature               = units< 0, 0, 0, 1, 0, 0, 0, T>;
  template <typename T> using amount                    = units< 0, 0, 0, 0, 1, 0, 0, T>;
  template <typename T> using current                   = units< 0, 0, 0, 0, 0, 1, 0, T>;
  template <typename T> using luminous_intensity        = units< 0, 0, 0, 0, 0, 0, 1, T>;
  template <typename T> using area                      = units< 2, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using volume                    = units< 3, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using volumetric_flow           = units< 3, 0,-1, 0, 0, 0, 0, T>;
  template <typename T> using area_density              = units<-2, 1, 0, 0, 0, 0, 0, T>;
  template <typename T> using density                   = units<-3, 1, 0, 0, 0, 0, 0, T>;
  template <typename T> using number_density            = units<-3, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using molar_density             = units<-3, 0, 0, 0, 1, 0, 0, T>;
  template <typename T> using frequency                 = units< 0, 0,-1, 0, 0, 0, 0, T>;
  template <typename T> using speed                     = units< 1, 0,-1, 0, 0, 0, 0, T>;
  template <typename T> using acceleration              = units< 1, 0,-2, 0, 0, 0, 0, T>;
  template <typename T> using momentum                  = units< 1, 1,-1, 0, 0, 0, 0, T>;
  template <typename T> using force                     = units< 1, 1,-2, 0, 0, 0, 0, T>;
  template <typename T> using angular_velocity          = units< 0, 0,-1, 0, 0, 0, 0, T>;
  template <typename T> using angular_acceleration      = units< 0, 0,-2, 0, 0, 0, 0, T>;
  template <typename T> using angular_momentum          = units< 2, 1,-1, 0, 0, 0, 0, T>;
  template <typename T> using torque                    = units< 2, 1,-2, 0, 0, 0, 0, T>;
  template <typename T> using pressure                  = units<-1, 1,-2, 0, 0, 0, 0, T>;
  template <typename T> using kinematic_viscosity       = units< 2, 0,-1, 0, 0, 0, 0, T>;
  template <typename T> using dynamic_viscosity         = units<-1, 1,-1, 0, 0, 0, 0, T>;
  template <typename T> using energy                    = units< 2, 1,-2, 0, 0, 0, 0, T>;
  template <typename T> using surface_energy            = units< 0, 1,-2, 0, 0, 0, 0, T>;
  template <typename T> using power                     = units< 2, 1,-3, 0, 0, 0, 0, T>;
  template <typename T> using intensity                 = units< 0, 1,-3, 0, 0, 0, 0, T>;
  template <typename T> using molar_mass                = units< 0, 1, 0, 0,-1, 0, 0, T>;
  template <typename T> using molar_volume              = units< 3, 0, 0, 0,-1, 0, 0, T>;
  template <typename T> using molar_heat_capacity       = units< 2, 1,-2,-1,-1, 0, 0, T>;
  template <typename T> using specific_heat_capacity    = units< 2, 0,-2,-1, 0, 0, 0, T>;
  template <typename T> using molar_energy              = units< 2, 1,-2, 0,-1, 0, 0, T>;
  template <typename T> using specific_energy           = units< 2, 0,-2, 0, 0, 0, 0, T>;
  template <typename T> using thermal_conductivity      = units< 1, 1,-3,-1, 0, 0, 0, T>;
  template <typename T> using wavenumber                = units<-1, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using attenuation               = units<-1, 0, 0, 0, 0, 0, 0, T>;
  template <typename T> using voltage                   = units< 2, 1,-3, 0, 0,-1, 0, T>;
  template <typename T> using electric_charge           = units< 0, 0, 1, 0, 0, 1, 0, T>;
  template <typename T> using electric_capacitance      = units<-2,-1, 4, 0, 0, 2, 0, T>;
  template <typename T> using electric_resistance       = units< 2, 1,-3, 0, 0,-2, 0, T>;
  template <typename T> using electric_conductance      = units<-2,-1, 3, 0, 0, 2, 0, T>;
  template <typename T> using electric_inductance       = units< 2, 1,-2, 0, 0,-2, 0, T>;
  template <typename T> using magnetic_flux             = units< 2, 1,-2, 0, 0,-2, 0, T>;
  template <typename T> using magnetic_induction        = units< 0, 1,-2, 0, 0,-1, 0, T>;
  template <typename T> using magnetic_flux_density     = units< 0, 1,-2, 0, 0,-1, 0, T>;
  template <typename T> using luminous_flux             = units< 0, 0, 0, 0, 0, 0, 1, T>;
  template <typename T> using illuminance               = units<-2, 0, 0, 0, 0, 0, 1, T>;
  template <typename T> using catalytic_activity        = units< 0, 0,-1, 0, 1, 0, 0, T>;
  template <typename T> using ionizing_radiation_dosage = units< 2, 0,-2, 0, 0, 0, 0, T>;
  template <typename T> using radioactivity             = units< 0, 0,-1, 0, 0, 0, 0, T>;




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
  constexpr quantity  kilo ## suffix (1e3   * suffix); \
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
  constexpr quantity prefix ##  kilo ## suffix (1e3   * prefix ## suffix); \
  constexpr quantity prefix ##  mega ## suffix (1e6   * prefix ## suffix); \
  constexpr quantity prefix ##  giga ## suffix (1e9   * prefix ## suffix); \
  constexpr quantity prefix ##  tera ## suffix (1e12  * prefix ## suffix); \
  constexpr quantity prefix ##  peta ## suffix (1e15  * prefix ## suffix); \
  constexpr quantity prefix ##   exa ## suffix (1e18  * prefix ## suffix); \
  constexpr quantity prefix ## zetta ## suffix (1e21  * prefix ## suffix); \
  constexpr quantity prefix ## yotta ## suffix (1e24  * prefix ## suffix); 

  constexpr mass<double> gram(1e-3);
  SI_MODERN_PREFIXED_UNITS(mass<double>, gram)

  constexpr length<double>  meter(1.0);
  SI_MODERN_PREFIXED_UNITS(length<double>, meter)

  constexpr length<double>  metre (1.0);
  SI_MODERN_PREFIXED_UNITS(length<double>, metre)

  constexpr time<double>  second (1.0);
  SI_MODERN_PREFIXED_UNITS(time<double>, second)

  constexpr temperature<double>  kelvin (1.0);
  SI_MODERN_PREFIXED_UNITS(temperature<double>, kelvin)

  constexpr amount<double>  mole (1.0);
  SI_MODERN_PREFIXED_UNITS(amount<double>, mole)

  constexpr force<double>  newton (1.0);
  SI_MODERN_PREFIXED_UNITS(force<double>, newton)

  constexpr energy<double>  joule (1.0);
  SI_MODERN_PREFIXED_UNITS(energy<double>, joule)

  constexpr power<double>  watt (1.0);
  SI_MODERN_PREFIXED_UNITS(power<double>, watt)

  constexpr pressure<double>  pascal (1.0);
  SI_MODERN_PREFIXED_UNITS(pressure<double>, pascal)

  constexpr frequency<double>  hertz (1.0);
  SI_MODERN_PREFIXED_UNITS(frequency<double>, hertz)

  constexpr current<double>  ampere (1.0);
  SI_MODERN_PREFIXED_UNITS(current<double>, ampere)

  constexpr electric_charge<double>  coulomb (1.0);
  SI_MODERN_PREFIXED_UNITS(electric_charge<double>, coulomb)

  constexpr voltage<double>  volt (1.0);
  SI_MODERN_PREFIXED_UNITS(voltage<double>, volt)

  constexpr electric_capacitance<double> farad (1.0);
  SI_MODERN_PREFIXED_UNITS(electric_capacitance<double>, farad)

  constexpr electric_resistance<double>  ohm (1.0);
  SI_MODERN_PREFIXED_UNITS(electric_resistance<double>, ohm)

  constexpr electric_conductance<double>  siemens (1.0);
  SI_MODERN_PREFIXED_UNITS(electric_conductance<double>, siemens)

  constexpr magnetic_flux<double>  weber (1.0);
  SI_MODERN_PREFIXED_UNITS(magnetic_flux<double>, weber)

  constexpr magnetic_induction<double>  tesla (1.0);
  SI_MODERN_PREFIXED_UNITS(magnetic_induction<double>, tesla)

  constexpr electric_inductance<double>  henry (1.0);
  SI_MODERN_PREFIXED_UNITS(electric_inductance<double>, henry)

  constexpr catalytic_activity<double>  katal (1.0);
  SI_MODERN_PREFIXED_UNITS(catalytic_activity<double>, katal)

  constexpr luminous_intensity<double>  candela (1.0);
  SI_MODERN_PREFIXED_UNITS(luminous_intensity<double>, candela)

  constexpr luminous_flux<double>  lumen (1.0);
  SI_MODERN_PREFIXED_UNITS(luminous_flux<double>, lumen)

  constexpr illuminance<double>  lux (1.0);
  SI_MODERN_PREFIXED_UNITS(illuminance<double>, lux)

  constexpr radioactivity<double>  becquerel (1.0);
  SI_MODERN_PREFIXED_UNITS(radioactivity<double>, becquerel)

  constexpr ionizing_radiation_dosage<double>  gray (1.0);
  SI_MODERN_PREFIXED_UNITS(ionizing_radiation_dosage<double>, gray)

  constexpr ionizing_radiation_dosage<double>  sievert (1.0);
  SI_MODERN_PREFIXED_UNITS(ionizing_radiation_dosage<double>, sievert)

  /*
  special consideration is needed for exponents of base units, 
  whose values do not scale as described with SI_MODERN_PREFIXED_UNITS
  */
  constexpr area<double>      meter2 (1.0);
  constexpr area<double> yoctometer2 (1e-48);
  constexpr area<double> zeptometer2 (1e-42);
  constexpr area<double>  attometer2 (1e-36);
  constexpr area<double> femtometer2 (1e-30);
  constexpr area<double>  picometer2 (1e-24);
  constexpr area<double>  nanometer2 (1e-18);
  constexpr area<double> micrometer2 (1e-12);
  constexpr area<double> millimeter2 (1e-6 );
  constexpr area<double> centimeter2 (1e-4 );
  constexpr area<double>  decimeter2 (1e-2 );
  constexpr area<double>  decameter2 (1e2  );
  constexpr area<double> hectometer2 (1e4  );
  constexpr area<double>  kilometer2 (1e6  );
  constexpr area<double>  megameter2 (1e12 );
  constexpr area<double>  gigameter2 (1e18 );
  constexpr area<double>  terameter2 (1e24 );
  constexpr area<double>  petameter2 (1e30 );
  constexpr area<double>   exameter2 (1e36 );
  constexpr area<double> zettameter2 (1e42 );
  constexpr area<double> yottameter2 (1e48 );

  constexpr volume<double>      meter3 (1.0);
  constexpr volume<double> yoctometer3 (1e-72);
  constexpr volume<double> zeptometer3 (1e-63);
  constexpr volume<double>  attometer3 (1e-54);
  constexpr volume<double> femtometer3 (1e-45);
  constexpr volume<double>  picometer3 (1e-36);
  constexpr volume<double>  nanometer3 (1e-27);
  constexpr volume<double> micrometer3 (1e-18);
  constexpr volume<double> millimeter3 (1e-9 );
  constexpr volume<double> centimeter3 (1e-6 );
  constexpr volume<double>  decimeter3 (1e-3 );
  constexpr volume<double>  decameter3 (1e3  );
  constexpr volume<double> hectometer3 (1e6  );
  constexpr volume<double>  kilometer3 (1e9  );
  constexpr volume<double>  megameter3 (1e18 );
  constexpr volume<double>  gigameter3 (1e27 );
  constexpr volume<double>  terameter3 (1e36 );
  constexpr volume<double>  petameter3 (1e45 );
  constexpr volume<double>   exameter3 (1e54 );
  constexpr volume<double> zettameter3 (1e63 );
  constexpr volume<double> yottameter3 (1e72 );

  constexpr units<0,0,2,0,0,0,0> second2  (1.0);
  constexpr units<4,0,0,0,0,0,0> kelvin4 (1.0);
  constexpr amount<double> molecule = mole/6.02214076e23;

  // SI DEFINED PHYSICAL CONSTANTS
  // all values for constants taken from https://www.nist.gov/pml/special-publication-330/sp-330-section-2#2.1

  constexpr electric_charge<double> elementary_charge_constant (1.602176634e-19); // coulomb
  constexpr units<0, 0, 0, 0,-1, 0, 0, double> avogadro_constant          (6.02214076e23);  // 1/mole
  constexpr units<1, 0,-1, 0, 0, 0, 0, double> speed_of_light             (299792458.0);    // meter/second
  constexpr units<2, 1,-2,-1, 0, 0, 0, double> boltzmann_constant         (1.380649e-23);   // joule/kelvin
  constexpr units<2, 1,-2,-1,-1, 0, 0, double> universal_gas_constant     (8.314472);       // joule/kelvin/mole
  constexpr units<0, 1,-3,-4, 0, 0, 0, double> stephan_boltzmann_constant (5.670373e-8);    // watt/(meter2*kelvin4)
  constexpr units<2, 1,-1, 0, 0, 0, 0, double> planck_constant            (6.62607015e-34); // joule*second
  constexpr units<3,-1,-2, 0, 0, 0, 0, double> gravitational_constant     (6.67428e-11);    // meter3/(kilogram*second2)

  // NON SI UNITS MENTIONED IN THE SI
  // see https://en.wikipedia.org/wiki/Non-SI_units_mentioned_in_the_SI
  // NOTE: the "astronomical unit" is defined under "IAU units for astronomy"
  constexpr time<double> minute    (second*60.0);
  constexpr time<double> hour      (minute*60.0);
  constexpr time<double> day       (hour*24.0);
  constexpr float radian   (1.0);
  constexpr float degree   (3.1415926535897932384626433 / 180.0);
  constexpr float arcminute(degree/60);
  constexpr float arcsecond(arcminute/60);
  constexpr mass<double> dalton    (1.660538921e-27);

  constexpr area<double> are (100.0);
  SI_MODERN_PREFIXED_UNITS(area<double>, are)

  constexpr volume<double>  liter (0.001);
  SI_MODERN_PREFIXED_UNITS(volume<double>, liter)

  constexpr volume<double>  litre (0.001);
  SI_MODERN_PREFIXED_UNITS(volume<double>, litre)

  constexpr mass<double>  tonne (1e3);
  SI_MODERN_PREFIXED_UNITS(mass<double>, tonne)

  constexpr mass<double>  metric_ton (1e3);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(mass<double>, metric_, ton)

  constexpr energy<double>  electronvolt (1e3* 1.602176634e-19);
  SI_MODERN_PREFIXED_UNITS(energy<double>, electronvolt)

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
  constexpr quantity  kilo ## suffix ( 1e3   * suffix ); \
  constexpr quantity  mega ## suffix ( 1e6   * suffix );

  // see https://en.wikipedia.org/wiki/Non-SI_units_mentioned_in_the_SI
  constexpr length<double>   angstrom ( 1e-10 );
  constexpr length<double>   micron   ( 1e-6 );
  constexpr length<double>   nautical_mile ( 1852.0 );
  constexpr speed<double>    knots    ( speed<double>(0.514444) );

  constexpr area<double>  barn (100.0 * femtometer2);
  SI_MODERN_PREFIXED_UNITS(area<double>, barn)

  constexpr pressure<double> bar (1e5);
  SI_MODERN_PREFIXED_UNITS(pressure<double>,  bar)

  constexpr pressure<double> meter_mercury (1000.0 * 133.322387415);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure<double>, meter_mercury)

  constexpr pressure<double> atmosphere ( 101325.0 );

  constexpr acceleration<double>  gal (acceleration<double>(0.1));
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(acceleration<double>, gal)

  constexpr force<double>  dyne (1e-5);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(force<double>, dyne)

  constexpr energy<double> erg (1e-7);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy<double>, erg)

  constexpr pressure<double> barye (1e-1);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure<double>, barye)

  constexpr auto kayser (100.0 / meter);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, kayser)

  constexpr kinematic_viscosity<double> stokes (1e-4);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(kinematic_viscosity<double>, stokes)

  constexpr dynamic_viscosity<double> poise (0.1);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(dynamic_viscosity<double>, poise)

    constexpr units<-1, 0, 0, 0, 0, 1, 0, double> oersted (79.57747);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, oersted)

  constexpr auto gauss (1e-4 * tesla);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, gauss)

  constexpr auto maxwell (1e-8 * weber);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, maxwell)

  constexpr auto phot (1e4 * lux);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, phot)

  constexpr units<-2, 0, 0,   0, 0,   0, 1, double> stlib (1e-4);  // 1 candela/centimeter2
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

  constexpr pressure<double> torr (133.32);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(pressure<double>, torr)

  constexpr volumetric_flow<double> sverdrup (1e6);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(volumetric_flow<double>, sverdrup)

  constexpr force<double> pond (9.806650*millinewton);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(force<double>, pond)

  constexpr power<double> metric_horsepower (735.49875);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(power<double>, metric_, horsepower)

  constexpr auto eotvos = units<0,0,-2, 0,0, 0,0>(1e-9);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, eotvos)

  constexpr auto jansky = units<0,1,-4, 0,0, 0,0>(1e-26);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, jansky)

  constexpr voltage<double> abvolt (1e-8);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(voltage<double>, abvolt)

  constexpr current<double> abampere (10.0);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(current<double>, abampere)

  constexpr electric_charge<double> abcoulomb (10.0);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_charge<double>, abcoulomb)

  constexpr electric_conductance<double> absiemens (1e9);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_conductance<double>, absiemens)

  constexpr electric_resistance<double> abohm (1e-9);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_resistance<double>, abohm)

  constexpr auto gilbert (1e-4);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(auto, gilbert)

  constexpr electric_capacitance<double> abfarad (1e9);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_capacitance<double>, abfarad)


  constexpr electric_charge<double> statcoulomb (3.33564e-10 );
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_charge<double>, statcoulomb)

  constexpr electric_conductance<double> statsiemens (1.112650*picosiemens);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_conductance<double>, statsiemens)

  constexpr electric_resistance<double> statohm (8.987551787e11);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_resistance<double>, statohm)

  constexpr electric_capacitance<double> statfarad (1.1126*picofarad);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(electric_capacitance<double>, statfarad)

  constexpr voltage<double> statvolt (299.792458);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(voltage<double>, statvolt)


  constexpr radioactivity<double> curie (3.7e10);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(radioactivity<double>, curie)

  constexpr radioactivity<double> rutherford (1e6);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(radioactivity<double>, rutherford)

  constexpr ionizing_radiation_dosage<double> rad (0.010);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(ionizing_radiation_dosage<double>, rad)

  constexpr ionizing_radiation_dosage<double> rem (0.010);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(ionizing_radiation_dosage<double>, rem)

  constexpr auto rontgen = units< 0,-1, 1, 0, 0, 1, 0, double>(2.58e-4);
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
  */
  constexpr energy<double>      calorie (4.184);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy<double>, calorie)

  constexpr energy<double>      Calorie (4184.0);
  SI_DEPRECATED_PRE_1960_PREFIXED_UNITS(energy<double>, Calorie)

  /*
  COMMON UNITS OF TIME
  Some units of time are ambiguous. 
  If a unit has a commonly accepted definition in everyday parlance then we adopt that definition as a shorthand but provide longhand alternatives for clarity to readers.
  If there is no commonly accepted definition for everyday parlance then we deliberately omit the shorthand.
  */
  constexpr time<double> week     (7.0*day);

  /*
    There are several definitions for a "month", but in everyday parlance it refers to the time it takes 
    for the moon to return to a phase for an observer on earth, i.e. the "synodic month".
  */
  constexpr time<double> synodic_month (29.53059 * day);
  constexpr time<double> sidereal_month (27.32166 * day);
  constexpr time<double> tropical_month (27.32158 * day);
  constexpr time<double> anomalistic_month (27.55455 * day);
  constexpr time<double> nodal_month (27.21222 * day);
  constexpr time<double> month (synodic_month);
  /*
    A "year" has been referenced in scientific literature, predominantly geology, anthropology, and astronomy.
    It is commonly used in combination with modern metric prefixes, namely "gigayear".
  As with "ton", when used with prefixes in literature the prefixed units are always larger than the base unit
    however the definition of the base "year" is ambiguous.
    In everyday parlance it refers to the time reported on a calendar, and since the Gregorian calendar is internationally adopted, this refers to the average Gregorian year.
  Both the IUPAC and the IUGS recommend adopting "annum" for clarity, which is 31556925.445 seconds or ~365.24219265 ephemeris days.
  */

  constexpr time<double>      annum (31556925.445 * second); // ~365.24219265 days
  SI_MODERN_PREFIXED_UNITS(time<double>, annum)

  constexpr time<double> tropical_year (365.24219265 * day);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(time<double>, tropical_, year)

  constexpr time<double> solar_year (365.24219265 * day);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(time<double>, solar_, year)

  constexpr time<double> gregorian_year (365.2425 * day);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(time<double>, gregorian_, year)

  constexpr time<double>      julian_year (365.25 * day);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(time<double>, julian_, year)

  constexpr time<double>      j2000_sidereal_year (365.25 * day);
  SI_MODERN_DOUBLE_PREFIXED_UNITS(time<double>, j2000_sidereal_, year)

  constexpr time<double> year      (gregorian_year);
  constexpr time<double> decade    (10.0 * year);
  constexpr time<double> century   (100.0* year);
  constexpr time<double> millenium (1e3  * year);
  SI_MODERN_PREFIXED_UNITS(time<double>, year)

  /*
  IAU UNITS FOR ASTRONOMY AND CONSTANTS FOR EARTH
  */
  constexpr time<double>         earth_age            ( 4.54e9 * year );
  constexpr mass<double>         earth_mass           ( 5.972e24 );
  constexpr length<double>       earth_radius         ( 6.367e6 );
  constexpr acceleration<double> standard_gravity     ( 9.80665 );
  constexpr temperature<double>  standard_temperature ( 273.15 );
  constexpr pressure<double>     standard_pressure    ( 101325.0 );
  constexpr molar_volume<double> standard_molar_volume( 22.414);
  constexpr length<double>       astronomical_unit    ( 149597870700.0 );
  constexpr intensity<double>    global_solar_constant( 1361.0 );

  constexpr mass<double>         jupiter_mass         ( 1.898e27 );
  constexpr length<double>       jupiter_radius       ( 71e6 ); 

  constexpr mass<double>         solar_mass           ( 1.9891e30 );
  constexpr length<double>       solar_radius         ( 695.7e6 );
  constexpr power<double>        solar_luminosity     ( 3.828e26 );
  constexpr temperature<double>  solar_temperature    ( 5772.0 );

  constexpr length<double>       lightyear            ( 9460730472580800.0 );
  constexpr length<double>       parsec               ( 3.0857e16 );

  constexpr time<double>         observable_universe_age          ( 13.787e9 * year );
  constexpr length<double>       observable_universe_radius       ( 4.4e26 );
  constexpr mass<double>         observable_universe_ordinary_mass( 1.5e53 );
  constexpr volume<double>       observable_universe_volume       ( 4e80 );


  template<typename T>
  struct celcius_type {};

  constexpr celcius_type<double> celcius;

  template<typename T1, typename T2>
  constexpr temperature<T1> operator*(const T1 C, const celcius_type<T2>)
  {
    return C*kelvin + standard_temperature;
  }
  template<typename T1, typename T2>
  constexpr T1 operator/(const temperature<T1> K, const celcius_type<T2>)
  {
    return ((K - standard_temperature) / kelvin);
  }
}
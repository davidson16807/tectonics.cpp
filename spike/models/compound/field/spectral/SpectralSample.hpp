#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{
	template<typename T>
    struct SpectralSample
    {
    	T entry;
        si::wavenumber<double> nlo;
        si::wavenumber<double> nhi;
    	si::pressure<double> pressure;
    	si::temperature<double> temperature;

    	SpectralSample(
            const T entry, 
            const si::wavenumber<double> nlo,
            const si::wavenumber<double> nhi,
            const si::pressure<double> pressure, 
            const si::temperature<double> temperature
        ): 
            entry(entry), 
            nlo(nlo),
            nhi(nhi),
            pressure(pressure), 
            temperature(temperature) 
        {}

	    constexpr bool operator==(const SpectralSample<T>& other) const 
	    {
	    	return 
               entry == other.entry 
            && nlo == other.nlo
            && nhi == other.nhi
            && pressure == other.pressure 
            && temperature == other.temperature;
	    }
        constexpr bool operator!=(const SpectralSample<T>& other) const 
        {
            return !((*this)==other);
        }
    };

}}
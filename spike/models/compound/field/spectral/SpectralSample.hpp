#pragma once

#include <si.hpp>

namespace compound { 
namespace field{
	template<typename T>
    struct SpectralSample
    {
    	T entry;
        si::wavenumber nlo;
        si::wavenumber nhi;
    	si::pressure pressure;
    	si::temperature temperature;

    	SpectralSample(
            const T entry, 
            const si::wavenumber nlo,
            const si::wavenumber nhi,
            const si::pressure pressure, 
            const si::temperature temperature
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
#pragma once

#include <si.hpp>

namespace compound { 
namespace field{
	template<typename T>
    struct SpectralSample
    {
    	T value;
        si::wavenumber nlo;
        si::wavenumber nhi;
    	si::pressure pressure;
    	si::temperature temperature;

    	SpectralSample(
            const T value, 
            const si::wavenumber nlo,
            const si::wavenumber nhi,
            const si::pressure pressure, 
            const si::temperature temperature
        ): 
            value(value), 
            nlo(nlo),
            nhi(nhi),
            pressure(pressure), 
            temperature(temperature) 
        {}

	    constexpr bool operator==(const SpectralSample& other) const 
	    {
	    	return 
               value == other.value 
            && nlo == other.nlo
            && nhi == other.nhi
            && pressure == other.pressure 
            && temperature == other.temperature;
	    }
    };

}}
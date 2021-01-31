#pragma once

#include <si.hpp>

namespace compound { 
namespace field{
	template<typename T>
    struct StateSample
    {
    	T value;
    	si::pressure pressure;
    	si::temperature temperature;

    	StateSample(T value, si::pressure pressure, si::temperature temperature)
    	: value(value), pressure(pressure), temperature(temperature) {}

	    constexpr bool operator==(const StateSample& other) const 
	    {
	    	return value == other.value && pressure == other.pressure && temperature == other.temperature;
	    }
    };

}}
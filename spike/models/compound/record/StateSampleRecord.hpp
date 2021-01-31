#pragma once

#include <si.hpp>

namespace compound { 
namespace record{
	template<typename T>
    struct StateSampleRecord
    {
    	T value;
    	si::pressure pressure;
    	si::temperature temperature;

    	StateSampleRecord(T value, si::pressure pressure, si::temperature temperature)
    	: value(value), pressure(pressure), temperature(temperature) {}

	    constexpr bool operator==(const StateSampleRecord& other) const 
	    {
	    	return value == other.value && pressure == other.pressure && temperature == other.temperature;
	    }
    };

}}
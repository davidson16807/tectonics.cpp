#pragma once

// std libraries
#include <functional>          // function

// in-house libraries
#include <units/si.hpp>

namespace compound { 
namespace field {
	template<typename T1>
    using SpectralFunction = std::function<T1(
    	const si::wavenumber<double> nlo, 
    	const si::wavenumber<double> nhi, 
    	const si::pressure<double> p, 
    	const si::temperature<double> T)>;
}}
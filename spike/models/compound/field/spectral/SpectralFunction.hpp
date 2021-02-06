#pragma once

// std libraries
#include <functional>          // function

// in-house libraries
#include <units/si.hpp>

namespace compound { 
namespace field {
	template<typename T1>
    using SpectralFunction = std::function<T1(
    	const si::wavenumber nlo, 
    	const si::wavenumber nhi, 
    	const si::pressure p, 
    	const si::temperature T)>;
}}
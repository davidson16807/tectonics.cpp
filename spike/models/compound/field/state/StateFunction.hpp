#pragma once

// std libraries
#include <functional>          // function

namespace compound { 
namespace field {
	template<typename T1>
    using StateFunction = std::function<T1(
    	const si::pressure p, 
    	const si::temperature T)>;
}}
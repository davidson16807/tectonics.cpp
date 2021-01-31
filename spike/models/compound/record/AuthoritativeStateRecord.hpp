#pragma once

// std libraries
#include <functional>          // function

namespace compound { 
namespace record {
	template<typename T1>
    using AuthoritativeStateRecord = std::function<T1(const si::pressure p, const si::temperature T)>;
}}
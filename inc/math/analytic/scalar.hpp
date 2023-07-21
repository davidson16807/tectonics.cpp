#pragma once

namespace math {

    constexpr float distance(const float a, const float b) 
    {
        return std::abs(a-b);
    }
    constexpr double distance(const double a, const double b) 
    {
        return std::abs(a-b);
    }

    constexpr float derivative(const float k) 
    {
        return 0.0f;
    }
    constexpr double derivative(const double k) 
    {
        return 0.0;
    }

}
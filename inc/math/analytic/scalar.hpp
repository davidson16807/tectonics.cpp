#pragma once

namespace analytic {

    constexpr float distance(const float a, const float b) 
    {
        float d = a - b;
        return d < 0.0f ? -d : d;
    }
    constexpr double distance(const double a, const double b) 
    {
        float d = a - b;
        return d < 0.0 ? -d : d;
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


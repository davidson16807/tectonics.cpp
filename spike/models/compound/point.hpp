#pragma once

#include <units/si.hpp>

namespace compound
{
    template<typename T>
    struct point{

        si::pressure<T> pressure;
        si::temperature<T> temperature;

        point(const si::pressure<T> pressure, const si::temperature<T> temperature) :
            pressure(pressure),
            temperature(temperature)
        {
        }

        point(const si::temperature<T> temperature, const si::pressure<T> pressure) :
            pressure(pressure),
            temperature(temperature)
        {
        }

        constexpr point<T>& operator=(const point<T>& other)
        {
            pressure = other.pressure;
            temperature = other.temperature;
            return *this;
        }

        point<T>& operator+=(const point<T> offset)
        {
            pressure += offset.pressure;
            temperature += offset.temperature;
            return *this;
        }

        point<T>& operator-=(const point<T> offset)
        {
            pressure -= offset.pressure;
            temperature -= offset.temperature;
            return *this;
        }

        point<T>& operator*=(const T scalar)
        {
            pressure *= scalar;
            temperature *= scalar;
            return *this;
        }

        point<T>& operator/=(const T scalar)
        {
            pressure /= scalar;
            temperature /= scalar;
            return *this;
        }

    };

    template<typename T>
    point<T> operator+(const point<T>& relation, const point<T>& other)
    {
        point<T> result = relation;
        result += other;
        return result;
    }

    template<typename T>
    point<T> operator-(const point<T>& relation, const point<T>& other)
    {
        point<T> result = relation;
        result -= other;
        return result;
    }

    template<typename T>
    point<T> operator*(const point<T>& relation, const T scalar)
    {
        point<T> result = relation;
        result *= scalar;
        return result;
    }

    template<typename T>
    point<T> operator*(const T scalar, const point<T>& relation)
    {
        point<T> result = relation;
        result *= scalar;
        return result;
    }

    template<typename T>
    point<T> operator/(const point<T>& relation, const T scalar)
    {
        point<T> result = relation;
        result /= scalar;
        return result;
    }

    template<typename T>
    point<T> operator-(const point<T>& relation)
    {
        point<T> result = relation;
        result *= T(-1);
        return result;
    }


}
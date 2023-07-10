#pragma once

#include <units/si.hpp>

namespace compound {
namespace phase {

    struct LinearPhaseRegion
    {
        LinearTemperatureBoundary Tlo;
        LinearTemperatureBoundary Thi;
        LinearPressureBoundary Plo;
        LinearPressureBoundary Phi;
        int phase_id;
        constexpr LinearPhaseRegion(
            const int phase_id,
            const LinearTemperatureBoundary Tlo,
            const LinearTemperatureBoundary Thi,
            const LinearPressureBoundary Plo,
            const LinearPressureBoundary Phi
        ):
            Tlo(Tlo),
            Thi(Thi),
            Plo(Plo),
            Phi(Phi),
            phase_id(phase_id)
        {
        }
        constexpr bool contains(const si::pressure<double> p, const si::temperature<double> T) const
        {
            return Tlo(p) < T&&T < Thi(p) &&
                   Plo(T) < p&&p < Phi(T);
        }
        constexpr bool contains(const point<double> pT) const
        {
            return contains(pT.pressure, pT.temperature);
        }
    }


    constexpr LinearPhaseRegion supercritical(const si::pressure<double> p, const si::temperature<double> T)
    {
        return LinearPhaseRegion(
            phase::supercritical,
            LinearTemperatureBoundary(temperature),
            unbounded_high_temperature(),
            LinearPressureBoundary(pressure),
            unbounded_high_pressure(),
        );
    }

    constexpr LinearPhaseRegion supercritical(const si::temperature<double> T, const si::pressure<double> p)
    {
        return LinearPhaseRegion(
            phase::supercritical,
            LinearTemperatureBoundary(temperature),
            unbounded_high_temperature(),
            LinearPressureBoundary(pressure),
            unbounded_high_pressure(),
        );
    }

    constexpr LinearPhaseRegion supercritical(point<double> critical_point)
    {
        return supercritical(critical_point.pressure, critical_point.temperature);
    }
}}
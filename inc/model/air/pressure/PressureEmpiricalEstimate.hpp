#pragma once

#include <vector>

#include <field/poles/MonopoleScalar.hpp>
#include <field/poles/NaiveMultipole.hpp>

#include <model/light/Exposure.hpp>

/*

`air::EmpiricalPressureEstimate` represents a 3d field (i.e. of the same signature of classes from `field::`) 
that stores an estimate for wind velocity on a world having some known properties like radius and rotation rate.

The estimate starts with the Rhines scale length, 
which characterizes the length at which a circulation cell 
on the world is overhwelmed by Rossby waves.
The Rhines scale is defined as L = √(U/β) where β = 2Ωcosϕ/a, 
U is wind velocity, Ω is rotation rate, 
ϕ is latitude, and a is equatorial radius.
The number of circulation cells is modeled as N≈P/2L.
The perimeter of an ellipse is exactly P = 4a∫₀ᵖ√(1-e²sin²ϕ)dϕ 
where p=π/2, and e is eccentricity, therefore P∝a, 
and N∝a/L(ϕ̄) for a characteristic latitude ϕ̄, 
and given L∝√(a/Ω), we see N ∝ a/√(a/Ω), 
or N ∝ √(aΩ), or N∝√(a/Π) where Π is period.
Experiments on global climate models ("GCMs") by Wang et al. (2018) 
support that N monotonically increases with respect to a,
however N∝√(a/Π) fails to match data from the solar system (see EmpiricalPressureEstimate.ods). 
A much better match with solar system data is N∝³√(a/Π).
This could be accounted for if ϕ̄ or U scales with Ω or a, 
such as if cosϕ̄∝1/N and U is independant, 
or U∝³√(aΩ) and ϕ̄ is independant, 
or both are dependant on U in some other way.
If U∝³√(aΩ), then wind speeds would scale in roughly 
the same way as the number of circulation cells,
but this is not the case in terms of observed max wind speed.
Regardless of cause, we will assume N∝³√(a/Π) 
since we have only ~1/60 second to work with
and higher fidelity models are unable to run in this time.

*/

namespace air
{

    template<typename scalar, typename Pressure, typename Length, typename Time, glm::qualifier precision = glm::defaultp>
    class PressureEmpiricalEstimate
    {

        using vec3 = glm::vec<3,scalar,precision>;

        Time& rotation_period;
        Length world_radius;
        vec3 north;

    public:

        constexpr explicit EmpiricalPressureEstimate(
            const Time& rotation_period, 
            const Length world_radius,
            const vec3 north
        ):
            rotation_period(rotation_period),
            world_radius(world_radius),
            north(glm::normalize(north))
        {}

        using value_type = Pressure;

        [[nodiscard]] Pressure operator()(const vec3& V) const noexcept
        {
            vec3 Vhat = glm::normalize(V);
            scalar latitude = atan2(glm::dot(Vhat, north), glm::length(Vhat - glm::dot(Vhat, north)));
            return -std::cos((circulation_cell_count - 1) * latitude);
        }

    };

}

/*
tests:
*/


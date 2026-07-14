#pragma once

#include <vector>

#include <field/poles/MonopoleScalar.hpp>
#include <field/poles/NaiveMultipole.hpp>

#include <model/light/Exposure.hpp>

/*

`light::CollimatedInsolation` represents a field (i.e. of the same signature of classes from `field::`) 
that stores the cross-spectrum "top-of-atmosphere" irradiance for a world at the origin with an arbitrary number of light sources.
The light sources are modeled as collimated light sources where irradiance is constant for any distance.

`light::CollimatedInsolation` correctly models several aspects of the problem:
* it supports light sources that represent luminosity as vectors of wavelength bins
* it correctly models occlusion

`light::CollimatedInsolation` introduces several assumptions that are either intrinsic to the class or will be addressed in time:
* it assumes all light sources are collimated
  this is a valid approximation for any light source where distance is much greater than the diameter of the world
  this is applicable for the vast majority of worlds, so it is intrinsic to the class and will remain unchanged
* it assumes all light sources are unattenuated by things like atmosphere 
  this is intrinsic to the class - another class (`AttenuatedInsolation`) will be introduced to relax this assumption
* it assumes the only occlusion that occurs is due to the world at the origin

TODO:
* add logic for occlusion by the planet at the origin
* add logic for atmospheric attenuation

*/

namespace air
{

    template<typename scalar, typename Irradiance, typename Temperature, typename Time, glm::qualifier precision = glm::defaultp>
    class CollimatedInsolation
    {

        constexpr scalar s0 = 0;

        using vec3 = glm::vec<3,scalar,precision>;

        using LightSource = light::BlackBodyPointSource<Irradiance, Temperature>;
        using LightExposure = light::Exposure<scalar, Time, LightSource>;

        std::vector<LightExposure> exposures;
        scalar world_radius;
        scalar s4pi;

    public:

        constexpr explicit CollimatedInsolation(
            const std::vector<LightExposure>& exposures, 
            const scalar world_radius,
            const scalar pi
        ):
            exposures(exposures),
            world_radius(world_radius),
            s4pi(scalar(4)*pi)
        {}

        using value_type = Irradiance;

        [[nodiscard]] Irradiance operator()(const vec3& V) const noexcept
        {
            const scalar R2 = world_radius * world_radius;
            Irradiance I(0);
            Irradiance Ii(0);
            vec3 L, Lhat;
            scalar l, r2, VL;
            for (const auto& exposure : exposures) // denoted by "i"
            {
                L = exposure.offset;
                l = glm::length(L);
                Lhat = L/l;
                r2 = glm::length2(V - glm::dot(V, Lhat) * Lhat); // cross sectional radius
                VL = glm::dot(V, L);
                Ii = r2 < R2 && VL < s0? Irradiance(0) : exposure.luminosity / (s4pi * l * l);
                I += Ii;
            }
            return I;
        }

    };

}

/*
tests:
* insolations are non-negative
* insolations are invariant to the order of exposures
* contributions add independantly
* given a single exposure, insolation is linear to luminosity
* given a single exposure of offset L, insolation(Lhat*x) is positive where x>0 and zero where x<0
* insolation is rotationally invariant

exposure offsets such as Lhat can be sampled along a fibonacci grid
*/


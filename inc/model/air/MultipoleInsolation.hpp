#pragma once

#include <vector>

#include <field/poles/MonopoleVector.hpp>
#include <field/poles/NaiveMultipole.hpp>

#include <model/light/Exposure.hpp>

/*

`light::Insolation` instantiates a field that represents the cross-spectrum "top-of-atmosphere" irradiance from an arbitrary number of light sources

`light::Insolation` correctly models several aspects of the problem:
* it correctly models drop off with distance for each point in the field
  this is often insignificant and costly though - other classes will be introduced to exploit assumptions for a performance gain
* it supports light sources that represent luminosity as vectors of wavelength bins

`light::Insolation` introduces several assumptions that are either intrinsic to the class or will be addressed in time:
* it assumes all light sources are point sources 
  this is intrinsic to the class and will remain unchanged
* it assumes all light sources are unattenuated by things like atmosphere 
  this is intrinsic to the class - other classes will be introduced to relax this assumption

*/

namespace light
{

    template <typename scalar, typename vector, typename Power>
    class MultipoleInsolation
    {

        static constexpr scalar pi = 3.14159265358979323846264338327950288419;

        static constexpr scalar s1_4pi = scalar(1) / (scalar(4) * pi);

        using Monopole = field::MonopoleScalar<2, Power, vector>;
        using Multipole = field::NaiveMultipole<scalar, vector, Monopole>;

        Insolation(){}

        [[nodiscard]] Multipole operator() (const LightExposures& exposures) const {
            Multipole insolation;
            for (std::size_t i = 0; i < exposures.size(); ++i)
            {
                insolation.add(exposures[i].source.luminosity() * s1_4pi, exposures[i].offset);
            }
            return insolation;
        }

    };

}


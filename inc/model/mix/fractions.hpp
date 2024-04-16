#pragma once

#include <cmath>

#include <unit/si.hpp>

namespace mix{

    /*
    `Fractions` calculates fractions for a collection of mass pools. 

    `Fractions` exists primarily to simplify the implementation of functionality that calculates the properties of mixtures.
    It is desireable for such functionality to be type-safe, but it still must allow calculation using different types of quantity like density and mass.
    This is accomplished using several helper functions (shown below) to construct instances of `Fractions` that operate on different units.

    Sometimes the amount of time that's available is extremely limited, so `Fractions` must operate quickly.
    It must provide ways for calculation to occur usingly only small footprints in stack memory, 
    and it must reuse intermediate calculations that are relatively slow (namely the sum of properties in a container).
    To accomplish this, calculation occurs in a two step process: 
     * calculate the sum of a container
     * calculate the fraction of an element in that container relative to the sum
    Again, type-safety is desireable so the design should enforce that the parameters of each step are mutually consistent.
    This means that the two-step process repeats in functorial categories where only the data type of objects differ (e.g. mass, density).
    Since an object of a polymorphic class is effectively a functorial category together with a data type that operates within it,
    `Fractions` is implemented as a class.
    Instances of `Fractions` serve as proof objects that indicate the first step of the 2-step process has occured.

    Method overloads are available to handle containers of fractions in case convenience is more important than performance.
    This case can still be handled conveniently, the only consequence of the design being 
    that you need to call twice in order to get the fractions for a container, e.g.:
    ```c++
    auto fractions = mass_fractions(masses)(masses)
    ```
    */
    template<typename unit>
    class Fractions
    {
        unit sum;
    public:
        Fractions(const std::vector<unit>& properties)
        {
            using scalar = typename unit::value_type;
            sum *= scalar(0);
            for (std::size_t i=0; i<properties.size(); i++) 
            {
                sum += properties[i];
            }
        }
        auto operator() (const unit property) const 
        {
            return property/sum;
        }
        auto operator() (const std::vector<unit>& properties) const 
        {
            using scalar = typename unit::value_type;
            std::vector<scalar> fractions;
            for (std::size_t i=0; i<properties.size(); i++) 
            {
                fractions.push_back(properties[i]/sum);
            }
            return fractions;
        }
    };

    template<typename scalar>        auto volume_fractions(const std::vector<si::volume<scalar>>& volumes)  { return Fractions<si::volume<scalar>>(volumes); }
    // template<typename scalar, int M> auto volume_fractions(const std::array<si::volume<scalar>,M>& volumes) { return Fractions<si::volume<scalar>>(volumes); }

    template<typename scalar>        auto mass_fractions  (const std::vector<si::mass<scalar>>& masses)        { return Fractions<si::mass<scalar>>(masses);    }
    // template<typename scalar, int M> auto mass_fractions  (const std::array<si::mass<scalar>,M>& masses)       { return Fractions<si::mass<scalar>>(masses);    }
    template<typename scalar>        auto mass_fractions  (const std::vector<si::density<scalar>>& densities)  { return Fractions<si::density<scalar>>(densities); }
    // template<typename scalar, int M> auto mass_fractions  (const std::array<si::density<scalar>,M>& densities) { return Fractions<si::density<scalar>>(densities); }

    template<typename scalar>        auto molar_fractions (const std::vector<si::amount<scalar>>& moles)                     { return Fractions<si::amount<scalar>>(moles);            }
    // template<typename scalar, int M> auto molar_fractions (const std::array<si::amount<scalar>,M>& moles)                    { return Fractions<si::amount<scalar>>(moles);            }
    template<typename scalar>        auto molar_fractions (const std::vector<si::molar_density<scalar>>& molar_densities)    { return Fractions<si::molar_density<scalar>>(molar_densities);  }
    // template<typename scalar, int M> auto molar_fractions (const std::array<si::molar_density<scalar>,M>& molar_densities)   { return Fractions<si::molar_density<scalar>>(molar_densities);  }
    template<typename scalar>        auto molar_fractions (const std::vector<si::number_density<scalar>>& number_densities)  { return Fractions<si::number_density<scalar>>(number_densities); }
    // template<typename scalar, int M> auto molar_fractions (const std::array<si::number_density<scalar>,M>& number_densities) { return Fractions<si::number_density<scalar>>(number_densities); }

    std::vector<double> log(const std::vector<double>& fractions){
        std::vector<double> result;
        for (std::size_t i=0; i<fractions.size(); i++) {
            result.emplace_back(std::log(fractions[i]));
        }
        return result;
    }

    // std::vector<double> mass_fractions(const std::vector<double>& molar_fractions, const std::vector<si::molar_mass<double>>& molar_masses){
    //     std::vector<double> result;
    //     si::molar_mass<double> average_molar_mass(0);
    //     for (std::size_t j=0; j<molar_fractions.size(); j++) {
    //         average_molar_mass += (molar_fractions[j] * molar_masses[j]);
    //     }
    //     for (std::size_t i=0; i<molar_fractions.size(); i++) {
    //         result.emplace_back(molar_fractions[i] * molar_masses[i] / average_molar_mass);
    //     }
    //     return result;
    // }

    // std::vector<double> molar_fractions(const std::vector<double>& mass_fractions, const std::vector<si::molar_mass<double>>& molar_masses){
    //     si::mass<double> average_mass(0);
    //     for (std::size_t j=0; j<mass_fractions.size(); j++) {
    //         average_mass += (mass_fractions[j] / molar_masses[j]);
    //     }
    //     std::vector<double> result;
    //     for (std::size_t i=0; i<mass_fractions.size(); i++) {
    //         result.emplace_back((mass_fractions[i] / molar_masses[i]) / average_mass);
    //     }
    //     return result;
    // }

}
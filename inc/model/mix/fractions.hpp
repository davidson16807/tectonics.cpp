#pragma once

#include <cmath>

#include <unit/si.hpp>

namespace mix{

    /*
    `PropertyFractions` calculates Propertyfractions for a collection of mass pools. 

    `PropertyFractions` exists primarily to simplify the implementation of functionality that calculates the properties of mixtures.
    It is desireable for such functionality to be type-safe, but it still must allow calculation using different types of quantity like density and mass.
    This is accomplished using several `*Fractions` classes (shown below) to construct instances of `PropertyFractions` that operate on different dimensions.

    Sometimes the amount of time that's available is extremely limited, so `PropertyFractions` must operate quickly.
    It must provide ways for calculation to occur usingly only small footprints in stack memory, 
    and it must reuse intermediate calculations that are relatively slow (namely the sum of properties in a container).
    To accomplish this, calculation occurs in a two step process: 
     * calculate the sum of a container
     * calculate the fraction of an element in that container relative to the sum
    Again, type-safety is desireable so the design should enforce that the parameters of each step are mutually consistent.
    This means that the two-step process repeats in functorial categories where only the data type of objects differ (e.g. mass, density).
    Since an object of a polymorphic class is effectively a functorial category together with a data type that operates within it,
    `PropertyFractions` is implemented as a class.
    Instances of `PropertyFractions` serve as proof objects that indicate the first step of the 2-step process has occured.

    Method overloads are available to handle containers of Propertyfractions in case convenience is more important than performance.
    This case can still be handled conveniently, the only consequence of the design being 
    that you need to call twice in order to get the Propertyfractions for a container, e.g.:
    ```c++
    auto Propertyfractions = mass_Propertyfractions(masses)(masses)
    ```

    PF:   C  → C → F
    XF:  ()  → C → PF
    PR: T×XF → C → P
    XR:  ()  → T → PR
    SP: T×XR → C → P
    SX:  ()  → T → SP

    */

    template<typename unit>
    class PropertyFractions
    {
        using scalar = typename unit::value_type;

        unit sum;
    public:
        using value_type = typename unit::value_type;
        template<typename container>
        PropertyFractions(const container& constituents)
        {
            sum *= scalar(0);
            for (std::size_t i=0; i<constituents.size(); i++) 
            {
                sum += constituents[i];
            }
        }
        inline auto operator() (const unit property) const 
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

    template<typename scalar>
    struct VolumeFractions
    {
        VolumeFractions(){}
        template<int M>
        auto operator() (const std::array<si::volume<scalar>,M>& volumes) const { return PropertyFractions<si::volume<scalar>>(volumes); }
        auto operator() (const std::vector<si::volume<scalar>>& volumes) const { return PropertyFractions<si::volume<scalar>>(volumes); }
    };

    template<typename scalar>
    struct MassFractions
    {
        MassFractions(){}
        template<int M>
        auto operator() (const std::array<si::mass<scalar>,M>& masses) const { return PropertyFractions<si::mass<scalar>>(masses); }
        auto operator() (const std::vector<si::mass<scalar>>& masses) const { return PropertyFractions<si::mass<scalar>>(masses); }
        template<int M>
        auto operator() (const std::array<si::density<scalar>,M>& densities) const { return PropertyFractions<si::density<scalar>>(densities); }
        auto operator() (const std::vector<si::density<scalar>>& densities) const { return PropertyFractions<si::density<scalar>>(densities); }
    };

    template<typename scalar>
    struct MolarFractions
    {
        MolarFractions(){}
        template<int M>
        auto operator() (const std::array<si::amount<scalar>,M>& amounts) const { return PropertyFractions<si::amount<scalar>>(amounts); }
        auto operator() (const std::vector<si::amount<scalar>>& amounts) const { return PropertyFractions<si::amount<scalar>>(amounts); }
        template<int M>
        auto operator() (const std::array<si::molar_density<scalar>,M>& molar_densities) const { return PropertyFractions<si::molar_density<scalar>>(molar_densities); }
        auto operator() (const std::vector<si::molar_density<scalar>>& molar_densities) const { return PropertyFractions<si::molar_density<scalar>>(molar_densities); }
        template<int M>
        auto operator() (const std::array<si::number_density<scalar>,M>& number_densities) const { return PropertyFractions<si::number_density<scalar>>(number_densities); }
        auto operator() (const std::vector<si::number_density<scalar>>& number_densities) const { return PropertyFractions<si::number_density<scalar>>(number_densities); }
    };

}


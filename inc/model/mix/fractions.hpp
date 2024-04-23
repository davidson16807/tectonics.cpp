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

    struct Identity
    {
        constexpr Identity(){}
        template<typename id, typename scalar>
        inline constexpr scalar operator() (const id i, const scalar constituent) const 
        {
            return constituent;
        }
    };

    template<typename unit, typename Get>
    class PropertyFractions
    {
        using scalar = typename unit::value_type;

        const Get get;
              unit sum;
    public:
        using value_type = typename unit::value_type;
        template<typename Constituents>
        PropertyFractions(const Constituents& constituents, const Get& get):
            get(get),
            sum(get(0,constituents[0]))
        {
            for (std::size_t i=1; i<constituents.size(); i++) 
            {
                sum += get(i, constituents[i]);
            }
        }
        template<typename id>
        inline auto operator() (const id i, const unit constituent) const 
        {
            return get(i, constituent)/sum;
        }
        auto operator() (const std::vector<unit>& constituents) const 
        {
            using scalar = typename unit::value_type;
            std::vector<scalar> fractions;
            for (std::size_t i=0; i<constituents.size(); i++) 
            {
                fractions.push_back(get(i, constituents[i])/sum);
            }
            return fractions;
        }
    };

    template<typename scalar, typename Get>
    struct VolumeFractions
    {
        const Get get;
        VolumeFractions(const Get& get):get(get) {}
        template<int M>
        auto operator() (const std::array<si::volume<scalar>,M>& volumes) const { return PropertyFractions<si::volume<scalar>,Get>(volumes, get); }
        auto operator() (const std::vector<si::volume<scalar>>& volumes) const { return PropertyFractions<si::volume<scalar>,Get>(volumes, get); }
    };
    template<typename scalar, typename Get> 
    auto volume_fractions(const Get& get) { return VolumeFractions<scalar,Get>(get); }
    template<typename scalar> 
    auto volume_fractions() { return volume_fractions<scalar>(Identity()); }

    template<typename scalar, typename Get>
    struct MassFractions
    {
        const Get get;
        MassFractions(const Get& get):get(get) {}
        template<int M>
        auto operator() (const std::array<si::mass<scalar>,M>& masses) const { return PropertyFractions<si::mass<scalar>,Get>(masses, get); }
        auto operator() (const std::vector<si::mass<scalar>>& masses) const { return PropertyFractions<si::mass<scalar>,Get>(masses, get); }
        template<int M>
        auto operator() (const std::array<si::density<scalar>,M>& densities) const { return PropertyFractions<si::density<scalar>,Get>(densities, get); }
        auto operator() (const std::vector<si::density<scalar>>& densities) const { return PropertyFractions<si::density<scalar>,Get>(densities, get); }
    };
    template<typename scalar, typename Get> 
    auto mass_fractions(const Get& get) { return MassFractions<scalar,Get>(get); }
    template<typename scalar> 
    auto mass_fractions() { return mass_fractions<scalar>(Identity()); }

    template<typename scalar, typename Get>
    struct MolarFractions
    {
        const Get get;
        MolarFractions(const Get& get):get(get) {}
        template<int M>
        auto operator() (const std::array<si::amount<scalar>,M>& amounts) const { return PropertyFractions<si::amount<scalar>,Get>(amounts, get); }
        auto operator() (const std::vector<si::amount<scalar>>& amounts) const { return PropertyFractions<si::amount<scalar>,Get>(amounts, get); }
        template<int M>
        auto operator() (const std::array<si::molar_density<scalar>,M>& molar_densities) const { return PropertyFractions<si::molar_density<scalar>,Get>(molar_densities, get); }
        auto operator() (const std::vector<si::molar_density<scalar>>& molar_densities) const { return PropertyFractions<si::molar_density<scalar>,Get>(molar_densities, get); }
        template<int M>
        auto operator() (const std::array<si::number_density<scalar>,M>& number_densities) const { return PropertyFractions<si::number_density<scalar>,Get>(number_densities, get); }
        auto operator() (const std::vector<si::number_density<scalar>>& number_densities) const { return PropertyFractions<si::number_density<scalar>,Get>(number_densities, get); }
    };
    template<typename scalar, typename Get> 
    auto molar_fractions(const Get& get) { return MolarFractions<scalar,Get>(get); }
    template<typename scalar> 
    auto molar_fractions() { return molar_fractions<scalar>(Identity()); }

}


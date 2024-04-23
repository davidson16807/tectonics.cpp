#pragma once

// in house libraries
#include <model/compound/relation/relation.hpp>

namespace mix{

    struct Identity
    {
        constexpr Identity(){}
        template<typename id, typename scalar>
        inline constexpr scalar operator() (const id i, const scalar constituent) const 
        {
            return constituent;
        }
    };

    template<typename scalar>
    struct CheckVolume
    {
        CheckVolume() {}
        inline auto operator() (const si::volume<scalar> property) const { return property; }
    };

    template<typename scalar>
    struct CheckMass
    {
        CheckMass() {}
        inline auto operator() (const si::mass<scalar> property) const { return property; }
        inline auto operator() (const si::density<scalar> property) const { return property; }
    };

    template<typename scalar>
    struct CheckMolar
    {
        CheckMolar() {}
        inline auto operator() (const si::amount<scalar> property) const { return property; }
        inline auto operator() (const si::molar_density<scalar> property) const { return property; }
        inline auto operator() (const si::number_density<scalar> property) const { return property; }
    };

    template<typename Table, typename Get, typename Check>
    class LinearRule
    {
        const Table table;
        const Get get;
        const Check check;
    public:
        LinearRule(const Table& table, const Get& get, const Check& check):
            table(table),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto result = (check(get(0,constituents[0]))/sum) * table[0];
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                result += (check(get(i,constituents[i]))/sum) * table[i];
            }
            return result;
        }
    };
    template<typename Table, typename Get, typename Check>
    auto linear_rule(const Table& table, const Get& get, const Check& check)
    {
        return LinearRule<Table,Get,Check>(table, get, check);
    }

    template<typename Table, typename Get, typename Check>
    class LogRule
    {
        const Table table;
        const Get get;
        const Check check;
    public:
        LogRule(const Table& table, const Get& get, const Check& check):
            table(table),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto result = (check(get(0,constituents[0]))/sum) * table[0];
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                result += (check(get(i,constituents[i]))/sum) * log(table[i]);
            }
            return exp(result);
        }
    };
    template<typename Table, typename Get, typename Check>
    auto log_rule(const Table& table, const Get& get, const Check& check)
    {
        return LinearRule<Table,Get,Check>(table, get, check);
    }

    template<typename scalar> auto acentric_factor                  (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_mass                       (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); }
    template<typename scalar> auto molecular_diameter               (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_volume_as_liquid           (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Amgat's Law"
    template<typename scalar> auto critical_temperature             (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Kay's Rule"
    template<typename scalar> auto critical_compressibility         (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // mole weighted arithmetic mean
    template<typename scalar> auto critical_volume                  (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Amgat's Law"
    template<typename scalar> auto latent_heat_of_vaporization      (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); }
    template<typename scalar> auto latent_heat_of_fusion            (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); }
    template<typename scalar> auto latent_heat_of_sublimation       (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); }
    template<typename scalar> auto specific_heat_capacity_as_gas    (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_liquid (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_solid  (const auto& table, const auto& get_mass  ){ return linear_rule (table, get_mass,  CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_gas       (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_liquid    (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_solid     (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto vapor_pressure_as_liquid         (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Raoult's Law"
    template<typename scalar> auto vapor_pressure_as_solid          (const auto& table, const auto& get_moles ){ return linear_rule (table, get_moles, CheckMolar<scalar> ()); } // "Raoult's Law", see Goldfarb 2008 for sample of validity

    template<typename scalar> auto acentric_factor                  (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_mass                       (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); }
    template<typename scalar> auto molecular_diameter               (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_volume_as_liquid           (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Amgat's Law"
    template<typename scalar> auto critical_temperature             (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Kay's Rule"
    template<typename scalar> auto critical_compressibility         (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // mole weighted arithmetic mean
    template<typename scalar> auto critical_volume                  (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Amgat's Law"
    template<typename scalar> auto latent_heat_of_vaporization      (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); }
    template<typename scalar> auto latent_heat_of_fusion            (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); }
    template<typename scalar> auto latent_heat_of_sublimation       (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); }
    template<typename scalar> auto specific_heat_capacity_as_gas    (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_liquid (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_solid  (const auto& table){ return linear_rule (table, Identity(), CheckMass <scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_gas       (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_liquid    (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_solid     (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Kopp's Law"
    template<typename scalar> auto vapor_pressure_as_liquid         (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Raoult's Law"
    template<typename scalar> auto vapor_pressure_as_solid          (const auto& table){ return linear_rule (table, Identity(), CheckMolar<scalar> ()); } // "Raoult's Law", see Goldfarb 2008 for sample of validity

    template<typename scalar> auto dynamic_viscosity_as_liquid (const auto& table, const auto& get_molar){ return logarithmic_rule(table, get_molar, CheckMolar<scalar>()); }
    template<typename scalar> auto dynamic_viscosity_as_solid  (const auto& table, const auto& get_molar){ return logarithmic_rule(table, get_molar, CheckMolar<scalar>()); }

    template<typename scalar> auto dynamic_viscosity_as_liquid (const auto& table){ return logarithmic_rule(table, Identity(), CheckMolar<scalar>()); }
    template<typename scalar> auto dynamic_viscosity_as_solid  (const auto& table){ return logarithmic_rule(table, Identity(), CheckMolar<scalar>()); }

    template<typename scalar, typename Table, typename Get, typename Check>
    class ParallelRule
    {
        const Table table;
        const Get get;
        const Check check;
    public:
        ParallelRule(const Table& table, const Get& get, const Check& check):
            table(table),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto result = (check(get(0,constituents[0]))/sum) / table[0];
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                result += (check(get(i,constituents[i]))/sum) / table[i];
            }
            return scalar(1) / result;
        }
    };
    template<typename scalar, typename Table, typename Get, typename Check>
    auto parallel_rule(const Table& table, const Get& get, const Check& check)
    {
        return ParallelRule<scalar,Table,Get,Check>(table, get, check);
    }

    // Much like how `ParallelRule` is the linear rule under a transport of structure to the inverse domain,
    // The `SquareParallelRule` is the linear rule under a transport of structure to the inverse square domain.
    // It is also given the names "DIPPER-9H", "Vredeveld" (1973), etc.
    template<typename scalar, typename Table, typename Get, typename Check>
    class SquareParallelRule
    {
        const Table table;
        const Get get;
        const Check check;
    public:
        SquareParallelRule(const Table& table, const Get& get, const Check& check):
            table(table),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto lambda = table[0];
            auto result = (check(get(0,constituents[0]))/sum) / (lambda*lambda);
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                lambda = table[i];
                result += (check(get(i,constituents[i]))/sum) / (lambda*lambda);
            }
            return scalar(1) / result;
        }
    };
    template<typename scalar, typename Table, typename Get, typename Check>
    auto square_parallel_rule(const Table& table, const Get& get, const Check& check)
    {
        return SquareParallelRule<scalar,Table,Get,Check>(table, get, check);
    }

    template<typename scalar> auto density_as_liquid                      (const auto& table, const auto& get_mass   ){ return parallel_rule        (table, get_mass,    CheckMass <scalar> ()); }
    template<typename scalar> auto density_as_solid                       (const auto& table, const auto& get_mass   ){ return parallel_rule        (table, get_mass,    CheckMass <scalar> ()); }
    template<typename scalar> auto molar_density_as_liquid                (const auto& table, const auto& get_moles  ){ return parallel_rule        (table, get_moles,   CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_density_as_solid                 (const auto& table, const auto& get_moles  ){ return parallel_rule        (table, get_moles,   CheckMolar<scalar> ()); }
    template<typename scalar> auto tensile_modulus_as_solid               (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto shear_modulus_as_solid                 (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto bulk_modulus_as_solid                  (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto pwave_modulus_as_solid                 (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto lame_parameter_as_solid                (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto poisson_ratio_as_solid                 (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto tensile_yield_strength_as_solid        (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto shear_yield_strength_as_solid          (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto compressive_yield_strength_as_solid    (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto tensile_fracture_strength_as_solid     (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto shear_fracture_strength_as_solid       (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto compressive_fracture_strength_as_solid (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_solid          (const auto& table, const auto& get_volume ){ return parallel_rule        (table, get_volume,  CheckVolume<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_liquid         (const auto& table, const auto& get_mass   ){ return square_parallel_rule (table, get_mass,    CheckMass <scalar> ()); }

    template<typename scalar> auto density_as_liquid                      (const auto& table){ return parallel_rule        (table, Identity(), CheckMass <scalar> ()); }
    template<typename scalar> auto density_as_solid                       (const auto& table){ return parallel_rule        (table, Identity(), CheckMass <scalar> ()); }
    template<typename scalar> auto molar_density_as_liquid                (const auto& table){ return parallel_rule        (table, Identity(), CheckMolar<scalar> ()); }
    template<typename scalar> auto molar_density_as_solid                 (const auto& table){ return parallel_rule        (table, Identity(), CheckMolar<scalar> ()); }
    template<typename scalar> auto tensile_modulus_as_solid               (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto shear_modulus_as_solid                 (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto bulk_modulus_as_solid                  (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto pwave_modulus_as_solid                 (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto lame_parameter_as_solid                (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto poisson_ratio_as_solid                 (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto tensile_yield_strength_as_solid        (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto shear_yield_strength_as_solid          (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto compressive_yield_strength_as_solid    (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto tensile_fracture_strength_as_solid     (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto shear_fracture_strength_as_solid       (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto compressive_fracture_strength_as_solid (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_solid          (const auto& table){ return parallel_rule        (table, Identity(), CheckVolume<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_liquid         (const auto& table){ return square_parallel_rule (table, Identity(), CheckMass <scalar> ()); }


    template<typename Table, typename MolarMassTable, typename Get, typename Check>
    class HerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const Get get;
        const Check check;
    public:
        HerningZippererRule(const Table& table, const MolarMassTable& molar_masses, const Get& get, const Check& check):
            table(table),
            molar_masses(molar_masses),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto numerator   = (check(get(0,constituents[0]))/sum) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole)) * table[0];
            auto denominator = (check(get(0,constituents[0]))/sum) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole));
            for (std::size_t i=1; i<constituents.size(); i++) {
                numerator   += (check(get(i,constituents[i]))/sum) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole)) * table[i];
                denominator += (check(get(i,constituents[i]))/sum) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole));
            }
            return numerator/denominator;
        }
    };
    template<typename Table, typename MolarMassTable, typename Get, typename Check>
    auto herning_zipperer_rule(const Table& table, const MolarMassTable& molar_masses, const Get& get, const Check& check)
    {
        return HerningZippererRule<Table,MolarMassTable,Get,Check>(table, molar_masses, get, check);
    }

    template<typename Table, typename MolarMassTable, typename Get, typename Check>
    class WassiljewaHerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const Get get;
        const Check check;
    public:
        WassiljewaHerningZippererRule(const Table& table, const MolarMassTable& molar_masses, const Get& get, const Check& check):
            table(table),
            molar_masses(molar_masses),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto numerator   = (check(get(0,constituents[0]))/sum) * table[0];
            auto denominator = (check(get(0,constituents[0]))/sum) * std::sqrt(molar_masses[0]/molar_masses[0]);
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        denominator += (check(get(i,constituents[i]))/sum) * std::sqrt(molar_masses[j]/molar_masses[i]);
                    }
                }
                if (i!=0)
                {
                    numerator += (check(get(i,constituents[i]))/sum) * table[i];
                }
            }
            return numerator/denominator;
        }
    };
    template<typename Table, typename MolarMassTable, typename Get, typename Check>
    auto wassiljewa_herning_zipperer_rule(const Table& table, const MolarMassTable& molar_masses, const Get& get, const Check& check)
    {
        return WassiljewaHerningZippererRule<Table,MolarMassTable,Get,Check>(table, molar_masses, get, check);
    }

    template<typename Table, typename MolarDensityTable, typename Get, typename Check>
    class WinterfeldScrivenDavisRule
    {
        const Table table;
        const MolarDensityTable molar_densities;
        const Get get;
        const Check check;
    public:
        WinterfeldScrivenDavisRule(const Table& table, const MolarDensityTable& molar_densities, const Get& get, const Check& check):
            table(table),
            molar_densities(molar_densities),
            get(get),
            check(check)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto molar_density_of_mix = molar_density_as_liquid(constituents);
            auto sum = check(get(0,constituents[0]));
            for (std::size_t i=1; i<constituents.size(); i++)
            {
                sum += check(get(i,constituents[i]));
            }
            auto result = std::sqrt(table[0]*table[0]) * 
                molar_density_of_mix * (get(check(0,constituents[0]))/sum) * molar_density_of_mix * (get(check(0,constituents[0]))/sum) /
                molar_densities[0]                                  * molar_densities[0];
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        result += std::sqrt(table[i]*table[j]) * 
                            molar_density_of_mix * (get(check(i,constituents[i]))/sum) * molar_density_of_mix * (get(check(i,constituents[j]))/sum) /
                            molar_densities[i]                                * molar_densities[j];
                    }
                }
            }
            return result;
        }
    };
    template<typename Table, typename MolarDensityTable, typename Get, typename Check>
    auto winterfeld_scriven_davis_rule(const Table& table, const MolarDensityTable& molar_densities, const Get& get, const Check& check)
    {
        return WinterfeldScrivenDavisRule<Table,MolarDensityTable,Get,Check>(table, molar_densities, get, check);
    }

    template<typename scalar> auto dynamic_viscosity_as_gas    (const auto& table, const auto& molar_masses,    const auto& get_moles) { return herning_zipperer_rule <scalar>(table, molar_masses, get_moles, CheckMolar<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_gas (const auto& table, const auto& molar_masses,    const auto& get_moles) { return wassiljewa_herning_zipperer_rule <scalar>(table, molar_masses, get_moles, CheckMolar<scalar>()); }
    template<typename scalar> auto surface_tension_as_liquid   (const auto& table, const auto& molar_densities, const auto& get_moles) { return winterfeld_scriven_davis_rule <scalar>(table, molar_densities, get_moles, CheckMolar<scalar>()); }

    template<typename scalar> auto dynamic_viscosity_as_gas    (const auto& table, const auto& molar_masses)    { return herning_zipperer_rule <scalar>(table, molar_masses, Identity(), CheckMolar<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_gas (const auto& table, const auto& molar_masses)    { return wassiljewa_herning_zipperer_rule <scalar>(table, molar_masses, Identity(), CheckMolar<scalar>()); }
    template<typename scalar> auto surface_tension_as_liquid   (const auto& table, const auto& molar_densities) { return winterfeld_scriven_davis_rule <scalar>(table, molar_densities, Identity(), CheckMolar<scalar>()); }

    // template<typename Table, typename Get, typename Check>
    // class GeneralRelationRule
    // {
    //     const Table table;
    //     const Get get;
    //     const Check check;
    // public:
    //     GeneralRelationLinearRule(const Table& table, const Get& get, const Check& check):
    //         table(table),
    //         get(get),
    //         check(check)
    //     {}
    //     template<typename Constituents>
    //     auto operator() (const Constituents& constituents) const 
    //     {
    //         return compound::relation::GenericRelation<Tx, Ty>(
    //             [=](Tx x){
    //                 auto result = (get(check(i,constituents[0]))/sum)*table[0];
    //                 for (std::size_t i=1; i<constituents.size(); i++) {
    //                     result += (get(check(i,constituents[i]))/sum)*table[i];
    //                 }
    //                 return result;
    //             }
    //         );
    //     }
    // };

    // "Amgat's Law"
    template<typename scalar>
    auto density_as_gas(
        const si::molar_mass<scalar>  molar_mass,
        const si::pressure<scalar>    critical_pressure,
        const si::temperature<scalar> critical_temperature,
        const scalar                  critical_compressibility
    ){
        return compound::relation::GasDensityStateRelation(
            molar_mass,
            critical_pressure,
            critical_temperature,
            critical_compressibility
        );
    }

    template<typename scalar>
    auto critical_pressure(
        const si::temperature<double> critical_temperature, 
        const si::molar_volume<double> critical_volume, 
        const double critical_compressibility
    ){ 
        return si::universal_gas_constant * critical_temperature * critical_compressibility / critical_volume; 
    } // "Prausnitz-Gunn"

    template<typename scalar>
    auto critical_compressibility(const scalar acentric_factor){ return scalar(0.291) - scalar(0.08)*acentric_factor; } // Yorizane

}


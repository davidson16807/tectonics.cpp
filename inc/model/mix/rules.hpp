#pragma once

// in house libraries
#include <model/compound/relation/relation.hpp>

namespace mix{

    template<typename Table, typename GetFractions>
    class LinearRule
    {
        const Table table;
        const GetFractions get_fractions;
    public:
        LinearRule(const Table& table, const GetFractions& get_fractions):
            table(table),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        typename Table::value_type operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            typename Table::value_type result = fractions(0,constituents[0])*table[0];
            for (std::size_t i=1; i<constituents.size(); i++) 
            {
                result += fractions(i,constituents[i])*table[i];
            }
            return result;
        }
    };
    template<typename Table, typename GetFractions>
    auto linear_rule(const Table& table, const GetFractions& get_fractions)
    {
        return LinearRule<Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar> auto acentric_factor                  (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto molar_mass                       (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto molecular_diameter               (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto molar_volume_as_liquid           (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Amgat's Law"
    template<typename scalar> auto critical_temperature             (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Kay's Rule"
    template<typename scalar> auto critical_compressibility         (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // mole weighted arithmetic mean
    template<typename scalar> auto critical_volume                  (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Amgat's Law"
    template<typename scalar> auto latent_heat_of_vaporization      (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); }
    template<typename scalar> auto latent_heat_of_fusion            (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); }
    template<typename scalar> auto latent_heat_of_sublimation       (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); }
    template<typename scalar> auto specific_heat_capacity_as_gas    (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_liquid (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_solid  (const auto& table, const auto& get_mass  ){ return linear_rule (table, mass_fractions  <scalar>(get_mass)); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_gas       (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_liquid    (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_solid     (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Kopp's Law"
    template<typename scalar> auto vapor_pressure_as_liquid         (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Raoult's Law"
    template<typename scalar> auto vapor_pressure_as_solid          (const auto& table, const auto& get_moles ){ return linear_rule (table, molar_fractions <scalar>(get_moles)); } // "Raoult's Law", see Goldfarb 2008 for sample of validity

    template<typename scalar> auto acentric_factor                  (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); }
    template<typename scalar> auto molar_mass                       (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); }
    template<typename scalar> auto molecular_diameter               (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); }
    template<typename scalar> auto molar_volume_as_liquid           (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Amgat's Law"
    template<typename scalar> auto critical_temperature             (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Kay's Rule"
    template<typename scalar> auto critical_compressibility         (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // mole weighted arithmetic mean
    template<typename scalar> auto critical_volume                  (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Amgat's Law"
    template<typename scalar> auto latent_heat_of_vaporization      (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); }
    template<typename scalar> auto latent_heat_of_fusion            (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); }
    template<typename scalar> auto latent_heat_of_sublimation       (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); }
    template<typename scalar> auto specific_heat_capacity_as_gas    (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_liquid (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto specific_heat_capacity_as_solid  (const auto& table){ return linear_rule (table, mass_fractions  <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_gas       (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_liquid    (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto molar_heat_capacity_as_solid     (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Kopp's Law"
    template<typename scalar> auto vapor_pressure_as_liquid         (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Raoult's Law"
    template<typename scalar> auto vapor_pressure_as_solid          (const auto& table){ return linear_rule (table, molar_fractions <scalar>()); } // "Raoult's Law", see Goldfarb 2008 for sample of validity

    template<typename scalar, typename Table, typename GetFractions>
    class ParallelRule
    {
        const Table table;
        const GetFractions get_fractions;
    public:
        ParallelRule(const Table& table, const GetFractions& get_fractions):
            table(table),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            auto result = fractions(0,constituents[0]) / table[0];
            for (std::size_t i=1; i<table.size(); i++){
                result += fractions(i,constituents[i]) / table[i];
            }
            return scalar(1) / result;
        }
    };
    template<typename scalar, typename Table, typename GetFractions>
    auto parallel_rule(const Table& table, const GetFractions& get_fractions)
    {
        return ParallelRule<scalar,Table,GetFractions>(table, get_fractions);
    }

    // Much like how `ParallelRule` is the linear rule under a transport of structure to the inverse domain,
    // The `SquareParallelRule` is the linear rule under a transport of structure to the inverse square domain.
    // It is also given the names "DIPPER-9H", "Vredeveld" (1973), etc.
    template<typename scalar, typename Table, typename GetFractions>
    class SquareParallelRule 
    {
        const Table table;
        const GetFractions get_fractions;
    public:
        SquareParallelRule(const Table& table, const GetFractions& get_fractions):
            table(table),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            auto lambda = table[0];
            auto result = fractions(0,constituents[0]) / (lambda*lambda);
            for (std::size_t i=1; i<table.size(); i++){
                lambda = table[i];
                result += fractions(i,constituents[i]) / (lambda*lambda);
            }
            return scalar(1) / si::sqrt(lambda);
        }
    };
    template<typename scalar, typename Table, typename GetFractions>
    auto square_parallel_rule(const Table& table, const GetFractions& get_fractions)
    {
        return SquareParallelRule<scalar,Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar> auto density_as_liquid                      (const auto& table, const auto& get_mass  ){ return parallel_rule        (table, mass_fractions  <scalar>(get_mass)); }
    template<typename scalar> auto density_as_solid                       (const auto& table, const auto& get_mass  ){ return parallel_rule        (table, mass_fractions  <scalar>(get_mass)); }
    template<typename scalar> auto molar_density_as_liquid                (const auto& table, const auto& get_moles ){ return parallel_rule        (table, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto molar_density_as_solid                 (const auto& table, const auto& get_moles ){ return parallel_rule        (table, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto tensile_modulus_as_solid               (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto shear_modulus_as_solid                 (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto bulk_modulus_as_solid                  (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto pwave_modulus_as_solid                 (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto lame_parameter_as_solid                (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto poisson_ratio_as_solid                 (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto tensile_yield_strength_as_solid        (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto shear_yield_strength_as_solid          (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto compressive_yield_strength_as_solid    (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto tensile_fracture_strength_as_solid     (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto shear_fracture_strength_as_solid       (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto compressive_fracture_strength_as_solid (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto thermal_conductivity_as_solid          (const auto& table, const auto& get_volume){ return parallel_rule        (table, volume_fractions<scalar>(get_volume)); }
    template<typename scalar> auto thermal_conductivity_as_liquid         (const auto& table, const auto& get_mass  ){ return square_parallel_rule (table, mass_fractions  <scalar>(get_mass)); }

    template<typename scalar> auto density_as_liquid                      (const auto& table){ return parallel_rule        (table, mass_fractions  <scalar>()); }
    template<typename scalar> auto density_as_solid                       (const auto& table){ return parallel_rule        (table, mass_fractions  <scalar>()); }
    template<typename scalar> auto molar_density_as_liquid                (const auto& table){ return parallel_rule        (table, molar_fractions <scalar>()); }
    template<typename scalar> auto molar_density_as_solid                 (const auto& table){ return parallel_rule        (table, molar_fractions <scalar>()); }
    template<typename scalar> auto tensile_modulus_as_solid               (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto shear_modulus_as_solid                 (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto bulk_modulus_as_solid                  (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto pwave_modulus_as_solid                 (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto lame_parameter_as_solid                (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto poisson_ratio_as_solid                 (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto tensile_yield_strength_as_solid        (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto shear_yield_strength_as_solid          (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto compressive_yield_strength_as_solid    (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto tensile_fracture_strength_as_solid     (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto shear_fracture_strength_as_solid       (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto compressive_fracture_strength_as_solid (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_solid          (const auto& table){ return parallel_rule        (table, volume_fractions<scalar>()); }
    template<typename scalar> auto thermal_conductivity_as_liquid         (const auto& table){ return square_parallel_rule (table, mass_fractions  <scalar>()); }

    template<typename Table, typename GetFractions>
    class LogarithmicRule
    {
        const Table table;
        const GetFractions get_fractions;
    public:
        LogarithmicRule(const Table& table, const GetFractions& get_fractions):
            table(table),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            auto result = fractions(0,constituents[0])*table[0];
            for (std::size_t i=1; i<constituents.size(); i++) 
            {
                result += fractions(i,constituents[i])*std::log(table[i]);
            }
            return result;
        }
    };
    template<typename Table, typename GetFractions>
    auto logarithmic_rule(const Table& table, const GetFractions& get_fractions)
    {
        return LogarithmicRule<Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar> auto dynamic_viscosity_as_liquid (const auto& table, const auto& get_property){ return logarithmic_rule(table, mass_fractions<scalar>(get_property)); }
    template<typename scalar> auto dynamic_viscosity_as_solid  (const auto& table, const auto& get_property){ return logarithmic_rule(table, mass_fractions<scalar>(get_property)); }

    template<typename scalar> auto dynamic_viscosity_as_liquid (const auto& table){ return logarithmic_rule(table, mass_fractions<scalar>()); }
    template<typename scalar> auto dynamic_viscosity_as_solid  (const auto& table){ return logarithmic_rule(table, mass_fractions<scalar>()); }

    template<typename Table, typename MolarMassTable, typename GetFractions>
    class HerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const GetFractions get_fractions;
    public:
        HerningZippererRule(const Table& table, const MolarMassTable& molar_masses, const GetFractions& get_fractions):
            table(table),
            molar_masses(molar_masses),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            auto numerator   = fractions(0,constituents[0]) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole)) * table[0];
            auto denominator = fractions(0,constituents[0]) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole));
            for (std::size_t i=1; i<constituents.size(); i++) {
                numerator   += fractions(i,constituents[i]) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole)) * table[i];
                denominator += fractions(i,constituents[i]) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole));
            }
            return numerator/denominator;
        }
    };
    template<typename Table, typename MolarMassTable, typename GetFractions>
    auto herning_zipperer_rule(const Table& table, const MolarMassTable& molar_masses, const GetFractions& get_fractions)
    {
        return HerningZippererRule<Table,MolarMassTable,GetFractions>(table, molar_masses,get_fractions);
    }

    template<typename Table, typename MolarMassTable, typename GetFractions>
    class WassiljewaHerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const GetFractions get_fractions;
    public:
        WassiljewaHerningZippererRule(const Table& table, const MolarMassTable& molar_masses, const GetFractions& get_fractions):
            table(table),
            molar_masses(molar_masses),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto numerator = fractions(0,constituents[0]) * table[0];
            auto denominator = fractions(0,constituents[0]) * std::sqrt(molar_masses[0]/molar_masses[0]);
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        denominator += fractions(i,constituents[i]) * std::sqrt(molar_masses[j]/molar_masses[i]);
                    }
                }
                if (i!=0)
                {
                    numerator += fractions(i,constituents[i]) * table[i];
                }
            }
            return numerator/denominator;
        }
    };
    template<typename Table, typename MolarMassTable, typename GetFractions>
    auto wassiljewa_herning_zipperer_rule(const Table& table, const MolarMassTable& molar_masses, const GetFractions& get_fractions)
    {
        return WassiljewaHerningZippererRule<Table,MolarMassTable,GetFractions>(table, molar_masses, get_fractions);
    }

    template<typename Table, typename MolarDensityTable, typename GetFractions>
    class WinterfeldScrivenDavisRule
    {
        const Table table;
        const MolarDensityTable molar_densities;
        const GetFractions get_fractions;
    public:
        WinterfeldScrivenDavisRule(const Table& table, const MolarDensityTable& molar_densities, const GetFractions& get_fractions):
            table(table),
            molar_densities(molar_densities),
            get_fractions(get_fractions)
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto molar_density_of_mix = molar_density_as_liquid(constituents);
            auto result = std::sqrt(table[0]*table[0]) * 
                molar_density_of_mix * fractions(0,constituents[0]) * molar_density_of_mix * fractions(0,constituents[0]) /
                molar_densities[0]                                  * molar_densities[0];
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        result += std::sqrt(table[i]*table[j]) * 
                            molar_density_of_mix * fractions(i,constituents[i]) * molar_density_of_mix * fractions(i,constituents[j]) /
                            molar_densities[i]                                * molar_densities[j];
                    }
                }
            }
            return result;
        }
    };
    template<typename Table, typename MolarDensityTable, typename GetFractions>
    auto winterfeld_scriven_davis_rule(const Table& table, const MolarDensityTable& molar_densities, const GetFractions& get_fractions)
    {
        return WinterfeldScrivenDavisRule<Table,MolarDensityTable,GetFractions>(table, molar_densities, get_fractions);
    }

    template<typename scalar> auto dynamic_viscosity_as_gas    (const auto& table, const auto& molar_masses, const auto& get_moles)    { return herning_zipperer_rule <scalar>(table, molar_masses, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto thermal_conductivity_as_gas (const auto& table, const auto& molar_masses, const auto& get_moles)    { return wassiljewa_herning_zipperer_rule <scalar>(table, molar_masses, molar_fractions <scalar>(get_moles)); }
    template<typename scalar> auto surface_tension_as_liquid   (const auto& table, const auto& molar_densities, const auto& get_moles) { return winterfeld_scriven_davis_rule <scalar>(table, molar_densities, molar_fractions <scalar>(get_moles)); }

    template<typename scalar> auto dynamic_viscosity_as_gas    (const auto& table, const auto& molar_masses)    { return herning_zipperer_rule <scalar>(table, molar_masses, molar_fractions <scalar>(Identity())); }
    template<typename scalar> auto thermal_conductivity_as_gas (const auto& table, const auto& molar_masses)    { return wassiljewa_herning_zipperer_rule <scalar>(table, molar_masses, molar_fractions <scalar>(Identity())); }
    template<typename scalar> auto surface_tension_as_liquid   (const auto& table, const auto& molar_densities) { return winterfeld_scriven_davis_rule <scalar>(table, molar_densities, molar_fractions <scalar>(Identity())); }

    // template<typename Table, typename GetFractions>
    // class GeneralRelationRule
    // {
    //     const Table table;
    //     const GetFractions get_fractions;
    // public:
    //     GeneralRelationLinearRule(const Table& table, const GetFractions& get_fractions):
    //         table(table),
    //         get_fractions(get_fractions)
    //     {}
    //     template<typename Constituents>
    //     auto operator() (const Constituents& constituents) const 
    //     {
    //         auto fractions = get_fractions(constituents);
    //         return compound::relation::GenericRelation<Tx, Ty>(
    //             [=](Tx x){
    //                 auto result = fractions(i,constituents[0])*table[0];
    //                 for (std::size_t i=1; i<constituents.size(); i++) {
    //                     result += fractions(i,constituents[i])*table[i];
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


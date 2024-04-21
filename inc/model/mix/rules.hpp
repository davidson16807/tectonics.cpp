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
            typename Table::value_type result = fractions(constituents[0])*table[0];
            for (std::size_t i=1; i<constituents.size(); i++) 
            {
                result += fractions(constituents[i])*table[i];
            }
            return result;
        }
    };
    template<typename Table, typename GetFractions>
    LinearRule<Table,GetFractions> linear_rule(const Table& table, const GetFractions& get_fractions)
    {
        return LinearRule<Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar, typename Table> auto acentric_factor                  (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); }
    template<typename scalar, typename Table> auto molar_mass                       (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); }
    template<typename scalar, typename Table> auto molecular_diameter               (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); }
    template<typename scalar, typename Table> auto molar_volume_as_liquid           (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Amgat's Law"
    template<typename scalar, typename Table> auto critical_temperature             (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Kay's Rule"
    template<typename scalar, typename Table> auto critical_compressibility         (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // mole weighted arithmetic mean
    template<typename scalar, typename Table> auto critical_volume                  (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Amgat's Law"
    template<typename scalar, typename Table> auto latent_heat_of_vaporization      (const Table& table){ return linear_rule (table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto latent_heat_of_fusion            (const Table& table){ return linear_rule (table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto latent_heat_of_sublimation       (const Table& table){ return linear_rule (table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto specific_heat_capacity_as_gas    (const Table& table){ return linear_rule (table, MassFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto specific_heat_capacity_as_liquid (const Table& table){ return linear_rule (table, MassFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto specific_heat_capacity_as_solid  (const Table& table){ return linear_rule (table, MassFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto molar_heat_capacity_as_gas       (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto molar_heat_capacity_as_liquid    (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto molar_heat_capacity_as_solid     (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Kopp's Law"
    template<typename scalar, typename Table> auto vapor_pressure_as_liquid         (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Raoult's Law"
    template<typename scalar, typename Table> auto vapor_pressure_as_solid          (const Table& table){ return linear_rule (table, MolarFractions<scalar>()); } // "Raoult's Law", see Goldfarb 2008 for sample of validity


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
            auto result = fractions(constituents[0]) / table[0];
            for (std::size_t i=1; i<table.size(); i++){
                result += fractions(constituents[i]) / table[i];
            }
            return scalar(1) / result;
        }
    };
    template<typename scalar, typename Table, typename GetFractions>
    ParallelRule<scalar,Table,GetFractions> parallel_rule(const Table& table, const GetFractions& get_fractions)
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
            auto result = fractions(constituents[0]) / (lambda*lambda);
            for (std::size_t i=1; i<table.size(); i++){
                lambda = table[i];
                result += fractions(constituents[i]) / (lambda*lambda);
            }
            return scalar(1) / si::sqrt(lambda);
        }
    };
    template<typename scalar, typename Table, typename GetFractions>
    SquareParallelRule<scalar,Table,GetFractions> square_parallel_rule(const Table& table, const GetFractions& get_fractions)
    {
        return SquareParallelRule<scalar,Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar, typename Table> auto density_as_liquid                      (const Table& table){ return parallel_rule        (table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto density_as_solid                       (const Table& table){ return parallel_rule        (table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto molar_density_as_liquid                (const Table& table){ return parallel_rule        (table, MolarFractions<scalar>()); }
    template<typename scalar, typename Table> auto molar_density_as_solid                 (const Table& table){ return parallel_rule        (table, MolarFractions<scalar>()); }
    template<typename scalar, typename Table> auto tensile_modulus_as_solid               (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto shear_modulus_as_solid                 (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto bulk_modulus_as_solid                  (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto pwave_modulus_as_solid                 (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto lame_parameter_as_solid                (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto poisson_ratio_as_solid                 (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto tensile_yield_strength_as_solid        (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto shear_yield_strength_as_solid          (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto compressive_yield_strength_as_solid    (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto tensile_fracture_strength_as_solid     (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto shear_fracture_strength_as_solid       (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto compressive_fracture_strength_as_solid (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto thermal_conductivity_as_solid          (const Table& table){ return parallel_rule        (table, VolumeFractions<scalar>()); }
    template<typename scalar, typename Table> auto thermal_conductivity_as_liquid         (const Table& table){ return square_parallel_rule (table, MassFractions<scalar>()); }

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
            auto result = fractions(constituents[0])*table[0];
            for (std::size_t i=1; i<constituents.size(); i++) 
            {
                result += fractions(constituents[i])*std::log(table[i]);
            }
            return result;
        }
    };
    template<typename Table, typename GetFractions>
    LogarithmicRule<Table,GetFractions> logarithmic_rule(const Table& table, const GetFractions& get_fractions)
    {
        return LogarithmicRule<Table,GetFractions>(table, get_fractions);
    }

    template<typename scalar, typename Table> auto dynamic_viscosity_as_liquid (const Table& table){ return logarithmic_rule(table, MassFractions<scalar>()); }
    template<typename scalar, typename Table> auto dynamic_viscosity_as_solid  (const Table& table){ return logarithmic_rule(table, MassFractions<scalar>()); }

    template<typename scalar, typename Table, typename MolarMassTable>
    class HerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const MolarFractions<scalar> get_fractions;
    public:
        HerningZippererRule(const Table& table, const MolarMassTable& molar_masses):
            table(table),
            molar_masses(molar_masses),
            get_fractions()
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto fractions = get_fractions(constituents);
            auto numerator   = fractions(constituents[0]) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole)) * table[0];
            auto denominator = fractions(constituents[0]) * std::sqrt(molar_masses[0]/(si::kilogram/si::mole));
            for (std::size_t i=1; i<constituents.size(); i++) {
                numerator   += fractions(constituents[i]) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole)) * table[i];
                denominator += fractions(constituents[i]) * std::sqrt(molar_masses[i]/(si::kilogram/si::mole));
            }
            return numerator/denominator;
        }
    };
    template<typename scalar, typename Table, typename MolarMassTable>
    HerningZippererRule<scalar,Table,MolarMassTable> herning_zipperer_rule(
        const Table& table, const MolarMassTable& molar_masses)
    {
        return HerningZippererRule<scalar,Table,MolarMassTable>(table, molar_masses);
    }

    template<typename scalar, typename Table, typename MolarMassTable>
    class WassiljewaHerningZippererRule
    {
        const Table table;
        const MolarMassTable molar_masses;
        const MolarFractions<scalar> get_fractions;
    public:
        WassiljewaHerningZippererRule(const Table& table, const MolarMassTable& molar_masses):
            table(table),
            molar_masses(molar_masses),
            get_fractions()
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto numerator = fractions(constituents[0]) * table[0];
            auto denominator = fractions(constituents[0]) * std::sqrt(molar_masses[0]/molar_masses[0]);
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        denominator += fractions(constituents[i]) * std::sqrt(molar_masses[j]/molar_masses[i]);
                    }
                }
                if (i!=0)
                {
                    numerator += fractions(constituents[i]) * table[i];
                }
            }
            return numerator/denominator;
        }
    };
    template<typename scalar, typename Table, typename MolarMassTable>
    WassiljewaHerningZippererRule<scalar,Table,MolarMassTable> wassiljewa_herning_zipperer_rule(
        const Table& table, const MolarMassTable& molar_masses)
    {
        return WassiljewaHerningZippererRule<scalar,Table,MolarMassTable>(table, molar_masses);
    }

    template<typename scalar, typename Table, typename MolarDensityTable>
    class WinterfeldScrivenDavisRule
    {
        const Table table;
        const MolarDensityTable molar_densities;
        const MolarFractions<scalar> get_fractions;
    public:
        WinterfeldScrivenDavisRule(const Table& table, const MolarDensityTable& molar_densities):
            table(table),
            molar_densities(molar_densities),
            get_fractions()
        {}
        template<typename Constituents>
        auto operator() (const Constituents& constituents) const 
        {
            auto molar_density_of_mix = molar_density_as_liquid(constituents);
            auto result = std::sqrt(table[0]*table[0]) * 
                molar_density_of_mix * fractions(constituents[0]) * molar_density_of_mix * fractions(constituents[0]) /
                molar_densities[0]                                * molar_densities[0];
            for (std::size_t i=0; i<constituents.size(); i++) {
                for (std::size_t j=0; j<constituents.size(); j++) {
                    if (i!=0 && j!=0)
                    {
                        result += std::sqrt(table[i]*table[j]) * 
                            molar_density_of_mix * fractions(constituents[i]) * molar_density_of_mix * fractions(constituents[j]) /
                            molar_densities[i]                                * molar_densities[j];
                    }
                }
            }
            return result;
        }
    };
    template<typename scalar, typename Table, typename MolarDensityTable>
    WinterfeldScrivenDavisRule<scalar,Table,MolarDensityTable> winterfeld_scriven_davis_rule(
        const Table& table, const MolarDensityTable& molar_densities)
    {
        return WinterfeldScrivenDavisRule<scalar,Table,MolarDensityTable>(table, molar_densities);
    }

    template<typename scalar, typename Table, typename MolarMassTable>    auto dynamic_viscosity_as_gas    (const Table& table, const MolarMassTable& molar_masses)      { return herning_zipperer_rule            <scalar>(table, molar_masses); }
    template<typename scalar, typename Table, typename MolarMassTable>    auto thermal_conductivity_as_gas (const Table& table, const MolarMassTable& molar_masses)      { return wassiljewa_herning_zipperer_rule <scalar>(table, molar_masses); }
    template<typename scalar, typename Table, typename MolarDensityTable> auto surface_tension_as_liquid   (const Table& table, const MolarDensityTable& molar_densities){ return winterfeld_scriven_davis_rule    <scalar>(table, molar_densities); }

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
    //                 auto result = fractions(constituents[0])*table[0];
    //                 for (std::size_t i=1; i<constituents.size(); i++) {
    //                     result += fractions(constituents[i])*table[i];
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


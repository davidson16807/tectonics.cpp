#pragma once

// 3rd party libraries

namespace mixture{
namespace solid{

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> linear_mix(const std::vector<PolynomialRailyardRelation<Tx,Ty,Plo,Phi>>& relations, const std::vector<float>& ratios)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result;
        for (std::size_t i=0; i<relations.size(); i++){
            const float Tscale = float(relations[i].xunits / result.xunits);
            const float yscale = float(relations[i].yunits / result.yunits);
            result.yard += yscale * ratios[i] * compose(relations[i].yard, analytic::Scaling(Tscale));
        }
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> logarithmic_mix(const std::vector<ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>>& relations, const std::vector<float>& ratios)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result;
        for (std::size_t i=0; i<relations.size(); i++){
            const float Tscale = float(relations[i].xunits / result.xunits);
            const float yscale = float(relations[i].yunits / result.yunits);
            result.yard += yscale * ratios[i] * compose(relations[i].yard, analytic::Scaling(Tscale));
        }
        return result;
    }

    std::vector<double> mass_fractions(const std::vector<si::mass<double>>& masses){
        si::mole<double> sum(0);
        for (std::size_t i=0; i<masses.size(); i++) {
            sum += masses[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<masses.size(); i++) {
            result.emplace_back(masses[i]/sum);
        }
        return masses;
    }

    std::vector<double> mass_fractions(const std::vector<si::density<double>>& densities){
        si::mole<double> sum(0);
        for (std::size_t i=0; i<densities.size(); i++) {
            sum += densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<densities.size(); i++) {
            result.emplace_back(densities[i]/sum);
        }
        return densities;
    }

    std::vector<double> molar_fractions(const std::vector<si::mole<double>>& moles){
        si::mole<double> sum(0);
        for (std::size_t i=0; i<moles.size(); i++) {
            sum += moles[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<moles.size(); i++) {
            result.emplace_back(moles[i]/sum);
        }
        return moles;
    }

    std::vector<double> molar_fractions(const std::vector<si::molar_density<double>>& molar_densities){
        si::mole<double> sum(0);
        for (std::size_t i=0; i<molar_densities.size(); i++) {
            sum += molar_densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<molar_densities.size(); i++) {
            result.emplace_back(molar_densities[i]/sum);
        }
        return molar_densities;
    }

    std::vector<double> molar_fractions(const std::vector<double>& numbers){
        numbers sum(0);
        for (std::size_t i=0; i<numbers.size(); i++) {
            sum += numbers[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<numbers.size(); i++) {
            result.emplace_back(numbers[i]/sum);
        }
        return numbers;
    }

    std::vector<double> molar_fractions(const std::vector<si::number_density<double>>& number_densities){
        si::mole<double> sum(0);
        for (std::size_t i=0; i<number_densities.size(); i++) {
            sum += number_densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<number_densities.size(); i++) {
            result.emplace_back(number_densities[i]/sum);
        }
        return number_densities;
    }

    template<typename TProperty>
    si::molar_mass<double> molar_mass(const std::vector<si::molar_mass<double>>& molar_masses, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        si::molar_mass<double> result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * molar_masses[i];
        }
        return result;
    }

    template<typename TProperty>
    si::pressure<double> molecular_diameter(const std::vector<si::molecular_diameter<double>>& molar_masses, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        si::molecular_diameter<double> result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * molar_masses[i];
        }
        return result;
    }

    // "Kay's Rule"
    template<typename TProperty>
    si::temperature<double> critical_temperature(const std::vector<si::temperature<double>>& critical_point_temperatures, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        si::temperature<double> result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * critical_point_temperatures[i];
        }
        return result;
    }

    // arithmetic mixing rule
    template<typename TProperty>
    double critical_compressibility(const std::vector<double>& critical_point_compressibilities, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        double result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * critical_point_compressibilities[i];
        }
        return result;
    }

    // Yorizane
    double critical_compressibility(const double acentric_factor){
        return 0.291 - 0.08*acentric_factor;
    }

    // "Amgat's Law"
    template<typename TProperty>
    si::molar_volume<double> critical_volume(const std::vector<si::molar_volume<double>>& volumes, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        si::molar_volume<double> result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * volumes[i];
        }
        return result;
    }

    // "Prausnitz-Gunn"
    template<typename TProperty>
    si::pressure<double> critical_pressure(const si::temperature<double> critical_temperature, const si::molar_volume<double> critical_volume, const double critical_compressibility){
        return si::universal_gas_constant * critical_temperature * critical_compressibility / critical_volume;
    }

    // "Amgat's Law"
    template<typename TProperty>
    double molar_volume_as_liquid(const std::vector<si::molar_volume<double>>& volumes, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        si::molar_volume<double> result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * volumes[i];
        }
        return result;
    }

    // "Amgat's Law"
    template<typename TProperty>
    double density_as_gas(
        const si::molar_mass<double>  molar_mass,
        const si::pressure<double>    critical_pressure,
        const si::temperature<double> critical_temperature,
        const double                  critical_compressibility
    ){
        return GasDensityStateRelation(
            molar_mass,
            critical_pressure,
            critical_temperature,
            critical_compressibility
        );
    }

    // arithmetic mixing rule for specific volume
    template<typename TProperty>
    auto density_as_liquid(const std::vector<LiquidDensityTemperatureRelation>& densities, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::density<double>>(
            [](si::temperature<double> T) { 
                si::specific_volume<double> specific_volume(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                    specific_volume += fractions[i] / densities[i];
                }
                return 1.0 / specific_volume;
            }
        );
    }

    // arithmetic mixing rule for specific volume
    template<typename TProperty>
    auto density_as_solid(const std::vector<SolidDensityTemperatureRelation>& densities, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::density<double>>(
            [](si::temperature<double> T) { 
                si::specific_volume<double> specific_volume(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                    specific_volume += fractions[i] / densities[i];
                }
                return 1.0 / specific_volume;
            }
        );
    }

    // arithmetic mixing rule
    template<typename TProperty>
    double acentric_factor(const std::vector<double>& acentric_factors, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        double result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * acentric_factors[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_gas(const std::vector<GasHeatCapacityStateRelation>& specific_heat_capacities_as_gas, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        GasHeatCapacityStateRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * specific_heat_capacities_as_gas[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    double molar_heat_capacity_as_gas(const std::vector<GasPropertyStateRelation<si::molar_heat_capacity<double>>>& molar_heat_capacities_as_gas, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        GasMolarHeatCapacityStateRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * molar_heat_capacities_as_gas[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_liquid(const std::vector<LiquidHeatCapacityTemperatureRelation>& specific_heat_capacities_as_liquid, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        LiquidHeatCapacityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * specific_heat_capacities_as_liquid[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    double molar_heat_capacity_as_liquid(const std::vector<LiquidMolarHeatCapacityTemperatureRelation>& molar_heat_capacities_as_liquid, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        LiquidMolarHeatCapacityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * molar_heat_capacities_as_liquid[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    double specific_heat_capacity_as_solid(const std::vector<SolidHeatCapacityTemperatureRelation>& specific_heat_capacities_as_solid, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        SolidHeatCapacityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += fractions[i] * specific_heat_capacities_as_solid[i];
        }
        return result;
    }

    // "Kopp's Law"
    template<typename TProperty>
    auto molar_heat_capacity_as_solid(const std::vector<SolidMolarHeatCapacityTemperatureRelation>& molar_heat_capacities_as_solid, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        SolidMolarHeatCapacityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += float(fractions[i]) * molar_heat_capacities_as_solid[i];
        }
        return result;
    }

    // "Herning-Zipperer"
    template<typename TProperty>
    auto dynamic_viscosity_as_gas(const std::vector<GasDynamicViscosityStateRelation>& dynamic_viscosity_as_gas, const std::vector<TProperty>& molar_property, const std::vector<si::molar_mass<double>>& molar_masses){
        std::vector<double> fractions = molar_fractions(molar_property);
        GasDynamicViscosityStateRelation numerator(0);
        double denominator(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            numerator   += fractions[i] * std::sqrt(molar_masses[i]/(si::kilogram/si::mole)) * dynamic_viscosity_as_gas[i];
            denominator += fractions[i] * std::sqrt(molar_masses[i]/(si::kilogram/si::mole));
        }
        return numerator/denominator;
    }

    // logarithmic mixing rule
    template<typename TProperty>
    auto dynamic_viscosity_as_liquid(const std::vector<LiquidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_liquid, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        LiquidDynamicViscosityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += exp(fractions[i] * ln(dynamic_viscosity_as_liquid[i]));
        }
        return result;
    }

    // logarithmic mixing rule
    template<typename TProperty>
    auto dynamic_viscosity_as_solid(const std::vector<SolidDynamicViscosityTemperatureRelation>& dynamic_viscosity_as_solid, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        SolidDynamicViscosityTemperatureRelation result;
        for (std::size_t i=0; i<relations.size(); i++){
            const float Tscale = float(relations[i].xunits / result.xunits);
            const float yscale = float(relations[i].yunits / result.yunits);
            result.yard += yscale * ratios[i] * compose(relations[i].yard, analytic::Scaling(Tscale));
        }
        return result;
    }

    // DIPPR 9H
    template<typename TProperty>
    auto thermal_conductivity_as_liquid(const std::vector<LiquidThermalConductivityTemperatureRelation>& thermal_conductivity_as_liquid, const std::vector<TProperty>& mass_property){
        std::vector<double> fractions = mass_fractions(mass_property);
        return GenericRelation<si::temperature<double>, si::thermal_conductivity<double>>(
            [](si::temperature<double> T) { 
                si::thermal_conductivity<double> result(0);
                for (std::size_t i=0; i<fractions.size(); i++) {
                    auto lambda = thermal_conductivity_as_liquid[i](T);
                    result += fractions[i] / (lambda*lambda);
                }
                return std::sqrt(result);
            }
        );
    }

    // Wassiljewa-Herning-Zipperer
    template<typename TProperty>
    auto thermal_conductivity_as_gas(const std::vector<GasThermalConductivityStateRelation>& thermal_conductivity_as_gas, const std::vector<TProperty>& mass_property, const std::vector<si::molar_mass<double>>& molar_masses){
        std::vector<double> fractions = mass_fractions(mass_property);
        GasThermalConductivityStateRelation numerator;
        for (std::size_t i=0; i<fractions.size(); i++) {
            double denominator(0);
            for (std::size_t j=0; j<fractions.size(); j++) {
                denominator += fractions[i] * std::sqrt(molar_masses[j]/molar_masses[i]);
            }
            numerator += fractions[i] * thermal_conductivity_as_gas[i];
        }
        return numerator / denominator;
    }

    // "Raoult's Law"
    template<typename TProperty>
    auto vapor_pressure_as_liquid(const std::vector<LiquidMolarHeatCapacityTemperatureRelation>& vapor_pressures_as_liquid, const std::vector<TProperty>& molar_property){
        std::vector<double> fractions = molar_fractions(molar_property);
        LiquidMolarHeatCapacityTemperatureRelation result(0);
        for (std::size_t i=0; i<fractions.size(); i++) {
            result += float(fractions[i]) * vapor_pressures_as_liquid[i];
        }
        return result;
    }

    const std::vector<LatentHeatTemperatureRelation> latent_heat_of_vaporization;
    const std::vector<LatentHeatTemperatureRelation> latent_heat_of_sublimation;
    const std::vector<LatentHeatTemperatureRelation> latent_heat_of_fusion;
    const std::vector<LiquidVaporPressureTemperatureRelation> vapor_pressure_as_liquid;
    const std::vector<SolidVaporPressureTemperatureRelation> vapor_pressure_as_solid;
    const std::vector<SolidThermalConductivityTemperatureRelation> thermal_conductivity_as_solid;
    const std::vector<SolidRefractiveIndexWavenumberRelation> refractive_index;
    const std::vector<SolidExtinctionCoefficientWavenumberRelation> extinction_coefficient;
    const std::vector<SolidAbsorptionCoefficientWavenumberRelation> absorption_coefficient;
    const std::vector<SolidShearYieldStrengthTemperatureRelation> shear_yield;
    const std::vector<SolidTensileYieldStrengthTemperatureRelation> tensile_yield;
    const std::vector<SolidCompressiveYieldStrengthTemperatureRelation> compressive_yield;
    const std::vector<SolidShearFractureStrengthTemperatureRelation> shear_fracture;
    const std::vector<SolidTensileFractureStrengthTemperatureRelation> tensile_fracture;
    const std::vector<SolidCompressiveFractureStrengthTemperatureRelation> compressive_fracture;
    const std::vector<SolidBulkModulusTemperatureRelation> bulk_modulus;
    const std::vector<SolidTensileModulusTemperatureRelation> tensile_modulus;
    const std::vector<SolidLameParameterTemperatureRelation> lame_parameter;
    const std::vector<SolidShearModulusTemperatureRelation> shear_modulus;
    const std::vector<SolidPoissonRatioTemperatureRelation> poisson_ratio;
    const std::vector<SolidPwaveModulusTemperatureRelation> pwave_modulus;

    const std::vector<point<double>> critical_point;
    const std::vector<point<double>> triple_point;
    const std::vector<point<double>> freezing_point;
    const std::vector<point<double>> boiling_point;
    const std::vector<point<double>> solid_point;
    const std::vector<point<double>> liquid_point;
    const std::vector<point<double>> gas_point;

    const std::vector<si::temperature<double>> freezing_point_temperature;
    const std::vector<si::pressure<double>> freezing_point_pressure;
    const std::vector<si::temperature<double>> boiling_point_temperature;
    const std::vector<si::pressure<double>> boiling_point_pressure;
    const std::vector<si::temperature<double>> triple_point_temperature;
    const std::vector<si::pressure<double>> triple_point_pressure;

    const std::vector<double> molecular_degrees_of_freedom;
}}

/*
mixture laws:
    gases:
        thermal conductivity: wassiljewa-herning-zipperer (chemicals)
    liquids:
        vapor pressure: Raoult's Law, has no effect on structure of approximations
        heat capacity:  mass weighted arithmetic mean (chemicals)
        thermal conductivity: dippr9 (chemicals)
        surface tension: weinaug-katz, winterfeld-scriven-davis, diguilio-teja (chemicals)
        density: costald (chemicals)
    solids:
        vapor pressure: Raoult's Law, has no effect on structure of approximations
        strengths:      Rule of mixtures (see wikipedia)
        moduli:         Rule of mixtures (see wikipedia)
        thermal conductivity: Rule of mixtures (see wikipedia)
*/

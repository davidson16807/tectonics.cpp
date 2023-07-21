#pragma once

#include <cmath>

#include <units/si.hpp>

namespace mix{

    std::vector<double> log(const std::vector<double>& fractions){
        std::vector<double> result;
        for (std::size_t i=0; i<fractions.size(); i++) {
            result.emplace_back(std::log(fractions[i]));
        }
        return result;
    }

    std::vector<double> volume_fractions(const std::vector<si::volume<double>>& volumes){
        si::volume<double> sum(0);
        for (std::size_t i=0; i<volumes.size(); i++) {
            sum += volumes[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<volumes.size(); i++) {
            result.emplace_back(volumes[i]/sum);
        }
        return result;
    }

    std::vector<double> mass_fractions(const std::vector<si::mass<double>>& masses){
        si::mass<double> sum(0);
        for (std::size_t i=0; i<masses.size(); i++) {
            sum += masses[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<masses.size(); i++) {
            result.emplace_back(masses[i]/sum);
        }
        return result;
    }

    std::vector<double> mass_fractions(const std::vector<si::density<double>>& densities){
        si::density<double> sum(0);
        for (std::size_t i=0; i<densities.size(); i++) {
            sum += densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<densities.size(); i++) {
            result.emplace_back(densities[i]/sum);
        }
        return result;
    }

    std::vector<double> molar_fractions(const std::vector<si::amount<double>>& moles){
        si::amount<double> sum(0);
        for (std::size_t i=0; i<moles.size(); i++) {
            sum += moles[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<moles.size(); i++) {
            result.emplace_back(moles[i]/sum);
        }
        return result;
    }

    std::vector<double> molar_fractions(const std::vector<si::molar_density<double>>& molar_densities){
        si::molar_density<double> sum(0);
        for (std::size_t i=0; i<molar_densities.size(); i++) {
            sum += molar_densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<molar_densities.size(); i++) {
            result.emplace_back(molar_densities[i]/sum);
        }
        return result;
    }

    std::vector<double> molar_fractions(const std::vector<si::number_density<double>>& number_densities){
        si::number_density<double> sum(0);
        for (std::size_t i=0; i<number_densities.size(); i++) {
            sum += number_densities[i];
        }
        std::vector<double> result;
        for (std::size_t i=0; i<number_densities.size(); i++) {
            result.emplace_back(number_densities[i]/sum);
        }
        return result;
    }

    std::vector<double> mass_fractions(const std::vector<double>& molar_fractions, const std::vector<si::molar_mass<double>>& molar_masses){
        std::vector<double> result;
        si::molar_mass<double> average_molar_mass(0);
        for (std::size_t j=0; j<molar_fractions.size(); j++) {
            average_molar_mass += (molar_fractions[j] * molar_masses[j]);
        }
        for (std::size_t i=0; i<molar_fractions.size(); i++) {
            result.emplace_back(molar_fractions[i] * molar_masses[i] / average_molar_mass);
        }
        return result;
    }

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

    // struct Fractions
    // {
    //     const std::vector<si::molar_mass<double>> molar_mass;
    //     const std::vector<double> molar_fractions;
    //     const std::vector<double> mass_fractions;

    //     template<typename TProperty>
    //     Fractions(
    //         const std::vector<TProperty>& molar_property,
    //         const std::vector<si::molar_mass<double>>& molar_masses
    //     ):
    //         molar_masses(molar_masses),
    //         molar_fractions(molar_fractions(molar_property)),
    //         mass_fractions(mass_fractions(molar_fractions(molar_property), molar_masses))
    //     {}

    //     template<typename TProperty>
    //     Fractions(
    //         const std::vector<TProperty>& mass_property,
    //         const std::vector<si::molar_mass<double>>& molar_masses
    //     ):
    //         molar_masses(molar_masses),
    //         molar_fractions(molar_fractions(mass_fractions(mass_property), molar_masses)),
    //         mass_fractions(mass_fractions(mass_property))
    //     {}

    // };

    // std::vector<double> mass_fractions(const Fractions& fractions){
    //     return fractions.mass_fractions;
    // }

    // std::vector<double> molar_fractions(const Fractions& fractions){
    //     return fractions.mass_fractions;
    // }

}
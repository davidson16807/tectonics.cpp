#pragma once

#include <array>

// in-house libraries
#include <units/si.hpp>

#include <model/stratum/Stratum.hpp>

namespace strata
{
    template <std::size_t L, std::size_t M>
    struct Strata
    {
        std::array<stratum::Stratum<M>, L> content;
        std::size_t count;

        ~Strata(){}
        Strata(): content(), count(0){}
        Strata(const std::initializer_list<stratum::Stratum<M>>& vector): content(), count(vector.size())
        {
            assert(vector.size() <= L);
            int j(0);
            for (auto i = vector.begin(); i != vector.end(); ++i)
            {
                content[j] = *i;
                ++j;
            }
        }

        // DERIVED ATTRIBUTES, regular functions of the form: Strata -> primitive
        si::mass<double> mass_pool(const std::size_t id) const {
            si::mass<double> total_mass(0.0);
            for (std::size_t i=0; i<count; i++)
            {
                total_mass += content[i].mass_pools[id].mass;
            }
            return total_mass;
        }
        si::mass<double> mass() const {
            si::mass<double> total_mass(0.0);
            for (std::size_t i=0; i<count; i++)
            {
                total_mass += content[i].mass();
            }
            return total_mass;
        }
        si::volume<double> volume(const std::array<si::density<double>, M>& mass_pool_densities) const {
            si::volume<double> total_volume(0.0);
            for (std::size_t i=0; i<count; i++)
            {
                total_volume += content[i].volume(mass_pool_densities);
            }
            return total_volume;
        }
        si::density<double> density(const std::array<si::density<double>, M>& mass_pool_densities) const {
            return mass() / volume(mass_pool_densities);
        }

    };
}

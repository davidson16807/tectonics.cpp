#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/mineral/_test_tools.hpp>

#include "Stratum.hpp"
#include "StratumStore.hpp"
#include "StratumSummary.hpp"

namespace rock
{

    template<std::size_t M, typename Tgenerator>
    Stratum<M> get_random(Tgenerator& generator)
    {
        std::uniform_real_distribution<float> uniform;
        si::time<double> age_of_world_when_first_deposited =        65.0 * uniform(generator) * si::gigayear;
        si::time<double> age_of_world_when_last_deposited = si::min(65.0 * uniform(generator) * si::gigayear + age_of_world_when_first_deposited, 65.0 * si::gigayear);
        Stratum<M> output(
            age_of_world_when_first_deposited, age_of_world_when_last_deposited            
        );
        for (std::size_t i = 0; i < M; ++i)
        {
            output.minerals[i] = rock::get_random(generator);
        }
        return output;
    }

    template<std::size_t M>
    struct StratumAdapter{
        const MineralAdapter minerals;
        const float float_threshold;

        StratumAdapter():
            minerals(),
            float_threshold(0.02)
        {}

        bool equal(const Stratum<M>& a, const Stratum<M>& b) const {
            if(si::distance(a.age_of_world_when_first_deposited, b.age_of_world_when_first_deposited) 
                > float_threshold * si::max(si::second, si::max(a.age_of_world_when_first_deposited,b.age_of_world_when_first_deposited)) )
            {
                return false;
            }
            if(si::distance(a.age_of_world_when_last_deposited, b.age_of_world_when_last_deposited) 
                > float_threshold * si::max(si::second, si::max(a.age_of_world_when_last_deposited,b.age_of_world_when_last_deposited)) )
            {
                return false;
            }
            for (std::size_t pool_i = 0; pool_i < M; ++pool_i)
            {
                if(!minerals.equal(a.minerals[pool_i], b.minerals[pool_i])){
                    return false;
                }
            }
            return true;
        }

        bool is_valid(Stratum<M>& a) const {
            if(a.age_of_world_when_first_deposited/si::second < -float_threshold)
            {
                return false;
            }
            if(a.age_of_world_when_last_deposited/si::second < -float_threshold)
            {
                return false;
            }
            for (std::size_t pool_i = 0; pool_i < M; ++pool_i)
            {
                if (!minerals.is_valid(a.minerals[pool_i]))
                {
                    return false;
                }
            }
            return true;
        }

        std::string print(const Stratum<M>& a) const {
            std::ostringstream os;
            os << "age of world when first deposited: ";
            os << si::to_string(a.age_of_world_when_first_deposited);
            os << "\n";
            os << "age of world when last deposited: ";
            os << si::to_string(a.age_of_world_when_last_deposited);
            os << "\n";
            os << "minerals: ";
            os << "\n";
            for (int i = 0; i < int(GrainType::count); ++i)
            {
                os << minerals.print(a.minerals[i]);
                os << "\n";
            }
            os << "\n";
            return os.str();
        }

        std::string print(const StratumStore<M>& a) const {
            std::ostringstream os;
            os << "[encapsulated]";
            return os.str();
        }

        std::string print(const double a) const {
            std::ostringstream os;
            os << std::to_string(a);
            return os.str();
        }

        std::string print(const float a) const {
            std::ostringstream os;
            os << std::to_string(a);
            return os.str();
        }

    };

    struct StratumSummaryAdapter{
        const float float_threshold;

        StratumSummaryAdapter():
            float_threshold(0.001)
        {}

        bool equal(const StratumSummary& a, const StratumSummary& b) const {
            if(si::distance(a.density(), b.density()) 
                > si::max(si::density<float>(si::kilogram/si::meter3), float_threshold * si::max(a.density(),b.density())) )
            {
                return false;
            } 
            if(si::distance(a.thickness(), b.thickness()) 
                > si::max(si::length<float>(si::meter), float_threshold * si::max(a.thickness(),b.thickness())) )
            {
                return false;
            } 
            if(a.plate_ids_bitset() != b.plate_ids_bitset())
            {
                return false;
            }
            return true;
        }

        std::string print(const StratumSummary& a) const {
            std::ostringstream os;
            os << "thickness:       ";
            os << si::to_string(a.thickness());
            os << "\n";
            os << "density:         ";
            os << si::to_string(a.density());
            os << "\n";
            os << "plate id bitset: ";
            os << a.plate_ids_bitset().to_string();
            os << "\n";
            return os.str();
        }

        std::string print(const double a) const {
            std::ostringstream os;
            os << std::to_string(a);
            return os.str();
        }

        std::string print(const float a) const {
            std::ostringstream os;
            os << std::to_string(a);
            return os.str();
        }

    };

}


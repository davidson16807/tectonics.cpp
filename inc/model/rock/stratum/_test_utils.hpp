#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/mineral/_test_utils.hpp>

#include "Stratum.hpp"
#include "StratumStore.hpp"

namespace rock
{

    template<std::size_t M, typename Tgenerator>
    Stratum<M> get_random(Tgenerator& generator)
    {
        std::uniform_real_distribution<float> uniform;
        Stratum<M> output(
            32768.0f * uniform(generator) * si::kelvin, 
            6e12f    * uniform(generator) * si::pascal, 
            65000.0f * uniform(generator) * si::megayear
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
            if(si::distance(a.max_pressure_received, b.max_pressure_received) 
                > si::max(si::pascal, float_threshold * si::max(a.max_pressure_received,b.max_pressure_received)) )
            {
                return false;
            } 
            if(si::distance(a.max_temperature_received, b.max_temperature_received) 
                > si::max(si::kelvin, float_threshold * si::max(a.max_temperature_received,b.max_temperature_received)) )
            {
                return false;
            } 
            if(si::distance(a.age_of_world_when_deposited, b.age_of_world_when_deposited) 
                > float_threshold * si::max(si::second, si::max(a.age_of_world_when_deposited,b.age_of_world_when_deposited)) )
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
            if(a.max_pressure_received/si::pascal < -float_threshold)
            {
                return false;
            }
            if(a.max_temperature_received/si::kelvin < -float_threshold)
            {
                return false;
            }
            if(a.age_of_world_when_deposited/si::second < -float_threshold)
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
            os << "max pressure:    ";
            os << si::to_string(a.max_pressure_received);
            os << "\n";
            os << "max temperature: ";
            os << si::to_string(a.max_temperature_received);
            os << "\n";
            os << "age of world when deposited: ";
            os << si::to_string(a.age_of_world_when_deposited);
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

}


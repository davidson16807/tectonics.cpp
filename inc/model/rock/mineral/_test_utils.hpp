#pragma once

// standard libraries
#include <string>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include "Mineral.hpp"
#include "MineralStore.hpp"
#include "GrainType.hpp"

#include <math/special.hpp>
#include <unit/si.hpp>

namespace rock
{

    template<typename Tgenerator>
    rock::Mineral get_random(Tgenerator& generator)
    {
        rock::Mineral output;
        output.mass = generator() * si::kilogram;
        for (int j = 0; j < int(rock::GrainType::count); ++j)
        {
            output.grain_type_relative_volume[j] = generator();
        }
        return output;
    }

    struct MineralAdapter{
        const float float_threshold;
        const float part_threshold;

        MineralAdapter():
            float_threshold(1e-4),
            part_threshold(0.06)
        {}

        bool equal(const Mineral& a, const Mineral& b) const {
            if (si::distance(a.mass,b.mass) > float_threshold * si::max(si::kilogram, si::max(a.mass,b.mass)))
            {
                return false;
            }
            float a_total_relative_volume(a.grain_type_total_relative_volume());
            float b_total_relative_volume(b.grain_type_total_relative_volume());
            for (int grain_i = 0; grain_i < int(rock::GrainType::count); ++grain_i)
            {
                float a_ratio = a.grain_type_relative_volume[grain_i] / a_total_relative_volume;
                float b_ratio = b.grain_type_relative_volume[grain_i] / b_total_relative_volume;
                if (math::distance(a_ratio, b_ratio) > part_threshold * std::max(1.0f, std::max(a_ratio, b_ratio)))
                {
                    return false;
                }
            }
            return true;
        }

        bool is_valid(Mineral& a) const {
            if(a.mass/si::kilogram < -float_threshold){
                return false;
            }
            for (int grain_i = 0; grain_i < int(rock::GrainType::count); ++grain_i)
            {
                if(a.grain_type_relative_volume[grain_i] < -float_threshold){
                    return false;
                }
            }
            return true;
        }

        std::string print(const Mineral& a) const {
            std::ostringstream os;
            os << "mass:      ";
            os << si::to_string(a.mass);
            os << "\n";
            os << "fractions: ";
            for (int i = 0; i < int(GrainType::count); ++i)
            {
                os << std::to_string(a.grain_type_relative_volume[i]);
                os << ", ";
            }
            os << "\n";
            return os.str();
        }

        std::string print(const MineralStore& a) const {
            std::ostringstream os;
            os << "[encapsulated]";
            return os.str();
        }

    };

}


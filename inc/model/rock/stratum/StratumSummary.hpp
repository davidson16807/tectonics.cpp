#pragma once

// C libraries
#include <cmath> // std::max

// std libraries
#include <bitset>

// in house libraries
#include <unit/si.hpp>

namespace rock{

    class StratumSummary
    {
        using uint = unsigned int;

        static constexpr si::density<float> kilogram_per_meter3 = si::density<float>(si::kilogram/si::meter3);
        static constexpr si::length<float> meter = si::length<float>(si::meter);
        static constexpr unsigned int max_density_in_kilograms_per_meter3 = 1>>24-1;

        std::bitset<8> plate_ids_bitset_; 
        unsigned int density_in_kilograms_per_meter3 : 24;
        float thickness_in_meters;
    public:

        // identity under combination
        constexpr StratumSummary():
            plate_ids_bitset_(0),
            density_in_kilograms_per_meter3(max_density_in_kilograms_per_meter3),
            thickness_in_meters(0)
        {}

        constexpr StratumSummary(
            const std::bitset<8> plate_ids_bitset,
            const si::density<float> density,
            const si::length<float>  thickness
        ):
            plate_ids_bitset_(plate_ids_bitset),
            density_in_kilograms_per_meter3(std::max(max_density_in_kilograms_per_meter3, uint(density/kilogram_per_meter3))),
            thickness_in_meters(thickness/meter)
        {}

        constexpr StratumSummary(
            const StratumSummary& summary
        ):
            plate_ids_bitset_(summary.plate_ids_bitset_),
            density_in_kilograms_per_meter3(summary.density_in_kilograms_per_meter3),
            thickness_in_meters(summary.thickness_in_meters)
        {}

        si::density<float> density() const
        {
            return density_in_kilograms_per_meter3 * kilogram_per_meter3;
        }
        void density(const si::density<float> density)
        {
            density_in_kilograms_per_meter3 = std::max(max_density_in_kilograms_per_meter3, uint(density/kilogram_per_meter3));
        }

        si::length<float> thickness() const
        {
            return thickness_in_meters * meter;
        }
        void thickness(const si::length<float> thickness)
        {
            thickness_in_meters = thickness/meter;
        }

        std::bitset<8> plate_ids_bitset() const
        {
            return plate_ids_bitset_;
        }
        void plate_ids_bitset(const std::bitset<8> value)
        {
            plate_ids_bitset_ = value;
        }


        si::area_density<float> area_density() const
        {
            return density() * thickness();
        }

        int largest_plate_id() const
        {
            return int(std::log2(float(plate_ids_bitset_.to_ulong())));
        }

        int plate_count() const
        {
            return plate_ids_bitset_.count();
        }

        bool includes(const int plate_id) const
        {
            return plate_ids_bitset_.test(plate_id);
        }

    };

}


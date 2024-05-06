#pragma once

// in house libraries

namespace rock{

    class StratumSummary
    {
        std::bitset<8> plate_id_bitset; 
        int density_in_kilograms_per_meter3 : 24;
        float thickness_in_meters;
    public:
        // identity under combination
        constexpr StratumSummary():
            plate_id_bitset(0),
            density_in_kilograms_per_meter3(1<<24),
            thickness_in_meters(0)
        {}
        constexpr StratumSummary(
            const std::bitset<8> plate_id_bitset,
            const si::density<float> density,
            const si::length<float>  thickness
        ):
            plate_id_bitset(plate_id_bitset),
            density_in_kilograms_per_meter3(density/(si::kilogram/si::meter3)),
            thickness_in_meters(thickness/si::meter)
        {}
        si::density<float> density() const
        {
            return density_in_kilograms_per_meter3 * si::kilogram/si::meter3;
        }
        void density(const si::density<float> density)
        {
            density_in_kilograms_per_meter3 = density/(si::kilogram/si::meter3);
        }
        si::length<float> thickness() const
        {
            return thickness_in_meters * si::meter;
        }
        void thickness(const si::length<float> thickness)
        {
            thickness_in_meters = thickness/si::meter;
        }
        std::bitset<8> plate_id_bitset() const
        {
            return plate_id_bitset;
        }
        void plate_id_bitset(const std::bitset<8> plate_ids_bitset_)
        {
            plate_id_bitset = plate_ids_bitset_;
        }

        /* 
        `largest_plate_id` is a convenience method for a common use case 
        where the StratumSummary contains only one plate.
        */
        int largest_plate_id() const
        {
            return int(std::log2(float(plate_id_bitset.to_ulong())));
        }
        /* 
        `largest_plate_id` is a convenience method  
        */
        int plate_count() const
        {
            return plate_id_bitset.count();
        }

    };

}


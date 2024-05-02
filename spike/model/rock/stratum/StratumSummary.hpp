#pragma once

// in house libraries

namespace rock{

    class StratumSummary
    {
        int plate_id_bitfield : 8; 
        int density_in_kilograms_per_meter3 : 24;
        float thickness_in_meters;
    public:
        constexpr StratumSummary(
            const int plate_id_bitfield,
            const si::density<float> density,
            const si::length<float>  thickness
        ):
            plate_id_bitfield(plate_id_bitfield),
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
        int plate_id_bitfield() const
        {
            return plate_id_bitfield;
        }
        void plate_id_bitfield(const int plate_id_bitfield_)
        {
            plate_id_bitfield = plate_id_bitfield_;
        }
    };

}


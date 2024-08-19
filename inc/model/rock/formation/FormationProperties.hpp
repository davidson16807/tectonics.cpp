#pragma once

#include <algorithm> // std::max

// in house libraries
#include "Formation.hpp"

namespace rock{

    // NOTE: `M` is mineral count
    template<int M>
    class FormationMass
    {

        using mass = si::mass<float>;
        using masses = std::vector<mass>;

    public:
        FormationMass(){}

        void operator()(const Formation<M>& formation, masses& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = formation[i].mass();
            }
        }

        mass operator()(const Formation<M>& formation) const {
            mass out(0);
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out += formation[i].mass();
            }
            return out;
        }

    };

    // // NOTE: `M` is mineral count
    // template<int M>
    // class FormationVolume
    // {

    //     using volume = si::volume<float>;
    //     using volumes = std::vector<volume>;

    // public:
    //     FormationVolume(){}

    //     void operator()(const Formation<M>& formation, volumes& out) const {
    //         for (std::size_t i = 0; i < out.size(); ++i)
    //         {
    //             out[i] = formation[i].thickness() * grid.vertex_dual_area(i);
    //         }
    //     }

    //     volume operator()(const Formation<M>& formation) const {
    //         volume out(0);
    //         for (std::size_t i = 0; i < formation.size(); ++i)
    //         {
    //             out += formation[i].thickness() * grid.vertex_dual_area(i);
    //         }
    //         return out;
    //     }

    // };

}

 
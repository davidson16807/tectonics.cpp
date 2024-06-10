#pragma once

// in house libraries
#include <model/rock/stratum/StratumProperties.hpp>

namespace rock{

    template<typename StratumProperty>
    class FormationProperty
    {
        const StratumProperty property;
    public:
        FormationProperty(const StratumProperty& property):
            property(property)
        {}
        void operator() (const auto& formation, auto& out) const
        {
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = property(formation[i]);
            }
        }
    };

}


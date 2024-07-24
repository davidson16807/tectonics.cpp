#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/stratum/_test_tools.hpp>

#include "Formation.hpp"
#include "FormationSummary.hpp"

namespace rock
{

    template<int M>
    struct FormationAdapter{
        const StratumAdapter<M> strata;

        FormationAdapter()
        {}

        bool equal(const Formation<M>& a, const Formation<M>& b) const {
            if (a.size() != b.size())
            {
                return false;
            }

            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (!strata.equal(a[i],b[i]))
                {
                    return false;
                }
            }

            return true;
        }

        std::string print(const Formation<M>& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << strata.print(a[i]);
            }
            return os.str();
        }

    };

    struct FormationSummaryAdapter{
        const StratumSummaryAdapter strata;

        FormationSummaryAdapter()
        {}

        bool equal(const FormationSummary& a, const FormationSummary& b) const {
            if (a.size() != b.size())
            {
                return false;
            }

            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (!strata.equal(a[i],b[i]))
                {
                    return false;
                }
            }

            return true;
        }

        std::string print(const FormationSummary& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << strata.print(a[i]);
            }
            return os.str();
        }

    };

}


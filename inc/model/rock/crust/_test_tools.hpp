#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include <model/rock/column/_test_tools.hpp>
#include <model/rock/formation/_test_tools.hpp>

#include "CrustSummary.hpp"

namespace rock
{

    struct CrustSummaryAdapter{
        const ColumnSummaryAdapter subadapter;

        CrustSummaryAdapter()
        {}

        bool equal(const CrustSummary& a, const CrustSummary& b) const {
            if (a.size() != b.size())
            {
                return false;
            }

            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (!subadapter.equal(a[i],b[i]))
                {
                    return false;
                }
            }

            return true;
        }

        std::string print(const CrustSummary& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << subadapter.print(a[i]) << std::endl;
            }
            return os.str();
        }

        std::string print(const FormationSummary& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << subadapter.print(a[i]) << std::endl;
            }
            return os.str();
        }

    };

    template<int M, int F>
    struct CrustAdapter{
        const FormationAdapter<M> subadapter;

        CrustAdapter()
        {}

        bool equal(const Crust<M,F>& a, const Crust<M,F>& b) const {
            if (a.size() != b.size())
            {
                return false;
            }

            for (std::size_t i = 0; i < a.size(); ++i)
            {
                if (!subadapter.equal(a[i],b[i]))
                {
                    return false;
                }
            }

            return true;
        }

        std::string print(const Crust<M,F>& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << subadapter.print(a[i]) << std::endl;
            }
            return os.str();
        }

    };

}


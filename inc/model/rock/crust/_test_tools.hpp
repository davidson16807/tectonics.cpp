#pragma once

// std libraries
#include <random>

// 3rd party libraries
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>
// #include <unit/_test_tools.cpp>

#include <model/rock/column/_test_tools.hpp>
#include <model/rock/formation/_test_tools.hpp>
#include <model/rock/formation/Formation.hpp>
#include <model/rock/formation/FormationSummary.hpp>
#include <model/rock/crust/Crust.hpp>
#include <model/rock/crust/CrustSummary.hpp>
#include <model/rock/crust/FormationType.hpp>

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
        const float threshold;

        CrustAdapter(const float threshold):
            threshold(threshold)
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

        template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
        bool equal(const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,float>& a, const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,float>& b) const {
            const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,float> denominator(si::abs(a)+si::abs(b));
            return denominator == 0.0*denominator || 2 * si::distance(a, b) / denominator < threshold;
        }

        std::string print(const Crust<M,F>& a) const {
            std::ostringstream os;
            for (std::size_t i = 0; i < a.size(); ++i)
            {
                os << formations::names[i] << ":" << std::endl;
                os << subadapter.print(a[i]) << std::endl;
            }
            return os.str();
        }
        template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
        std::string print(const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,float>& a) const {
            return si::to_string(a);
        }

    };

}


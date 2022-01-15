#pragma once

// in-house libraries
#include <units/si.hpp>

namespace compound { 
namespace relation {

    struct Step
    {
        float xmin;
        float xmax;
        float xslope;
    };

    struct Bell
    {
        float mean;
        float stdev;
    };

    struct BellDerivative
    {
        float xmin;
        float xmax;
        float xslope;
    };

    struct BellIntegral
    {
        float xmin;
        float xmax;
        float xslope;
    };

    template<typename OutputType>
    class CompositeSpectralRelation
    {
        float intercept
        std::vector<Step> steps;
        // std::vector<Bell> bells;
        // std::vector<BellIntegral> bell_integrals;
        // std::vector<BellDerivatives> bell_derivatives;

        std::function<OutputType(const si::pressure p, const si::temperature T)> generic_function;
    public:

        CompositeSpectralRelation(
            const std::function<OutputType(const si::pressure p, const si::temperature T)> f)
        : 
            f(f)
        {
        }

        OutputType invoke(const si::pressure p, const si::temperature T)
        {
            return f(p, T);
        }

        OutputType operator()(const si::pressure p, const si::temperature T)
        {
            return invoke(p, T);
        }

        /*
        `restriction() returns a relation that represents the restriction of the function represented by `this` to a region of parameter space.
        It is equivalent to the "restriction" of a function as understood within set theory.
        */
        CompositeSpectralRelation<OutputType> restriction(const si::pressure pmin, const si::pressure pmax, const si::temperature Tmin, const si::temperature Tmax)
        {
            return CompositeSpectralRelation<OutputType>(f);
        }

        /*
        constexpr CompositeSpectralRelation<OutputType> pipe(const CompositeSpectralRelation relation)
        {
            return CompositeSpectralRelation<OutputType>(
                [f, relation](const si::pressure p, const si::temperature T){
                    return relation(f(p,T));
                });
        }
        */
    };
}}
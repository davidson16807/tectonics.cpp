#pragma once

#include <string>

#include <index/glm/whole_specialization.hpp>
#include <index/adapted/glm/GlmStrings.hpp>
#include <index/adapted/glm/GlmMetric.hpp>
#include <index/adapted/boolean/BooleanStrings.hpp>
#include <index/adapted/boolean/BooleanMetric.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/scalar/ScalarMetric.hpp>
#include <index/adapted/metric/MetricOrder.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Metric.hpp>

#include <grid/dymaxion/Grid.hpp>

#include <raster/spheroidal/Strings.hpp>

namespace dymaxion {

    struct PolyStrings : public adapted::GlmStrings, public adapted::ScalarStrings<double>, public adapted::ScalarStrings<float>, public adapted::BooleanStrings {
        using adapted::ScalarStrings<double>::legend;
        using adapted::ScalarStrings<float>::legend;
        using adapted::BooleanStrings::legend;
        using adapted::GlmStrings::legend;

        using adapted::ScalarStrings<double>::character;
        using adapted::ScalarStrings<float>::character;
        using adapted::BooleanStrings::character;
        using adapted::GlmStrings::character;
    };
    struct PolyMetric : public adapted::GlmMetric, public adapted::ScalarMetric<double>, public adapted::ScalarMetric<float>, public adapted::BooleanMetric {
        using adapted::ScalarMetric<double>::length;
        using adapted::ScalarMetric<float>::length;
        using adapted::BooleanMetric::length;
        using adapted::GlmMetric::length;
        using adapted::ScalarMetric<double>::distance;
        using adapted::ScalarMetric<float>::distance;
        using adapted::BooleanMetric::distance;
        using adapted::GlmMetric::distance;
    };

    template<typename id, typename scalar>
    struct Adapter{
        Grid<id,id,scalar> grid;
        scalar threshold;
        std::size_t test_size;

        Adapter(const Grid<id,id,scalar>& grid, const scalar threshold, const std::size_t test_size):
            grid(grid),
            threshold(threshold),
            test_size(test_size)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            aggregated::Metric metric{PolyMetric{}};
            return metric.distance(a,b) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            spheroidal::Strings strings{
                PolyStrings{}, 
                aggregated::Order{
                    adapted::MetricOrder{
                        PolyMetric{}
                    }
                }
            };
            return strings.format(grid, a);
        }

        std::string print(const id a) const {
            return std::to_string(a);
        }

    };

}


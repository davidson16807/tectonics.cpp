
#include <index/glm/each_specialization.hpp>
#include <index/glm/whole_specialization.hpp>

#include <grid/dymaxion/Grid.hpp>

#include <raster/spheroidal/string_cast.hpp>

namespace dymaxion {

    template<typename id, typename scalar>
    struct Adapter{
        Grid<id,scalar> grid;
        scalar threshold;
        std::size_t test_size;

        Adapter(const Grid<id, scalar>& grid, const scalar threshold, const std::size_t test_size):
            grid(grid),
            threshold(threshold),
            test_size(test_size)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            return whole::max_distance(a,b) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            // return dymaxion::to_string(grid, a, 200);
            return spheroidal::to_string(grid, a);
        }

    };

}
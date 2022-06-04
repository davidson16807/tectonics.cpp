#pragma once

#include <glm/vec2.hpp>   // *vec2

namespace rasters 
{
    /*
    */
    template<typename Tid=std::uint16_t>
    struct HalfEdge2dGrid
    {

        ~HalfEdge2dGrid()
        {

        }
        HalfEdge2dGrid()
        {

        }

        const glm::vec<2,Tid,glm::defaultp> vertex2d first(const glm::vec<2,Tid,glm::defaultp> from) const
        {
            return from + glm::vec<2,Tid,glm::defaultp>(1,0);
        }

        const glm::vec<2,Tid,glm::defaultp> vertex2d counterclockwise(const glm::vec<2,Tid,glm::defaultp> from, const glm::vec<2,Tid,glm::defaultp> to) const
        {
            glm::vec<2,Tid,glm::defaultp> offset = to-from;
            glm::vec<2,Tid,glm::defaultp> offset2 = offset*offset;
            return to + (
                 offset2.x == offset2.y ? glm::vec<2,Tid,glm::defaultp>(0, -offset.y)
              :  offset2.x  > offset2.y ? glm::vec<2,Tid,glm::defaultp>(-offset.x, offset.x)
              :/*offset2.x  < offset2.y*/ glm::vec<2,Tid,glm::defaultp>(-offset.y, 0)
            );
        }
        const glm::vec<2,Tid,glm::defaultp> vertex2d clockwise(const glm::vec<2,Tid,glm::defaultp> from, const glm::vec<2,Tid,glm::defaultp> to) const
        {
            return counterclockwise(to, from);
        }


    };
}
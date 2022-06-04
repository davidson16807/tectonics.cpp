#pragma once

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <rasters/components/Bounded2dGridHash/Bounded2dGridHash.hpp>
#include <rasters/components/HalfEdge2dGrid/HalfEdge2dGrid.hpp>
#include <rasters/components/CollignonMesh/CollignonMesh.hpp>
#include <rasters/components/SpheroidMeshMetrics/SpheroidMeshMetrics.hpp>

namespace rasters 
{
    /*
    A `Grid` is a collection of interconnected grid cells on the surface of a mesh that are intended to map to values in a `Raster`. 
    A `CollignonGrid` serves as a bridge between this role and an entity within an Entity/Component System.
    In this Entity/Component System, the "components" are mathematical categories that are implemented as classes in which methods are arrows.
    These methods map between data types that can be used amongst several components, 
    which allows the functionality between components to be glued together.
    This forms an "entity", a category in which components are subcategories. 
    Functions that operate on rasters may then traverse the entity category to implement their own functionality. 

    A `CollignonGrid` is distinct in that it uses a `CollignonMesh` to represent vertices within the `Grid`.
    The `CollignonGrid` comes equipped with several other components to allow structures within the mesh to be counted, traversed, and measured.
    */
    template<typename Tid=std::uint16_t, typename Tfloat=float>
    class CollignonGrid
    {
    public:
        const CollignonMesh<Tfloat> cells;

        const HalfEdge2dGrid<Tid> half_edges;
        const SpheroidMeshMetrics<Tid> metrics;

        const Tid vertex_count_per_octant_meridian;
        const Tid vertex_count;
        const Tid arrow_count;
        const Tfloat radius;

        using size_type = Tid;
        using value_type = Tfloat;

        ~CollignonGrid()
        {

        }
        CollignonGrid(const Tid vertex_count_per_octant_meridian, const Tfloat radius) : 
            cells(radius, vertex_count_per_octant_meridian),

            cell_hash(vertex_count_per_octant_meridian),

            half_edges(),
            metrics(),

            vertex_count_per_octant_meridian(vertex_count_per_octant_meridian),
            vertex_count(4*vertex_count_per_octant_meridian*vertex_count_per_octant_meridian),
            arrow_count(4 * (6 * vertex_count_per_octant_meridian*vertex_count_per_octant_meridian - 2)), // minus 2 since corner vertices have two arrows to the same cell
            radius(radius)
        {
            // const Tid face_count_of_topleft_cell = 2;
            // const Tid face_count_per_octant_meridian = 2 * vertex_count_per_octant_meridian;
            // const Tid face_count_of_interior = 2 * vertex_count_of_interior;
            // face_count = 
            //     face_count_of_topleft_cell 
            //     + 4 * face_count_per_octant_meridian 
            //     + face_count_of_interior;

            // const Tid edge_count_of_topleft_cell = 3;
            // const Tid edge_count_per_topleft_octant_meridians = 3 * vertex_count_per_octant_meridian;
            // const Tid edge_count_per_bottomright_octant_meridians = 4 * vertex_count_per_octant_meridian;
            // const Tid edge_count_of_interior = 3 * vertex_count_of_interior;
            // edge_count = 
            //     edge_count_of_topleft_cell 
            //     + 2 * edge_count_per_topleft_octant_meridians 
            //     + 2 * edge_count_per_bottomright_octant_meridians
            //     + edge_count_of_interior;
        }

        const std::size_t cell_count(mapping mapping_type) const
        {
            return mapping_type == mapping::cell? vertex_count : arrow_count;
        }
        
    };
}
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
    A `LayeredCollignonGrid` serves as a bridge between this role and an entity within an Entity/Component System.
    In this Entity/Component System, the "components" are mathematical categories that are implemented as classes in which methods are arrows.
    These components might be as simple as an isomorphism, group, or congruence relation.
    The methods within components map between data types that can be used amongst several components, 
    which allows the functionality between components to be glued together.
    This forms an "entity", a category in which components are subcategories. 
    Functions that operate on rasters may then traverse the entity category to implement their own functionality. 

    A `LayeredCollignonGrid` is distinct in that it uses instances of `SpheroidLayering` and `CollignonMesh` to represent cells within the `Grid`.
    The `LayeredCollignonGrid` comes equipped with several other components to allow structures within the mesh (vertices, edges, faces, etc.) 
    to be counted, traversed, and measured.
    */
    template<typename Tid=std::uint16_t, typename Tfloat=float>
    class LayeredCollignonGrid
    {
    public:
        const CollignonMesh<Tfloat> columns;
        const SpheroidLayering<Tfloat> layering;

        const Interleaving<Tid> layer_interleaving;

        const HalfEdge2dGrid<Tid> half_edges;
        const SpheroidMeshMetrics<Tid> metrics;

        const Tid vertex_count_per_octant_meridian;
        const Tid vertex_count;
        const Tid arrow_count;
        const Tid layer_count;

        const Tfloat radius;
        const Tfloat column_height;

        ~LayeredCollignonGrid()
        {

        }
        LayeredCollignonGrid(const Tid vertex_count_per_octant_meridian, const Tid layer_count, const Tfloat radius, const column_height) : 
            columns(radius, vertex_count_per_octant_meridian),
            layering(radius, column_height, layer_count),

            column_hash(vertex_count_per_octant_meridian),
            layer_interleaving(layer_count),

            half_edges(),
            metrics(),

            vertex_count_per_octant_meridian(vertex_count_per_octant_meridian),
            vertex_count(4*vertex_count_per_octant_meridian*vertex_count_per_octant_meridian),
            arrow_count(4 * (6 * vertex_count_per_octant_meridian*vertex_count_per_octant_meridian - 2)), // minus 2 since corner vertices have two arrows to the same cell
            layer_count(layer_count),

            radius(radius),
            column_height(column_height)
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
            return layer_count * cell_count(mapping);
        }
        const std::size_t column_count(mapping mapping_type) const
        {
            return mapping_type == mapping::cell? vertex_count : arrow_count;
        }
        
    };
}
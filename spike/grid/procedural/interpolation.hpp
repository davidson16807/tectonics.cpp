
#include "Grid.hpp"

namespace procedural
{

    /*
    properties used:
        arrow_dual_lengths
        arrow_dual_normals
        arrow_lengths
        vertex_dual_areas
        arrow_vertex_ids
		arrow_target_vertex_id
        arrow_source_vertex_id
    */

    /*
    `Interpolation` is a wrapper around a `Grid` that introduces concepts known to mathematical morphology.
    */
    template<typename id, typename scalar>
	class Interpolation(){
        using ivec2 = glm::vec<2,id,glm::defaultp>;\
        using vec2 = glm::vec<2,scalar,glm::defaultp>;\
        using vec3 = glm::vec<3,scalar,glm::defaultp>;\

	public:
	    const Raster<unsigned int, Grid, Tmap>& scratch;
		const Grid<id,scalar> grid;

        inline constexpr explicit Interpolation(const Grid& grid) : 
        	grid(grid)
    	{}

        inline constexpr Interpolation(const Interpolation& calculus) : grid(calculus.grid) {} // copy constructor

	    template<typename T, typename Grid2, typename Grid, rasters::mapping Tmap>
	    void nearest_neighbor(
	        const LayeredRaster<T, Grid2, Tmap>& input, 
	        LayeredRaster<T, Grid, Tmap>& out
	    ) {
	        // TODO: relax this assertion to work for arbitrary combinations of layer schemes
	        assert(*input.grid.layering == *out.grid.layering);
	        input.grid.voronoi->get_values(out.grid.metrics->vertex_positions, scratch);
	        int V = out.grid.structure->vertex_count;
	        int L = out.grid.layering->layer_count;
	        for (int i = 0; i < V; ++i)
	        {
	            for (int j = 0; j < L; ++j)
	            {
	                out[i*L+j] = input[scratch[i]*L+j];
	            }
	        }
	    }

	}

}


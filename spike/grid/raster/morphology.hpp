
#include "Grid.hpp"

namespace collignon 
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
    `VectorCalculus` is a wrapper around a `Grid` that introduces concepts known to mathematical morphology.
    */
    template<typename id, typename scalar>
	class Morphology{
        using ivec2 = glm::vec<2,id,glm::defaultp>;\
        using vec2 = glm::vec<2,scalar,glm::defaultp>;\
        using vec3 = glm::vec<3,scalar,glm::defaultp>;\

	public:
        const Raster<bool, Grid, mapping::arrow>& scratch1;
        const Raster<bool, Grid, mapping::arrow>& scratch2;
		const Grid<id,scalar> grid;

        inline constexpr explicit Morphology(const Grid& grid) : 
        	grid(grid)
    	{}

        inline constexpr Morphology(const Morphology& calculus) : grid(calculus.grid) {} // copy constructor


		void dilate(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			fill( out, false );
			for (unsigned int i = 0; i < grid.arrow_count; ++i)
			{
				out[grid.arrow_source_memory_id(i)] = 
					out[grid.arrow_source_memory_id(i)] || 
					a  [grid.arrow_source_memory_id(i)] || 
					a  [grid.arrow_target_memory_id(i)];
			}
		}

		void dilate(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			Raster<bool,Grid>* temp_in  = &out;
			Raster<bool,Grid>* temp_out = &scratch1;
			Raster<bool,Grid>* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &a)
			{
				each::copy(out, a);
			} 
			else 
			{
				each::copy(*temp_in, a);
				for (uint i = 0; i < radius; ++i)
				{
					temp_swap = temp_out;
					temp_out  = temp_in;
					temp_in   = temp_swap;
					dilate(*temp_in, *temp_out);
				}
				if (radius % 2 == 0)
				{
					each::copy(out, *temp_out);
				}
			}
		}


		void erode(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			fill( out, false );
			for (unsigned int i = 0; i < grid.arrow_count; ++i)
			{
				out[grid.arrow_source_memory_id(i)] = 
					out[grid.arrow_source_memory_id(i)] && 
					a  [grid.arrow_source_memory_id(i)] && 
					a  [grid.arrow_target_memory_id(i)];
			}
		}

		void erode(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			Raster<bool,Grid>* temp_in  = &out;
			Raster<bool,Grid>* temp_out = &scratch1;
			Raster<bool,Grid>* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &a)
			{
				each::copy(out, a);
			} 
			else 
			{
				each::copy(*temp_in, a);
				for (uint i = 0; i < radius; ++i)
				{
					temp_swap = temp_out;
					temp_out  = temp_in;
					temp_in   = temp_swap;
					erode(*temp_in, *temp_out);
				}
				if (radius % 2 == 0)
				{
					each::copy(out, *temp_out);
				}
			}
		}


		void opening(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			erode ( a,       scratch1 );
			dilate( scratch1, out      );
		}

		void opening(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius )
		{
			erode ( a,        scratch1, radius, scratch2 );
			dilate( scratch1, out,      radius, scratch2 );
		}

		void closing(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			dilate( a,       scratch1  );
			erode ( scratch1, out      );
		}

		void closing(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius )
		{
			dilate( a,        scratch1, radius, scratch2 );
			erode ( scratch1, out,      radius, scratch2 );
		}

		void white_top_hat(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			closing( a, out );
			differ ( out,  a, out );
		}

		void white_top_hat(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			closing( a, out, radius, scratch1, scratch2 );
			differ ( out,  a, out                             );
		}

		void black_top_hat(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			opening( a, out );
			differ ( a,  out, out );
		}

		void black_top_hat(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			opening( a, out, radius, scratch1, scratch2 );
			differ ( a,  out, out );
		}


		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its dilation
		// Its name eludes to the "margin" concept within the html box model
		void margin(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			dilate( a, out );
			differ( out,  a, out );
		}

		void margin(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			dilate( a, out, radius, scratch1 );
			differ( out,  a, out );
		}

		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its erosion
		// Its name eludes to the "padding" concept within the html box model
		void padding(const Raster<bool,Grid>& a, Raster<bool,Grid>& out)
		{
			erode  ( a, out );
			differ ( a,  out, out );
		}
		void padding(const Raster<bool,Grid>& a, Raster<bool,Grid>& out, uint radius)
		{
			erode  ( a, out, radius, scratch1 );
			differ ( a,  out, out );
		}

	};

}


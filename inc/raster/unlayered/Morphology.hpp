#pragma once

namespace unlayered
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
    `Morphology` introduces concepts known to mathematical morphology.
    */
	class Morphology{

	public:

        inline constexpr explicit Morphology()
    	{}

        inline constexpr Morphology(const Morphology& morphology)
        {} // copy constructor

    	template<typename Grid, typename In, typename Out>
		void dilate(const Grid& grid, const In& mask, Out& out) const
		{
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                source_value = mask[i];
                for (j = 0; j < N; ++j)
                {
					out[i] = out[i] || source_value || mask[grid.arrow_target_memory_id(i,j)];
                }
            }
		}

    	template<typename Grid, typename In, typename Out>
		void dilate(const Grid& grid, const In& mask, Out& out, const unsigned int radius, const Out& scratch) const
		{
			In* temp_in  = &out;
			In* temp_out = &scratch;
			In* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &mask)
			{
				each::copy(out, mask);
			} 
			else 
			{
				each::copy(*temp_in, mask);
				for (unsigned int i = 0; i < radius; ++i)
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

    	template<typename Grid, typename In, typename Out>
		void erode(const Grid& grid, const In& mask, Out& out) const
		{
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(1);
                source_value = mask[i];
                for (j = 0; j < N; ++j)
                {
					out[i] = out[i] && source_value && mask[grid.arrow_target_memory_id(i,j)];
                }
            }
		}

    	template<typename Grid, typename In, typename Out>
		void erode(const Grid& grid, const In& mask, Out& out, const unsigned int radius, const Out& scratch) const
		{
			In* temp_in  = &out;
			In* temp_out = &scratch;
			In* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &mask)
			{
				each::copy(out, mask);
			} 
			else 
			{
				each::copy(*temp_in, mask);
				for (unsigned int i = 0; i < radius; ++i)
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



    	template<typename Grid, typename In, typename Out>
		void opening(const Grid& grid, const In& mask, Out& out, Out& scratch1) const
		{
			erode ( grid, mask, scratch1 );
			dilate( grid, scratch1, out  );
		}

    	template<typename Grid, typename In, typename Out>
		void opening(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			erode ( grid, mask, scratch1, radius, scratch2 );
			dilate( grid, scratch1, out,  radius, scratch2 );
		}

    	template<typename Grid, typename In, typename Out>
		void closing(const Grid& grid, const In& mask, Out& out, Out& scratch1) const
		{
			dilate( grid, mask, scratch1  );
			erode ( grid, scratch1, out   );
		}

    	template<typename Grid, typename In, typename Out>
		void closing(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			dilate( grid, mask, scratch1, radius, scratch2 );
			erode ( grid, scratch1, out,  radius, scratch2 );
		}

    	template<typename Grid, typename In, typename Out>
		void white_top_hat(const Grid& grid, const In& mask, Out& out, Out& scratch1) const
		{
			closing      ( grid, mask, out,  scratch1 );
			each::differ (       out,  mask, out );
		}

    	template<typename Grid, typename In, typename Out>
		void white_top_hat(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			closing      ( grid, mask, out, radius, scratch1, scratch2 );
			each::differ (       out,  mask, out                       );
		}

    	template<typename Grid, typename In, typename Out>
		void black_top_hat(const Grid& grid, const In& mask, Out& out, Out& scratch1) const
		{
			opening      ( grid, mask, out, scratch1 );
			each::differ (       mask, out, out );
		}

    	template<typename Grid, typename In, typename Out>
		void black_top_hat(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			opening      ( grid, mask, out, radius, scratch1, scratch2 );
			each::differ (       mask, out, out );
		}

		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its dilation
		// Its name eludes to the "margin" concept within the html box model

    	template<typename Grid, typename In, typename Out>
		void margin(const Grid& grid, const In& mask, Out& out) const
		{
			dilate      ( grid, mask, out );
			each::differ(       out,  mask, out );
		}

    	template<typename Grid, typename In, typename Out>
		void margin(const Grid& grid, const In& mask, Out& out, const unsigned int radius, const Out& scratch) const
		{
			dilate      ( grid, mask, out, radius, scratch );
			each::differ(       out,  mask, out );
		}

		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its erosion
		// Its name eludes to the "padding" concept within the html box model

    	template<typename Grid, typename In, typename Out>
		void padding(const Grid& grid, const In& mask, Out& out) const
		{
			erode        ( grid, mask, out );
			each::differ (       mask,  out, out );
		}

    	template<typename Grid, typename In, typename Out>
		void padding(const Grid& grid, const In& mask, Out& out, const unsigned int radius, const Out& scratch) const
		{
			erode        ( grid, mask, out, radius, scratch );
			each::differ (       mask,  out, out );
		}

	};

}


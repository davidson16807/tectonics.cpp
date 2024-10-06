#pragma once

namespace unlayered
{

    /*
    properties used:
		arrow_target_id
    */

    /*
    `Morphology` introduces concepts known to mathematical morphology.
    */
    template<typename Bitsets>
	class Morphology {

		const Bitsets bitsets;

	public:

        inline constexpr explicit Morphology(const Bitsets& bitsets):
        	bitsets(bitsets)
    	{}

        inline constexpr Morphology(const Morphology& morphology):
        	bitsets(morphology.bitsets)
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
					out[i] = out[i] || source_value || mask[grid.arrow_target_id(i,j)];
                }
            }
		}

    	template<typename Grid, typename In, typename Out>
		void dilate(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch) const
		{
			In* temp_in  = &out;
			In* temp_out = &scratch;
			In* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &mask)
			{
				bitsets.copy(mask, out);
			} 
			else 
			{
				bitsets.copy(mask, *temp_in);
				for (unsigned int i = 0; i < radius; ++i)
				{
					temp_swap = temp_out;
					temp_out  = temp_in;
					temp_in   = temp_swap;
					dilate(grid, *temp_in, *temp_out);
				}
				if (radius % 2 == 0)
				{
					bitsets.copy(*temp_out, out);
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
					out[i] = out[i] && source_value && mask[grid.arrow_target_id(i,j)];
                }
            }
		}

    	template<typename Grid, typename In, typename Out>
		void erode(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch) const
		{
			In* temp_in  = &out;
			In* temp_out = &scratch;
			In* temp_swap= &out;
			assert(radius >= 0);
			if (radius == 0 && &out != &mask)
			{
				bitsets.copy(mask, out);
			} 
			else 
			{
				bitsets.copy(mask, *temp_in);
				for (unsigned int i = 0; i < radius; ++i)
				{
					temp_swap = temp_out;
					temp_out  = temp_in;
					temp_in   = temp_swap;
					erode(grid, *temp_in, *temp_out);
				}
				if (radius % 2 == 0)
				{
					bitsets.copy(*temp_out, out);
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
			bitsets.differ (       out,  mask, out );
		}

    	template<typename Grid, typename In, typename Out>
		void white_top_hat(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			closing      ( grid, mask, out, radius, scratch1, scratch2 );
			bitsets.differ (       out,  mask, out                       );
		}

    	template<typename Grid, typename In, typename Out>
		void black_top_hat(const Grid& grid, const In& mask, Out& out, Out& scratch1) const
		{
			opening      ( grid, mask, out, scratch1 );
			bitsets.differ (       mask, out, out );
		}

    	template<typename Grid, typename In, typename Out>
		void black_top_hat(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch1, Out& scratch2) const
		{
			opening      ( grid, mask, out, radius, scratch1, scratch2 );
			bitsets.differ (       mask, out, out );
		}

		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its dilation

    	template<typename Grid, typename In, typename Out>
		void outshell(const Grid& grid, const In& mask, Out& out) const
		{
			dilate      ( grid, mask, out );
			bitsets.differ(       out,  mask, out );
		}

    	template<typename Grid, typename In, typename Out>
		void outshell(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch) const
		{
			dilate      ( grid, mask, out, radius, scratch );
			bitsets.differ(       out,  mask, out );
		}

		// NOTE: this is not a standard concept in math morphology
		// It is meant to represent the difference between a figure and its erosion

    	template<typename Grid, typename In, typename Out>
		void inshell(const Grid& grid, const In& mask, Out& out) const
		{
			erode        ( grid, mask, out );
			bitsets.differ (       mask,  out, out );
		}

    	template<typename Grid, typename In, typename Out>
		void inshell(const Grid& grid, const In& mask, Out& out, const unsigned int radius, Out& scratch) const
		{
			erode        ( grid, mask, out, radius, scratch );
			bitsets.differ (       mask,  out, out );
		}

	};

}


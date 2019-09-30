#pragma once

#include <many/types.hpp>     	 // floats, etc.

#include "Grid.hpp"

namespace rasters
{

	void dilate(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		fill(out, false);
		aggregate_into(a, grid.arrow_vertex_id_from, [](bool ai, bool bi){ return ai || bi; }, out);
	}
	void dilate(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		tmany<bool>* temp_in  = &out;
		tmany<bool>* temp_out = &scratch;
		tmany<bool>* temp_swap= &out;
		copy(*temp_in, a);
		for (uint i = 0; i < radius; ++i)
		{
			temp_swap = temp_out;
			temp_out  = temp_in;
			temp_in   = temp_swap;
			dilate(grid, *temp_in, *temp_out);
		}
		if (radius % 2 == 0)
		{
			copy(out, *temp_out);
		}
	}
	void dilate(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		dilate(grid, a, out, radius, scratch);
	}

	void erode(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		fill(out, true);
		aggregate_into(a, grid.arrow_vertex_id_from, [](bool ai, bool bi){ return ai && bi; }, out);
	}
	void erode(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		tmany<bool>* temp_in  = &out;
		tmany<bool>* temp_out = &scratch;
		tmany<bool>* temp_swap= &out;
		copy(*temp_in, a);
		for (uint i = 0; i < radius; ++i)
		{
			temp_swap = temp_out;
			temp_out  = temp_in;
			temp_in   = temp_swap;
			erode(grid, *temp_in, *temp_out);
		}
		if (radius % 2 == 0)
		{
			copy(out, *temp_out);
		}
	}
	void erode(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		erode(grid, a, out, radius, scratch);
	}

	void opening(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		erode ( grid, a, out );
		dilate( grid, a, out );
	}
	void opening(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		erode ( grid, a, scratch, radius, out     );
		dilate( grid, a, out,     radius, scratch );
	}
	void opening(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		opening(grid, a, out, radius, scratch);
	}

	void closing(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		dilate( grid, a, out );
		erode ( grid, a, out );
	}
	void closing(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		dilate( grid, a, scratch, radius, out     );
		erode ( grid, a, out,     radius, scratch );
	}
	void closing(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		closing(grid, a, out, radius, scratch);
	}

	void white_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		closing( grid, a, out );
		differ ( out,  a, out );
	}
	void white_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		closing( grid, a, out, radius, scratch );
		differ ( out,  a, out                  );
	}
	void white_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		white_top_hat(grid, a, out, radius, scratch);
	}

	void black_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		opening( grid, a, out );
		differ ( a,  out, out );
	}
	void black_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		opening( grid, a, out, radius, scratch );
		differ ( a,  out, out                  );
	}
	void black_top_hat(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		black_top_hat(grid, a, out, radius, scratch);
	}


	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its erosion
	// Its name eludes to the "margin" concept within the html box model
	void margin(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		dilate( grid, a, out );
		differ( out,  a, out );
	}
	void margin(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		dilate( grid, a, out, radius, scratch );
		differ( out,  a, out                  );
	}
	void margin(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		margin(grid, a, out, radius, scratch);
	}
	
	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its dilation
	// Its name eludes to the "padding" concept within the html box model
	void padding(const Grid& grid, const tmany<bool>& a, tmany<bool>& out)
	{
		erode  ( grid, a, out );
		differ ( a,  out, out );
	}
	void padding(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius, tmany<bool>& scratch)
	{
		erode  ( grid, a, out, radius, scratch );
		differ ( a,  out, out                  );
	}
	void padding(const Grid& grid, const tmany<bool>& a, tmany<bool>& out, uint radius)
	{
		tmany<bool> scratch(a.size());
		padding(grid, a, out, radius, scratch);
	}

}
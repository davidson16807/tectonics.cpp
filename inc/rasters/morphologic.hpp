#pragma once

#include <many/types.hpp>     	 // floats, etc.

#include "Grid.hpp"

namespace rasters
{

	void dilate(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		fill( out, false );
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			out[grid.arrow_vertex_id_from[i]] = 
				out[grid.arrow_vertex_id_from[i]] || 
				a  [grid.arrow_vertex_id_from[i]] || 
				a  [grid.arrow_vertex_id_to[i]];
		}
	}
	void dilate(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch)
	{
		std::vector<bool>* temp_in  = &out;
		std::vector<bool>* temp_out = &scratch;
		std::vector<bool>* temp_swap= &out;
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
	void dilate(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch(a.size());
		dilate(grid, a, out, radius, scratch);
	}

	void erode(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		fill(out, true);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			out[grid.arrow_vertex_id_from[i]] = 
				out[grid.arrow_vertex_id_from[i]] && 
				a  [grid.arrow_vertex_id_from[i]] && 
				a  [grid.arrow_vertex_id_to[i]];
		}
	}
	void erode(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch)
	{
		std::vector<bool>* temp_in  = &out;
		std::vector<bool>* temp_out = &scratch;
		std::vector<bool>* temp_swap= &out;
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
	void erode(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch(a.size());
		erode(grid, a, out, radius, scratch);
	}

	void opening(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, std::vector<bool>& scratch)
	{
		erode ( grid, a,       scratch );
		dilate( grid, scratch, out      );
	}
	void opening(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		std::vector<bool> scratch(a.size());
		opening(grid, a, out, scratch);
	}
	void opening(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch1, std::vector<bool>& scratch2 )
	{
		erode ( grid, a,        scratch1, radius, scratch2 );
		dilate( grid, scratch1, out,      radius, scratch2 );
	}
	void opening(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch1(a.size());
		std::vector<bool> scratch2(a.size());
		opening(grid, a, out, radius, scratch1, scratch2);
	}

	void closing(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, std::vector<bool>& scratch)
	{
		dilate( grid, a,       scratch  );
		erode ( grid, scratch, out      );
	}
	void closing(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		std::vector<bool> scratch(a.size());
		closing(grid, a, out, scratch);
	}
	void closing(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch1, std::vector<bool>& scratch2 )
	{
		dilate( grid, a,        scratch1, radius, scratch2 );
		erode ( grid, scratch1, out,      radius, scratch2 );
	}
	void closing(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch1(a.size());
		std::vector<bool> scratch2(a.size());
		closing(grid, a, out, radius, scratch1, scratch2);
	}

	void white_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		closing( grid, a, out );
		differ ( out,  a, out );
	}
	void white_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch1, std::vector<bool>& scratch2)
	{
		closing( grid, a, out, radius, scratch1, scratch2 );
		differ ( out,  a, out                             );
	}
	void white_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch1(a.size());
		std::vector<bool> scratch2(a.size());
		white_top_hat(grid, a, out, radius, scratch1, scratch2);
	}

	void black_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		opening( grid, a, out );
		differ ( a,  out, out );
	}
	void black_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch1, std::vector<bool>& scratch2)
	{
		opening( grid, a, out, radius, scratch1, scratch2 );
		differ ( a,  out, out                             );
	}
	void black_top_hat(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch1(a.size());
		std::vector<bool> scratch2(a.size());
		black_top_hat(grid, a, out, radius, scratch1, scratch2);
	}


	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its dilation
	// Its name eludes to the "margin" concept within the html box model
	void margin(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		dilate( grid, a, out );
		differ( out,  a, out );
	}
	void margin(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch)
	{
		dilate( grid, a, out, radius, scratch );
		differ( out,  a, out                  );
	}
	void margin(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch(a.size());
		margin(grid, a, out, radius, scratch);
	}
	
	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its erosion
	// Its name eludes to the "padding" concept within the html box model
	void padding(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out)
	{
		erode  ( grid, a, out );
		differ ( a,  out, out );
	}
	void padding(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius, std::vector<bool>& scratch)
	{
		erode  ( grid, a, out, radius, scratch );
		differ ( a,  out, out                  );
	}
	void padding(const Grid& grid, const std::vector<bool>& a, std::vector<bool>& out, uint radius)
	{
		std::vector<bool> scratch(a.size());
		padding(grid, a, out, radius, scratch);
	}

}
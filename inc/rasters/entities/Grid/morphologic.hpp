#pragma once

#include "../raster.hpp"

namespace rasters
{

	template <typename Tgrid>
	void dilate(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		fill( out, false );
		for (unsigned int i = 0; i < a.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			out[a.grid.cache->arrow_vertex_id_from[i]] = 
				out[a.grid.cache->arrow_vertex_id_from[i]] || 
				a  [a.grid.cache->arrow_vertex_id_from[i]] || 
				a  [a.grid.cache->arrow_vertex_id_to[i]];
		}
	}
	template <typename Tgrid>
	void dilate(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch)
	{
		raster<bool,Tgrid>* temp_in  = &out;
		raster<bool,Tgrid>* temp_out = &scratch;
		raster<bool,Tgrid>* temp_swap= &out;
		copy(*temp_in, a);
		for (uint i = 0; i < radius; ++i)
		{
			temp_swap = temp_out;
			temp_out  = temp_in;
			temp_in   = temp_swap;
			dilate(*temp_in, *temp_out);
		}
		if (radius % 2 == 0)
		{
			copy(out, *temp_out);
		}
	}
	template <typename Tgrid>
	void dilate(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch(a.grid);
		dilate(a, out, radius, scratch);
	}

	template <typename Tgrid>
	void erode(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		fill(out, true);
		for (unsigned int i = 0; i < a.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			out[a.grid.cache->arrow_vertex_id_from[i]] = 
				out[a.grid.cache->arrow_vertex_id_from[i]] && 
				a  [a.grid.cache->arrow_vertex_id_from[i]] && 
				a  [a.grid.cache->arrow_vertex_id_to[i]];
		}
	}
	template <typename Tgrid>
	void erode(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch)
	{
		raster<bool,Tgrid>* temp_in  = &out;
		raster<bool,Tgrid>* temp_out = &scratch;
		raster<bool,Tgrid>* temp_swap= &out;
		copy(*temp_in, a);
		for (uint i = 0; i < radius; ++i)
		{
			temp_swap = temp_out;
			temp_out  = temp_in;
			temp_in   = temp_swap;
			erode(*temp_in, *temp_out);
		}
		if (radius % 2 == 0)
		{
			copy(out, *temp_out);
		}
	}
	template <typename Tgrid>
	void erode(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch(a.grid);
		erode(a, out, radius, scratch);
	}

	template <typename Tgrid>
	void opening(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, raster<bool,Tgrid>& scratch)
	{
		erode ( a,       scratch );
		dilate( scratch, out      );
	}
	template <typename Tgrid>
	void opening(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		raster<bool,Tgrid> scratch(a.grid);
		opening(a, out, scratch);
	}
	template <typename Tgrid>
	void opening(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch1, raster<bool,Tgrid>& scratch2 )
	{
		erode ( a,        scratch1, radius, scratch2 );
		dilate( scratch1, out,      radius, scratch2 );
	}
	template <typename Tgrid>
	void opening(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch1(a.grid);
		raster<bool,Tgrid> scratch2(a.grid);
		opening(a, out, radius, scratch1, scratch2);
	}

	template <typename Tgrid>
	void closing(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, raster<bool,Tgrid>& scratch)
	{
		dilate( a,       scratch  );
		erode ( scratch, out      );
	}
	template <typename Tgrid>
	void closing(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		raster<bool,Tgrid> scratch(a.grid);
		closing(a, out, scratch);
	}
	template <typename Tgrid>
	void closing(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch1, raster<bool,Tgrid>& scratch2 )
	{
		dilate( a,        scratch1, radius, scratch2 );
		erode ( scratch1, out,      radius, scratch2 );
	}
	template <typename Tgrid>
	void closing(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch1(a.grid);
		raster<bool,Tgrid> scratch2(a.grid);
		closing(a, out, radius, scratch1, scratch2);
	}

	template <typename Tgrid>
	void white_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		closing( a, out );
		differ ( out,  a, out );
	}
	template <typename Tgrid>
	void white_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch1, raster<bool,Tgrid>& scratch2)
	{
		closing( a, out, radius, scratch1, scratch2 );
		differ ( out,  a, out                             );
	}
	template <typename Tgrid>
	void white_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch1(a.grid);
		raster<bool,Tgrid> scratch2(a.grid);
		white_top_hat(a, out, radius, scratch1, scratch2);
	}

	template <typename Tgrid>
	void black_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		opening( a, out );
		differ ( a,  out, out );
	}
	template <typename Tgrid>
	void black_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch1, raster<bool,Tgrid>& scratch2)
	{
		opening( a, out, radius, scratch1, scratch2 );
		differ ( a,  out, out                             );
	}
	template <typename Tgrid>
	void black_top_hat(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch1(a.grid);
		raster<bool,Tgrid> scratch2(a.grid);
		black_top_hat(a, out, radius, scratch1, scratch2);
	}


	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its dilation
	// Its name eludes to the "margin" concept within the html box model
	template <typename Tgrid>
	void margin(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		dilate( a, out );
		differ( out,  a, out );
	}
	template <typename Tgrid>
	void margin(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch)
	{
		dilate( a, out, radius, scratch );
		differ( out,  a, out                  );
	}
	template <typename Tgrid>
	void margin(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch(a.grid);
		margin(a, out, radius, scratch);
	}
	
	// NOTE: this is not a standard concept in math morphology
	// It is meant to represent the difference between a figure and its erosion
	// Its name eludes to the "padding" concept within the html box model
	template <typename Tgrid>
	void padding(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out)
	{
		erode  ( a, out );
		differ ( a,  out, out );
	}
	template <typename Tgrid>
	void padding(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius, raster<bool,Tgrid>& scratch)
	{
		erode  ( a, out, radius, scratch );
		differ ( a,  out, out                  );
	}
	template <typename Tgrid>
	void padding(const raster<bool,Tgrid>& a, raster<bool,Tgrid>& out, uint radius)
	{
		raster<bool,Tgrid> scratch(a.grid);
		padding(a, out, radius, scratch);
	}

}
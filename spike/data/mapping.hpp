#pragma once

namespace data
{

	/*
	`mapping` is used as a template parameter for rasters to indicate whether 
	the Raster maps each element of its container to a cell or an arrow within its grid.
	*/
	enum mapping
	{
		cell,
		arrow
	};

}


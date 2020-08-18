#pragma once

#include "types.hpp"

namespace many
{

	void unite(const series<bool>& a, const bool b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void unite(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void intersect(const series<bool>& a, const bool b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void intersect(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void differ(const series<bool>& a, const bool b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void differ(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		many::transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void negate(const series<bool>& a, series<bool>& out)
	{
		many::transform(a, [](bool ai){ return !ai; }, out);
	}





}
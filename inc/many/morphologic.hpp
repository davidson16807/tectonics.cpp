#pragma once

#include "types.hpp"

namespace many
{

	void unite(const tmany<bool>& a, const bool b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void unite(const tmany<bool>& a, const tmany<bool>& b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void intersect(const tmany<bool>& a, const bool b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void intersect(const tmany<bool>& a, const tmany<bool>& b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void differ(const tmany<bool>& a, const bool b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void differ(const tmany<bool>& a, const tmany<bool>& b, tmany<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void negate(const tmany<bool>& a, tmany<bool>& out)
	{
		transform(a, [](bool ai){ return !ai; }, out);
	}





}
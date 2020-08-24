#pragma once

#include "types.hpp"

namespace many
{

	void unite(const series<bool>& a, const bool b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai || bi; }, a, b);
	}

	void unite(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai || bi; }, a, b);
	}

	void intersect(const series<bool>& a, const bool b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai && bi; }, a, b);
	}

	void intersect(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai && bi; }, a, b);
	}

	void differ(const series<bool>& a, const bool b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai && !bi; }, a, b);
	}

	void differ(const series<bool>& a, const series<bool>& b, series<bool>& out)
	{
		out.store([](bool ai, bool bi){ return ai && !bi; }, a, b);
	}

	void negate(const series<bool>& a, series<bool>& out)
	{
		out.store([](bool ai){ return !ai; }, a);
	}





}
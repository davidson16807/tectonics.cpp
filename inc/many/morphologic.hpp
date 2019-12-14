#pragma once

#include "types.hpp"

namespace many
{

	void unite(const std::vector<bool>& a, const bool b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void unite(const std::vector<bool>& a, const std::vector<bool>& b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void intersect(const std::vector<bool>& a, const bool b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void intersect(const std::vector<bool>& a, const std::vector<bool>& b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void differ(const std::vector<bool>& a, const bool b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void differ(const std::vector<bool>& a, const std::vector<bool>& b, std::vector<bool>& out)
	{
		apply(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void negate(const std::vector<bool>& a, std::vector<bool>& out)
	{
		apply(a, [](bool ai){ return !ai; }, out);
	}





}
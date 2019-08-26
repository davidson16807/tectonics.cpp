#pragma once

#include "composite.hpp"

namespace many
{

	void unite(const composite<bool>& a, const bool b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void unite(const composite<bool>& a, const composite<bool>& b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	void intersect(const composite<bool>& a, const bool b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void intersect(const composite<bool>& a, const composite<bool>& b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	void differ(const composite<bool>& a, const bool b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void differ(const composite<bool>& a, const composite<bool>& b, composite<bool>& out)
	{
		transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	void negate(const composite<bool>& a, composite<bool>& out)
	{
		transform(a, [](bool ai){ return !ai; }, out);
	}

	bool all(const composite<bool>& a)
	{
		bool out = true;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i];
		}
		return out;
	}
	bool any(const composite<bool>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return out;
	}
	bool none(const composite<bool>& a)
	{
		bool out = false;
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i];
		}
		return !out;
	}





}
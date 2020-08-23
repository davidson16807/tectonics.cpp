#pragma once

#include "Layers.hpp"

namespace layers
{

	template <std::size_t N>
	void unite(const Layers<bool,N>& a, const bool b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	template <std::size_t N>
	void unite(const Layers<bool,N>& a, const Layers<bool,N>& b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai || bi; }, out);
	}

	template <std::size_t N>
	void intersect(const Layers<bool,N>& a, const bool b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	template <std::size_t N>
	void intersect(const Layers<bool,N>& a, const Layers<bool,N>& b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai && bi; }, out);
	}

	template <std::size_t N>
	void differ(const Layers<bool,N>& a, const bool b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	template <std::size_t N>
	void differ(const Layers<bool,N>& a, const Layers<bool,N>& b, Layers<bool,N>& out)
	{
		layers::transform(a, b, [](bool ai, bool bi){ return ai && !bi; }, out);
	}

	template <std::size_t N>
	void negate(const Layers<bool,N>& a, Layers<bool,N>& out)
	{
		layers::transform(a, [](bool ai){ return !ai; }, out);
	}

}
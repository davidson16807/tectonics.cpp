#pragma once

#include "types.hpp"

namespace series
{


	template <typename T, typename Tout>
	void unite(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai || bi; }, a, b);
	}
	template <typename T, typename Tout>
	void unite(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai || bi; }, a, b);
	}
	template <typename T, typename Tout>
	void unite(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai || bi; }, a, b);
	}
	


	template <typename T, typename Tout>
	void intersect(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && bi; }, a, b);
	}
	template <typename T, typename Tout>
	void intersect(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && bi; }, a, b);
	}
	template <typename T, typename Tout>
	void intersect(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && bi; }, a, b);
	}
	


	template <typename T, typename Tout>
	void differ(const T& a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && !bi; }, a, b);
	}
	template <typename T, typename Tout>
	void differ(const T& a, const typename T::value_type b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && !bi; }, a, b);
	}
	template <typename T, typename Tout>
	void differ(const typename T::value_type a, const T& b, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai, Ti bi){ return ai && !bi; }, a, b);
	}
	

	template <typename T, typename Tout>
	void negate(const T& a, Tout& out)
	{
		typedef typename T::value_type Ti;
		out.store([](Ti ai){ return !ai; }, a);
	}





}
#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace iterated
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	struct Identity // A.K.A. `Copy`
	{
		Identity(){}
		template <typename In1, typename Out>
		void operator() (const In1& a, Out& out) const
		{
			assert(compatible(a,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = a[i];
			}
		}
	};

	struct Ternary // A.K.A. `CopyIf`
	{
		Ternary(){}
		template <typename If, typename Else, typename Condition, typename Out>
		void operator() (const Condition& condition, const If& if_, const Else& else_, Out& out) const
		{
			assert(compatible(if_,out));
			assert(compatible(else_,out));
			assert(compatible(condition,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = condition[i]? if_[i] : else_[i];
			}
		}
	};

	struct Index
	{
		Index(){}
		template <typename F, typename In1, typename Out>
		void operator() (const F& f, const In1& a, Out& out) const
		{
			assert(compatible(a,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = f[a[i]];
			}
		}
	};

	template <typename Op>
	class Unary
	{
		const Op op;
	public:
		Unary(const Op& op):
			op(op)
		{}
		template <typename In1, typename Out>
		void operator() (const In1& a, Out& out) const
		{
			assert(compatible(a,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = op(a[i]);
			}
		}
	};

	template <typename Op>
	class Binary
	{
		const Op op;
	public:
		Binary(const Op& op):
			op(op)
		{}
		template <typename In1, typename In2, typename Out>
		void operator() (const In1& a, const In2& b, Out& out) const
		{
			assert(compatible(a,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = op(a[i], b[i]);
			}
		}
	};

	template <typename Op>
	class Trinary
	{
		const Op op;
	public:
		Trinary(const Op& op):
			op(op)
		{}
		template <typename In1, typename In2, typename In3, typename Out>
		void operator() (const In1& a, const In2& b, const In3& c, Out& out) const
		{
			assert(compatible(a,out));
			auto size = out.size();
			for (auto i = 0*size; i < size; ++i)
			{
				out[i] = op(a[i], b[i], c[i]);
			}
		}
	};

}


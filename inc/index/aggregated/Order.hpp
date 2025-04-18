#pragma once

// C libraries
#include <assert.h>  /* assert */
#include <stdexcept> /* out_of_range */
	
// in-house libraries

namespace aggregated
{

	template <typename ElementOrder>
	class Order
	{
		const ElementOrder elements;
	public:
		Order(const ElementOrder& elements):
			elements(elements)
		{}
		Order():
			elements()
		{}

		template <typename T>
		auto min(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto extremum = (a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = (a[i]);
				extremum = elements.less_than(ai, extremum) && elements.less_than(lo, ai) && elements.less_than(ai, hi)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		auto max(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto extremum = (a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = (a[i]);
				extremum = elements.greater_than(ai, extremum) && elements.less_than(lo, ai) && elements.less_than(ai, hi)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		auto min(const T& a) const
		{
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto extremum = (a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = (a[i]);
				extremum = elements.less_than(ai, extremum)? ai : extremum;
			}
			return extremum;
		}

		template <typename T>
		auto max(const T& a) const
		{
			if (a.size() < 1)
			{
				throw std::out_of_range("cannot find the minimum value of an empty series");
			}
			auto extremum = (a[0]);
			for (auto i = 0*a.size()+1; i < a.size(); ++i)
			{
				auto ai = (a[i]);
				extremum = elements.greater_than(ai, extremum)? ai : extremum;
			}
			return extremum;
		}

		// component-wise min
		template <typename T>
		typename T::size_type min_id(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typename T::value_type extremum = (a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = (a[i]);
				if (elements.less_than(ai, extremum) && elements.less_than(lo, ai) && elements.less_than(ai, hi))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		template <typename T>
		typename T::size_type max_id(const T& a, const typename T::value_type lo, const typename T::value_type hi) const
		{
			typename T::value_type extremum = (a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = (a[i]);
				if (elements.greater_than(ai, extremum) && elements.less_than(lo, ai) && elements.less_than(ai, hi))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		// component-wise min
		template <typename T>
		typename T::size_type min_id(const T& a) const
		{
			typename T::value_type extremum = (a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = (a[i]);
				if (elements.less_than(ai, extremum))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		template <typename T>
		typename T::size_type max_id(const T& a) const
		{
			typename T::value_type extremum = (a[0]);
			typename T::size_type extremum_id = 0;
			for (auto i = 0*a.size(); i < a.size(); ++i)
			{
				auto ai = (a[i]);
				if (elements.greater_than(ai, extremum))
				{
					extremum = ai;
					extremum_id = i;
				}
			}
			return extremum_id;
		}

		template <typename In1, typename In2>
		bool equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return false;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.not_equal(a[i], b[i])){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool not_equal(const In1& a, const In2& b) const
		{
			if (a.size() != b.size()){
				return true;
			}
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.not_equal(a[i], b[i])){
					return true;
				}
			}
			return false;
		}

		template <typename In1, typename In2>
		bool greater_than(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.less_than_equal(a[i], b[i])){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool less_than(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.greater_than_equal(a[i], b[i])){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool greater_than_equal(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.less_than(a[i], b[i])){
					return false;
				}
			}
			return true;
		}

		template <typename In1, typename In2>
		bool less_than_equal(const In1& a, const In2& b) const
		{
			auto size = a.size();
			for (auto i = 0*size; i < size; ++i)
			{
				if (elements.greater_than(a[i], b[i])){
					return false;
				}
			}
			return true;
		}

	};

}


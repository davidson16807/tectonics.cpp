#pragma once

namespace each
{

	template <typename T1, typename Tout>
	bool compatible(const T1& a, Tout& out)
	{
		return 
			out.size() == a.size();
	}

	template <typename T1, typename T2, typename Tout>
	bool compatible(const T1& a, const T2& b, Tout& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0;
	}

	template <typename T1, typename T2, typename T3, typename Tout>
	bool compatible(const T1& a, const T2& b, const T3& c, Tout& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() >= c.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0 &&
			out.size() % c.size() == 0;
	}

}

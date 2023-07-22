#pragma once

// 3rd party libraries
#include <glm/vector_relational.hpp>

// in-house libraries
#include "../each.hpp"

namespace whole {

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator==(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::equal(a,b);
	// }

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator!=(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::notEqual(a,b);
	// }

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator>(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::greaterThan(a,b);
	// }

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator>=(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::greaterThanEqual(a,b);
	// }

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator<(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::lessThan(a,b);
	// }

	// template<glm::length_t L, typename T, glm::qualifier Q>
	// bool operator<=(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q> b)
	// {
	// 	return glm::lessThanEqual(a,b);
	// }


	template <typename T1, typename T2, typename Tout>
	void equal(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::equal(a[i] , b[i]);
		}
	}

	template <typename T1, typename T2, typename Tout>
	void notEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::notEqual(a[i] , b[i]);
		}
	}

	template <typename T1, typename T2, typename Tout>
	void greaterThan(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::greaterThan(a[i] ,b[i]);
		}
	}

	template <typename T1, typename T2, typename Tout>
	void lessThan(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::lessThan(a[i] ,b[i]);
		}
	}

	template <typename T1, typename T2, typename Tout>
	void greaterThanEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::greaterThanEqual(a[i] , b[i]);
		}
	}

	template <typename T1, typename T2, typename Tout>
	void lessThanEqual(const T1& a, const T2& b, Tout& out)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = glm::lessThanEqual(a[i] , b[i]);
		}
	}

}
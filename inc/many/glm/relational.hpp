#pragma once

#include <glm/vector_relational.hpp>

#include "../composite.hpp"

namespace many
{



	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b)
	{
		bool out(true);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b)
	{
		bool out(false);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b) > threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b)
	{
		bool out(true);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b[i]) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b)
	{
		bool out(false);
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b[i]) > threshold;
		}
		return out;
	}





	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<bool>& out)
	{
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<bool>& out)
	{
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) > threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<bool>& out)
	{
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<bool>& out)
	{
		T threshold(MANY_EPSILON);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) > threshold;
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const composite<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}





	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const composite<glm::vec<L,T,Q>>& a, const composite<glm::vec<L,T,Q>>& b, composite<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}





}
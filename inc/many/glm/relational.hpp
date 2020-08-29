#pragma once

#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>
#include <glm/vector_relational.hpp>

#include "../types.hpp"
#include "../relational.hpp"
#include "geometric.hpp"

namespace many
{

	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,Q>>& out, T threshold = T(MANY_EPSILON))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::equal(ai,bi); }, a, b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,Q>>& out, T threshold = T(MANY_EPSILON))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::notEqual(ai,bi); }, a, b);
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,Q>>& out, T threshold = T(MANY_EPSILON))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::equal(ai,bi); }, a, b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,Q>>& out, T threshold = T(MANY_EPSILON))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::notEqual(ai,bi); }, a, b);
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::greaterThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::greaterThanEqual(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::lessThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::lessThanEqual(ai,bi); }, a, b); 
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::greaterThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::greaterThanEqual(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::lessThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const series<glm::vec<L,T,Q>>& a, const series<glm::vec<L,T,Q>>& b, series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::lessThanEqual(ai,bi); }, a, b); 
	}

}
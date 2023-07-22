#pragma once

#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>
#include <glm/vector_relational.hpp>

#include "../types.hpp"
#include "../relational.hpp"
#include "geometric.hpp"

namespace series
{

	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,Q>>& out, T threshold = T(epsilon))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::equal(ai,bi); }, a, b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,Q>>& out, T threshold = T(epsilon))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::notEqual(ai,bi); }, a, b);
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,Q>>& out, T threshold = T(epsilon))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::equal(ai,bi); }, a, b);
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,Q>>& out, T threshold = T(epsilon))
	{
		out.store([threshold](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::notEqual(ai,bi); }, a, b);
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::greaterThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::greaterThanEqual(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::lessThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const Series<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q> ai, const glm::vec<L,T,Q> bi){ return glm::lessThanEqual(ai,bi); }, a, b); 
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::greaterThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::greaterThanEqual(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::lessThan(ai,bi); }, a, b); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const Series<glm::vec<L,T,Q>>& a, const Series<glm::vec<L,T,Q>>& b, Series<glm::vec<L,bool,glm::defaultp>>& out)
	{
		out.store([](const glm::vec<L,T,Q>& ai, const glm::vec<L,T,Q>& bi){ return glm::lessThanEqual(ai,bi); }, a, b); 
	}

}
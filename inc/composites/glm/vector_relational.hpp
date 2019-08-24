#pragma once

#include <glm/vector_relational.hpp>

#include "../many.hpp"

namespace composites
{
	using namespace glm;




	template<length_t L, typename T, qualifier Q>
	void greaterThan(const many<vec<L,T,Q>>& a, const vec<L,T,Q> b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void greaterThanEqual(const many<vec<L,T,Q>>& a, const vec<L,T,Q> b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void lessThan(const many<vec<L,T,Q>>& a, const vec<L,T,Q> b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void lessThanEqual(const many<vec<L,T,Q>>& a, const vec<L,T,Q> b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}







	template<length_t L, typename T, qualifier Q>
	void greaterThan(const many<vec<L,T,Q>>& a, const many<vec<L,T,Q>>& b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void greaterThanEqual(const many<vec<L,T,Q>>& a, const many<vec<L,T,Q>>& b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void lessThan(const many<vec<L,T,Q>>& a, const many<vec<L,T,Q>>& b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<length_t L, typename T, qualifier Q>
	void lessThanEqual(const many<vec<L,T,Q>>& a, const many<vec<L,T,Q>>& b, many<vec<L,bool,defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}




}
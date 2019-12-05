#pragma once

#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>

#include "../types.hpp"
#include "../relational.hpp"

namespace many
{






	template <glm::length_t L, class T, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, T cosine_threshold, T length_threshold)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= glm::dot(glm::normalize(a[i]),glm::normalize(b[i])) > cosine_threshold && !(glm::length(b[i])/glm::length(a[i]) < length_threshold || glm::length(a[i])/glm::length(b[i]) < length_threshold);
		}
		return out;
	}
	template <glm::length_t L, class T, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, T cosine_threshold, T length_threshold)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= glm::dot(glm::normalize(a[i]),glm::normalize(b)) > cosine_threshold && !(glm::length(b)/glm::length(a[i]) < length_threshold || glm::length(a[i])/glm::length(b) < length_threshold);
		}
		return out;
	}

	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,unsigned int,Q>>& a, const glm::vec<L,unsigned int,Q> b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b == glm::vec<L,unsigned int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,unsigned int,Q>>& a, const glm::vec<L,unsigned int,Q> b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b != glm::vec<L,unsigned int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,unsigned int,Q>>& a, const tmany<glm::vec<L,unsigned int,Q>>& b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b[i] == glm::vec<L,unsigned int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,unsigned int,Q>>& a, const tmany<glm::vec<L,unsigned int,Q>>& b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b[i] != glm::vec<L,unsigned int, Q>(0);
		}
		return out;
	}





	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,unsigned int,Q>>& a, const glm::vec<L,unsigned int,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b == glm::vec<L, unsigned int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,unsigned int,Q>>& a, const glm::vec<L,unsigned int,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b != glm::vec<L, unsigned int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,unsigned int,Q>>& a, const tmany<glm::vec<L,unsigned int,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] == glm::vec<L, unsigned int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,unsigned int,Q>>& a, const tmany<glm::vec<L,unsigned int,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] != glm::vec<L, unsigned int, Q>(0);
		}
	}







	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,int,Q>>& a, const glm::vec<L,int,Q> b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b == glm::vec<L,int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,int,Q>>& a, const glm::vec<L,int,Q> b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b != glm::vec<L,int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,int,Q>>& a, const tmany<glm::vec<L,int,Q>>& b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b[i] == glm::vec<L,int, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,int,Q>>& a, const tmany<glm::vec<L,int,Q>>& b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b[i] != glm::vec<L,int, Q>(0);
		}
		return out;
	}





	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,int,Q>>& a, const glm::vec<L,int,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b == glm::vec<L, int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,int,Q>>& a, const glm::vec<L,int,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b != glm::vec<L, int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,int,Q>>& a, const tmany<glm::vec<L,int,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] == glm::vec<L, int, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,int,Q>>& a, const tmany<glm::vec<L,int,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] != glm::vec<L, int, Q>(0);
		}
	}








	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,bool,Q>>& a, const glm::vec<L,bool,Q> b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b == glm::vec<L,bool, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,bool,Q>>& a, const glm::vec<L,bool,Q> b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b != glm::vec<L,bool, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,bool,Q>>& a, const tmany<glm::vec<L,bool,Q>>& b)
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= a[i] - b[i] == glm::vec<L,bool, Q>(0);
		}
		return out;
	}
	template<glm::length_t L, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,bool,Q>>& a, const tmany<glm::vec<L,bool,Q>>& b)
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= a[i] - b[i] != glm::vec<L,bool, Q>(0);
		}
		return out;
	}





	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,bool,Q>>& a, const glm::vec<L,bool,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b == glm::vec<L, bool, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,bool,Q>>& a, const glm::vec<L,bool,Q> b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b != glm::vec<L, bool, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,bool,Q>>& a, const tmany<glm::vec<L,bool,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] == glm::vec<L, bool, Q>(0);
		}
	}
	template<glm::length_t L, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,bool,Q>>& a, const tmany<glm::vec<L,bool,Q>>& b, tmany<bool>& out)
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = a[i] - b[i] != glm::vec<L, bool, Q>(0);
		}
	}




	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, T threshold = T(MANY_EPSILON))
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, T threshold = T(MANY_EPSILON))
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b) > threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool equal(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, T threshold = T(MANY_EPSILON))
	{
		bool out(true);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out &= distance(a[i], b[i]) <= threshold;
		}
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	bool notEqual(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, T threshold = T(MANY_EPSILON))
	{
		bool out(false);
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out |= distance(a[i], b[i]) > threshold;
		}
		return out;
	}



	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<bool>& out, T threshold = T(MANY_EPSILON))
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<bool>& out, T threshold = T(MANY_EPSILON))
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b) > threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void equal(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<bool>& out, T threshold = T(MANY_EPSILON))
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) <= threshold;
		}
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void notEqual(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<bool>& out, T threshold = T(MANY_EPSILON))
	{
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			out[i] = distance(a[i], b[i]) > threshold;
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const tmany<glm::vec<L,T,Q>>& a, const glm::vec<L,T,Q> b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}





	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThan(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void greaterThanEqual(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::greaterThanEqual, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThan(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThan, out); 
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	void lessThanEqual(const tmany<glm::vec<L,T,Q>>& a, const tmany<glm::vec<L,T,Q>>& b, tmany<glm::vec<L,bool,glm::defaultp>>& out)
	{
		transform(a, b, glm::lessThanEqual, out); 
	}





}
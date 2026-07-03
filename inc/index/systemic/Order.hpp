#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatible.hpp"

namespace systemic
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	#define SYSTEMIC_UNARY_METHOD(METHOD, NAME) \
	template <typename A, typename Out>\
	void NAME (const A& a, Out& out) const\
	{\
		assert(compatible(a,out));\
		auto size = a.size();\
		out.clear();\
		out.reserve(a.size());\
		for (auto i = 0*size; i < size; ++i)\
		{\
			auto entity = a.entity_for_index(i); \
			auto component = a.component_for_index(i); \
			out.add(entity, METHOD(a[i]));\
		}\
	}

	#define SYSTEMIC_BINARY_METHOD(METHOD, NAME) \
	template <typename A, typename B, typename Out>\
	void NAME (const A& a, const B& b, Out& out) const\
	{\
		assert(compatible(a,b,out));\
		auto size = a.size();\
		out.clear();\
		out.reserve(std::min(a.size(), b.size()));\
		for (auto i = 0*size; i < size; ++i)\
		{\
			auto entity = a.entity_for_index(i); \
			auto ai = a.component_for_index(i); \
			auto bi = b.component_for_entity(entity); \
			out.add(entity, METHOD(a[i], b[i]));\
		}\
	}

	#define SYSTEMIC_TRINARY_METHOD(METHOD, NAME) \
	template <typename A, typename B, typename C, typename Out>\
	void NAME (const A& a, const B& b, const C& c, Out& out) const\
	{\
		assert(compatible(a,b,out));\
		auto size = a.size();\
		out.clear();\
		out.reserve(std::min(a.size(), std::min(b.size(), c.size())));\
		for (auto i = 0*size; i < size; ++i)\
		{\
			auto entity = a.entity_for_index(i); \
			auto ai = a.component_for_index(i); \
			auto bi = b.component_for_entity(entity); \
			auto ci = c.component_for_entity(entity); \
			out.add(entity, METHOD(a[i], b[i], c[i]));\
		}\
	}

	template <typename ComponentOrder>
	class Order
	{
		const ComponentOrder components;
	public:
		Order(const ComponentOrder& components):
			components(components)
		{}
		Order():
			components()
		{}
		SYSTEMIC_BINARY_METHOD(components.greater_than,        greater_than)
		SYSTEMIC_BINARY_METHOD(components.less_than,           less_than)
		SYSTEMIC_BINARY_METHOD(components.greater_than_equal,  greater_than_equal)
		SYSTEMIC_BINARY_METHOD(components.less_than_equal,     less_than_equal)
		SYSTEMIC_BINARY_METHOD(components.equal,               equal)
		SYSTEMIC_BINARY_METHOD(components.not_equal,           not_equal)

		SYSTEMIC_BINARY_METHOD(components.min,   min)
		SYSTEMIC_BINARY_METHOD(components.max,   max)

		SYSTEMIC_TRINARY_METHOD(components.clamp, clamp)

	};

	#undef SYSTEMIC_UNARY_METHOD
	#undef SYSTEMIC_BINARY_METHOD
	#undef SYSTEMIC_TRINARY_METHOD

}

